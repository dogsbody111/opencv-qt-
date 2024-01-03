#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    //获取可用摄像头设备列表
    cameras = QCameraInfo::availableCameras();

    for (int i = 0; i < cameras.size(); ++i) {
        qDebug() << "可用摄像头个数："<<cameras.size() <<"name:"<< cameras.at(i).description();
        // 创建一个新的 QLabel 用于显示摄像头画面
        QLabel* label = new QLabel(this);
        label->setObjectName("cameraLabel" + QString::number(i));
         label->setFixedSize(300, 200);
         //间距
         ui->verticalLayout->setSpacing(10);
        labels.append(label);

        ui->verticalLayout->addWidget(label);

        QCamera *camera = new QCamera(cameras.at(i));
        cameraList.append(camera);

        QCameraViewfinder *viewfinder = new QCameraViewfinder(this);
        viewfinderList.append(viewfinder);

        //设置 摄像头画面的显示位置
        // 将 QCameraViewfinder 添加到垂直布局中
        ui->verticalLayout->addWidget(viewfinder);

        // 设置 QCameraViewfinder 为 QLabel 的子部件
        label->setParent(viewfinder);

        if (labels.size() != viewfinderList.size()) {
            qDebug() << " labels and viewfinderList sizes 不相同!";
            return;  // 或者进行其他处理，例如抛出异常
        }
        //ui->verticalLayout->addWidget(viewfinder);

        // 设置摄像头的视图为 QCameraViewfinder
        camera->setViewfinder(viewfinder);


        //捕获摄像头图像
        QCameraImageCapture *imageCapture = new QCameraImageCapture(camera);
        imageCaptureList.append(imageCapture);

        connect(imageCapture, &QCameraImageCapture::imageCaptured,
                this, &Widget::imageCaptured);

    }

    // 初始化摄像头打开状态
    cameraOpened = false;

    // 创建定时器，用于定期更新视频帧
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
            this, &Widget::updateFrame);

    // 显示可用摄像头在下拉框中
    for (const QCameraInfo &cameraInfo : cameras) {
        ui->comboBox->addItem(cameraInfo.description());
    }
}

Widget::~Widget()
{
    delete ui;
}

//数据更新
void Widget:: updateFrame() {
    if (cameraOpened) {
        qDebug() << "cameraList size: " << cameraList.size();
        qDebug() << "viewfinderList size: " << viewfinderList.size();
        qDebug() << "labels size: " << labels.size();

        for (int i = 0; i < cameraList.size(); ++i) {
            qDebug() << "Processing camera #" << i;
            if (i < viewfinderList.size() && i < labels.size()) {
                QCameraViewfinder *viewfinder = viewfinderList[i];
                if ( viewfinder&&viewfinder->isVisible())
                {
                    QImage frame = viewfinder->grab().toImage();
                    if (!frame.isNull()) {
                        cv::Mat matFrame = QImageToMat(frame);
                        if (matFrame.empty()) {
                            qWarning() << "cv::Mat is empty!";
                            return;
                        }
                        // 转换颜色通道顺序（BGR 到 RGB）
                        cv::cvtColor(matFrame, matFrame, cv::COLOR_BGR2RGB);
                        QImage qtImage(matFrame.data, matFrame.cols, matFrame.rows, matFrame.step, QImage::Format_RGB888);

                        // 将 QImage 转换为 QPixmap
                        QPixmap pixmap = QPixmap::fromImage(qtImage);

                        // 设置 QCameraViewfinder 的大小为 label 的大小
                        viewfinder->setFixedSize(labels[i]->size());
                        if (labels[i]) {
                            // 在对应的标签上显示图像
                            labels[i]->setPixmap(pixmap.scaled(labels[i]->size(), Qt::KeepAspectRatio));
                        } else {
                            qDebug() << "Label没有";
                        }
                    }
                }
                else {
                    qDebug() << "Viewfinder is not visible.";
                }
            } else {
                qDebug() << "Invalid index for viewfinder or labels.";
            }
        }
    }
}

//打开摄像头
void Widget::on_open_clicked()
{

    if (!cameraOpened) {
        int selectedCameraIndex=0;
        // 获取选择的摄像头索引
        //int selectedCameraIndex = cameraList.size();
        // 检查索引是否有效
        //&& selectedCameraIndex < cameras.size()
        for( selectedCameraIndex = 0 ;selectedCameraIndex<(cameraList.size());++selectedCameraIndex) {

            cameraList[selectedCameraIndex]->setViewfinder(viewfinderList[selectedCameraIndex]);
            cameraList[selectedCameraIndex]->start();
            imageCaptureList[selectedCameraIndex]->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
            cameraList[selectedCameraIndex]->setCaptureMode(QCamera::CaptureStillImage);
            imageCaptureList[selectedCameraIndex]->capture();

            // 连接图像捕获信号
            connect(imageCaptureList[selectedCameraIndex],SIGNAL(imageCaptured(int,QImage)),
                    this,SLOT(imageCaptured(int,QImage)));
            // 开始捕获
            imageCaptureList[selectedCameraIndex]->capture();

            cameraOpened = true;
            ui->open->setText("关闭摄像头");
            timer->start(30); // 每30毫秒更新一帧
        }
        //qDebug() << "无效的摄像头索引";
    } else {
        // 关闭摄像头
        for (int i = 0; i < cameraList.size(); ++i) {

            cameraList[i]->stop();
            imageCaptureList[i]->deleteLater();  // 使用 deleteLater() 而不是 delete
            cameraList[i]->deleteLater();  // 使用 deleteLater() 而不是 delete

            cameraList[i]->stop();
            disconnect(imageCaptureList[i],SIGNAL(imageCaptured(int,QImage)),
                       this,SLOT(imageCaptured(int,QImage)));
            // 清理摄像头对象
            delete cameraList[i];
            cameraList[i] = nullptr;

            // 清理图像捕获对象
            delete imageCaptureList[i];
            imageCaptureList[i] = nullptr;

            // 清理 QCameraViewfinder 对象
            // 如果 labels[i] 是 QCameraViewfinder 的子部件，也需要删除
            if (labels[i]) {
                labels[i]->parent()->deleteLater();  // Assuming QCameraViewfinder is the parent
            }
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

void Widget::onCameraSelected(const QCameraInfo &cameraInfo) {
    if (cameraOpened) {
        on_open_clicked();
    }

    startCamera(cameraInfo);
}
//搜索对话框的点击事件
void Widget::on_open_3_clicked()
{
    CameraSearchDialog *dialog = new CameraSearchDialog(this);
    connect(dialog, &CameraSearchDialog::cameraSelected,
            this, &Widget::onCameraSelected);
    // dialog->exec();
}
//启动摄像头
void Widget::startCamera(const QCameraInfo &cameraInfo)
{
    camera = new QCamera(cameraInfo);
    camera->setViewfinder(viewfinder);

    camera->start();
    if (camera) {
        camera->stop();
        camera->deleteLater();  // 使用 deleteLater() 而不是 delete
    }
    if (!timer->isActive()) {
        timer->start(30);
    }
}
void Widget::imageCaptured(int id, const QImage& preview)
{
    // 处理捕获的图像，id 是摄像头的索引
    cv::Mat frame = QImageToMat(preview);

    if (!frame.empty()) {
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        QImage qtImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

        // 在对应的标签上显示图像
        labels[id]->setPixmap(QPixmap::fromImage(qtImage).scaled(labels[id]->size(), Qt::KeepAspectRatio));
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
