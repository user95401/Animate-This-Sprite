# Animate This Sprite!

This mod allows you to add custom animations 
to Geometry Dash texture objects using a JSON configuration file.

You can define the animate config by creating a corresponding JSON file (the `{name}.animate.json`). 
The mod automatically scans all search paths, loads the JSON configurations, and applies animations at runtime.

[Download example resource pack](https://github.com/user95401/Animate-This-Sprite/releases/download/test-pack/user95401.animate-this-sprite.test-pack.zip)

*to load with texture loader by geode team*

## File Organization

- **Resource Pack Directory:**  
  Create or modify your resource pack directory.

- **Image Assets:**  
  Place your image files inside a folder in your resource pack. 
  Its also supports sprite sheet frames.

- **JSON Animation Configurations:**  
  For any animated sprite, create a JSON file named following the pattern `yourAssetName.animate.json`. For example, if your sprite file is `"GJ_button_01.png"`, the JSON file should be named `GJ_button_01.png.animate.json`.
  
`WARN: don't forget .png, that is also part of name!`

## JSON Animator Config File Content

Your JSON configuration files control which images will be used for the animation and the timing between frames. The configuration contains at least two keys:

- **"frames"**:  
  `This key can be either a string or an array of strings. It supports both a single image name and a numerical range for frame sequences.`
  
  **Examples:**
  
  \- Single sprite (xd):
    ```json
    { "names": "frame.png", "delay": 0.1 }
    ```
    
  \- Animation sequence with a range:
    ```json
    { "names": "frame{1-5}.png", "delay": 0.1 }
    ```
    
  \- With leading zeros in the file names:
    ```json
    { "names": "frame_{001-015}.png", "delay": 0.1 }
    ```
    
  \- Array of mixed entries:
    ```json
    {"names":["frame_{1-5}.png", "frame_final.png"]}
    ```

- **"delay"** (not required):  
  `A floating-point number that specifies the time (in seconds) between each frame. The default in the code is 0.1 seconds if not provided.`

- **"spritesheets"** (not required):  
  `Define string or array of strings. Mod will load sprite sheets by that value.`

  **Examples:**

  \- One sprite sheet:
    ```json
    { "names": "frame_{001-015}.png", "spritesheets": "frames.plist" }
    ```

  \- Array of sprite sheets:
    ```json
    { 
        "names": "frame_{001-015}.png", 
        "spritesheets": [
            "frames1.plist"
            "frames2.plist"
        ]
    }
    ```

## JSON Animator Config File Name Variables

You can put `pack.png.animate.json` to
```bash
{ModsSaveDir}/geode.texture-loader/unzipped/user95401.animate-this-sprite.test-pack.zip/
```
for real full path
```bash
C:/Users/user95401/AppData/Local/GeometryDash/geode/mods/geode.texture-loader/unzipped/user95401.animate-this-sprite.test-pack.zip/
```

[Explore example resource pack...](https://github.com/user95401/Animate-This-Sprite/releases/download/test-pack/user95401.animate-this-sprite.test-pack.zip)

- **{GameDir\}**: Directory where Geometry Dash is
- **{SaveDir\}**: Directory where GD saves its files
- {GeodeDir\}: Directory where Geode is
- **{GeodeSaveDir\}**: Directory where Geode saves its files
- {GeodeResourcesDir\}: Directory where Geode's resources are stored
- {GeodeLogDir\}: Directory where Geode's resources are stored
- {TempDir\}: Directory to store temporary files
- {ModsDir\}: Directory where mods are stored by default
- **{ModsSaveDir\}**: Directory where mods' save data is stored
- **{ModRuntimeDir\}**: Directory where mods' unzipped packages are stored at runtime
- **{ModConfigDir\}**: Directory where mods' config files lie
- {IndexDir\}: Directory where Geode stores the cached index
- {CrashlogsDir\}: Directory where crashlogs are stored
- {ModPersistentDir\}: Directory where mods' persistent files lie, this directory is not deleted even when Geode is uninstalled

## Naming Conventions & Range Support

- **Range Syntax:**  
  To simplify your animation sequence, you can use a range pattern in your file name:
  
  **- Format:** `prefix{start-end}suffix`  

    For example, `"frame_{1-5}.png"` tells the mod to create frames:
    ```
    frame_1.png, 
    frame_2.png, 
    frame_3.png, 
    frame_4.png, 
    frame_5.png
    ```

  If your file names include leading zeros, the code will maintain that format. (e.g., `"frame_{01-15}.png"` or `"frame_{001-015}.png"`)

- **For Scale Factor...**
  You SHOULD NOT put the `-hd` or `-uhd` to animator file name. Mod will not load it so! You should just add suffix to TEXTURE filename or spritesheet (as always was).