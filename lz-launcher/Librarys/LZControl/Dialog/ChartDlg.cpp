#include "ChartDlg.h"
#include "ui_ChartDlg.h"
#include <QLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include "../LZGraphicsView/Items/Item.h"

ChartDlg::ChartDlg(QWidget *parent) :
    BaseDlg(parent),
    ui(new Ui::ChartDlg)
{
    ui->setupUi(this);

    this->setMinimumWidth(300);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    m_mainLayout = mainLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    this->setLayout(mainLayout);
    //mainLayout->setContentsMargins(0, 0, 0, 0);
   // mainLayout->setSpacing(5);



    this->adjustSize();
}

ChartDlg::~ChartDlg()
{
    delete ui;
}
void ChartDlg::setItem(Item *p)
{
    m_item = p;
    updateView();
}


void ChartDlg::updateView()
{
    std::function<void(QWidget *parent
                     , QGridLayout *grid
                     , const QString& name
                     , const QString& value
                     , const bool haveClick
                      )> addItemCB =
                [=](QWidget *parent
                  , QGridLayout *grid
                  , const QString& name
                  , const QString& value
                  , const bool haveClick
                   )
    {
        int row = grid->rowCount();

        QLabel *label = new QLabel(parent);
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        label->setText(name);
        grid->addWidget(label, row, 0);

        QHBoxLayout* layout = new QHBoxLayout();
        layout->setAlignment(Qt::AlignLeft);
        label = new QLabel(parent);
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        label->setText(value);
        layout->addWidget(label);
        if (haveClick)
        {
            QPushButton *btn = new QPushButton(parent);
            btn->setText("set");
            layout->addStretch();
            layout->addWidget(btn);
        }

        grid->addLayout(layout, row, 1);
    };


    if (nullptr != m_item)
    {
        QGroupBox *chartGroup = new QGroupBox(this);
        chartGroup->setTitle("Item");
        QGridLayout *chartLayout = new QGridLayout(chartGroup);
        chartLayout->setAlignment(Qt::AlignLeft);
        chartGroup->setLayout(chartLayout);

        addItemCB(chartGroup, chartLayout, "类型:", m_item->getTypeName(), false);
        if (nullptr != m_item->getChart())
        {
            addItemCB(chartGroup, chartLayout, "名称:", m_item->getChart()->m_name, true);
            addItemCB(chartGroup, chartLayout, "值:", m_item->getChart()->m_value.toString(), true);
        }

        m_mainLayout->addWidget(chartGroup);
    }


}
