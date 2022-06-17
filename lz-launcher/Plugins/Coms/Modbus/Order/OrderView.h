#ifndef ORDERVIEW_H
#define ORDERVIEW_H

#include <QWidget>
#include "OrderModel.h"
#include <QMenu>
#include <QAction>

namespace Ui {
class OrderView;
}

class OrderView : public QWidget
{
    Q_OBJECT

public:
    explicit OrderView(QWidget *parent = nullptr);
    ~OrderView();
private:
    void addItem();
protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
private slots:
    void on_pressed(const QModelIndex &index);
    void on_triggered(QAction *action);
    void on_customContextMenuRequested(const QPoint &pos);
public:
    void on_changeDeviceName();
private:
    Ui::OrderView *ui;

    OrderModel *m_model = nullptr;
    QMenu *m_menu = nullptr;
    QAction *m_deleteActon = nullptr;
};

#endif // ORDERVIEW_H
