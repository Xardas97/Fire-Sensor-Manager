#include "alarmmanager.h"

#include <QFile>
#include <QAudioOutput>

AlarmManager::AlarmManager()
    : m_player{new QMediaPlayer()}
{
    m_chosenVolume = 0.25;
    m_chosenAlarm = Alarm::Classic;
}

void AlarmManager::play()
{
    play(m_chosenAlarm, QMediaPlayer::Loops::Infinite);
}

void AlarmManager::play(Alarm alarm, int loops)
{
    if (m_player->playbackState() != QMediaPlayer::StoppedState)
        stop();

    repickAlarm(alarm);
    remakeOutput(m_chosenVolume);

    if (!m_alarm)
    {
        qDebug() << "No alarm is available";
        return;
    }

    m_player->setLoops(loops);
    m_player->setAudioOutput(m_output.get());
    m_player->setSourceDevice(m_alarm.get());

    m_player->play();
}

void AlarmManager::repickAlarm(Alarm alarm)
{
    if (m_player->playbackState() != QMediaPlayer::StoppedState)
        return;

    auto alarmPath = getAlarmPath(alarm);

    QFile file(alarmPath);
    file.open(QIODevice::ReadOnly);
    if (!file.isOpen())
    {
        qDebug() << "Failed to open the alarm file at " << alarmPath;
        return;
    }

    QByteArray arr = file.readAll();

    auto buffer = std::make_unique<QBuffer>();
    buffer->setData(arr);
    buffer->open(QIODevice::ReadOnly);
    if (!buffer->isOpen())
    {
        qDebug() << "Failed to open the alarm buffer";
        return;
    }

    m_alarm = std::move(buffer);
}

QString AlarmManager::getAlarmPath(Alarm alarm)
{
    return R"(:/Resources/Sounds/)" + getAlarmName(alarm) + ".wav";
}

QString AlarmManager::getAlarmName(Alarm alarm)
{
    switch (alarm)
    {
        case Alarm::Classic:       return "ClassicAlarm";
        case Alarm::DigitalClock:  return "DigitalClockAlarm";
        case Alarm::Buzzer:        return "BuzzerAlarm";
        case Alarm::Facility:      return "FacilityAlarm";
        case Alarm::Vintage:       return "VintageAlarm";
        default:                   return QString {};
    }
}

void AlarmManager::remakeOutput(float volume)
{
    if (m_player->playbackState() != QMediaPlayer::StoppedState)
        return;

    m_output = std::make_unique<QAudioOutput>();
    m_output->setVolume(volume);
}

void AlarmManager::stop()
{
    m_player->stop();
}

Alarm AlarmManager::alarm()
{
    return m_chosenAlarm;
}

void AlarmManager::setAlarm(Alarm alarm)
{
    m_chosenAlarm = alarm;
}

float AlarmManager::volume()
{
    return m_chosenVolume;
}

void AlarmManager::setVolume(float volume)
{
    m_chosenVolume = volume;
}
