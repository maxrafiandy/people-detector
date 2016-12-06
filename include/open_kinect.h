/*******************************************************************
Copyright (C) 2016  Max Rafiandy

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

#ifndef _OPENKINECT_H_INCLUDED
#define _OPENKINECT_H_INCLUDED

#include "libfreenect.hpp"
#include "mutex.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core/utility.hpp>

using namespace cv;
using namespace std;

class open_kinect : public Freenect::FreenectDevice
{
public:
    open_kinect(freenect_context *_ctx, int _index);
    // Do not call directly even in child
    virtual void VideoCallback(void* _rgb, uint32_t timestamp);
    // Do not call directly even in child
    virtual void DepthCallback(void* _depth, uint32_t timestamp);
    bool getVideo(Mat& output);
    bool getDepth(Mat& output);

private:
    vector<uint8_t> m_buffer_depth;
    vector<uint8_t> m_buffer_rgb;
    vector<uint16_t> m_gamma;
    Mat depthMat;
    Mat rgbMat;
    Mat ownMat;
    mutex m_rgb_mutex;
    mutex m_depth_mutex;
    bool m_new_rgb_frame;
    bool m_new_depth_frame;
};

#endif //_OPENKINECT_H_INCLUDED
