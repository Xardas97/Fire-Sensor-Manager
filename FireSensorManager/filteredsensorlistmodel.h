#ifndef FILTEREDSENSORLISTMODEL_H
#define FILTEREDSENSORLISTMODEL_H

#include <QSortFilterProxyModel>

class FilteredSensorListModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(bool replaceFilterEnabled READ replaceFilterEnabled WRITE setReplaceFilterEnabled NOTIFY replaceFilterEnabledChanged)
    Q_PROPERTY(bool activeFilterEnabled READ activeFilterEnabled WRITE setActiveFilterEnabled NOTIFY activeFilterEnabledChanged)

public:
    explicit FilteredSensorListModel(QObject* parent = nullptr);

    bool replaceFilterEnabled() const;
    void setReplaceFilterEnabled(bool replaceFilterEnabled);
    bool activeFilterEnabled() const;
    void setActiveFilterEnabled(bool activeFilterEnabled);

signals:
    void replaceFilterEnabledChanged();
    void activeFilterEnabledChanged();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    bool m_replaceFilterEnabled = true;
    bool m_activeFilterEnabled = false;
};

#endif // FILTEREDSENSORLISTMODEL_H
