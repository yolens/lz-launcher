#include "MainForm.h"
#include "ui_MainForm.h"
#include "LZLib.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);

    ui->pushButton_coms->setFixedSize(180, 180);
    ui->pushButton_views->setFixedSize(180, 180);


}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::init()
{
     std::function<void()> callback = [=](){
         QString mes;
         QMap<LDevice::DeviceState, int> stateList = Plugin::DataCenterPlugin()->deviceStateList();
         mes += tr("连接成功:%1").arg(stateList.value(LDevice::DeviceState::connectSuc));
         mes += "\r\n";
         mes += tr("连接失败:%1").arg(stateList.value(LDevice::DeviceState::connectFail));
         ui->pushButton_coms->setText(mes);
     };
     callback();
     Plugin::DataCenterPlugin()->registerDeviceState(callback);
}

void MainForm::on_pushButton_views_clicked()
{
    emit action(MainForm::Views);
}

void MainForm::on_pushButton_coms_clicked()
{
    emit action(MainForm::Coms);
}
