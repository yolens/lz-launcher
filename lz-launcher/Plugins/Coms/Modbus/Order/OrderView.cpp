#include "OrderView.h"
#include "ui_OrderView.h"
#include "ModbusData.h"
#include "OrderCom.h"

OrderView::OrderView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderView)
{
    ui->setupUi(this);

    m_model = new OrderModel(this);
    m_model->setOrderData(ModbusData::instance()->orderList());
    ui->tableView->setModel(m_model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

OrderView::~OrderView()
{
    delete ui;
}

void OrderView::on_pushButton_add_clicked()
{
    OrderCom *p = new OrderCom();
    p->setName("test1");
    p->setMark("hlldfdfs");
    p->setValue(0.2345);
    ModbusData::instance()->insert(p);
    m_model->reset();
}

