// camerathread.cpp
#include "camerathread.h"

CameraThread::CameraThread(int cameraIndex, QObject *parent)
    : QThread(parent), cameraIndex(cameraIndex), stopThread(false)
{
}

CameraThread::~CameraThread()
{
    stopThread = true;
    wait();  // 等待线程结束
}

void CameraThread::run()
{
    capture.open(cameraIndex);
    if (!capture.isOpened()) {
        emit frameAvailable(QImage());  // 发送空图像信号表示摄像头打开失败
        return;
    }

    while (!stopThread) {
        cv::Mat frame;
        capture >> frame;
        if (!frame.empty()) {
            QImage qtImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            emit frameAvailable(qtImage);
        }
    }

    capture.release();
}
