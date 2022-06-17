#include "DeviceCom.h"
#include "ui_DeviceCom.h"
#include <QtSql>
#include "ModbusData.h"
#include <QModbusRtuSerialMaster>
#include <QSerialPort>
#include <QMessageBox>
#include <QStyleOption>
#include <QPainter>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

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
    this->setStyleSheet(".DeviceCom{background-color: rgb(255, 170, 127);}");
    ui->textEdit_mark->setFixedHeight(60);
    this->setType(LOrder::Modbus);
    foreach (auto i, ParityL)
    {
        ui->comboBox_parity->addItem(i);
    }
    foreach (auto i, BaudRateL)
    {
        ui->comboBox_baud->addItem(i);
    }
    foreach (auto i, DataBitsL)
    {
        ui->comboBox_data->addItem(i);
    }
    foreach (auto i, StopBitsL)
    {
        ui->comboBox_stop->addItem(i);
    }

    m_worker = new DeviceWorker();
    connect(this, &DeviceCom::create_device, m_worker, &DeviceWorker::on_create_device);
    connect(this, &DeviceCom::connect_device, m_worker, &DeviceWorker::on_connect_device);
    connect(this, &DeviceCom::action_data, m_worker, &DeviceWorker::on_action_data);
    connect(m_worker, &DeviceWorker::message, this, &DeviceCom::on_message);
    connect(m_worker, &DeviceWorker::result, this, &DeviceCom::on_result);
    m_worker->start();


    emit create_device();

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

void DeviceCom::on_result(const int code)
{
    DeviceWorker::Result result = (DeviceWorker::Result)code;
    switch (result)
    {
    case DeviceWorker::UnconnectedResult:
        ui->pushButton_connect->setText("连接");
        break;
    case DeviceWorker::ConnectedResult:
        ui->pushButton_connect->setText("断开");
        emit action_data();
        break;
    case DeviceWorker::FinishResult:
    {
        DeviceWorker::DeviceInfo info;
        info.port = this->port();
        info.parity = ParityM.value(this->parity());
        info.baudRate = BaudRateM.value(this->baudRate());
        info.dataBits = DataBitsM.value(this->dataBits());
        info.stopBits = StopBitsM.value(this->stopBits());
        emit connect_device(info);
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
}

void DeviceCom::execute(LOrder* order)
{
   // emit action_data();
   // m_orderList.push_back(order);
    m_worker->addIn(order);

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
    m_worker->actionDataStop();

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

