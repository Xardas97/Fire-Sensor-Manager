#ifndef MAPIMAGEPROVIDER_H
#define MAPIMAGEPROVIDER_H

#include <QQuickImageProvider>

class MapImageProvider : public QQuickImageProvider
{
public:
    MapImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

private:
    QPixmap m_pixmap;
};

#endif // MAPIMAGEPROVIDER_H
