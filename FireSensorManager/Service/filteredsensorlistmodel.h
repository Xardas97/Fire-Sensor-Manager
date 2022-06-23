#ifndef FILTEREDSENSORLISTMODEL_H
#define FILTEREDSENSORLISTMODEL_H

#include <QSortFilterProxyModel>

class FilteredSensorListModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(bool inactiveFilterEnabled READ inactiveFilterEnabled WRITE setinActiveFilterEnabled NOTIFY inactiveFilterEnabledChanged)
    Q_PROPERTY(bool placedFilterEnabled   READ placedFilterEnabled   WRITE setPlacedFilterEnabled   NOTIFY placedFilterEnabledChanged)
    Q_PROPERTY(bool unplacedFilterEnabled READ unplacedFilterEnabled WRITE setUnplacedFilterEnabled NOTIFY unplacedFilterEnabledChanged)

public:
    explicit FilteredSensorListModel(QObject* parent = nullptr);

    bool inactiveFilterEnabled() const;
    void setinActiveFilterEnabled(bool inactiveFilterEnabled);
    bool placedFilterEnabled() const;
    void setPlacedFilterEnabled(bool placedFilterEnabled);
    bool unplacedFilterEnabled() const;
    void setUnplacedFilterEnabled(bool unplacedFilterEnabled);

signals:
    void inactiveFilterEnabledChanged();
    void placedFilterEnabledChanged();
    void unplacedFilterEnabledChanged();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    bool m_inactiveFilterEnabled = false;
    bool m_placedFilterEnabled = false;
    bool m_unplacedFilterEnabled = false;
};

#endif // FILTEREDSENSORLISTMODEL_H
