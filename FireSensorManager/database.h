#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <QObject>
#include <functional>
#include <unordered_map>

class Sensor;
class QPixmap;

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    void loadMaps(std::function<void(int, const QPixmap&)> addFunc);
    void saveMaps(const std::unordered_map<int, std::vector<QPixmap>>& maps);

    auto loadSensors() -> std::vector<std::unique_ptr<Sensor>>;
    void saveSensors(const std::vector<std::shared_ptr<Sensor>>& sensors);

    bool loadSensorData(Sensor& sensor);
    void saveSensorData(const Sensor& sensor);

private:
    bool open();
    void close();
    bool createTables();

    bool createMapsTable();
    bool createSensorsTable();

    void saveMap(int floor, const QPixmap& pixmap);

    void saveSensor(const Sensor& sensor);
};

#endif // DATABASE_H
