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

#include "Worker.h"
#include "MobileClient.h"
#include <QMetaObject>
Worker::Worker(QObject *parent):
    QObject(parent)
{

}

Worker::~Worker()
{

}

void Worker::process()
{
    for(;;)
    {
        if(!is_connected)
        {
            break;
        }
        MobileClient *mc = new MobileClient();
        mc->set_host(SERVER_ADDRESS);
        mc->set_port(SERVER_PORT);
        bool connected = mc->open_connection();
        usleep(800000);
        if (connected)
        {
            qDebug() << "receive " << mc->get_received_bytes() << " bytes";
            mc->get_image();
            QVariant src = status_message;
            QMetaObject::invokeMethod(qml_root, "reload_image",Q_ARG(QVariant, src));
            mc->close_connection();
        }
        free(mc);
    }
    emit finished();
    status_message = "Stream stopped!";
}
