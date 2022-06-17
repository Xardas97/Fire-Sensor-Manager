#ifndef MAPIMAGEPROVIDER_H
#define MAPIMAGEPROVIDER_H

#include <set>
#include <memory>
#include <unordered_map>
#include <QQuickImageProvider>

class Database;

class MapImageProvider : public QQuickImageProvider
{
    Q_OBJECT

public:
    MapImageProvider(std::shared_ptr<Database> database);
    virtual ~MapImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    bool upload(int floor, const QUrl& pixmap);
    void removeFloor(int floor);

    short floorSize(int floor) const;
    std::set<int> availableFloors() const;

signals:
    void floorAdded(int floor);
    void floorRemoved(int floor);

private:
    void add(int floor, const QPixmap& pixmap);
    QPixmap* findPixmap(int floor, short floorPart);

    std::shared_ptr<Database> m_database;
    std::unordered_map<int, std::vector<QPixmap>> m_maps;
};

#endif // MAPIMAGEPROVIDER_H
