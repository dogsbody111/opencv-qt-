// search.cpp
#include "search.h"
#include <QLabel>
CameraSearchDialog::CameraSearchDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Available Cameras");
    setupUI();
}

void CameraSearchDialog::setupUI() {
    layout = new QVBoxLayout(this);

    // 获取附近可用摄像头列表
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    if (cameras.isEmpty()) {
        layout->addWidget(new QLabel("没有可用摄像头", this));
    } else {
        for (const QCameraInfo &cameraInfo : cameras) {
            QString cameraName = cameraInfo.description();
            QPushButton *cameraButton = new QPushButton(cameraName, this);

            connect(cameraButton, &QPushButton::clicked,
                    [this, cameraInfo]() {
                emit cameraSelected(cameraInfo);
                accept();  // 关闭对话框
            });

            layout->addWidget(cameraButton);
        }
    }

    setLayout(layout);
}
//处理摄像头按钮的点击事件
void CameraSearchDialog::onCameraButtonClicked(const QCameraInfo &cameraInfo) {
    emit cameraSelected(cameraInfo);
    accept();  // 关闭对话框
}
