#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pView = new LZGraphicsView(this);
    //setCentralWidget(m_pView);
    m_pView->resize(600,400);
    m_pView->move(30, 30);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    m_pView->addItem(Item::ItemType::Node, QPoint(130,130));
}


void MainWindow::on_pushButton_2_clicked()
{
    m_pView->addItem(Item::ItemType::Line, QPoint(130,130));
}

