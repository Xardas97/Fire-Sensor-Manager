#include "alarmmanager.h"

#include <QFile>
#include <QAudioOutput>

AlarmManager::AlarmManager()
    : m_player{new QMediaPlayer()}
{
    m_player->setLoops(QMediaPlayer::Loops::Infinite);
}

void AlarmManager::play()
{
    if (m_player->playbackState() != QMediaPlayer::StoppedState)
        stop();

    repickAlarm();
    remakeOutput();

    if (!m_alarm)
    {
        qDebug() << "No alarm is available";
        return;
    }

    m_player->setAudioOutput(m_output.get());
    m_player->setSourceDevice(m_alarm.get());

    m_player->play();
}

void AlarmManager::repickAlarm()
{
    if (m_player->playbackState() != QMediaPlayer::StoppedState)
        return;

    QFile file(R"(:/Resources/Sounds/ClassicAlarm.wav)");
    file.open(QIODevice::ReadOnly);
    if (!file.isOpen())
    {
        qDebug() << "Failed to open the alarm file";
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

void AlarmManager::remakeOutput()
{
    if (m_player->playbackState() != QMediaPlayer::StoppedState)
        return;

    m_output = std::make_unique<QAudioOutput>();
    m_output->setVolume(100);
}

void AlarmManager::stop()
{
    m_player->stop();
}
