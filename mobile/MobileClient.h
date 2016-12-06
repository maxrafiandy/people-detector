/* * *
 * Copyright (C) 2016  Max Rafiandy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * * */

#ifndef MOBILECLIENT_H
#define MOBILECLIENT_H
/* Qt headers */
#include <QDebug>
#include <QImage>
/* standard C/C++ headers */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
/* OpenCV Header */
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
/* Project headers */
#include "ExternedVars.h"

#define BUFFER_SIZE 230400

class MobileClient
{
public:
    MobileClient();
    virtual ~MobileClient();
    void set_port(int);
    void set_host(char*);
    bool open_connection();
    void close_connection();
    uchar *get_buffer();
    cv::Mat get_cv_mat();
    QImage get_image();
    uint64_t get_received_bytes();

private:
    void print_error(char*);
    int sockfd, portno=3210;
    struct sockaddr_in serv_addr;
    struct hostent *server=gethostbyname("127.0.0.1");
    uchar buffer[BUFFER_SIZE];
    cv::Mat rgb_mat;
};

#endif // MOBILECLIENT_H
