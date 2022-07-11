#include "ChartDlg.h"
#include "ui_ChartDlg.h"
#include <QLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "LZLib.h"
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

#include <QDebug>
#include <QSpinBox>

void ChartDlg::updateView()
{
    m_valueSetList.clear();


    std::function<void(const QString& title, QGroupBox** gb, QGridLayout** gl)> createGroup = [=](const QString& title, QGroupBox** gb, QGridLayout** gl){
        QGroupBox *group = new QGroupBox(this);
        group->setTitle(title);
        QGridLayout *grid = new QGridLayout(group);
        grid->setAlignment(Qt::AlignLeft);
        group->setLayout(grid);
        *gb = group;
        *gl = grid;
    };

    std::function<void(QGridLayout*, const QString&, QWidget*)> addItem = [=](QGridLayout* grid, const QString& name, QWidget* widget){
        int row = grid->rowCount();

        QLabel *label = new QLabel(this);
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        label->setText(name);
        grid->addWidget(label, row, 0);

        QHBoxLayout* layout = new QHBoxLayout();
        layout->setAlignment(Qt::AlignLeft);
        layout->addWidget(widget);

        grid->addLayout(layout, row, 1);
    };

    QWidget *infoWidget = new QWidget(this);
    QVBoxLayout* infoLayout = new QVBoxLayout();
    infoLayout->setAlignment(Qt::AlignTop);
    infoWidget->setLayout(infoLayout);


    if (nullptr != m_item)
    {
        std::function<void()> setValueCB;
        QLineEdit *lineEdit;
        QSpinBox *spinBox;
       // QDoubleSpinBox *doubleSpinBox;

        QGroupBox *group;
        QGridLayout *grid;
        createGroup("Chart", &group, &grid);

        lineEdit = new QLineEdit(this); lineEdit->setText(m_item->getTypeName()); lineEdit->setEnabled(false);
        addItem(grid, "类型:", lineEdit);

        LChart *pChart = m_item->getChart();
        if (nullptr != pChart)
        {
            lineEdit = new QLineEdit(this);
            lineEdit->setText(pChart->m_name);
            setValueCB = [=]{pChart->m_name = lineEdit->text();};
            m_valueSetList.push_back(setValueCB);
            addItem(grid, "名称:", lineEdit);

            /*doubleSpinBox = new QDoubleSpinBox(this);
            doubleSpinBox->setRange(-qInf(), qInf());
            doubleSpinBox->setValue(pChart->m_value.toDouble());
            setValueCB = [=]{pChart->m_value = doubleSpinBox->value();};
            m_valueSetList.push_back(setValueCB);
            addItem(grid, "值:", doubleSpinBox);*/
            LOrder *pOrder = m_item->getOrder();
            if (nullptr != pOrder)
            {
                lineEdit = new QLineEdit(this);
                connect(lineEdit, &QLineEdit::editingFinished, this, [=](){
                    QVariant u64 = lineEdit->text();
                    u64 = LZLib::instance()->toLonglong(pOrder->byteType(), u64);
                    u64 = LZLib::instance()->fromLonglong(pOrder->byteType(), u64);
                    lineEdit->setText(u64.toString());
                });
                lineEdit->setText(pChart->m_value.toString());
                setValueCB = [=]{
                    pChart->m_value = lineEdit->text();
                };
                m_valueSetList.push_back(setValueCB);
                addItem(grid, "值:", lineEdit);
            }


            spinBox = new QSpinBox(this);
            spinBox->setRange(0, std::numeric_limits<int>::max());
            spinBox->setValue(m_item->getChart()->m_delay);
            setValueCB = [=]{m_item->getChart()->m_delay = spinBox->value();};
            m_valueSetList.push_back(setValueCB);
            addItem(grid, "延迟(ms):", spinBox);

        }
        infoLayout->addWidget(group);

        if (m_item->getItemType() == LCType::LC_Branch)
        {
            createGroup("Point", &group, &grid);

            foreach(const auto& p, m_item->getPointList())
            {
                if (p->attribute == LPAttribute::output)
                {
                    QWidget *w = new QWidget(this);
                    QHBoxLayout* layout = new QHBoxLayout();
                    layout->setAlignment(Qt::AlignLeft);
                    w->setLayout(layout);

                    lineEdit = new QLineEdit(this);
                    connect(lineEdit, &QLineEdit::editingFinished, this, [=](){
                        QVariant u64 = lineEdit->text();
                        LOrder::ByteType bt = LOrder::DEC;
                        if (u64.toString().contains("."))
                            bt = LOrder::Float;
                        u64 = LZLib::instance()->toLonglong(bt, u64);
                        u64 = LZLib::instance()->fromLonglong(bt, u64);
                        lineEdit->setText(u64.toString());
                    });
                    lineEdit->setText(p->minValue.toString());
                    setValueCB = [=]{
                        p->minValue = lineEdit->text();
                    };
                    m_valueSetList.push_back(setValueCB);
                    layout->addWidget(lineEdit);

                    QLabel *label = new QLabel(this);
                    label->setText("<= value <=");
                    layout->addWidget(label);

                    lineEdit = new QLineEdit(this);
                    connect(lineEdit, &QLineEdit::editingFinished, this, [=](){
                        QVariant u64 = lineEdit->text();
                        LOrder::ByteType bt = LOrder::DEC;
                        if (u64.toString().contains("."))
                            bt = LOrder::Float;
                        u64 = LZLib::instance()->toLonglong(bt, u64);
                        u64 = LZLib::instance()->fromLonglong(bt, u64);
                        lineEdit->setText(u64.toString());
                    });
                    lineEdit->setText(p->maxValue.toString());
                    setValueCB = [=]{
                        p->maxValue = lineEdit->text();
                    };
                    m_valueSetList.push_back(setValueCB);
                    layout->addWidget(lineEdit);

                    addItem(grid, QString("P%1:").arg(m_item->getPointList().indexOf(p)), w);
                }
            }

            infoLayout->addWidget(group);
        }
    }
    m_mainLayout->addWidget(infoWidget);
    infoWidget->setEnabled(false);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignRight);
    m_mainLayout->addLayout(buttonLayout);
    QPushButton *editButton = new QPushButton(this);
    editButton->setCheckable(true);
    connect(editButton, &QPushButton::clicked, this, [=](bool checked){
        infoWidget->setEnabled(!checked);
        editButton->setText(checked ? "edit" : "save");

        foreach(const auto& cb, m_valueSetList)
        {
            cb();
        }
        m_item->updateData();
    });
    editButton->setText("edit");
    editButton->setChecked(true);
    buttonLayout->addWidget(editButton);

    this->adjustSize();

}
