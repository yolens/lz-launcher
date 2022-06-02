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
    }
    this->setStyleSheet(".DeviceCom{background-color: rgb(255, 170, 127);}");
    ui->textEdit_mark->setFixedHeight(60);
    this->setType(LDevice::Modbus);
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

    m_modbusDevice = new QModbusRtuSerialMaster(this);
    connect(m_modbusDevice, &QModbusClient::errorOccurred, this, &DeviceCom::on_errorOccurred);
    connect(m_modbusDevice, &QModbusClient::stateChanged,this, &DeviceCom::on_stateChanged);
}

DeviceCom::~DeviceCom()
{
    delete ui;
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

void DeviceCom::action(const LOrder* order)
{

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
        info->setType((Type)query.value(type).toInt());
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

bool DeviceCom::connectDevice()
{

    if (m_modbusDevice->state() != QModbusDevice::ConnectedState)
    {
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
            ui->lineEdit_port->text());
    #if QT_CONFIG(modbus_serialport)
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
            ParityM.value(ui->comboBox_parity->currentText()));
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
            BaudRateM.value(ui->comboBox_baud->currentText()));
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
            DataBitsM.value(ui->comboBox_data->currentText()));
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
            StopBitsM.value(ui->comboBox_stop->currentText()));
    #endif

        if (!m_modbusDevice->connectDevice())
        {
        //    QMessageBox::information(nullptr, "fail", "fail");
        }
    }
    else
    {
        m_modbusDevice->disconnectDevice();
    }
    return true;
}

void DeviceCom::on_errorOccurred(QModbusDevice::Error error)
{
    ui->label_message->setText(QString("%1,%2").arg(m_modbusDevice->errorString()).arg(error));
}

void DeviceCom::on_stateChanged(QModbusDevice::State state)
{
    switch (state)
    {
    case QModbusDevice::UnconnectedState:
        ui->label_message->setText("disconnect");
        ui->pushButton_connect->setText("connect");
        break;
    case QModbusDevice::ConnectingState:
        ui->label_message->setText("connecting");
        break;
    case QModbusDevice::ConnectedState:
        ui->label_message->setText("connected");
        ui->pushButton_connect->setText("disconnect");
        break;
    case QModbusDevice::ClosingState:
        ui->label_message->setText("closing");
        break;
    default:
        break;
    }

}

void DeviceCom::on_pushButton_connect_clicked()
{
    connectDevice();
}

void DeviceCom::write(const LOrder* order)
{
    if (nullptr == m_modbusDevice)
        return;
    char kk[10];
    kk[0] = 1;
    kk[1] = 0;
    kk[2] = 1;
    QModbusDataUnit writeUnit = writeRequest(order);
    QModbusDataUnit::RegisterType table = writeUnit.registerType();
    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
        if (table == QModbusDataUnit::Coils)
            writeUnit.setValue(i, kk[i]);
        else
            writeUnit.setValue(i, kk[i]);
    }

    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, 1)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    ui->label_message->setText(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                               .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
                } else if (reply->error() != QModbusDevice::NoError) {
                    ui->label_message->setText(tr("Write response error: %1 (code: 0x%2)").
                                               arg(reply->errorString()).arg(reply->error(), -1, 16));
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        ui->label_message->setText(tr("Write error: ") + m_modbusDevice->errorString());
    }
}
QModbusDataUnit DeviceCom::writeRequest(const LOrder* order) const
{
    const auto table =
        static_cast<QModbusDataUnit::RegisterType>(QModbusDataUnit::Coils);

    int startAddress = 0;
    Q_ASSERT(startAddress >= 0 && startAddress < 10);

    // do not go beyond 10 entries
    quint16 numberOfEntries = qMin((quint16)3, quint16(10 - startAddress));
    return QModbusDataUnit(table, startAddress, numberOfEntries);
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
}


void DeviceCom::on_pushButton_delete_clicked()
{
    write(nullptr);
    return;
    ModbusData::instance()->remove(this);
    emit updateView();
}

