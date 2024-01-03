#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QThread>
#include <QImage>
#include <opencv2/opencv.hpp>

class CameraThread : public QThread {
    Q_OBJECT

public:
    explicit CameraThread(int cameraIndex, QObject *parent = nullptr);
    ~CameraThread();

signals:
    void frameAvailable(const QImage &frame);

protected:
    void run() override;

private:
    int cameraIndex;
    cv::VideoCapture capture;
    bool stopThread;
};

#endif // CAMERATHREAD_H
