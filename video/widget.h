#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QTimer>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "search.h"
#include <QCameraInfo>
#include <QCameraViewfinder>
#include "camerathread.h"
#include <QLabel>
#include<QCameraImageCapture>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_open_clicked();

    void updateFrame();

    void onCameraSelected(const QCameraInfo &cameraInfo);

    void on_open_3_clicked();

    // void onComboBoxIndexChanged(int index);

    void imageCaptured(int id, const QImage& preview);

    cv::Mat QImageToMat(const QImage &image);

private:
    void startCamera(const QCameraInfo &cameraInfo);

    Ui::Widget *ui;

    QList<QCameraInfo> cameras;
    QList<QLabel*> labels;
    QList<QCamera *> cameraList;
    QList<QCameraViewfinder *> viewfinderList;
    QList<QCameraImageCapture *> imageCaptureList;
    QList< QImage*> frameList;


    cv::VideoCapture capture1;
    cv::VideoCapture capture2;
    cv::Mat frame;
    QTimer *timer;
    bool cameraOpened;

    QCameraViewfinder *viewfinder;
    QCamera *camera;
};
#endif // WIDGET_H
