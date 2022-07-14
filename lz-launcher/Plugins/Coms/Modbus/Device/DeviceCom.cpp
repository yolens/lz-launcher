#include "DeviceCom.h"
#include "ui_DeviceCom.h"
#include <QtSql>
#include "ModbusData.h"
#include <QModbusRtuSerialMaster>
#include <QSerialPort>
#include <QMessageBox>
#include <QStyleOption>
#include <QPainter>

DeviceCom::DeviceCom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceCom)
{
    ui->setupUi(this);

    static bool initialized = false;
    if (!initialized)
    {
        initialized = true;

        QSqlQuery query;
        query.exec(CREATE_SQL_Device_Modbus);
        foreach (QString col, ALTER_Device_Modbus_LIST)
        {
            QString cmd = ALTER_SQL_Device_Modbus.arg(col);
            query.exec(cmd);
        }
    }
    changeDeviceState(LDevice::DeviceState::disenabled);
    ui->textEdit_mark->setFixedHeight(60);
    this->setType(LOrder::Modbus);
    foreach (const auto& i, ParityL)
    {
        ui->comboBox_parity->addItem(i);
    }
    foreach (const auto& i, BaudRateL)
    {
        ui->comboBox_baud->addItem(i);
    }
    foreach (const auto& i, DataBitsL)
    {
        ui->comboBox_data->addItem(i);
    }
    foreach (const auto& i, StopBitsL)
    {
        ui->comboBox_stop->addItem(i);
    }

    connect(ui->checkBox_enable, &QCheckBox::toggled, this, [=]{
        if (ui->checkBox_enable->isChecked())
        {
            changeDeviceState(LDevice::DeviceState::connectFail);
            ui->pushButton_connect->setEnabled(true);
        }
        else
        {
            changeDeviceState(LDevice::DeviceState::disenabled);
            ui->pushButton_connect->setEnabled(false);
        }
    });

    m_worker = new DeviceWorker();
    connect(this, &DeviceCom::start_work, m_worker, &DeviceWorker::on_start_work);
    connect(m_worker, &DeviceWorker::message, this, &DeviceCom::on_message);
    connect(m_worker, &DeviceWorker::result, this, &DeviceCom::on_result);
    m_worker->start();

    emit start_work();
   // ui->comboBox_parity->setItemData(ui->comboBox_parity->findText("Even"), false, Qt::UserRole-1);

}

DeviceCom::~DeviceCom()
{
    if (nullptr != m_worker)
    {
        delete m_worker;
        m_worker = nullptr;
    }
    delete ui;
}

LDevice::DeviceState DeviceCom::deviceState()
{
    return m_deviceState;
}

void DeviceCom::changeDeviceState(const LDevice::DeviceState state)
{
    m_deviceState = state;
    switch (state)
    {
    case LDevice::DeviceState::connectSuc:
        this->setStyleSheet(".DeviceCom{background-color:rgb(0,255,0);}");
        break;
    case LDevice::DeviceState::connectFail:
        this->setStyleSheet(".DeviceCom{background-color:rgb(255,0,0);}");
        break;
    case LDevice::DeviceState::disenabled:
        this->setStyleSheet(".DeviceCom{background-color:rgb(130,130,130);}");
        break;
    default:
        break;
    }
    Plugin::DataCenterPlugin()->noticeDeviceState();
}

void DeviceCom::on_result(const int code)
{
    DeviceWorker::Result result = (DeviceWorker::Result)code;
    switch (result)
    {
    case DeviceWorker::CreateFinished:
    {
        DeviceWorker::DeviceInfo info;
        info.port = this->port();
        info.parity = ParityM.value(this->parity());
        info.baudRate = BaudRateM.value(this->baudRate());
        info.dataBits = DataBitsM.value(this->dataBits());
        info.stopBits = StopBitsM.value(this->stopBits());
        m_worker->connectDevice(info);
    }
        break;
    case DeviceWorker::UnconnectedResult:
    {
        changeDeviceState(LDevice::DeviceState::connectFail);
        foreach (QObject* obj, this->children())
        {
            if (obj->isWidgetType())
            {
                static_cast<QWidget*>(obj)->setEnabled(true);
            }
        }
        ui->pushButton_connect->setText("连接");
    }
        break;
    case DeviceWorker::ConnectedResult:
    {
        changeDeviceState(LDevice::DeviceState::connectSuc);

        foreach (QObject* obj, this->children())
        {
            if (obj->isWidgetType())
            {
                static_cast<QWidget*>(obj)->setEnabled(false);
            }
        }
        ui->pushButton_connect->setEnabled(true);
        ui->pushButton_connect->setText("断开");
        //emit action_data();
    }
        break;
    case DeviceWorker::FinishResult:
    {
        //emit create_device();

    }
        break;
    default:
        break;
    }
}

void DeviceCom::on_message(const QString& msg)
{
    qDebug() << "recv: " << msg << QThread::currentThreadId();
    ui->label_message->setText(msg);
}

void DeviceCom::init()
{
    ui->lineEdit_name->setText(this->name());
    ui->lineEdit_port->setText(this->port());
    ui->comboBox_parity->setCurrentText(this->parity());
    ui->comboBox_baud->setCurrentText(this->baudRate());
    ui->comboBox_data->setCurrentText(this->dataBits());
    ui->comboBox_stop->setCurrentText(this->stopBits());
    ui->textEdit_mark->setText(this->mark()); 
    ui->checkBox_enable->setChecked(this->useEnable());
    emit ui->checkBox_enable->toggled(true);
}

void DeviceCom::connectDevice()
{
    on_pushButton_connect_clicked();
}

bool DeviceCom::execute(LOrder* order)
{
    m_worker->addIn(order);
    return true;
}

bool DeviceCom::loadDb()
{
    QList<LDevice*> list;
    QSqlQuery query;
    if (!query.exec(SELECT_SQL_Device_Modbus))
        return false;
    QSqlRecord rec = query.record();
    int id = rec.indexOf("id");
    int type = rec.indexOf("type");
    int name = rec.indexOf("name");
    int mark = rec.indexOf("mark");
    int port = rec.indexOf("port");
    int parity = rec.indexOf("parity");
    int baudRate = rec.indexOf("baudRate");
    int dataBits = rec.indexOf("dataBits");
    int stopBits = rec.indexOf("stopBits");
    int useEnable = rec.indexOf("useEnable");

    while (query.next())
    {
        DeviceCom *info = new DeviceCom;
        info->id = query.value(id).toInt();
        info->setType((LOrder::Type)query.value(type).toInt());
        info->setName(query.value(name).toString());
        info->setMark(query.value(mark).toString());
        info->setPort(query.value(port).toString());
        info->setParity(query.value(parity).toString());
        info->setBaudRate(query.value(baudRate).toString());
        info->setDataBits(query.value(dataBits).toString());
        info->setStopBits(query.value(stopBits).toString());
        info->setUseEnable(query.value(useEnable).toBool());
        list.push_back(info);
    }
    ModbusData::instance()->setDeviceList(list);
    return true;
}


bool DeviceCom::updateDb()
{
    QSqlQuery query;
    if (!query.prepare(UPDATE_SQL_Device_Modbus))
        return false;
    bindValue(query);
    query.bindValue(":id",this->id);
    if (!query.exec())
        return false;
    return true;
}
bool DeviceCom::insertDb()
{
    QSqlQuery query;
    if (!query.prepare(INSERT_SQL_Device_Modbus))
        return false;

    bindValue(query);
    if (!query.exec())
        return false;
    this->id  = query.lastInsertId().toInt();
    return true;
}
bool DeviceCom::removeDb()
{
    QSqlQuery query;
    if (!query.prepare(DELETE_SQL_Device_Modbus))
        return false;
    query.bindValue(":id", this->id);
    if (!query.exec())
        return false;
    return true;
}

bool DeviceCom::bindValue(QSqlQuery& query)
{
    query.bindValue(":type", this->type());
    query.bindValue(":name", this->name());
    query.bindValue(":mark", this->mark());
    query.bindValue(":port", this->port());
    query.bindValue(":parity", this->parity());
    query.bindValue(":baudRate", this->baudRate());
    query.bindValue(":dataBits", this->dataBits());
    query.bindValue(":stopBits", this->stopBits());
    query.bindValue(":useEnable", this->useEnable());
    return true;
}

QString DeviceCom::port()
{
    return m_port;
}
QString DeviceCom::parity()
{
    return m_parity;
}
QString DeviceCom::baudRate()
{
    return m_baudRate;
}
QString DeviceCom::dataBits()
{
    return m_dataBits;
}
QString DeviceCom::stopBits()
{
    return m_stopBits;
}
bool DeviceCom::useEnable()
{
    return m_useEnable;
}
void DeviceCom::setPort(const QString& value)
{
    m_port = value;
}
void DeviceCom::setParity(const QString& value)
{
    m_parity = value;
}
void DeviceCom::setBaudRate(const QString& value)
{
    m_baudRate = value;
}
void DeviceCom::setDataBits(const QString& value)
{
    m_dataBits = value;
}
void DeviceCom::setStopBits(const QString& value)
{
    m_stopBits = value;
}

void DeviceCom::setUseEnable(const bool value)
{
    m_useEnable = value;
}

void DeviceCom::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}


void DeviceCom::on_pushButton_connect_clicked()
{
    if (ui->checkBox_enable->isChecked())
    {
        m_worker->createDevice();
    }
}


void DeviceCom::on_pushButton_save_clicked()
{
    this->setName(ui->lineEdit_name->text());
    this->setPort(ui->lineEdit_port->text());
    this->setParity(ui->comboBox_parity->currentText());
    this->setBaudRate(ui->comboBox_baud->currentText());
    this->setDataBits(ui->comboBox_data->currentText());
    this->setStopBits(ui->comboBox_stop->currentText());
    this->setMark(ui->textEdit_mark->document()->toPlainText());
    this->setUseEnable(ui->checkBox_enable->isChecked());
    ModbusData::instance()->update(this);
    emit changeDeviceName();
}


void DeviceCom::on_pushButton_delete_clicked()
{
    if (ModbusData::instance()->remove(this))
    {
        emit updateView();
        delete this;
    }
}


