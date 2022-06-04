#ifndef FILTEREDSENSORLISTMODEL_H
#define FILTEREDSENSORLISTMODEL_H

#include <QSortFilterProxyModel>

class FilteredSensorListModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(bool replaceFilterEnabled READ getReplaceFilterEnabled WRITE setReplaceFilterEnabled NOTIFY replaceFilterEnabledChanged)
    Q_PROPERTY(bool activeFilterEnabled READ getActiveFilterEnabled WRITE setActiveFilterEnabled NOTIFY activeFilterEnabledChanged)

public:
    explicit FilteredSensorListModel(QObject* parent = nullptr);

    bool getReplaceFilterEnabled() const;
    void setReplaceFilterEnabled(bool replaceFilterEnabled);
    bool getActiveFilterEnabled() const;
    void setActiveFilterEnabled(bool activeFilterEnabled);

signals:
    void replaceFilterEnabledChanged();
    void activeFilterEnabledChanged();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    bool replaceFilterEnabled = true;
    bool activeFilterEnabled = false;
};

#endif // FILTEREDSENSORLISTMODEL_H
