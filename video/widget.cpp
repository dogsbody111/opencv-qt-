#include "widget.h"
#include "ui_widget.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 初始化摄像头打开状态

    timer = new QTimer(this);
    cameraOpened = false;
    // 获取可用摄像头设备列表
    cameras = QCameraInfo::availableCameras();
    connect(timer, &QTimer::timeout,
            this, &Widget::localTime);
    // 创建定时器，用于定期更新视频帧
    connect(timer, &QTimer::timeout,
            this, &Widget::updateFrame);
    timer->start(1000);
    setupLabels();
    // 显示可用摄像头在下拉框中
    for (const QCameraInfo &cameraInfo : cameras) {
        ui->comboBox->addItem(cameraInfo.description());
    }

}

Widget::~Widget()
{
    delete ui;
}
//创建label和viewfinder和camera
void Widget::setupLabels() {
    colCount = 2; // 列数
    currentRow = 0; // 当前行
    for (int i = 0; i < cameras.size(); i++) {
        createLabel(i);
    }
    for (int i = 0; i < cameras.size(); i++) {
        createViewfinder(i);
        createMediaRecorder(i);
    }
    if (cameras.size() % colCount != 0) {
        currentRow++;//行
    }
    // 确保更新布局
    ui->gridLayout->update();
}
void Widget::createMediaRecorder(int index)
{
    QMediaRecorder *mediaRecorder = new QMediaRecorder(cameraList[index]);
    mediaRecorderList.append(mediaRecorder);

    QString fileName = "recording_" + QString::number(index) + ".avi";
    mediaRecorder->setOutputLocation(
                QUrl("file:///D:/qt5.12.9/protect/video/luzhi/filename.mp4"));

    connect(mediaRecorder, SIGNAL(error(QMediaRecorder::Error)),
            this, SLOT(handleError(QMediaRecorder::Error)));


}
// handleError 槽函数的实现
void Widget::handleError(QMediaRecorder::Error error)
{
    qWarning() << "录制遇到出错 " << error;
}
void Widget::createLabel(int index) {
    QLabel* label = new QLabel(this);
    label->setObjectName("cameraLabel" + QString::number(index));
    label->setFixedSize(200, 200);
    label->setStyleSheet("border: 2px solid black; background-color: transparent;");
    //在ui里放置label
    ui->gridLayout->addWidget(label, currentRow, index % colCount, 1, 1, Qt::AlignTop);
    labels.append(label);

    if ((index + 1) % colCount == 0) {
        currentRow++;
    }
}


void Widget::createViewfinder(int index) {
    QLabel* label = labels[index];// 获取相应的标签
    QCameraViewfinder *viewfinder = new QCameraViewfinder(label);
    viewfinderList.append(viewfinder);

    //动态分配内存防止越界，越界返回std::out_of_range 异常。
    QCamera *camera = new QCamera(cameras.at(index));
    cameraList.append(camera);

    // 将 QCameraViewfinder 显示在 QLabel 上
    QVBoxLayout *layout = new QVBoxLayout(label);

    layout->addWidget(viewfinder);
    layout->setContentsMargins(0, 0, 0, 0);

    // 设置viewfinder的大小策略为Expanding
    viewfinder->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //viewfinder->setFixedSize(label->size());

    camera->setViewfinder(viewfinder);

    QCameraImageCapture *imageCapture = new QCameraImageCapture(camera);
    imageCaptureList.append(imageCapture);

    if ((index + 1) % colCount == 0) {
        currentRow++;
    }
}
// 数据更新
void Widget::updateFrame() {
    if (cameraOpened) {
        for (int i = 0; i < cameraList.size(); ++i) {
            if (i < viewfinderList.size() && i < cameras.size()) {
                QCameraViewfinder *viewfinder = viewfinderList[i];
                QLabel *label = labels[i];
                if (viewfinder && viewfinder->isVisible()) {
                    // 使用信号槽连接，而不是定时器轮询
                    QCameraImageCapture *imageCapture = imageCaptureList[i];
                    connect(imageCapture, &QCameraImageCapture::imageCaptured,
                            this, [this, i](int, const QImage &frame) {
                        processCapturedImage(i, frame);
                    });
                    // 将 QCameraViewfinder 的图像显示到 QLabel 上
                    qDebug() << "Label size: " << label->size();
                    // 将 QImage 转换为 QPixmap
                    QPixmap pixmap(viewfinder->grab());
                    pixmap = pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::FastTransformation);
                    // 在对应的标签上显示图像
                    label->setPixmap(pixmap);
                } else {
                    qDebug() << "Viewfinder is not visible.";
                }
            } else {
                qDebug() << "Invalid index for viewfinder or labels.";
            }
        }
    }
}
void Widget::processCapturedImage(int index, const QImage &frame) {
    if (!frame.isNull()) {
        cv::Mat matFrame = QImageToMat(frame);
        if (matFrame.empty()) {
            qWarning() << "cv::Mat 是空!";
            return;
        }
        // 转换颜色通道顺序（BGR 到 RGB）
        cv::cvtColor(matFrame, matFrame, cv::COLOR_BGR2RGB);

        QImage qtImage(matFrame.data, matFrame.cols, matFrame.rows, matFrame.step, QImage::Format_RGB888);

        // 将 QImage 转换为 QPixmap
        QPixmap pixmap = QPixmap::fromImage(qtImage).scaled(labels[index]->size(), Qt::KeepAspectRatio, Qt::FastTransformation);;
        // 在对应的标签上显示图像
        labels[index]->setPixmap(pixmap);
    }
}
cv::Mat Widget::QImageToMat(const QImage &image)
{
    if (image.isNull()) {
        qWarning() << "QImage is null!";
        return cv::Mat();
    }
    QImage img = image.convertToFormat(QImage::Format_RGB32);
    switch (img.format()) {
    case QImage::Format_RGB32:
        // 处理 RGB32 格式的图像
        return cv::Mat(img.height(), img.width(), CV_8UC4, const_cast<uchar*>(img.bits()), img.bytesPerLine()).clone();
        break;
        // 添加其他格式的支持...
    default:
        qWarning() << "QImageTomat们没有支持格式";
        return cv::Mat();
    }
}
//打开摄像头
void Widget::on_open_clicked()
{
    if (!cameras.isEmpty()) {
        for (int i = 0; i < cameraList.size(); ++i) {
            // 启动摄像头
            cameraList[i]->start();
        }
        ui->open->setText("关闭摄像头");
        for (int i = 0; i < mediaRecorderList.size(); ++i) {
            mediaRecorderList[i]->record();
        }

        timer->start(30); // 每30毫秒更新一帧
    }
    else {
        // 关闭摄像头
        for (int i = 0; i < mediaRecorderList.size(); ++i) {
            mediaRecorderList[i]->stop();
        }
        for (int i = 0; i < cameraList.size(); ++i) {
            cameraList[i]->stop();
            imageCaptureList[i]->deleteLater();
            cameraList[i]->deleteLater();
            disconnect(imageCaptureList[i], SIGNAL(imageCaptured(int, QImage)),
                       this, SLOT(imageCaptured(int, QImage)));
        }
        timer->stop();
        // 清理列表
        cameras.clear();
        cameraList.clear();
        viewfinderList.clear();
        imageCaptureList.clear();
        cameraOpened = false;
        ui->open->setText("打开摄像头");
    }
}
void Widget::localTime()
{
    QString tm = QTime::currentTime().toString("hh:mm:ss");
    ui->labell->setText(tm);
    ui->labell->setStyleSheet("color:red;\
                              font-size:26px;\
            background-color: rgb(85, 255, 255);\
    border-radius: 15px;");
}
