# People-detector

# Dependencies:
- libfreenect
- OpenCV
- libboost
- Qt-creator and Qt-android-5.7 (To build android application) 

# How To
- Compile the source:
  -> Install all dependencies
  -> Go to source code directory and run: `make`
  -> if no error found, run: `sudo make install`. This will install all generated files to system.

- Run Camera:
  $`camera server_ip-address`. e.g: $`camera 192.168.1.1`

- Run Server:
  $ `server`
