# People-detector
Simple indoor People-tracking

# Dependencies:
- libfreenect (required only if use kinect camera)
- OpenCV
- LibBoost
- Qt-creator and Qt-android-5.7 (to build android application, need Android SDK and Android NDK) 

# How To
- Compile the source:
  -> *Install all dependencies*
  -> Go to source code directory and run: `make`
  -> if no error found, run: `sudo make install`. This will install all generated files to system.

- Run Camera:
  **camera server_ip-address**. e.g: `camera 192.168.1.1`

- Run Server:
  $ `server`
