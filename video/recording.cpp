#include "recording.h"
#include <QUrl>

Recording::Recording(QCamera *camera, QMediaRecorder *recorder, QObject *parent)
    : QObject(parent),
      camera(camera),
      recorder(recorder)
{
}

void Recording::startRecording()
{
    QString fileName = "filename.mp4";  // 修改为你的文件名
    recorder->setOutputLocation(QUrl::fromLocalFile("D:/qt5.12.9/protect/video/luzhi/" + fileName));
    recorder->record();
}

void Recording::stopRecording()
{
    recorder->stop();
}
