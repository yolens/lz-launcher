#include "LZWindow.h"
#include "ui_LZWindow.h"
#include <QDockWidget>
#include "LZLib.h"

LZWindow::LZWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LZWindow)
{
    ui->setupUi(this);

    createDockWindows();

  //  Item *item = m_pView->addItem(LCType::LC_Node, QPoint(130,130));

    QMap<int, QList<LOrder*>> data;
    QList<LOrder*> list;
    LOrder *com = nullptr;
    for (int i=0; i<10; i++)
    {
        com = new LOrder;
        com->id = i;
        com->setName(QString("指令 %1").arg(i));
        list.push_back(com);
    }
    data[1] = list;

    m_pTreeMode = new LTreeMode(data);
    ui->treeView->setModel(m_pTreeMode);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
}

LZWindow::~LZWindow()
{
    delete ui;
}

void LZWindow::init()
{
    m_pView->view("", Plugin::DataCenterPlugin()->getChartList(1));
    //Item *item = m_pView->addItem(LCType::LC_Node, QPoint(130,130));
    //item = m_pView->addItem(LCType::LC_Node, QPoint(130,130));
}


void LZWindow::createDockWindows()
{
    m_pView = new LZGraphicsView(this);
    m_pView->setFixedSize(600,600);
    m_pView->move(50,50);
    //this->setCentralWidget(m_pView);

    QDockWidget *dock;
    dock = new QDockWidget(tr("Customers"), this);
    //dock->setWidget(m_pView->getUndoView());
    dock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dock);


}

void LZWindow::on_pushButton_clicked()
{
    m_pView->addItem(LCType::LC_Node, QPoint(130,130));
}


void LZWindow::on_pushButton_2_clicked()
{
    m_pView->addItem(LCType::LC_Start, QPoint(130,130));
}


void LZWindow::on_pushButton_3_clicked()
{
    m_pView->addItem(LCType::LC_Finish, QPoint(130,130));
}


void LZWindow::on_pushButton_4_clicked()
{
    m_pView->startTest();
}

