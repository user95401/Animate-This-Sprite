# Animate This Sprite!

This mod allows you to add custom animations 
to Geometry Dash texture objects using a JSON configuration file.

You can define the animate config by creating a corresponding JSON file (the `{name}.animate.json`). 
The mod automatically scans all search paths, loads the JSON configurations, and applies animations at runtime.

[Download example resource pack](https://github.com/user95401/user95401/releases/download/AnimatedGradientBG/AnimatedGradientBG.zip)

## File Organization

- **Resource Pack Directory:**  
  Create or modify your resource pack directory.

- **Image Assets:**  
  Place your image files inside a folder in your resource pack. 
  Its also supports sprite sheet frames.

- **JSON Animation Configurations:**  
  For any animated sprite, create a JSON file named following the pattern `yourAssetName.animate.json`. For example, if your sprite file is `"GJ_button_01.png"`, the JSON file should be named `GJ_button_01.png.animate.json`.
  
`WARN: don't forget .png, that is also part of name!`

## JSON Configuration Content

Your JSON configuration files control which images will be used for the animation and the timing between frames. The configuration contains at least two keys:

- **"names"**:  
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

---

<img src="logo.png" width="150" alt="the mod's logo" />

*Updated logo.png to change mod's icon (rly)*

## Getting started
We recommend heading over to [the getting started section on our docs](https://docs.geode-sdk.org/getting-started/) for useful info on what to do next.

## Build instructions
For more info, see [our docs](https://docs.geode-sdk.org/getting-started/create-mod#build)
```sh
# Assuming you have the Geode CLI set up already
geode build
```

# Resources
* [Geode SDK Documentation](https://docs.geode-sdk.org/)
* [Geode SDK Source Code](https://github.com/geode-sdk/geode/)
* [Geode CLI](https://github.com/geode-sdk/cli)
* [Bindings](https://github.com/geode-sdk/bindings/)
* [Dev Tools](https://github.com/geode-sdk/DevTools)
