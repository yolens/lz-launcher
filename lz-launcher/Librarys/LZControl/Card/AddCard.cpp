#include "AddCard.h"
#include "ui_AddCard.h"

AddCard::AddCard(QWidget *parent) :
    Card(parent),
    ui(new Ui::AddCard)
{
    ui->setupUi(this);

    this->setStyleSheet(".AddCard{background-color:rgba(40, 255, 76, 153);border-radius: 10px;}");
}

AddCard::~AddCard()
{
    delete ui;
}

void AddCard::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void AddCard::on_pushButton_clicked()
{
    emit add_action();
}

