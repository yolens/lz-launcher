#include "LTreeMode.h"
#include <QMimeData>
#include <QDataStream>

LTreeMode::LTreeMode(QMap<int, QList<LOrder*>>& data, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_root = new LTreeItem(nullptr);
    setupModelData(data, m_root);
}

LTreeMode::~LTreeMode()
{
    delete m_root;
    for (int i=0; i<m_clearInfoList.size(); i++)
    {
        delete m_clearInfoList.at(i);
    }
    m_clearInfoList.clear();
}



int LTreeMode::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<LTreeItem*>(parent.internalPointer())->columnCount();
    return m_root->columnCount();
}



QVariant LTreeMode::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    LTreeItem *item = static_cast<LTreeItem*>(index.internalPointer());

    return item->data()->name();
}



Qt::ItemFlags LTreeMode::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}


QVariant LTreeMode::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if (m_root->data() != nullptr)
            return m_root->data()->name();
    }

    return QVariant();
}



QModelIndex LTreeMode::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    LTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<LTreeItem*>(parent.internalPointer());

    LTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}



QModelIndex LTreeMode::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    LTreeItem *childItem = static_cast<LTreeItem*>(index.internalPointer());
    LTreeItem *parentItem = childItem->parentItem();

    if (parentItem == m_root)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}



int LTreeMode::rowCount(const QModelIndex &parent) const
{
    LTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<LTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
#pragma execution_character_set("utf-8")
#include <QDebug>
void LTreeMode::setupModelData(QMap<int, QList<LOrder*>>& data, LTreeItem *parent)
{
    m_clearInfoList.clear();
    LTreeItem *parentItem, *childItem;
    QMap<int, QList<LOrder*>>::iterator it;
    for (it = data.begin(); it != data.end(); it++)
    {
        LOrder* info = new LOrder; //节点头信息
        m_clearInfoList.append(info);
        info->setName("指令集");

        parentItem = new LTreeItem(info, parent);
        parent->appendChild(parentItem);
        for (int i=0; i<it.value().size(); i++)
        {
            childItem = new LTreeItem((LOrder*)it.value().at(i), parentItem);
            parentItem->appendChild(childItem);
        }
    }
}


QMimeData* LTreeMode::mimeData(const QModelIndexList &indexes) const
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
    LTreeItem* node = (LTreeItem*)index.internalPointer();
    if (node->childCount() <= 0)
    {
        QByteArray encoded;
        QDataStream stream(&encoded, QIODevice::WriteOnly);
        stream << (qint64)(node->data());
        mimeD->setData("Node/OrderTree", encoded);
    }
    else
    {
        return nullptr;
    }

    return mimeD;
}
