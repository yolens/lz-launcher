﻿#include "MainWindow.h"
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

    connect(ui->page_main, &MainForm::action, this, &MainWindow::on_action);
    switchPage(ui->page_main);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    m_pWindow->init();
    ui->page_comsMgr->init();
}

void MainWindow::on_action(const int type)
{
    MainForm::PageType pageType = (MainForm::PageType)type;
    switch (pageType)
    {
    case MainForm::Views:
        m_pWindow->show();
        break;
    case MainForm::Coms:
        switchPage(ui->page_comsMgr);
        break;
    default:
        switchPage(ui->page_main);
        break;
    }
}

void MainWindow::switchPage(QWidget *w)
{
    ui->stackedWidget->setCurrentWidget(w);
    if (w == ui->page_main)
        ui->pushButton_back->hide();
    else
        ui->pushButton_back->show();
}

void MainWindow::on_pushButton_back_clicked()
{
    switchPage(ui->page_main);
}
