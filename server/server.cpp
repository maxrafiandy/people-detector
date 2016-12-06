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

/* standard C/C++ headers */
#include <stdlib.h>
#include <unistd.h>
/* libboost headers */
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/thread/thread.hpp>
/* OpenCV headers */
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>

using boost::asio::ip::tcp;
//using boost::asio::ip::udp;
using namespace  std;
using namespace cv;

Mat img = Mat::zeros(320,240, CV_8UC3);
HOGDescriptor hog;
string cascadeName;
string nestedCascadeName;

boost::thread *thrd_report;
boost::thread *thrd_stream;

static void create_stream_thread();
static void create_report_thread();
static int found_size = 0;
static int face_size = 0;

static void peopleDetectAndDraw(const HOGDescriptor &hog, Mat &img)
{
    found_size = 0;
    vector<Rect> found, found_filtered;
    double t = (double) getTickCount();

    hog.detectMultiScale(img, found, 0.1, Size(4,4), Size(8,8), 1.05, 2);
    t = (double) getTickCount() - t;

    /*for(size_t i = 0; i < found.size(); i++ )
    {
        Rect r = found[i];

        size_t j;
        // remove all small detections inside a bigger detection.
        for ( j = 0; j < found.size(); j++ )
            if ( j != i && (r & found[j]) == r )
                break;

        if ( j == found.size() )
            found_filtered.push_back(r);
    }*/
    found_size = found.size();
    for (size_t i = 0; i < found.size(); i++)
    {
        Rect r = found[i];
        rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 3);

    }
}


static void faceDetectAndDraw( Mat& img, CascadeClassifier& cascade,
	double scale, bool tryflip )
{
    double t = 0;
    face_size = 0;
    vector<Rect> faces, faces2;
    const static Scalar colors[] =
    {
        Scalar(255,0,0),
        Scalar(255,128,0),
        Scalar(255,255,0),
        Scalar(0,255,0),
        Scalar(0,128,255),
        Scalar(0,255,255),
        Scalar(0,0,255),
        Scalar(255,0,255)
    };
    Mat gray, smallImg;

    cvtColor( img, gray, COLOR_BGR2GRAY );
    double fx = 1 / scale;
    resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );

    t = (double)getTickCount();
    cascade.detectMultiScale( smallImg, faces,
                              1.1, 2, 0
                              //|CASCADE_FIND_BIGGEST_OBJECT
                              //|CASCADE_DO_ROUGH_SEARCH
                              |CASCADE_SCALE_IMAGE,
                              Size(30, 30) );
    if( tryflip )
    {
        flip(smallImg, smallImg, 1);
        cascade.detectMultiScale( smallImg, faces2,
                                  1.1, 2, 0
                                  //|CASCADE_FIND_BIGGEST_OBJECT
                                  //|CASCADE_DO_ROUGH_SEARCH
                                  |CASCADE_SCALE_IMAGE,
                                  Size(30, 30) );
        for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); ++r )
        {
            faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
        }
    }

    t = (double)getTickCount() - t;
    printf( "detection time = %g ms\n", t*1000/getTickFrequency());
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Rect r = faces[i];
        Scalar color = colors[i%8];
        rectangle(img, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
                  cvPoint(cvRound((r.x + r.width-1)*scale), cvRound((r.y + r.height-1)*scale)),
                  color, 3, 8, 0);
    }
    face_size = faces.size();
}


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

static void create_stream_thread()
{
    for (;;)
    {
        try
        {
            boost::asio::io_service io_service;
            tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 3211));

            tcp::socket socket(io_service);
            acceptor.accept(socket);
            cerr << "Wait for stream ..." << endl;

            boost::system::error_code ignored_error;
            /* 230400 = 320*240*3, it means
             * dimention of Mat multiply by
             * number of channel */

            std::string message = std::string((char *)img.data,230400);
            cout<<"sending...."<<endl;

            /* use socket.write_some() instead if
             * if you dont need performance otherwise
             * use boost::asio::write */
            //socket.write_some(boost::asio::buffer(message), ignored_error);
            boost::asio::write(socket,boost::asio::buffer(message),ignored_error);
            cout<<"send image finished"<<endl;
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
        waitKey(1000);
    }
}

static void create_report_thread()
{
    for(;;)
    {
        usleep(1000);
        try
        {
            boost::asio::io_service io_service;
            tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 3212));
            usleep(1000);
            tcp::socket socket(io_service);
            acceptor.accept(socket);

            boost::system::error_code ignored_error;
            /* 230400 = 320*240*3, it means
             * dimention of Mat multiply by
             * number of channel */
            std::string message = "NOT_DETECTED";

            if (found_size != 0 || face_size != 0)
            {
                message = "DETECTED";
            }

            cout<<"sending report...."<<endl;

            /* use socket.write_some() instead if
             * if you dont need performance otherwise
             * use boost::asio::write */
            socket.write_some(boost::asio::buffer(message), ignored_error);
            //boost::asio::write(socket,boost::asio::buffer(message));//,230400));
            cout<<"report sent"<<endl;
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}


int main()
{
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    bool tryflip = true;
    CascadeClassifier cascade, nestedCascade;
    double scale;

    cascadeName = "../share/trained_cascade.xml";
    scale = 1;

    if( !cascade.load( cascadeName ) )
    {
        cerr << "Warning[1]: Could not load ../share/trained_cascade.xml" << endl;
        cascadeName = "share/trained_cascade.xml";
        if(!cascade.load( cascadeName ) )
        {
            cerr << "Warning[2]: Could not load share/trained_cascade.xml" << endl;
            cascadeName = "/usr/local/share/trained_cascade.xml";
            if(!cascade.load( cascadeName ) )
            {
                cerr << "ERROR: Could not load any classifier cascade" << endl;
                return -1;
            }
            cerr << "Using /usr/local/share/trained_cascade.xml (Warning[1,2] Fixed)" << endl;
        }
        else
        {
            cerr << "Using share/trained_cascade.xml (Warning[1] Fixed)" << endl;
        }
    }

    /* create server window */
    //namedWindow("Server",CV_WINDOW_AUTOSIZE);
    thrd_stream = new boost::thread(&create_stream_thread);
    thrd_report = new boost::thread(&create_report_thread);
    try
    {
        boost::asio::io_service io_service;
        /* the size of reciev mat frame is caculate by 320*240*3 */
        boost::array<char, 230400> buf;

        /* choose one: use TCP/IP or UDP */
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 3200));
        //udp::socket socket(io_service, udp::endpoint(udp::v4(), 3200));

        for (;;)
        {
            cerr << "Waiting for request . . ." << endl;
            boost::system::error_code ignore_error;
            /* this section is for UDP procotol */
            //udp::endpoint remote_endpoint;
            //size_t len = socket.receive_from(boost::asio::buffer(buf),
            //                    remote_endpoint, 0, error);

            //if (error && error != boost::asio::error::message_size)
            //    throw boost::system::system_error(error);
            /* end of UDP procotol section */

            /* this section is for TCP/IP procotol */
            tcp::socket socket(io_service);
            acceptor.accept(socket);

            /* use socket.read_some() instead if
             * if you dont need performance otherwise
             * use boost::asio::read */
            //size_t len = socket.read_some(boost::asio::buffer(buf), ignore_error);

            size_t len = boost::asio::read(socket,boost::asio::buffer(buf),ignore_error);
            /* end of TCP/IP procotol section */

            /* disp the data size recieved to ensure that
             * all packages were sent */
            cout<<"get data length :"<<len<<endl;

            /* convert the recieved mat frame to vector */
            std::vector<uchar> vectordata(buf.begin(),buf.end());
            cv::Mat data_mat(vectordata,true);

            /* reshape to 3 channel and 240 rows */
            img = data_mat.reshape(3,240);
            cout<<"reshape over"<<endl;

            /* create a thread (thrd1) to perform people-detection
             * create a thread (thrd2) to perform face-detection
             * both thrd1 & thrd2 should run simultaneously */
            boost::thread thrd1(&peopleDetectAndDraw,hog,img);
            boost::thread thrd2(&faceDetectAndDraw,img, cascade, scale, tryflip);

            /* to avoid program creating a continously child-task
             * make sure all thread are already joined before
             * next loop */
            thrd1.join();
            thrd2.join();

            /* show image in server side */
            imshow("Server",img);
            waitKey(100);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    thrd_stream->join();
    return 0;
}
