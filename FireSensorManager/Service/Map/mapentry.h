#ifndef MAPENTRY_H
#define MAPENTRY_H

#include <memory>
#include <vector>
#include <QPixmap>

class Sensor;

class MapEntry
{
public:
    MapEntry(QPixmap pixmap);

    auto pixmap() -> QPixmap&;
    auto pixmap() const -> const QPixmap&;
    auto placedSensors() const -> const std::vector<std::shared_ptr<Sensor>>&;

    void addSensor(std::shared_ptr<Sensor> sensor);
    void removeSensor(std::shared_ptr<Sensor> sensor);

private:
    QPixmap m_pixmap;
    std::vector<std::shared_ptr<Sensor>> m_placedSensors;
};

#endif // MAPENTRY_H
