// search.h
#ifndef SEARCH_H
#define SEARCH_H
#include <QDialog>
#include <QCameraInfo>
#include <QPushButton>
#include <QVBoxLayout>

class CameraSearchDialog : public QDialog {
    Q_OBJECT

public:
    CameraSearchDialog(QWidget *parent = nullptr);

signals:
    void cameraSelected(const QCameraInfo &cameraInfo);

private:
    void setupUI();

private slots:
    void onCameraButtonClicked(const QCameraInfo &cameraInfo);

private:
    QVBoxLayout *layout;
};

#endif // SEARCH_H
