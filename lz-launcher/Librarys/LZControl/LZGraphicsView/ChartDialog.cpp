#include "ChartDialog.h"
#include "ui_ChartDialog.h"

ChartDialog::ChartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChartDialog)
{
    ui->setupUi(this);
}

ChartDialog::~ChartDialog()
{
    delete ui;
}

void ChartDialog::setInfo(const ChartInfo& info)
{
    m_info = info;

    ui->lineEdit_value->setText(m_info.value.toString());
    ui->spinBox_delay->setValue(m_info.delay);
}

ChartDialog::ChartInfo& ChartDialog::info()
{
    return m_info;
}

void ChartDialog::on_pushButton_ok_clicked()
{
    m_info.value = ui->lineEdit_value->text();
    m_info.delay = ui->spinBox_delay->value();
    done(0);
}
