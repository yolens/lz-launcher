#ifndef ADDCARD_H
#define ADDCARD_H

#include "Card.h"

namespace Ui {
class AddCard;
}

class LZCONTROL_EXPORT AddCard : public Card
{
    Q_OBJECT

public:
    explicit AddCard(QWidget *parent = nullptr);
    ~AddCard();
protected:
    virtual void paintEvent(QPaintEvent * event) override;
private slots:
    void on_pushButton_clicked();

private:
    Ui::AddCard *ui;
};

#endif // ADDCARD_H
