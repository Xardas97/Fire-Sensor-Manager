#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <QObject>

class Sensor;

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    auto loadSensors() -> std::vector<std::unique_ptr<Sensor>>;
    void saveSensors(const std::vector<std::shared_ptr<Sensor>>& sensors);

    bool loadSensorData(Sensor& sensor);
    void saveSensorData(const Sensor& sensor);

private:
    bool open();
    void close();
    bool createTables();

    void saveSensor(const Sensor& sensor);
};

#endif // DATABASE_H
