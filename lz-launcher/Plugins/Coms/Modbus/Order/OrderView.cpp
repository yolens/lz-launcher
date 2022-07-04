#include "OrderView.h"
#include "ui_OrderView.h"
#include "ModbusData.h"
#include "OrderCom.h"
#include <QComboBox>
#include <QMessageBox>

OrderView::OrderView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderView)
{
    ui->setupUi(this);

    m_model = new OrderModel(this);
    m_model->setOrderData(&ModbusData::instance()->orderList());
    ui->tableView->setModel(m_model);
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->tableView->setStyleSheet("QTableView{Selection-background-color:rgb(233,3,3);}");
   // ui->tableView->setStyleSheet("QTableView::item{selection-background-color:rgb(23,165,230)}");
   // ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setAlternatingRowColors(true);
 //   ui->tableView->resizeColumnsToContents();
    //ui->tableView->setStyleSheet("QTableView::item{selection-background-color:rgb(23,165,230)}");
    ui->tableView->setStyleSheet("QTableView{Selection-background-color:rgb(23,165,230);}");
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_menu = new QMenu();
    QAction *deleteActon = new QAction("删除", m_menu); deleteActon->setProperty("id", "delete");
    QAction *actionActon = new QAction("执行", m_menu); actionActon->setProperty("id", "action");
    QAction *addActon = new QAction("添加", m_menu); addActon->setProperty("id", "add");
    m_menu->addAction(actionActon);
    m_menu->addAction(addActon);
    m_menu->addAction(deleteActon);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &OrderView::on_customContextMenuRequested);
    connect(ui->tableView, &QTableView::pressed, this, &OrderView::on_pressed);
    connect(m_menu, &QMenu::triggered, this, &OrderView::on_triggered);

    QMap<int, QAbstractItemDelegate*> delegatesMap = m_model->itemDelegates();
    QMap<int, QAbstractItemDelegate*>::iterator it;
    for (it = delegatesMap.begin(); it != delegatesMap.end(); ++it) {
        ui->tableView->setItemDelegateForColumn(it.key(), it.value());
    }

    foreach (QAbstractSpinBox *sb, this->findChildren<QAbstractSpinBox*>())
    {
        sb->installEventFilter(this);
    }
    foreach (QComboBox *cb, this->findChildren<QComboBox*>())
    {
        cb->installEventFilter(this);
    }
}

OrderView::~OrderView()
{
    delete ui;
}


void OrderView::on_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->tableView->indexAt(pos);
    if (index.isValid())
    {
        m_menu->exec(QCursor::pos());
    }

}
void OrderView::on_pressed(const QModelIndex &index)
{
    m_model->pressIndex(index);
}

void OrderView::on_triggered(QAction *action)
{
    if (nullptr == action)
        return;
    QString id = action->property("id").toString();


    if ("delete" == id)
    {
        QMessageBox box(QMessageBox::NoIcon, "提示", "是否删除所有选中行的数据？", QMessageBox::Yes | QMessageBox::No, nullptr);
        if (box.exec() == QMessageBox::Yes)
        {
            QItemSelectionModel *selection = ui->tableView->selectionModel();
            QModelIndexList indexes = selection->selectedRows();
            m_model->removeRows(indexes);
        }
    }
    if ("add" == id)
    {
        addItem();
    }
    else if ("action" == id)
    {
        QItemSelectionModel *selection = ui->tableView->selectionModel();
        QModelIndexList indexes = selection->selectedRows();
        foreach (QModelIndex index, indexes) {
            OrderCom *order = (OrderCom*)m_model->itemFromIndex(index);
            if (nullptr != order)
            {
                LOrder *item = Plugin::DataCenterPlugin()->newOrder(order);
                Plugin::DataCenterPlugin()->execute(item);
            }
        }

    }
}

bool OrderView::eventFilter(QObject *watched, QEvent *event)
{
    if (watched->inherits("QAbstractSpinBox") || watched->inherits("QComboBox"))
    {
        if (QEvent::Wheel == event->type())
            return true;
    }
    return QWidget::eventFilter(watched, event);
}

void OrderView::on_changeDeviceName()
{
    m_model->updateDevice();
    m_model->reset();
}

void OrderView::addItem()
{
    OrderCom *p = new OrderCom();
    ModbusData::instance()->insert(p);
    m_model->reset();
}
