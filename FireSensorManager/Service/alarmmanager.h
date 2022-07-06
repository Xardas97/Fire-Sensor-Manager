#ifndef ALARMMANAGER_H
#define ALARMMANAGER_H

#include <memory>
#include <QObject>
#include <QBuffer>
#include <QMediaPlayer>
#include <QAudioOutput>

class Database;

enum Alarm { Classic, DigitalClock, Buzzer, Facility, Vintage };

class AlarmManager : public QObject
{
    Q_OBJECT
public:
    AlarmManager(std::shared_ptr<Database> database);
    ~AlarmManager();

public slots:
    void play();
    void play(Alarm alarm, int loops);
    void stop();

    Alarm alarm();
    void  setAlarm(Alarm alarm);
    float volume();
    void  setVolume(float volume);

private:
    void repickAlarm(Alarm alarm);
    void remakeOutput(float volume);

    QString getAlarmPath(Alarm alarm);
    QString getAlarmName(Alarm alarm);

    float m_chosenVolume;
    Alarm m_chosenAlarm;

    std::unique_ptr<QBuffer> m_alarm;
    std::unique_ptr<QAudioOutput> m_output;
    std::unique_ptr<QMediaPlayer> m_player;

    std::shared_ptr<Database> m_database;
};

#endif // ALARMMANAGER_H
