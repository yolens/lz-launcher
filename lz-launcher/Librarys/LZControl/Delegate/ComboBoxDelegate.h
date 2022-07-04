#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include "LZControl_global.h"
#include <QItemDelegate>

class LZCONTROL_EXPORT ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ComboBoxDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const  QModelIndex &index) const override;

    void setItems(QStringList list);
    void setDisenableItems(QStringList list);
private:
    QStringList m_list;
    QStringList m_disenableList;
};

#endif // COMBOBOXDELEGATE_H
