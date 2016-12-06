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

#include <QQmlComponent>
#include <QQmlEngine>
#include <QQmlContext>
#include <QGuiApplication>
#include <QQuickImageProvider>
#include <QThread>
#include <QImage>
#include <QAndroidJniObject>
#include <QtAndroid>
#include "MainQml.h"
#include "StreamProvider.h"
#include "Worker.h"
#include "MobileClient.h"

QImage q_img;
QThread *thread_stream, thread_img;
QObject *qml_root;
QQuickImageProvider *img_prov;
QScopedPointer<MainQml> main_qml(new MainQml);
Worker *worker;
char *SERVER_ADDRESS = "192.168.100.76";
int SERVER_PORT = 3211;
bool is_connected = false;
char *status_message;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlEngine qml_engine;

    QtAndroid::androidActivity().callMethod<void>("startService", "()V");

    thread_stream = new QThread;
    worker = new Worker;
    worker->moveToThread(thread_stream);
    QObject::connect(thread_stream, SIGNAL (started()), worker, SLOT (process()));
    QObject::connect(worker, SIGNAL (finished()), thread_stream, SLOT (quit()));
    img_prov = new StreamProvider();
    qml_engine.addImageProvider(QLatin1String("stream"), img_prov);
    qml_engine.rootContext()->setContextProperty("main_qml", main_qml.data());

    QQmlComponent component(&qml_engine, QUrl(QStringLiteral("qrc:/main.qml")));
    qml_root = component.create();

    return app.exec();
}
