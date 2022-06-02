#ifndef ORDERVIEW_H
#define ORDERVIEW_H

#include <QWidget>
#include "OrderModel.h"

namespace Ui {
class OrderView;
}

class OrderView : public QWidget
{
    Q_OBJECT

public:
    explicit OrderView(QWidget *parent = nullptr);
    ~OrderView();

private slots:
    void on_pushButton_add_clicked();

private:
    Ui::OrderView *ui;

    OrderModel *m_model = nullptr;
};

#endif // ORDERVIEW_H
