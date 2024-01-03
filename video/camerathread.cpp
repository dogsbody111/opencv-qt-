#include "CameraThread.h"
#include <QImage>

CameraThread::CameraThread(int cameraIndex, QObject *parent)
    : QThread(parent), cameraIndex(cameraIndex), stopThread(false) {
    // Open the camera
    capture.open(cameraIndex);
}

CameraThread::~CameraThread() {
    stopThread = true;
    wait(); // Wait for the thread to finish
    capture.release();
}

void CameraThread::run() {
    while (!stopThread) {
        cv::Mat frame;
        capture >> frame;

        if (!frame.empty()) {
            QImage qImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            qImage = qImage.rgbSwapped(); // BGR to RGB

            emit frameAvailable(qImage);
        }

        msleep(30); // Sleep for 30 milliseconds to control frame rate
    }
}
