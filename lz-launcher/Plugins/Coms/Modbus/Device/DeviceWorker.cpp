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

void DeviceWorker::on_create_device()
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

void DeviceWorker::on_connect_device(const DeviceWorker::DeviceInfo& info)
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

void DeviceWorker::on_action_data()
{
    if (nullptr == m_modbusDevice)
    {
        emit message("设备还未创建!");
        return;
    }
    m_stop = false;
    while (!m_stop)
    {
        m_sem.acquire();
        if (m_stop)
            break;
        LOrder *item = takeOut();
        if (nullptr != item)
        {
            if (item->rwType() == LOrder::Write)
            {
                write(item);
            }
            else
            {
                read(item);
            }
            if (item->valueCallback() != nullptr)
                item->valueCallback()(item->value());
            delete item;
        }
        QThread::msleep(1);
    }
    emit result(Result::FinishResult);
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

void DeviceWorker::on_read_ready()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
    {
        emit readFinished();
        return;
    }

    if (reply->error() == QModbusDevice::NoError)
    {
        quint64 u64 = 0;
        const QModbusDataUnit unit = reply->result();
        QModbusDataUnit::RegisterType table = unit.registerType();
        if (QModbusDataUnit::HoldingRegisters == table || QModbusDataUnit::InputRegisters == table)
        {
            for (int i = 0, total = int(unit.valueCount()); i < total; ++i)
            {
                const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress() + i)
                                         .arg(QString::number(unit.value(i)));
                qDebug() << "KKK= " << entry;

                quint64 temp = unit.value(i);
                u64 += temp << i*16;
            }
            qDebug() << "KKK33= " <<u64;
        }
        else
        {

        }

    }
    else if (reply->error() == QModbusDevice::ProtocolError)
    {
        emit message(tr("Read response error: %1 (Mobus exception: 0x%2)").
                     arg(reply->errorString()).
                     arg(reply->rawResult().exceptionCode(), -1, 16));
    }
    else
    {
        emit message(tr("Read response error: %1 (code: 0x%2)").
                     arg(reply->errorString()).
                     arg(reply->error(), -1, 16));
    }

    reply->deleteLater();
    emit readFinished();
}

void DeviceWorker::read(LOrder* order)
{
    if (auto *reply = m_modbusDevice->sendReadRequest(WriteReadRequest(order), order->serverAddress()))
    {
        if (!reply->isFinished())
        {
            QEventLoop loop;
            connect(this, &DeviceWorker::readFinished, &loop, &QEventLoop::quit);
            //connect(reply, &QModbusReply::finished, this, &DeviceWorker::on_read_ready);
            connect(reply, &QModbusReply::finished, this, [=]{
                auto reply = qobject_cast<QModbusReply *>(sender());
                if (!reply)
                {
                    emit readFinished();
                    return;
                }

                if (reply->error() == QModbusDevice::NoError)
                {
                    quint64 u64 = 0;
                    const QModbusDataUnit unit = reply->result();
                    QModbusDataUnit::RegisterType table = unit.registerType();
                    if (QModbusDataUnit::HoldingRegisters == table || QModbusDataUnit::InputRegisters == table)
                    {
                        for (int i = 0, total = int(unit.valueCount()); i < total; ++i)
                        {
                            const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress() + i)
                                                     .arg(QString::number(unit.value(i)));
                            qDebug() << "KKK111= " << entry;

                            quint64 temp = unit.value(i);
                            u64 += temp << i*16;
                        }

                        order->setValue(LZLib::instance()->fromLonglong(order->byteType(), QVariant(u64)));

                        qDebug() << "KKK33= " << u64 << order->value();
                    }
                    else
                    {

                    }

                }
                else if (reply->error() == QModbusDevice::ProtocolError)
                {
                    emit message(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                 arg(reply->errorString()).
                                 arg(reply->rawResult().exceptionCode(), -1, 16));
                }
                else
                {
                    emit message(tr("Read response error: %1 (code: 0x%2)").
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
            delete reply; // broadcast replies return immediately
        }
    }
    else
    {
        emit message(tr("Read error: ") + m_modbusDevice->errorString());
    }
}

void DeviceWorker::write(LOrder* order)
{
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
            connect(reply, &QModbusReply::finished, this, [this, reply]()
                {
                    if (reply->error() == QModbusDevice::ProtocolError)
                    {
                        emit message(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                     .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
                    } else if (reply->error() != QModbusDevice::NoError)
                    {
                        emit message(tr("Write response error: %1 (code: 0x%2)")
                                     .arg(reply->errorString()).arg(reply->error(), -1, 16));
                    }
                    //reply->deleteLater();
                    delete reply;
                    emit writeFinished();
                }
            );
            loop.exec();
        }
        else
        {
            // broadcast replies return immediately
            //reply->deleteLater();
            delete reply;
        }
    }
    else
    {
        emit message(tr("Write error: ") + m_modbusDevice->errorString());
    }

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
