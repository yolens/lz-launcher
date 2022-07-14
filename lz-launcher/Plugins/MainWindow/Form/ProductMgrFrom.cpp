#include "ProductMgrFrom.h"
#include "ui_ProductMgrFrom.h"
#include "LZLib.h"
#include <QPushButton>

ProductMgrFrom::ProductMgrFrom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductMgrFrom)
{
    ui->setupUi(this);
    m_productGroup = new QButtonGroup(this);
    m_unitGroup = new QButtonGroup(this);
    m_addProductCard = new AddCard();
    m_addProductCard->setMinimumSize(100, 100);
    connect(m_addProductCard, &Card::add_action, this, [=](){
        LProduct *newp = new LProduct;
        Plugin::DataCenterPlugin()->insertProduct(newp);
        newp->name = QString("product%1").arg(newp->id);
        Plugin::DataCenterPlugin()->updateProduct(newp);
        addProductCard(newp);
        adjustProductView();
    });

    m_addUnitCard = new AddCard();
    m_addUnitCard->setMinimumSize(100, 100);
    connect(m_addUnitCard, &Card::add_action, this, [=](){

        Card *card = dynamic_cast<Card*>(m_productGroup->checkedButton());
        if (nullptr == card)
            return;
        LUnit *unit = new LUnit;
        unit->productId = card->id();
        Plugin::DataCenterPlugin()->insertUnit(unit);
        unit->name = QString("uint%1").arg(unit->id);
        Plugin::DataCenterPlugin()->updateUnit(unit);
        addUnitCard(unit);
        adjustUnitView();
    });

    connect(ui->splitter, &QSplitter::splitterMoved, this, [=]{
        adjustProductView();
        adjustUnitView();
    });


}

ProductMgrFrom::~ProductMgrFrom()
{
    delete ui;
}

void ProductMgrFrom::init()
{
    foreach (auto &&p, Plugin::DataCenterPlugin()->getProductList())
    {
        addProductCard(p);
    }
    adjustProductView();

}

void ProductMgrFrom::addUnitCard(LUnit* p)
{
    Card *c = new NodeCard();
    c->setMinimumSize(100, 100);
    m_unitGroup->addButton(c);
    c->setId(p->id);
    c->setName(p->name);

    connect(c, &Card::name_change_action, this, [=](const QString& text){
        p->name = text;
        Plugin::DataCenterPlugin()->updateUnit(p);
    });

    connect(c, &Card::start_action, this, [=]{
        createWindow();
        m_pWindow->init(p, Plugin::DataCenterPlugin()->getProduct(p->productId));
    });

    connect(c, &Card::close_action, this, [=]{
        Card *uc = qobject_cast<Card*>(QObject::sender());
        if (nullptr == uc)
            return;
        Plugin::DataCenterPlugin()->removeUnit(Plugin::DataCenterPlugin()->getUnit(uc->id()));
        m_unitGroup->removeButton(uc);
        delete uc;

        adjustUnitView();
    });
}
#include <QDebug>
void ProductMgrFrom::createWindow()
{
    if (nullptr != m_pWindow)
    {
        qInfo() << "BBBBBBBBBBBBBBBBBBBBBBBBB";
        delete m_pWindow;
    }
    m_pWindow = new LZWindow();
    connect(m_pWindow, &LZWindow::destroyWindow, this, [=]{
        m_pWindow = nullptr;
        qInfo() << "AAAAAAAAAAAAAAAAAAAAA";
    });
    m_pWindow->show();
}

void ProductMgrFrom::addProductCard(LProduct* p)
{
    Card *c = new NodeCard();
    c->setMinimumSize(100, 100);
    m_productGroup->addButton(c);
    c->setId(p->id);
    c->setName(p->name);

    connect(c, &Card::start_action, this, [=]{
        createWindow();
        m_pWindow->init(p);
    });
    connect(c, &Card::name_change_action, this, [=](const QString& text){
        p->name = text;
        Plugin::DataCenterPlugin()->updateProduct(p);
    });
    connect(c, &Card::selected_action, this, [=]{
        QList<QAbstractButton *> buttonList = m_unitGroup->buttons();
        foreach (const auto& card, buttonList)
        {
            m_unitGroup->removeButton(c);
            delete card;
        }

        QList<LUnit*> unitList = Plugin::DataCenterPlugin()->getUnitList(p->id);
        foreach (const auto& unit, unitList)
        {
            if (unit->id != p->mainUnitId)
                addUnitCard(unit);
        }
        adjustUnitView();

    });

    connect(c, &Card::close_action, this, [=]{
        Card *pc = qobject_cast<Card*>(QObject::sender());
        if (nullptr == pc)
            return;
        Plugin::DataCenterPlugin()->removeProduct(Plugin::DataCenterPlugin()->getProduct(pc->id()));
        m_productGroup->removeButton(pc);
        delete pc;

        QList<QAbstractButton *> buttonList = m_unitGroup->buttons();
        foreach (const auto& uc, buttonList)
        {
            m_unitGroup->removeButton(uc);
            delete uc;
        }

        adjustUnitView();
        adjustProductView();
    });

}

void ProductMgrFrom::adjustUnitView()
{
    QList<QAbstractButton *> buttonList = m_unitGroup->buttons();
    if (nullptr == m_productGroup->checkedButton())
        m_addUnitCard->hide();
    else
        buttonList.push_back(m_addUnitCard);

    QList<QWidget*> wList;
    foreach (const auto& b, buttonList)
    {
        wList.append(b);
    }
    ui->srollArea_unit->view(wList);

}


void ProductMgrFrom::adjustProductView()
{
    QList<QAbstractButton *> buttonList = m_productGroup->buttons();
    buttonList.push_back(m_addProductCard);
    QList<QWidget*> wList;
    foreach (const auto& b, buttonList)
    {
        wList.append(b);
    }
    ui->srollArea_product->view(wList);
}





