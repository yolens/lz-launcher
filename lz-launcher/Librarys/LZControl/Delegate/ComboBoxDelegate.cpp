#include "ComboBoxDelegate.h"

#include <QComboBox>

ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    QComboBox *comboBox = new QComboBox(parent);
    comboBox->addItems(m_list);
    foreach (const auto& a, m_disenableList)
    {
        comboBox->setItemData(comboBox->findText(a), false, Qt::UserRole-1);
    }

    return comboBox;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentText(text);
}


void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                         const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString text = comboBox->currentText();
    model->setData(index, text, Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

void ComboBoxDelegate::setItems(QStringList list)
{
    m_list = list;
}

void ComboBoxDelegate::setDisenableItems(QStringList list)
{
    m_disenableList = list;
}
