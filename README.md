# WolvSec-Badge

Repository for WolvSec DEF CON Badge.

## Building the Docker Image

`docker build -t wolvsec-badge .`

## Building

1. Pull submodules: `git submodule update --init --recursive`.
2. Create a Python virtual environment and install the `scuba` package.
3. Run `mkdir build && cd build/ && scuba build`.

- If no errors are encountered, build files will be generated in the `build` directory.

## Deploying (USB mass storage device mode)
1. Boot the Pico into BOOTSEL mode by holding the BOOTSEL button while plugging in the device.
2. `./deploy.sh`
