#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <QObject>
#include <functional>
#include <unordered_map>

class Sensor;
class MapEntry;
class QSqlRecord;

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    auto maps() -> std::unordered_map<int, std::vector<std::shared_ptr<MapEntry>>>&;
    auto sensors() -> std::vector<std::shared_ptr<Sensor>>&;

    void saveData();

private:
    bool open();
    void close();

    bool createTables();
    bool createMapsTable();
    bool createSensorsTable();

    void loadData();
    void loadMapsAndTheirSensors();
    void loadMapItsSensors(const QSqlRecord& record);
    void loadSensorsForMap(MapEntry& map);
    void loadMaplessSensors();

    void saveMaps();
    void saveMap(int floor, MapEntry& pixmap);
    void saveSensors();
    void saveSensor(Sensor& sensor);

    std::vector<std::shared_ptr<Sensor>> m_sensors;
    std::unordered_map<int, std::vector<std::shared_ptr<MapEntry>>> m_maps;
};

#endif // DATABASE_H
