#include "DeviceWorker.h"
#include <QModbusRtuSerialMaster>
#include <QEventLoop>
#include <QtDebug>
#include "LZLib.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

DeviceWorker::DeviceWorker(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<DeviceWorker::DeviceInfo>("DeviceWorker::DeviceInfo");
    m_thread = new QThread();
    this->moveToThread(m_thread);
}

DeviceWorker::~DeviceWorker()
{
    m_stop = true;
    m_sem.release();
    if (nullptr != m_thread)
    {
        m_thread->quit();
        m_thread->wait();
        delete m_thread;
        m_thread = nullptr;
    }
}

void DeviceWorker::createAction()
{
    if (nullptr == m_modbusDevice)
    {
        m_modbusDevice = new QModbusRtuSerialMaster(this);
        connect(m_modbusDevice, &QModbusClient::errorOccurred, this, &DeviceWorker::on_errorOccurred);
        connect(m_modbusDevice, &QModbusClient::stateChanged,this, &DeviceWorker::on_stateChanged);
        emit message(m_modbusDevice != nullptr ? "创建成功!" : "创建失败!");
    }
    else
    {
        emit message("已经创建过了!");
    }
    emit result(Result::CreateFinished);
}

void DeviceWorker::connectAction(const DeviceWorker::DeviceInfo& info)
{
    if (nullptr == m_modbusDevice)
    {
        emit message("设备还未创建!");
        return;
    }
    if (m_modbusDevice->state() != QModbusDevice::ConnectedState)
    {
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
            info.port);
    #if QT_CONFIG(modbus_serialport)
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
            info.parity);
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
            info.baudRate);
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
            info.dataBits);
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
            info.stopBits);
    #endif

        if (!m_modbusDevice->connectDevice())
        {
            emit message("设备连接失败!");
        }
    }
    else
    {
        m_modbusDevice->disconnectDevice();
    }
}

void DeviceWorker::sendAction()
{
    LOrder *item = takeOut();
    if (nullptr != item)
    {
        memset(&m_comData, 0, sizeof(LZ::ComData));
        if (nullptr != m_modbusDevice)
        {
            if (item->rwType() == LOrder::Write)
            {
                m_comData.success = write(item);
            }
            else
            {
                m_comData.success = read(item);
            }
        }
        else
        {
            m_comData.success = false;

            appendMessage("设备未创建");
            emit message("设备还未创建!");
        }

        if (item->valueCallback() != nullptr)
        {
            item->valueCallback()(m_comData);
        }
        delete item;
    }
}

void DeviceWorker::appendMessage(const QString& str)
{
    if (m_comData.message.isEmpty())
        m_comData.message.append(str);
    else
        m_comData.message.append(QString("\r\n%1").arg(str));
}

void DeviceWorker::on_start_work()
{
    m_stop = false;
    while (!m_stop)
    {
        m_sem.acquire();

        if (m_stop)
            break;

        switch (m_action)
        {
        case Action::Create_action:
            createAction();
            m_action = Action::Send_action;
            break;
        case Action::Connect_action:
            connectAction(m_info);
            m_action = Action::Send_action;
            break;
        case Send_action:
            sendAction();
            break;
        default:
            break;
        }

        QThread::msleep(1);
    }
  //  emit result(Result::FinishResult);

}

void DeviceWorker::on_errorOccurred(QModbusDevice::Error error)
{
    emit message(QString("%1,%2").arg(m_modbusDevice->errorString()).arg(error));
}
void DeviceWorker::on_stateChanged(QModbusDevice::State state)
{
    switch (state)
    {
    case QModbusDevice::UnconnectedState:
        emit message("断开连接!");
        emit result(Result::UnconnectedResult);
        break;
    case QModbusDevice::ConnectingState:
        emit message("连接中...!");
        break;
    case QModbusDevice::ConnectedState:
        emit message("已连接!");
        emit result(Result::ConnectedResult);
        break;
    case QModbusDevice::ClosingState:
        emit message("关闭中...!");
        break;
    default:
        break;
    }
}

void DeviceWorker::start()
{
    m_thread->start();
}

void DeviceWorker::actionDataStop()
{
    if (!m_stop)
    {
        m_stop = true;
        m_sem.release();
    }
    else
    {
        emit result(Result::FinishResult);
    }
}

void DeviceWorker::createDevice()
{
    m_action = Action::Create_action;
    m_sem.release();
}
void DeviceWorker::connectDevice(const DeviceWorker::DeviceInfo& info)
{
    m_info = info;
    m_action = Action::Connect_action;
    m_sem.release();
}



void DeviceWorker::addIn(LOrder* order)
{
    m_mutex.lock();
    m_queue.enqueue(order);
    m_mutex.unlock();
    m_sem.release();
}

LOrder* DeviceWorker::takeOut()
{
    LOrder* order = nullptr;
    m_mutex.lock();
    if (m_queue.size() > 0)
    {
        order = m_queue.dequeue();
    }
    m_mutex.unlock();
    return order;
}


bool DeviceWorker::read(LOrder* order)
{
    bool ret = false;
    if (auto *reply = m_modbusDevice->sendReadRequest(WriteReadRequest(order), order->serverAddress()))
    {
        if (!reply->isFinished())
        {
            QEventLoop loop;
            connect(this, &DeviceWorker::readFinished, &loop, &QEventLoop::quit);
            connect(reply, &QModbusReply::finished, this, [=, &ret]{
                auto reply = qobject_cast<QModbusReply *>(sender());
                if (!reply)
                {
                    appendMessage("reply is nullptr");
                    emit readFinished();
                    return;
                }

                if (reply->error() == QModbusDevice::NoError)
                {
                    const QModbusDataUnit unit = reply->result();
                    QModbusDataUnit::RegisterType table = unit.registerType();
                    if (QModbusDataUnit::HoldingRegisters == table || QModbusDataUnit::InputRegisters == table)
                    {
                        quint64 u64 = 0;
                        for (int i = 0, total = int(unit.valueCount()); i < total; ++i)
                        {
                            const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress() + i)
                                                     .arg(QString::number(unit.value(i)));

                            appendMessage(entry);

                            quint64 temp = unit.value(i);
                            u64 += temp << i*16;
                        }

                        m_comData.value = LZLib::instance()->fromLonglong(order->byteType(), QVariant(u64));
                        appendMessage(QString("原始值：%1， 转换值：%2").arg(u64).arg(m_comData.value.toString()));
                        appendMessage("read success!");
                        ret = true;
                    }
                    else
                    {
                        appendMessage("类型出错");
                    }

                }
                else if (reply->error() == QModbusDevice::ProtocolError)
                {
                    emit message(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                 arg(reply->errorString()).
                                 arg(reply->rawResult().exceptionCode(), -1, 16));
                    appendMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                  arg(reply->errorString()).
                                  arg(reply->rawResult().exceptionCode(), -1, 16));
                }
                else
                {
                    emit message(tr("Read response error: %1 (code: 0x%2)").
                                 arg(reply->errorString()).
                                 arg(reply->error(), -1, 16));
                    appendMessage(tr("Read response error: %1 (code: 0x%2)").
                                  arg(reply->errorString()).
                                  arg(reply->error(), -1, 16));
                }

                //reply->deleteLater();
                delete reply;
                emit readFinished();
            });

            loop.exec();
        }
        else
        {
            appendMessage(tr("broadcast replies return immediately"));
            delete reply; // broadcast replies return immediately
        }
    }
    else
    {
        appendMessage(tr("Read error: ") + m_modbusDevice->errorString());
        emit message(tr("Read error: ") + m_modbusDevice->errorString());
    }
    return ret;
}

bool DeviceWorker::write(LOrder* order)
{
    bool ret = false;
    QVariant writeValue = LZLib::instance()->toLonglong(order->byteType(), order->value());
    QModbusDataUnit writeUnit = WriteReadRequest(order);
    QModbusDataUnit::RegisterType table = writeUnit.registerType();
    if (QModbusDataUnit::Coils == table)
    {
        QString binary = QString::number(writeValue.toULongLong(), 2);
        qDebug()<<"binary value= " << binary;
        for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i)
        {
            quint16 uv = (binary.size() > i) ? (binary.at(binary.size()-i-1).toLatin1() - QChar('0').toLatin1()) : 0; //ascii
            writeUnit.setValue(i, uv);
        }
    }
    else
    {
        quint64 u64 = writeValue.toULongLong();
        for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i)
        {
            quint16 uv = u64 >> i*16;
            qDebug() << "q64 = " << u64 << uv;
            writeUnit.setValue(i, uv);
        }
    }


    if (auto *reply = m_modbusDevice->sendWriteRequest(writeUnit, order->serverAddress()))
    {
        if (!reply->isFinished())
        {
            QEventLoop loop;
            connect(this, &DeviceWorker::writeFinished, &loop, &QEventLoop::quit);
            connect(reply, &QModbusReply::finished, this, [this, reply, &ret](){
                if (reply->error() == QModbusDevice::ProtocolError)
                {
                    emit message(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                 .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
                    appendMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                  .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
                } else if (reply->error() != QModbusDevice::NoError)
                {
                    emit message(tr("Write response error: %1 (code: 0x%2)")
                                 .arg(reply->errorString()).arg(reply->error(), -1, 16));
                    appendMessage(tr("Write response error: %1 (code: 0x%2)")
                                 .arg(reply->errorString()).arg(reply->error(), -1, 16));
                }
                else
                {
                    appendMessage("write success!");
                    ret = true;
                }
                //reply->deleteLater();
                delete reply;
                emit writeFinished();
            });
            loop.exec();
        }
        else
        {
            appendMessage("broadcast replies return immediately");
            // broadcast replies return immediately
            //reply->deleteLater();
            delete reply;
        }
    }
    else
    {
        emit message(tr("Write error: ") + m_modbusDevice->errorString());
        appendMessage(tr("Write error: ") + m_modbusDevice->errorString());
    }
    return ret;
}
QModbusDataUnit DeviceWorker::WriteReadRequest(LOrder* order) const
{
    const auto table = static_cast<QModbusDataUnit::RegisterType>(order->registerType());

    int startAddress = order->startAddress();
    //Q_ASSERT(startAddress >= 0 && startAddress < 10);

    quint16 numberMax = 64;
    if (QModbusDataUnit::HoldingRegisters == table || QModbusDataUnit::InputRegisters == table)
        numberMax = 4;
    quint16 numberOfEntries = qMin((quint16)order->numberOfValues(), numberMax);
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}
