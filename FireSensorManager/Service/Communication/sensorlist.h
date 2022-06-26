#ifndef SENSORLIST_H
#define SENSORLIST_H

#include <memory>
#include <vector>
#include <QAbstractListModel>

class Sensor;

class SensorList : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        DataRole = Qt::UserRole
    };

    explicit SensorList(std::vector<std::shared_ptr<Sensor>>& sensors, QObject* parent = nullptr);

    auto find(const Sensor& checkSensor) const -> std::shared_ptr<Sensor>;
    void add(std::shared_ptr<Sensor> sensor);
    auto remove(const Sensor& removeSensor) -> std::shared_ptr<Sensor>;

    auto sensors() -> std::vector<std::shared_ptr<Sensor>>&;

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void alarmedSensorsChanged();
    void sensorsWithStatusChanged();

private:
    std::vector<std::shared_ptr<Sensor>>& m_sensors;
    std::shared_ptr<Sensor> m_removedSensor;

    void connectSignals(std::shared_ptr<Sensor> sensor);
    void disconnectSignals(std::shared_ptr<Sensor> sensor);

    void onDataChanged();
};

#endif // SENSORLIST_H
