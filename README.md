## Small ePaper Shield refactored
This repository is a very small and fast version of the [SeeedStudio Small ePaper Library](https://github.com/Seeed-Studio/Small_ePaper_Shield) compatible with the Arduino MEGA (ATmega2560) and Arduino DUE (SAM3X8E). It might also work on other Arduino devices but I've only tested it on these two.

## Why does this exist?
The original driver for the small ePaper shield comes with a few problems:
 - The buffer size makes it almost impossible to use it together with other stuff since it's using all the RAM.
 - Handling text and images is generally pretty slow ( internally, not the display refresh rate )
 - It is not compatible with Arduino DUE. [Tweaking it](http://forum.arduino.cc/index.php?topic=238318.0) works more or less but it's **really** slow (40s refresh time tested) and unstable.

## So, what is this?
This is a small version of the original repositiore containing only the functions I needed for the project I've been working on. I didn't actually want to share this but since people asked me for it, I uploaded it here and who knows, maybe it actually fits your needs.
Major changes to the full library:
 - Buffer size decresed, don't know if it can still handle images.
 - Removed a lot of the matrix charmaps, chinese characters can't be used.
 - It works fast and stable on Arduino DUE!
 

## Usage
1. Set the correct display size [via constant](https://github.com/cybrox/Seeedstudio_ePaper_refactored/blob/master/ePaper.h#L19)
2. Call `EPAPER.begin();` once in your `setup` routine.  
3. After this, you can use it as stated in the [official documentations](https://github.com/Seeed-Studio/Small_ePaper_Shield/blob/master/README.md#3-int-drawcharchar-c-int-x-int-y).