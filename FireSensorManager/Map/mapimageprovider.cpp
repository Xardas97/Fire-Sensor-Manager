#include "mapimageprovider.h"

#include <QFile>
#include <QTextStream>
#include <QStandardPaths>

MapImageProvider::MapImageProvider()
    : QQuickImageProvider{QQuickImageProvider::Pixmap}
{
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    path += "/plan1.jpg";

    QImage image;
    auto loaded = image.load(path);

    if (!loaded)
    {
        qWarning() << "Failed to load image from: " << path;
        return;
    }

    m_pixmap = QPixmap::fromImage(image);
}

QPixmap MapImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    if (id != "plan1")
        return QPixmap{};

    auto chosenImage = &m_pixmap;

    auto originalSize = chosenImage->size();
    if (size)
        *size = originalSize;

    auto finalWidth = requestedSize.width() > 0 ? requestedSize.width() : originalSize.width();
    auto finalHeight = requestedSize.height() > 0 ? requestedSize.height() : originalSize.height();

    return chosenImage->scaled(finalWidth, finalHeight);
}
