#ifndef MAPENTRY_H
#define MAPENTRY_H

#include <memory>
#include <vector>
#include <QPixmap>

class Sensor;

class MapEntry
{
public:
    MapEntry(int id, int floor, QPixmap pixmap);

    int  id() const;
    void setId(int id);
    int  floor() const;
    auto pixmap() -> QPixmap&;
    auto pixmap() const -> const QPixmap&;
    auto placedSensors() const -> const std::vector<std::shared_ptr<Sensor>>&;

    void addSensor(std::shared_ptr<Sensor> sensor);
    void removeSensor(std::shared_ptr<Sensor> sensor);
    void removeAllSensors();

private:
    int m_id;
    int m_floor;
    QPixmap m_pixmap;
    std::vector<std::shared_ptr<Sensor>> m_placedSensors;
};

#endif // MAPENTRY_H
