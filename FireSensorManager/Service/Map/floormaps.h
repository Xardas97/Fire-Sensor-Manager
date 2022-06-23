#ifndef FLOORMAPS_H
#define FLOORMAPS_H

#include "mapentry.h"

#include <set>
#include <memory>
#include <QObject>
#include <unordered_map>

class Database;

class FloorMaps : public QObject
{
    Q_OBJECT

public:
    FloorMaps(std::shared_ptr<Database> database);
    virtual ~FloorMaps();

    MapEntry* mapEntry(int floor, short floorPart);

    bool upload(int floor, const QUrl& pixmap);
    void removeFloor(int floor);

    short floorSize(int floor) const;
    std::set<int> availableFloors() const;

signals:
    void floorAdded(int floor);
    void floorRemoved(int floor);
    void floorPartAdded(int floor);
    void floorPartRemoved(int floor);

private:
    void add(int floor, const MapEntry& map);

    std::shared_ptr<Database> m_database;
    std::unordered_map<int, std::vector<MapEntry>> m_maps;
};

#endif // FLOORMAPS_H
