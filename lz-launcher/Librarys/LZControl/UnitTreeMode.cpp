#include "UnitTreeMode.h"

UnitTreeItem::UnitTreeItem(LUnit *data, UnitTreeItem *parent)
    : m_parent(parent), m_data(data)
{

}


UnitTreeItem::~UnitTreeItem()
{
    qDeleteAll(m_childs);
}


void UnitTreeItem::appendChild(UnitTreeItem *item)
{
    m_childs.append(item);
}

UnitTreeItem *UnitTreeItem::child(int row)
{
    if (row < 0 || row >= m_childs.size())
        return nullptr;
    return m_childs.at(row);
}

int UnitTreeItem::childCount() const
{
    return m_childs.count();
}

int UnitTreeItem::columnCount() const
{
    return 1;
}


UnitTreeItem *UnitTreeItem::parentItem()
{
    return m_parent;
}

int UnitTreeItem::row() const
{
    if (m_parent)
        return m_parent->m_childs.indexOf(const_cast<UnitTreeItem*>(this));

    return 0;
}

LUnit* UnitTreeItem::data()
{
    return m_data;
}

UnitTreeMode::UnitTreeMode(QList<LUnit*>& data, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_root = new UnitTreeItem(nullptr);
    setupModelData(data, m_root);
}

UnitTreeMode::~UnitTreeMode()
{
    delete m_root;
    for (int i=0; i<m_clearInfoList.size(); i++)
    {
        delete m_clearInfoList.at(i);
    }
    m_clearInfoList.clear();
}



int UnitTreeMode::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<UnitTreeItem*>(parent.internalPointer())->columnCount();
    return m_root->columnCount();
}



QVariant UnitTreeMode::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    UnitTreeItem *item = static_cast<UnitTreeItem*>(index.internalPointer());

    return item->data()->name + "#" + QString::number(item->data()->id);
}



Qt::ItemFlags UnitTreeMode::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}


QVariant UnitTreeMode::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    Q_UNUSED(section);
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if (m_root->data() != nullptr)
            return m_root->data()->name;
    }

    return QVariant();
}



QModelIndex UnitTreeMode::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    UnitTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<UnitTreeItem*>(parent.internalPointer());

    UnitTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}



QModelIndex UnitTreeMode::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    UnitTreeItem *childItem = static_cast<UnitTreeItem*>(index.internalPointer());
    UnitTreeItem *parentItem = childItem->parentItem();

    if (parentItem == m_root)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}



int UnitTreeMode::rowCount(const QModelIndex &parent) const
{
    UnitTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<UnitTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
#pragma execution_character_set("utf-8")
#include <QDebug>
void UnitTreeMode::setupModelData(QList<LUnit*>& data, UnitTreeItem *parent)
{
    m_clearInfoList.clear();
    UnitTreeItem *parentItem, *childItem;

    LUnit* info = new LUnit; //节点头信息
    m_clearInfoList.append(info);
    info->name = "units";
    parentItem = new UnitTreeItem(info, parent);
    parent->appendChild(parentItem);
    foreach (const auto &i, data)
    {
        childItem = new UnitTreeItem(i, parentItem);
        parentItem->appendChild(childItem);
    }
}
#include <QDataStream>
#include <QMimeData>

QMimeData* UnitTreeMode::mimeData(const QModelIndexList &indexes) const
{
    QMimeData* mimeD = QAbstractItemModel::mimeData(indexes);				//先获取原始的mimeData;

    /*if (indexes.size() > 0)
    {
        QModelIndex index = indexes.at(0);

        TreeModel* node = (TreeModel*)index.internalPointer();
        QByteArray encoded;
        QDataStream stream(&encoded, QIODevice::WriteOnly);
        stream << (qint64)(node);
        mimeD->setData("Node/NodePtr", encoded);							//将自己需要的数据 存入到MimeData中
    }
    else*/
    QModelIndex index = indexes.at(0);
    UnitTreeItem* node = (UnitTreeItem*)index.internalPointer();
    if (node->childCount() <= 0)
    {
        QByteArray encoded;
        QDataStream stream(&encoded, QIODevice::WriteOnly);
        stream << (qint64)(node->data());
        mimeD->setData("Node/UnitTree", encoded);
    }
    else
    {
        return nullptr;
    }

    return mimeD;
}
