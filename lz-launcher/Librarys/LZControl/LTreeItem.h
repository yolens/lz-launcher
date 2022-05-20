#ifndef LTREEITEM_H
#define LTREEITEM_H
#include "LDB.h"

class LTreeItem
{
public:
    LTreeItem(LCom *data, LTreeItem *parent = nullptr);
    ~LTreeItem();

    void appendChild(LTreeItem *child);

    LTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    int row() const;
    LTreeItem *parentItem();
    LCom* data();

private:
    QVector<LTreeItem*> m_childs;
    LTreeItem           *m_parent;
    LCom                *m_data;
};

#endif // LTREEITEM_H
