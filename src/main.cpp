#include "Geode/Geode.hpp"
using namespace geode::prelude;

#include <regex>

auto FOUNDED_ANIMATE_FILES = std::map<std::string, std::filesystem::path>();
auto FOUND_ANIMATE_FILES() {
    FOUNDED_ANIMATE_FILES.clear();
    for (auto path : CCFileUtils::get()->getSearchPaths()) {
        //.animate.json
        for (auto file : file::readDirectory(path.c_str()).unwrapOrDefault())
            if (string::contains(file.string(), ".animate.json")) FOUNDED_ANIMATE_FILES[std::filesystem::path(file).filename().string()] = file;
    };
};

$on_mod(Loaded) {

    FOUND_ANIMATE_FILES();

    auto search_paths = {
        getMod()->getConfigDir().string(),
        getMod()->getSaveDir().string(),
        getMod()->getTempDir().string()
    };
    for (auto entry : search_paths) CCFileUtils::get()->addPriorityPath(entry.c_str());

}

#include <Geode/modify/GameManager.hpp>
class $modify(GameManagerFindAnimateFilesOnReload, GameManager) {

    void reloadAll(bool switchingModes, bool toFullscreen, bool borderless, bool fix, bool unused) {

        FOUND_ANIMATE_FILES();

        return GameManager::reloadAll(switchingModes, toFullscreen, borderless, fix, unused);
    }

};

static void attachAnimator(CCNode* node, std::string name) {
    //log::debug("SpriteAnimator::attach({}, {})", node, name);

    if (!node) return log::error("sprite = {}", node);

    auto file = fmt::format("{}.animate.json", name);
    if (!FOUNDED_ANIMATE_FILES.contains(file.c_str())) return;

    auto json_read = file::readJson(FOUNDED_ANIMATE_FILES[file]);
    if (json_read.err()) return log::error("sprite animator attach failed ar reading, {}. file: {}", json_read.err().value_or("unknown error"), file);

    auto json = json_read.unwrapOrDefault();

    //log::debug("{}", json.dump());

    auto frame = [](const char* name) -> CCSpriteFrame*
        {
            auto frame = typeinfo_cast<CCSpriteFrame*>(CCSpriteFrameCache::get()->m_pSpriteFrames->objectForKey(name));
            if (not frame) {
                frame = CCSprite::create(name)->displayFrame();
                CCSpriteFrameCache::get()->addSpriteFrame(frame, name);
                return frame;
            }
            return frame;
        };

    auto expandFrameName = [](const std::string& name) {
        std::vector<std::string> result;

        std::smatch match;
        if (std::regex_match(name, match, std::regex(R"(^(.*)\{(\d+)-(\d+)\}(.*)$)"))) {
            std::string prefix = match[1];
            std::string startStr = match[2];
            std::string endStr = match[3];
            std::string suffix = match[4];

            int start = std::stoi(startStr);
            int end = std::stoi(endStr);

            //zeros fill ya
            int width = static_cast<int>(startStr.length());

            for (int i = start; i <= end; ++i) {
                std::ostringstream oss;
                oss << prefix << std::setw(width) << std::setfill('0') << i << suffix;
                result.push_back(oss.str());
            }
        }
        else {
            //no pattern huh
            result.push_back(name);
        }

        return result;
        };

    auto frames = CCArray::create();
    if (json["names"].isArray()) {
        for (const auto& nameEntry : json["names"]) {

            for (const auto& frameName : expandFrameName(nameEntry.asString().unwrapOrDefault()))
                if (auto tmpSprite = frame(frameName.c_str()))
                    if (auto frame = tmpSprite) frames->addObject(frame);

        };
    }
    else {

        for (const auto& frameName : expandFrameName(json["names"].asString().unwrapOrDefault()))
            if (auto tmpSprite = frame(frameName.c_str()))
                if (auto frame = tmpSprite) frames->addObject(frame);

    }

    //log::debug("{}", frames);

    //iyea i know same thing exists in engine but i want made my analog :>
    auto ptrTag = reinterpret_cast<uintptr_t>(frames);
    node->setUserObject(fmt::format("animator-frames-array-{}", ptrTag), frames);
    node->runAction(CCRepeatForever::create(CCSpawn::create(CallFuncExt::create([node, frames, ptrTag] {
        if (!node) return log::error("node = {}", node);
        if (!frames) return log::error("frames = {}", frames);
        if (!node->getUserObject(fmt::format("animator-frames-array-{}", ptrTag))) return log::error("no animator-frames-array-{} in sprite!!!", ptrTag);

        int nextIndex = frames->getTag();
        nextIndex = nextIndex >= frames->count() ? 0 : nextIndex;

        //log::debug("nextIndex->{}", nextIndex);

        if (auto frame = typeinfo_cast<CCSpriteFrame*>(frames->objectAtIndex(nextIndex))) {
            if (auto casted = typeinfo_cast<CCSprite*>(node)) casted->setDisplayFrame(frame);
            if (auto casted = typeinfo_cast<CCSpriteBatchNode*>(node)) if(frame->getTexture()) casted->setTexture(frame->getTexture());
            if (auto casted = typeinfo_cast<CCScale9Sprite*>(node)) {
                auto size = casted->getContentSize();
                casted->setSpriteFrame(frame);
                casted->setContentSize(size); //size
            }
        }
        frames->setTag(nextIndex + 1);

        }), CCDelayTime::create(json["delay"].asDouble().unwrapOr(0.1f)), nullptr)));
}

#include <Geode/modify/CCSpriteBatchNode.hpp>
class $modify(CCSpriteBatchNodeAnimatorAttach, CCSpriteBatchNode) {

    static void onModify(auto & self) {
        auto names = {
            "cocos2d::CCSpriteBatchNode::create",
            //"cocos2d::CCSpriteBatchNode::createWithSpriteFrameName",
        };
        for (auto name : names) if (!self.setHookPriorityPost(name, Priority::Last)) {
            log::error("Failed to set hook priority for {}.", name);
        }
    }

    static CCSpriteBatchNode* create(const char* fileImage) {
        auto rtn = CCSpriteBatchNode::create(fileImage);
        attachAnimator(rtn, fileImage);
        return rtn;
    }

};

#include <Geode/modify/CCScale9Sprite.hpp>
class $modify(CCScale9SpriteAnimatorAttach, CCScale9Sprite) {

    static void onModify(auto & self) {
        auto names = {
            "cocos2d::extension::CCScale9Sprite::create",
            "cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName",
        };
        for (auto name : names) if (!self.setHookPriorityPost(name, Priority::Last)) {
            log::error("Failed to set hook priority for {}.", name);
        }
    }

    static CCScale9Sprite* create(const char* file, CCRect rect) {
        auto rtn = CCScale9Sprite::create(file, rect);
        attachAnimator(rtn, file);
        return rtn;
    }

    static CCScale9Sprite* createWithSpriteFrameName(const char* spriteFrameName, CCRect capInsets) {
        auto rtn = CCScale9Sprite::createWithSpriteFrameName(spriteFrameName, capInsets);
        attachAnimator(rtn, spriteFrameName);
        return rtn;
    }

    static CCScale9Sprite* createWithSpriteFrameName(const char* spriteFrameName) {
        auto rtn = CCScale9Sprite::createWithSpriteFrameName(spriteFrameName);
        attachAnimator(rtn, spriteFrameName);
        return rtn;
    }

};

#include <Geode/modify/CCSprite.hpp>
class $modify(CCSpriteAnimatorAttach, CCSprite) {

    static void onModify(auto & self) {
        auto names = {
            "cocos2d::CCSprite::create",
            "cocos2d::CCSprite::createWithSpriteFrameName",
        };
        for (auto name : names) if (!self.setHookPriorityPost(name, Priority::Last)) {
            log::error("Failed to set hook priority for {}.", name);
        }
    }

    static CCSprite* create(const char* pszFileName) {
        auto rtn = CCSprite::create(pszFileName);
        attachAnimator(rtn, pszFileName);
        return rtn;
    }

    static CCSprite* createWithSpriteFrameName(const char* pszSpriteFrameName) {
        auto rtn = CCSprite::createWithSpriteFrameName(pszSpriteFrameName);
        attachAnimator(rtn, pszSpriteFrameName);
        return rtn;
    }

};