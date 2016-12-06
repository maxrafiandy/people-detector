TEMPLATE = app

QT += qml quick widgets androidextras

CONFIG += c++11

android {
    QT += androidextras
    INCLUDEPATH += /opt/OpenCV-android-sdk/sdk/native/jni/include
    LIBS += -L/opt/OpenCV-android-sdk/sdk/native/3rdparty/libs/armeabi-v7a\
        -L/opt/OpenCV-android-sdk/sdk/native/libs/armeabi-v7a
    LIBS += -llibtiff -llibjpeg -llibjasper -llibpng \
        -lIlmImf -ltbb -lopencv_core -lopencv_androidcamera \
        -lopencv_flann -lopencv_imgproc -lopencv_highgui -lopencv_features2d \
        -lopencv_calib3d -lopencv_ml -lopencv_objdetect -lopencv_video \
        -lopencv_contrib -lopencv_photo -lopencv_java -lopencv_legacy \
        -lopencv_ocl -lopencv_stitching -lopencv_superres -lopencv_ts \
        -lopencv_videostab
}

SOURCES += main.cpp \
    MainQml.cpp \
    MobileClient.cpp \
    StreamProvider.cpp \
    Worker.cpp \
    NotificationClient.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    lib/libopencv_info.so \
    lib/libopencv_java.so \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/gradle.properties \
    android/local.properties \
    android/src/org/gnu/linux/MyActivity.java \
    android/src/org/gnu/linux/MyService.java \
    android/src/org/gnu/linux/MyRunnable.java \
    android/res/drawable-ldpi/warning.png

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/lib/libopencv_info.so \
        $$PWD/lib/libopencv_java.so
}

HEADERS += \
    MainQml.h \
    MobileClient.h \
    StreamProvider.h \
    ExternedVars.h \
    Worker.h \
    NotificationClient.h
