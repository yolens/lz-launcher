#ifndef LTREEITEM_H
#define LTREEITEM_H
#include "LDB.h"

class LTreeItem
{
public:
    LTreeItem(LOrder *data, LTreeItem *parent = nullptr);
    ~LTreeItem();

    void appendChild(LTreeItem *child);

    LTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    int row() const;
    LTreeItem *parentItem();
    LOrder* data();

private:
    QVector<LTreeItem*> m_childs;
    LTreeItem           *m_parent;
    LOrder              *m_data;
};

#endif // LTREEITEM_H
