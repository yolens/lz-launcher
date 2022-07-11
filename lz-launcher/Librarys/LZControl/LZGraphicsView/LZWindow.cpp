#include "LZWindow.h"
#include "ui_LZWindow.h"
#include <QDockWidget>
#include <QToolBar>
#include <QComboBox>
#include "LZLib.h"
#include "Drag/ChartDrag.h"
#include "Drag/PointDrag.h"
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

    ui->treeView_2->setHeaderHidden(true);
    ui->treeView_2->setSelectionMode(QAbstractItemView::SingleSelection);

    createMenus();
    createToolbars();
    createDockWindows();


}

LZWindow::~LZWindow()
{
    delete ui;
}

void LZWindow::init(LProduct *p)
{
    m_pProduct = p;
    if (!Plugin::DataCenterPlugin()->hasUnit(p->mainUnitId))
    {
        LUnit *unit = new LUnit;
        unit->productId = p->id;
        Plugin::DataCenterPlugin()->insertUnit(unit);
        p->mainUnitId = unit->id;
        Plugin::DataCenterPlugin()->updateProduct(p);
    }
    LUnit *uint = Plugin::DataCenterPlugin()->getUnit(p->mainUnitId);
    uint->name = p->name;
    init(uint, p);

    /*foreach (QObject* obj, this->children())
    {
        if (obj->isWidgetType())
        {
            static_cast<QWidget*>(obj)->setEnabled(false);
        }
    }
    m_editToolBar->setEnabled(true);*/
}

void LZWindow::init(LUnit *unit, LProduct *product)
{
    m_pProduct = product;
    m_pUnit = unit;
    this->setWindowTitle(unit->name);
    m_pView->view(unit);

    m_pTreeMode = new LTreeMode(Plugin::DataCenterPlugin()->getOrderList());
    ui->treeView->setModel(m_pTreeMode);

    QList<LUnit*> unitList = Plugin::DataCenterPlugin()->getUnitList(unit->productId);
    QList<LUnit*> removeList;
    foreach (const auto& u, unitList)
    {
        if (u->id == unit->id || product->mainUnitId == u->id)
            removeList.append(u);
    }
    foreach (const auto& u, removeList)
    {
        unitList.removeAll(u);
    }

    m_pUnitTreeMode = new UnitTreeMode(unitList);
    ui->treeView_2->setModel(m_pUnitTreeMode);

    this->showMaximized();
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

#include <QLayout>
void LZWindow::createToolbars()
{
    ChartDrag *nodeAction = new ChartDrag(m_editToolBar);
    nodeAction->setText("NodeItem");
    nodeAction->setType(LCType::LC_Node);

    ChartDrag *startAction = new ChartDrag(m_editToolBar);
    startAction->setText("StartItem");
    startAction->setType(LCType::LC_Start);

    ChartDrag *finishAction = new ChartDrag(m_editToolBar);
    finishAction->setText("FinishItem");
    finishAction->setType(LCType::LC_Finish);

    ChartDrag *threadAction = new ChartDrag(m_editToolBar);
    threadAction->setText("ThreadItem");
    threadAction->setType(LCType::LC_Thread);

    ChartDrag *branchAction = new ChartDrag(m_editToolBar);
    branchAction->setText("BranchItem");
    branchAction->setType(LCType::LC_Branch);

    PointDrag *pointAction = new PointDrag(m_editToolBar);
    pointAction->setText("PointItem");


    QAction *testAction = new QAction(tr("&Test"), this);
    connect(testAction, &QAction::triggered, this, [=]{
        m_pView->startTest();
    });

    QAction *stopAction = new QAction(tr("&Stop"), this);
    connect(stopAction, &QAction::triggered, this, [=]{
        m_pView->stopTest();
    });


    m_editToolBar = addToolBar(tr("Edit"));
    m_editToolBar->layout()->setSpacing(20);
    m_editToolBar->addWidget(nodeAction);
    m_editToolBar->addWidget(startAction);
    m_editToolBar->addWidget(finishAction);
    m_editToolBar->addWidget(threadAction);
    m_editToolBar->addWidget(branchAction);
    m_editToolBar->addWidget(pointAction);
    m_editToolBar->addAction(testAction);
    m_editToolBar->addAction(stopAction);


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

#include <QPushButton>
void LZWindow::createDockWindows()
{
    this->setCentralWidget(m_pView);

    QDockWidget *dock;
    dock = new QDockWidget(tr("Orders"), this);
    dock->setWidget(ui->treeView);
    dock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    dock = new QDockWidget(tr("Units"), this);
    dock->setWidget(ui->treeView_2);
    dock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}



