# Remal Arduino Boards
This repository contains all Remal Arduino-supported boards.

## Installation
1. **Open Arduino IDE**
   - Ensure you have the latest version installed.
   
2. **Add Board Manager URL**
   - Go to `File > Preferences`.
   - Add the following URL to `Additional Boards Manager URLs`:
     ```
     https://remal.io/remal-core/package_remalboards_index.json
     ```

3. **Install Boards**
   - Go to `Tools > Board > Boards Manager`.
   - Search for `Remal IoT` and install the latest versions of the boards you want to use.

4. **Select Your Board**
   - Go to `Tools > Board` and select the Remal board you want to use.

## Examples
- After selecting the board from `Tools > Board` in the Arduino IDE, go to `File > Examples` to find examples for each board.

## Currently Supported Boards
- [Shabakah v4.0](https://remal.io/remal-boards/shabakah-v4-0/)

### Legacy Boards
- [Shabakah Founders Edition (v3.1)](https://remal.io/remal-boards/shabakah-v3-1/)
- [Atlas Founders Edition (v2)](https://remal.io/remal-boards/atlas-v2/)
- [Nabd Founders Edition (v3.1)](https://remal.io/remal-boards/nabd-v3-1/)

## Troubleshooting
If you encounter any issues during the installation or use of Remal boards, please contact us directly at info@remal.io.

## Contributing
We welcome contributions to improve our board support and examples. Please fork this repository and submit pull requests with any enhancements or fixes.