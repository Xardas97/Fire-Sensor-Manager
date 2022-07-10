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

    Q_PROPERTY(Alarm alarm  READ alarm  WRITE setAlarm  NOTIFY alarmChanged)
    Q_PROPERTY(float volume READ volume WRITE setVolume NOTIFY volumeChanged)
public:
    AlarmManager(std::shared_ptr<Database> database, QString activeUserProfile);
    ~AlarmManager();

public:
    void setActiveUserProfile(QString activeUserProfile);

    Alarm alarm();
    void  setAlarm(Alarm alarm);
    float volume();
    void  setVolume(float volume);

public slots:
    void play();
    void play(Alarm alarm, int loops);
    void stop();

signals:
    void alarmChanged();
    void volumeChanged();

private:
    void repickAlarm(Alarm alarm);
    void remakeOutput(float volume);

    QString getAlarmPath(Alarm alarm);
    QString getAlarmName(Alarm alarm);

    void loadUserProfile();
    void saveUserProfile();

    QString m_activeUserProfile;
    float m_chosenVolume;
    Alarm m_chosenAlarm;

    std::unique_ptr<QBuffer> m_alarm;
    std::unique_ptr<QAudioOutput> m_output;
    std::unique_ptr<QMediaPlayer> m_player;

    std::shared_ptr<Database> m_database;
};

#endif // ALARMMANAGER_H
