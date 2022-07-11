#ifndef UNITTREEMODE_H
#define UNITTREEMODE_H

#include <QAbstractItemModel>

#include "LDB.h"

class UnitTreeItem
{
public:
    UnitTreeItem(LUnit *data, UnitTreeItem *parent = nullptr);
    ~UnitTreeItem();

    void appendChild(UnitTreeItem *child);

    UnitTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    int row() const;
    UnitTreeItem *parentItem();
    LUnit* data();

private:
    QVector<UnitTreeItem*> m_childs;
    UnitTreeItem           *m_parent;
    LUnit              *m_data;
};


class UnitTreeMode : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit UnitTreeMode(QList<LUnit*>& data, QObject *parent = nullptr);
    ~UnitTreeMode();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QMimeData* mimeData(const QModelIndexList &indexes) const override;
private:
    void setupModelData(QList<LUnit*>& data, UnitTreeItem *parent);

    UnitTreeItem       *m_root;
    QList<LUnit*>  m_clearInfoList;

};

#endif // UNITTREEMODE_H
