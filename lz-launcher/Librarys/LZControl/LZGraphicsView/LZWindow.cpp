#include "LZWindow.h"
#include "ui_LZWindow.h"
#include <QDockWidget>
#include <QToolBar>
#include <QComboBox>
#include "LZLib.h"
const int ZOOM_MAX = 180;
const int ZOOM_MIN = 20;
const int ZOOM_STEP = 10;

LZWindow::LZWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LZWindow)
{
    ui->setupUi(this);

    m_pView = new LZGraphicsView(this);
    connect(m_pView, &LZGraphicsView::sceneZoom, this, &LZWindow::on_sceneZoom);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);

    createMenus();
    createToolbars();
    createDockWindows();

    this->showMaximized();
}

LZWindow::~LZWindow()
{
    delete ui;
}

void LZWindow::init()
{
    m_pView->view("", Plugin::DataCenterPlugin()->getChartList(1));

    m_pTreeMode = new LTreeMode(Plugin::DataCenterPlugin()->getOrderList());
    ui->treeView->setModel(m_pTreeMode);

    /*foreach (QObject* obj, this->children())
    {
        if (obj->isWidgetType())
        {
            static_cast<QWidget*>(obj)->setEnabled(false);
        }
    }
    m_editToolBar->setEnabled(true);*/
}

void LZWindow::on_sceneZoom(const int value)
{
    QString scale = m_sceneScaleCombo->currentText();
    int current = scale.left(scale.indexOf(tr("%"))).toInt() + value*ZOOM_STEP;
    m_sceneScaleCombo->setCurrentText(tr("%1%").arg(current));
}

void LZWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
  //  m_fileMenu->addAction(exitAction);

    m_itemMenu = menuBar()->addMenu(tr("&Item"));
   // m_itemMenu->addAction(deleteAction);
    m_itemMenu->addSeparator();
  //  m_itemMenu->addAction(toFrontAction);
   // m_itemMenu->addAction(sendBackAction);

    m_aboutMenu = menuBar()->addMenu(tr("&Help"));
   // m_aboutMenu->addAction(aboutAction);
}

void LZWindow::createToolbars()
{
    QAction *nodeAction = new QAction(tr("&Node"), this);
    connect(nodeAction, &QAction::triggered, this, [=]{
        m_pView->addItem(LCType::LC_Node, QPoint(130,130));
    });

    QAction *startAction = new QAction(tr("&Start"), this);
    connect(startAction, &QAction::triggered, this, [=]{
        m_pView->addItem(LCType::LC_Start, QPoint(130,130));
    });

    QAction *finishAction = new QAction(tr("&Finish"), this);
    connect(finishAction, &QAction::triggered, this, [=]{
        m_pView->addItem(LCType::LC_Finish, QPoint(130,130));
    });

    QAction *testAction = new QAction(tr("&Test"), this);
    connect(testAction, &QAction::triggered, this, [=]{
        m_pView->startTest();
    });

    QAction *createAction = new QAction(tr("&Create"), this);
    connect(createAction, &QAction::triggered, this, [=]{
        LZWindow *w = new LZWindow();
        w->show();
    });

    m_editToolBar = addToolBar(tr("Edit"));
    m_editToolBar->addAction(nodeAction);
    m_editToolBar->addAction(startAction);
    m_editToolBar->addAction(finishAction);
    m_editToolBar->addAction(testAction);
    m_editToolBar->addAction(createAction);

    QComboBox *sceneScaleCombo = new QComboBox(this);
    m_sceneScaleCombo = sceneScaleCombo;
    QStringList scales;

    for (int i = ZOOM_MIN; i<= ZOOM_MAX; i+=ZOOM_STEP) {
        scales << tr("%1%").arg(i);
    }
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex((ZOOM_MAX-ZOOM_MIN)/ZOOM_STEP/2);
    connect(sceneScaleCombo, &QComboBox::currentTextChanged, this, [=](const QString &scale){
        double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
        QTransform oldMatrix = m_pView->transform();
        m_pView->resetTransform();
        m_pView->translate(oldMatrix.dx(), oldMatrix.dy());
        m_pView->scale(newScale, newScale);
    });

    /*QSlider *zoomSlider = new QSlider(Qt::Horizontal, this);
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(250);
    zoomSlider->setTickPosition(QSlider::TicksRight);*/

    m_pointerToolbar = addToolBar(tr("Pointer type"));
    m_pointerToolbar->addWidget(sceneScaleCombo);
    //m_pointerToolbar->addWidget(zoomSlider);
}


void LZWindow::createDockWindows()
{
    this->setCentralWidget(m_pView);

    QDockWidget *dock;
    dock = new QDockWidget(tr("Customers"), this);
    dock->setWidget(ui->treeView);
    dock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dock);

}



