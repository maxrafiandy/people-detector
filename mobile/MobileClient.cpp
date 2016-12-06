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

#include "MobileClient.h"

MobileClient::MobileClient()
{

}

MobileClient::~MobileClient()
{

}

void MobileClient::set_port(int port)
{
    portno = port;
}

void MobileClient::set_host(char *host)
{
    server = gethostbyname(host);
}

bool MobileClient::open_connection()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        print_error("Open socket: ");
        return false;
    }

    if (server == NULL)
    {
        print_error("No such host: ");
        return false;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        print_error("Connection Error: ");
        return false;
    }
    status_message = "Connected";
    return true;
}

void MobileClient::close_connection()
{
    status_message = "Connection closed";
    close(sockfd);
}

uchar *MobileClient::get_buffer()
{
    return &buffer[0];
}

cv::Mat MobileClient::get_cv_mat()
{
    uchar *buf = &buffer[0];
    std::vector<uchar> vectordata(buf, buf + BUFFER_SIZE);
    cv::Mat data_mat(vectordata,true);
    return data_mat.reshape(3,240);;
}

QImage MobileClient::get_image()
{
    cv::Mat tmp,img = get_cv_mat();
    cv::cvtColor(img, tmp, CV_BGR2RGB);
    assert(tmp.isContinuous());
    q_img = QImage(tmp.data, tmp.cols, tmp.rows, static_cast<int>(tmp.step), QImage::Format_RGB888).rgbSwapped();
    return q_img;
}


/* set port and server host before call this method */
uint64_t MobileClient::get_received_bytes()
{
    uint64_t nbytes = read(sockfd, buffer, BUFFER_SIZE);
    if (nbytes < 0)
    {
        print_error("ERROR reading from socket");
    }
    status_message = "Retrieving image from server";
    return nbytes;
}

void MobileClient::print_error(char *msg)
{
    status_message = msg;
    perror(msg);
}
