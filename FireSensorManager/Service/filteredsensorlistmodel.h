#ifndef FILTEREDSENSORLISTMODEL_H
#define FILTEREDSENSORLISTMODEL_H

#include <QSortFilterProxyModel>

class FilteredSensorListModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(bool activeFilterEnabled READ activeFilterEnabled WRITE setactiveFilterEnabled NOTIFY activeFilterEnabledChanged)
    Q_PROPERTY(bool placedFilterEnabled   READ placedFilterEnabled   WRITE setPlacedFilterEnabled   NOTIFY placedFilterEnabledChanged)
    Q_PROPERTY(bool unplacedFilterEnabled READ unplacedFilterEnabled WRITE setUnplacedFilterEnabled NOTIFY unplacedFilterEnabledChanged)
    Q_PROPERTY(bool alarmedFilterEnabled READ alarmedFilterEnabled WRITE setAlarmedFilterEnabled NOTIFY alarmedFilterEnabledChanged)

public:
    explicit FilteredSensorListModel(QObject* parent = nullptr);

    bool activeFilterEnabled() const;
    void setactiveFilterEnabled(bool activeFilterEnabled);
    bool placedFilterEnabled() const;
    void setPlacedFilterEnabled(bool placedFilterEnabled);
    bool unplacedFilterEnabled() const;
    void setUnplacedFilterEnabled(bool unplacedFilterEnabled);
    bool alarmedFilterEnabled() const;
    void setAlarmedFilterEnabled(bool unplacedFilterEnabled);

signals:
    void activeFilterEnabledChanged();
    void placedFilterEnabledChanged();
    void unplacedFilterEnabledChanged();
    void alarmedFilterEnabledChanged();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    bool m_activeFilterEnabled = false;
    bool m_placedFilterEnabled = false;
    bool m_unplacedFilterEnabled = false;
    bool m_alarmedFilterEnabled = false;
};

#endif // FILTEREDSENSORLISTMODEL_H
