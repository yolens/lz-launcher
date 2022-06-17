#include "LOrderModel.h"
#include "Delegate/ComboBoxDelegate.h"

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
/*
Qt::ItemFlags LOrderModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}*/

bool LOrderModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);      // 实现model 的项删除时，必须以beginRemoveRows 开头
    for (int i = 0; i < count; i++) {
        LOrder *item = m_dataList->at(row + i);
        //m_dataList->removeOne(item);
        remove(item);
    }
    endRemoveRows();                                    // endRemoveRows 结尾，这样model 就会自动刷新数据在界面上,最主要是内部更新index

    return true;
}

bool LOrderModel::removeRows(QModelIndexList indexes)
{
    QVector<int> vecRow;
    foreach (QModelIndex index, indexes) {
        vecRow.append(index.row());
    }

    qSort(vecRow.begin(), vecRow.end(), qGreater<int>());

    foreach (int row, vecRow) {
         removeRows(row, 1);
    }

    return true;
}

LOrder *LOrderModel::itemFromIndex(const QModelIndex &index) const
{
    if (index.isValid()) {
        //CommuOrder *item = static_cast<CommuOrder *>(index.internalPointer());
        LOrder *item = m_dataList->at(index.row());
        return item;
    }

    return nullptr;
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

void LOrderModel::addComboBoxDelegate(int col, QStringList list)
{
    ComboBoxDelegate *combobox = nullptr;
    if (m_delegateMap.contains(col))
        combobox = (ComboBoxDelegate *)m_delegateMap.value(col);
    if (nullptr == combobox)
        combobox = new ComboBoxDelegate(this);
    combobox->setItems(list);
    m_delegateMap.insert(col, combobox);
}

void LOrderModel::disenableComboBoxItem(int col, QStringList list) const
{
    if (m_delegateMap.contains(col))
    {
        ComboBoxDelegate *combobox = (ComboBoxDelegate *)m_delegateMap.value(col);
        combobox->setDisenableItems(list);
    }
}

QMap<int, QAbstractItemDelegate*>& LOrderModel::itemDelegates()
{
    return m_delegateMap;
}


