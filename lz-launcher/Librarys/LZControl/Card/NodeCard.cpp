#include "NodeCard.h"
#include "ui_NodeCard.h"

const QString UNCHECKED_STYLE = "#NodeCard{background-color:rgba(40, 48, 76, 153);border-radius: 10px;}\
                                 #NodeCard::hover{border:1px solid rgba(0, 0, 255, 255);}\
                                ";
const QString CHECKED_STYLE = "#NodeCard{background-color:rgba(255, 48, 76, 153);border-radius: 10px;}\
                               #NodeCard::hover{border:1px solid rgba(0, 0, 255, 255);}\
                              ";

NodeCard::NodeCard(QWidget *parent) :
    Card(parent),
    ui(new Ui::NodeCard)
{
    ui->setupUi(this);
    ui->pushButton_close->hide();

    this->setStyleSheet(UNCHECKED_STYLE);

    connect(this, &Card::toggled, this, [=](bool checked){
        if (checked)
        {
            this->setStyleSheet(CHECKED_STYLE);
            emit selected_action();
        }
        else
        {
            this->setStyleSheet(UNCHECKED_STYLE);
        }
    });

    connect(ui->pushButton_close, &QPushButton::clicked, [=]{
        emit close_action();
    });
}

NodeCard::~NodeCard()
{
    delete ui;
}

void NodeCard::setName(const QString& text)
{
    ui->lineEdit_name->setText(text);
}

void NodeCard::on_lineEdit_name_textChanged(const QString &arg1)
{
    emit name_change_action(arg1);
}


void NodeCard::on_pushButton_edit_clicked()
{
    emit start_action();
}


void NodeCard::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void NodeCard::enterEvent(QEvent* event)
{
    ui->pushButton_close->show();
    QWidget::enterEvent(event);
}
void NodeCard::leaveEvent(QEvent* event)
{
    ui->pushButton_close->hide();
    QWidget::leaveEvent(event);
}
