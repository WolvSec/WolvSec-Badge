# WolvSec-Badge

Repository for WolvSec DEF CON Badge.

## Building the Docker Image

`sudo docker build -t badge-image .`

## Running the Docker Image

`sudo docker run -it --rm -v${PWD}:/home/badge -v/media:/media badge-image`
- Mounts the directory that this README is in to `/home/badge`.
- Mounts your system's `/media/` directory inside of the container in order for `deploy.sh` to work.

## Building

In this directory:

```
mkdir build
cd build
cmake ..
make
```

- If no errors are encountered, build files will be generated in the `build` directory you created.
- You can safely run `rm -rf build/` to remove all generated build files.

## Deploying

`./deploy.sh`
