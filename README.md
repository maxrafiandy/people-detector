# People-detector
A simple indoor people-tracking. This program will send all captured images from camera-client to Server-side. The image processing will performed in Server-side and received images will forwarded to mobile-client. If any people detected (frontal-head and full-body), mobile-client drive an alarm. 

# Dependencies:
- libfreenect (required only if use kinect camera)
- OpenCV
- LibBoost
- Qt-creator and Qt-android-5.7 (to build android application, need Android SDK and Android NDK) 

# How To
- Compile the source:
  - *Install all dependencies*
  - Go to source code directory and run: `make`
  - if no error found, run: `sudo make install`. This will install all generated files to system.

- Run Camera:
  **camera server_ip-address**. e.g: `camera 192.168.1.1`

- Run Server:
  Just type `server` and run it
