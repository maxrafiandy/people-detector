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

/* only included when use kinect
 * camera as default capture device */
#include "../include/open_kinect.h"
/* Standard C/C++ headers */
#include <iostream>
#include <unistd.h>
/* libboost headers */
#include <boost/array.hpp>
#include <boost/asio.hpp>
/* OpenCv headers
 * actually, these opencv headers are
 * already included in open_kinect.h.
 * these header files could be commented
 * when use kinect as default devices */
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core/utility.hpp>

using boost::asio::ip::tcp;
//using boost::asio::ip::udp;
using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << "usage: camera <server-ip>\n";
    }
    char *server_ip = argv[1];

    /* init kinect camera
     * uncomment these 2 lines
     * to use a kinect camera */
    Freenect::Freenect freenect;
    open_kinect& device = freenect.createDevice<open_kinect>(0);

    /* init webcam
     * uncomment these 2 lines
     * to use a web-camera */
    //cv::VideoCapture cap;
    //cap.open(0);

    Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));
    //namedWindow("Livestream",CV_WINDOW_AUTOSIZE);

    /* start kinect to capture images
     * only used when capturing from
     * kinect camera */
    device.startVideo();

    for(;;)
    {
        int c = waitKey(500);
        // ESC button = 27
        if( c == 27 || c == 'q' || c == 'Q' )
        {
            //cvDestroyWindow("Livestream");
            break;
        }

        /* capture from kinect. ucomment this line
         * while capturing from kinect camera */
        device.getVideo(rgbMat);

        /* capture default webcam. ucomment this line
         * while capturing from default webcam */
        //cap >> rgbMat;

        if(rgbMat.empty())
        {
            break;
        }

        //cv::imshow("Livestream", rgbMat);
        cv::resize(rgbMat,rgbMat,cv::Size(320,240),INTER_CUBIC);
        rgbMat = (rgbMat.reshape(0,1)); // to make it continuous

        try
        {
            /* 230400 = 320*240*3, it means
             * dimention of Mat multiply by
             * number of channel */
            std::string message((char *)rgbMat.data,230400);
            cout<<"sending...."<<endl;

            boost::asio::io_service io_service;
            boost::system::error_code ignored_error;

            /* this section is for TCP/IP protocol */
            tcp::endpoint end_point(boost::asio::ip::address::from_string(server_ip), 3200);
            tcp::socket socket(io_service);
            socket.connect(end_point);

            /* use socket.write_some() instead if
             * if you dont need performance otherwise
             * use boost::asio::write */
            //socket.write_some(boost::asio::buffer(message), ignored_error);
            boost::asio::write(socket,boost::asio::buffer(message));//,230400));
            /* end of TCP/IP protocol section */

            /* this section is for UDP protocol */
            //udp::resolver resolver(io_service);
            //udp::resolver::query query(udp::v4(), server_ip, "people_detector");
            //udp::endpoint receiver_endpoint = *resolver.resolve(query);

            //udp::socket socket(io_service);
            //socket.open(udp::v4());
            //socket.send_to(boost::asio::buffer(message), receiver_endpoint);
            /* end of TCP/IP protocol section */
            cout<<"send image finished"<<endl;
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
        //usleep(100000);
    }
    /* stop kinect from capturing an image
     * only used when capturing from kinect camera */
    device.stopVideo();
    return 0;
}
