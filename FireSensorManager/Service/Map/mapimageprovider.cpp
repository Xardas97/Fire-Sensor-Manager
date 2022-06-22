#include "mapimageprovider.h"

#include "../database.h"

#include <QFile>
#include <vector>
#include <QTextStream>
#include <QStandardPaths>

MapImageProvider::MapImageProvider(std::shared_ptr<Database> database)
    : QQuickImageProvider{QQuickImageProvider::Pixmap},
      m_database(database)
{
    m_database->loadMaps([&](int floor, const MapEntry& map) { add(floor, map); });
}

QPixmap MapImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    auto location = id.split('/');
    if (location.size() != 2)
        return QPixmap{};

    auto floor = location[0].toInt();
    auto floorPart = location[1].toInt();

    auto chosenImage = findPixmap(floor, floorPart);
    if (!chosenImage)
        return QPixmap{};

    auto originalSize = chosenImage->size();
    if (size)
        *size = originalSize;

    auto finalWidth = requestedSize.width() > 0 ? requestedSize.width() : originalSize.width();
    auto finalHeight = requestedSize.height() > 0 ? requestedSize.height() : originalSize.height();
    return chosenImage->scaled(finalWidth, finalHeight, Qt::KeepAspectRatio);
}

void MapImageProvider::add(int floor, const MapEntry& map)
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

MapEntry* MapImageProvider::mapEntry(int floor, short floorPart)
{
    auto floorMapsIterator = m_maps.find(floor);
    if (floorMapsIterator == m_maps.end())
        return nullptr;

    auto floorMaps = &floorMapsIterator->second;
    if (floorMaps->size() <= (unsigned long long)floorPart)
        return nullptr;

    return &(*floorMaps)[floorPart];
}

QPixmap* MapImageProvider::findPixmap(int floor, short floorPart)
{
    auto requestedEntry = mapEntry(floor, floorPart);
    if (!requestedEntry)
        return nullptr;

    return &requestedEntry->pixmap();
}

bool MapImageProvider::upload(int floor, const QUrl& url)
{
    qDebug() << "Uploading map " << url.toLocalFile() << " to floor " << floor;

    QImage image;
    auto loaded = image.load(url.toLocalFile());
    if (!loaded)
    {
        qWarning() << "Failed to load image from the file";
        return false;
    }

    add(floor, MapEntry{QPixmap::fromImage(image)});

    qDebug() << "Successfully loaded the map";
    return true;
}

void MapImageProvider::removeFloor(int floor)
{
    auto floorMapsIterator = m_maps.find(floor);
    if (floorMapsIterator == m_maps.end())
        return;

    m_maps.erase(floorMapsIterator);
    emit floorRemoved(floor);
}

std::set<int> MapImageProvider::availableFloors() const
{
    std::set<int> availableFloors;

    for (auto& floor: m_maps)
    {
        availableFloors.insert(floor.first);
    }

    return availableFloors;
}

short MapImageProvider::floorSize(int floor) const
{
    auto floorMapsIterator = m_maps.find(floor);
    if (floorMapsIterator == m_maps.end())
        return 0;

    return floorMapsIterator->second.size();
}

MapImageProvider::~MapImageProvider()
{
    m_database->saveMaps(m_maps);
}
