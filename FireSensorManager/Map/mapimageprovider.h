#ifndef MAPIMAGEPROVIDER_H
#define MAPIMAGEPROVIDER_H

#include <memory>
#include <unordered_map>
#include <QQuickImageProvider>

class Database;

class MapImageProvider : public QQuickImageProvider
{
public:
    MapImageProvider(std::shared_ptr<Database> database);
    virtual ~MapImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    short floorSize(int floor);

private:
    void add(int floor, const QPixmap& pixmap);
    QPixmap* findPixmap(int floor, short floorPart);

    std::shared_ptr<Database> m_database;
    std::unordered_map<int, std::vector<QPixmap>> m_maps;
};

#endif // MAPIMAGEPROVIDER_H
