# Remal Arduino Boards
This repository contains all Remal IoT Arduino supported boards.

## Installation
1. Open Arduino IDE
2. Go to `File > Preferences`
3. Add the following URL to `Additional Boards Manager URLs`:
```
https://remal.io/remal-core/package_remalboards_index.json
```
4. Go to `Tools > Board > Boards Manager`
5. Search for `Remal IoT` and install the latest versions of the boards you want to use
6. Go to `Tools > Board` and select the board you want to use

## Examples
You can find examples for each board after selecting it from `Tools > Board` in Arduino IDE and then going to `File > Examples` look for the board name.

There is currently a bug in Arduino IDE that prevents the examples from showing up in the `File > Examples` menu. The fix is to simply select any other board and then select the board you want to use again and the examples should show up.

## Currently Supported Boards
- Shabakah v4.0

### Legacy Boards
- [Shabakah Founders Edition (v3.1)](https://remal.io/remal-boards/shabakah-v3-1/)
- [Atlas Founders Edition (v2)](https://remal.io/remal-boards/atlas-v2/)
- [Nabd Founders Edition (v3.1)](https://remal.io/remal-boards/nabd-v3-1/)