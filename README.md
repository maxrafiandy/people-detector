# LISENSE
*Copyright (C) 2016  Max Rafiandy*

This program is **free software**: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but *WITHOUT ANY WARRANTY*; without even the implied warranty of
*MERCHANTABILITY* or *FITNESS FOR A PARTICULAR PURPOSE*.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

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
