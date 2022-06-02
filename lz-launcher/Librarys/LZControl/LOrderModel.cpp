#include "LOrderModel.h"

LOrderModel::LOrderModel(QObject *parent) : QAbstractTableModel(parent)
{
    m_headList.clear();
    m_dataList = nullptr;
}

int LOrderModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_dataList == nullptr ? 0 : m_dataList->count();
}

int LOrderModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_headList.size();
}

QVariant LOrderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        return m_headList.at(section);
    } else {
        return QString("%1").arg(section + 1);
    }

}

Qt::ItemFlags LOrderModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void LOrderModel::appendHeadTitle(const QString& title)
{
    m_headList.append(title);
}

QList<LOrder*>* LOrderModel::dataList() const
{
    return m_dataList;
}

void LOrderModel::setOrderData(QList<LOrder*>* data)
{
    m_dataList = data;
}

void LOrderModel::reset()
{
    beginResetModel();
    endResetModel();
}
