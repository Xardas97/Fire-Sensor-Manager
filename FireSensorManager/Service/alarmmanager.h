#ifndef ALARMMANAGER_H
#define ALARMMANAGER_H

#include <memory>
#include <QObject>
#include <QBuffer>
#include <QMediaPlayer>
#include <QAudioOutput>

class AlarmManager : public QObject
{
    Q_OBJECT
public:
    AlarmManager();

public slots:
    void play();
    void stop();

private:
    void repickAlarm();
    void remakeOutput();

    std::unique_ptr<QBuffer> m_alarm;
    std::unique_ptr<QAudioOutput> m_output;
    std::unique_ptr<QMediaPlayer> m_player;
};

#endif // ALARMMANAGER_H
