## Small ePaper Shielf refactored
This repository is a very small and fast version of the [SeeedStudio Small ePaper Library](https://github.com/Seeed-Studio/Small_ePaper_Shield) compatible with the Arduino MEGA (ATmega2560) and Arduino DUE (SAM3X8E). It might also work on other Arduino devices but I've only tested it on these two.

## Why does this exist?
The original driver for the small ePaper shield comes with a few problems:
 - The buffer size makes it almost impossible to use it together with other stuff since it's using all the RAM.
 - Handling text and images is generally pretty slow ( internally, not the display refresh rate )
 - It is not compatible with Arduino DUE. Tweaking it works more or less but it's **really** slow (40s refresh time tested)

## So, what is this?
This is a small version of the original repositiore containing only the functions I needed for the project I've been working on. I didn't actually want to share this but since people asked me for it, I uploaded it here and who knows, maybe it actually fits your needs.
Major changes to the full library:
 - Buffer size decresed, don't know if it can handle images
 - Removed a lot of the matrix charmaps, chinese characters can't be used.
 - It works fast and stable on Arduino DUE