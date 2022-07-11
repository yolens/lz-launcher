#ifndef NODECARD_H
#define NODECARD_H

#include "Card.h"

namespace Ui {
class NodeCard;
}

class LZCONTROL_EXPORT NodeCard : public Card
{
    Q_OBJECT

public:
    explicit NodeCard(QWidget *parent = nullptr);
    ~NodeCard();

    virtual void setName(const QString& text) override;
protected:
    virtual void paintEvent(QPaintEvent * event) override;
    virtual void enterEvent(QEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
private slots:
    void on_lineEdit_name_textChanged(const QString &arg1);

    void on_pushButton_edit_clicked();

private:
    Ui::NodeCard *ui;
};

#endif // NODECARD_H
