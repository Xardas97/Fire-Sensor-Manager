#ifndef MAPIMAGEPROVIDER_H
#define MAPIMAGEPROVIDER_H

#include <QQuickImageProvider>

class FloorMaps;

class MapImageProvider : public QQuickImageProvider
{
    Q_OBJECT

public:
    MapImageProvider(std::shared_ptr<FloorMaps> floorMaps);
    virtual ~MapImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

private:
    std::shared_ptr<FloorMaps> m_floorMaps;
};

#endif // MAPIMAGEPROVIDER_H
