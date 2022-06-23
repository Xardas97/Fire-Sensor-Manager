#include "floormaps.h"

#include "../database.h"

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

void FloorMaps::add(int floor, const MapEntry& map)
{
    auto floorMapsIterator = m_maps.find(floor);
    if (floorMapsIterator == m_maps.end())
    {
        m_maps[floor] = std::vector<MapEntry>{map};
        emit floorAdded(floor);
        return;
    }

    floorMapsIterator->second.push_back(map);
    emit floorPartAdded(floor);
}

MapEntry* FloorMaps::mapEntry(int floor, short floorPart)
{
    auto floorMapsIterator = m_maps.find(floor);
    if (floorMapsIterator == m_maps.end())
        return nullptr;

    auto floorMaps = &floorMapsIterator->second;
    if (floorMaps->size() <= (unsigned long long)floorPart)
        return nullptr;

    return &(*floorMaps)[floorPart];
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

    add(floor, MapEntry{-1, QPixmap::fromImage(image)});

    qDebug() << "Successfully loaded the map";
    return true;
}

void FloorMaps::removeFloor(int floor)
{
    auto floorMapsIterator = m_maps.find(floor);
    if (floorMapsIterator == m_maps.end())
        return;

    m_maps.erase(floorMapsIterator);
    emit floorRemoved(floor);
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
