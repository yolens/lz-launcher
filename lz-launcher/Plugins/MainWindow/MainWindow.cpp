#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "IPlugin.h"
#include "LZLib.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pWindow = new LZWindow();
    m_pWindow->resize(1024, 768);
    m_pWindow->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    m_pWindow->init();
}

void MainWindow::on_pushButton_clicked()
{
    m_pWindow->show();
}

#include "ComsManagerDlg.h"
void MainWindow::on_pushButton_2_clicked()
{
    ComsManagerDlg dlg;
    dlg.init();
    dlg.exec();
}

