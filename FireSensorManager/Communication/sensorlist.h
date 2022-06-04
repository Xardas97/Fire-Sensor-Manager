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

    explicit SensorList(QObject* parent = nullptr);

    auto find(std::shared_ptr<Sensor> sensor) const -> std::shared_ptr<Sensor>;
    void add(std::shared_ptr<Sensor> sensor);
    void remove(std::shared_ptr<Sensor> sensor);

    auto sensors() -> std::vector<std::shared_ptr<Sensor>>&;

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<std::shared_ptr<Sensor>> m_sensors;

    void onDataChanged();
};

#endif // SENSORLIST_H
