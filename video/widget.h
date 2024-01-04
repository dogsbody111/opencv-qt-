#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QTimer>
#include<QTime>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "search.h"
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QLabel>
#include<QCameraImageCapture>
#include<QCameraViewfinder>
#include<QVBoxLayout>
#include<QMediaRecorder>
#include <QUrl>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    int rowCount;
    int colCount ; // 列数
    int currentRow ; // 当前行

private slots:
    void on_open_clicked();

    void updateFrame();

    //void imageCaptured(int id, const QImage& preview);

    cv::Mat QImageToMat(const QImage &image);

    void processCapturedImage(int index, const QImage &frame) ;

    void handleError(QMediaRecorder::Error error);

     void localTime();

private:
    //void startCamera(const QCameraInfo &cameraInfo);

    void setupLabels();

    void createViewfinder(int index);

    void createLabel(int index);

    void createMediaRecorder(int index);
    //void inyti();



    Ui::Widget *ui;

    QList<QCameraInfo> cameras;
    QList<QLabel*> labels;
    QList<QCamera *> cameraList;
    QList<QCameraViewfinder *> viewfinderList;
    QList<QCameraImageCapture *> imageCaptureList;
    QList<QMediaRecorder*> mediaRecorderList;


    cv::VideoCapture capture1;
    cv::VideoCapture capture2;
    cv::Mat frame;
    QTimer *timer;
    bool cameraOpened;

    QCameraViewfinder *viewfinder;
    QCamera *camera;
};
#endif // WIDGET_H
