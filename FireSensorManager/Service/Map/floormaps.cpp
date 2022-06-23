#include "floormaps.h"

#include "../database.h"
#include "../Communication/sensor.h"

#include <QUrl>
#include <QFile>
#include <vector>
#include <QDebug>
#include <QTextStream>
#include <QStandardPaths>

FloorMaps::FloorMaps(std::shared_ptr<Database> database)
    : m_database(database),
      m_maps(m_database->maps())
{ }

std::shared_ptr<MapEntry> FloorMaps::mapEntry(int floor, short floorPart)
{
    auto floorMapsIterator = m_maps.find(floor);
    if (floorMapsIterator == m_maps.end())
        return nullptr;

    auto floorMaps = &floorMapsIterator->second;
    if (floorMaps->size() <= (unsigned long long)floorPart)
        return nullptr;

    return (*floorMaps)[floorPart];
}

bool FloorMaps::upload(int floor, const QUrl& url)
{
    qDebug() << "Uploading map " << url.toLocalFile() << " to floor " << floor;

    QImage image;
    auto loaded = image.load(url.toLocalFile());
    if (!loaded)
    {
        qWarning() << "Failed to load image from the file";
        return false;
    }

    add(std::make_shared<MapEntry>(-1, floor, QPixmap::fromImage(image)));

    qDebug() << "Successfully loaded the map";
    return true;
}

void FloorMaps::add(std::shared_ptr<MapEntry> map)
{
    auto floor = map->floor();

    auto floorMapsIterator = m_maps.find(floor);
    if (floorMapsIterator == m_maps.end())
    {
        m_maps[floor] = std::vector<std::shared_ptr<MapEntry>>{map};
        emit floorAdded(floor);
        return;
    }

    floorMapsIterator->second.push_back(map);
    emit floorPartAdded(floor);
}

void FloorMaps::removeFloor(int floor)
{
    auto floorMapsIterator = m_maps.find(floor);
    if (floorMapsIterator == m_maps.end())
        return;

    auto maps = floorMapsIterator->second;
    for (auto& map: maps)
    {
        map->removeAllSensors();
    }

    m_maps.erase(floorMapsIterator);
    emit floorRemoved(floor);
}

bool FloorMaps::getSensorLocation(std::shared_ptr<Sensor> sensor, int& floor, short& floorPart) const
{
    if (!sensor->map())
        return false;

    auto desiredMap = sensor->map();

    auto floorMapsIterator = m_maps.find(desiredMap->floor());
    if (floorMapsIterator == m_maps.end())
        return false;

    auto floorMaps = &floorMapsIterator->second;
    auto found = std::find_if(floorMaps->cbegin(),
                              floorMaps->cend(),
                              [&desiredMap](std::shared_ptr<MapEntry> mapEntry){ return mapEntry.get() == desiredMap; });

    if (found == floorMaps->cend())
        return false;

    floor = desiredMap->floor();
    floorPart = found - floorMaps->cbegin();

    return true;
}

std::set<int> FloorMaps::availableFloors() const
{
    std::set<int> availableFloors;

    for (auto& floor: m_maps)
    {
        availableFloors.insert(floor.first);
    }

    return availableFloors;
}

short FloorMaps::floorSize(int floor) const
{
    auto floorMapsIterator = m_maps.find(floor);
    if (floorMapsIterator == m_maps.end())
        return 0;

    return floorMapsIterator->second.size();
}

FloorMaps::~FloorMaps() = default;
