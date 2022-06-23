#include "mapimageprovider.h"

#include "floormaps.h"

MapImageProvider::MapImageProvider(std::shared_ptr<FloorMaps> floorMaps)
    : QQuickImageProvider{QQuickImageProvider::Pixmap},
      m_floorMaps(floorMaps)
{ }

QPixmap MapImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    auto location = id.split('/');
    if (location.size() != 2)
        return QPixmap{};

    auto floor = location[0].toInt();
    auto floorPart = location[1].toInt();

    auto chosenMap = m_floorMaps->mapEntry(floor, floorPart);
    if (!chosenMap)
        return QPixmap{};

    auto& chosenImage = chosenMap->pixmap();

    auto originalSize = chosenImage.size();
    if (size)
        *size = originalSize;

    auto finalWidth = requestedSize.width() > 0 ? requestedSize.width() : originalSize.width();
    auto finalHeight = requestedSize.height() > 0 ? requestedSize.height() : originalSize.height();
    return chosenImage.scaled(finalWidth, finalHeight, Qt::KeepAspectRatio);
}

MapImageProvider::~MapImageProvider() = default;
