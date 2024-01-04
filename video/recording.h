#ifndef RECORDING_H
#define RECORDING_H

#include <QObject>
#include <QCamera>
#include <QMediaRecorder>
#include <QThread>

class Recording : public QObject
{
    Q_OBJECT
public:
    explicit Recording(QCamera* camera, QMediaRecorder* recorder, QObject *parent = nullptr);

signals:
    void recordingError(QMediaRecorder::Error error);

public slots:
    void startRecording();
    void stopRecording();

private:
    QCamera* camera;
    QMediaRecorder* recorder;
};

#endif // RECORDING_H
