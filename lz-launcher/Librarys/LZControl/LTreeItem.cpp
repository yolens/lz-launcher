#include "LTreeItem.h"

LTreeItem::LTreeItem(LOrder *data, LTreeItem *parent)
    : m_parent(parent), m_data(data)
{

}


LTreeItem::~LTreeItem()
{
    qDeleteAll(m_childs);
}


void LTreeItem::appendChild(LTreeItem *item)
{
    m_childs.append(item);
}

LTreeItem *LTreeItem::child(int row)
{
    if (row < 0 || row >= m_childs.size())
        return nullptr;
    return m_childs.at(row);
}

int LTreeItem::childCount() const
{
    return m_childs.count();
}

int LTreeItem::columnCount() const
{
    return 1;
}


LTreeItem *LTreeItem::parentItem()
{
    return m_parent;
}

int LTreeItem::row() const
{
    if (m_parent)
        return m_parent->m_childs.indexOf(const_cast<LTreeItem*>(this));

    return 0;
}

LOrder* LTreeItem::data()
{
    return m_data;
}
