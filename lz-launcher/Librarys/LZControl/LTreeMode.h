#ifndef LTREEMODE_H
#define LTREEMODE_H

#include <QAbstractItemModel>
#include "LTreeItem.h"

class LTreeMode : public QAbstractItemModel
{
public:
    explicit LTreeMode(QMap<int, QList<LOrder*>>& data, QObject *parent = nullptr);
    ~LTreeMode();

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
    void setupModelData(QMap<int, QList<LOrder*>>& data, LTreeItem *parent);

    LTreeItem       *m_root;
    QList<LOrder*>  m_clearInfoList;

};

#endif // LTREEMODE_H
