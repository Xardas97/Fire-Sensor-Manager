#ifndef FIRESENSORDETECTOR_H
#define FIRESENSORDETECTOR_H

#include <QObject>

class FireSensorDetector : public QObject
{
    Q_OBJECT
public:
    explicit FireSensorDetector(QObject *parent = nullptr);

public:
    void discoverSensors();
};

#endif // FIRESENSORDETECTOR_H
