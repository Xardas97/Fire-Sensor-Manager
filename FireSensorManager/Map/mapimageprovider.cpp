#include "mapimageprovider.h"

#include "database.h"

#include <QFile>
#include <vector>
#include <QTextStream>
#include <QStandardPaths>

MapImageProvider::MapImageProvider(std::shared_ptr<Database> database)
    : QQuickImageProvider{QQuickImageProvider::Pixmap},
      m_database(database)
{
    m_database->loadMaps([&](int floor, const QPixmap& pixmap) { add(floor, pixmap); });

    if (findPixmap(0, 0) == nullptr)
    {
        qDebug() << "Map not found in database, loading from file system...";

        auto path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        path += "/plan1.jpg";

        QImage image;

        auto loaded = image.load(path);
        if (!loaded)
        {
            qWarning() << "Failed to load image from: " << path;
            return;
        }

        qDebug() << "Adding loaded map to internal structure";
        add(0, QPixmap::fromImage(image));
    }
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

    return chosenImage->scaled(finalWidth, finalHeight);
}

void MapImageProvider::add(int floor, const QPixmap& pixmap)
{
    auto floorMapsIterator = m_maps.find(floor);
    if (floorMapsIterator == m_maps.end())
    {
        m_maps[floor] = std::vector<QPixmap>{pixmap};
        return;
    }

   floorMapsIterator->second.push_back(pixmap);
}

QPixmap* MapImageProvider::findPixmap(int floor, short floorPart)
{
    qDebug() << "Looking for image at: " << floor << "-" << floorPart;

    auto floorMapsIterator = m_maps.find(floor);
    if (floorMapsIterator == m_maps.end())
        return nullptr;

    auto floorMaps = &floorMapsIterator->second;
    if (floorMaps->size() <= (unsigned long long)floorPart)
        return nullptr;

    return &((*floorMaps)[floorPart]);
}

short MapImageProvider::floorSize(int floor)
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
