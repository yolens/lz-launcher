#ifndef DEVICEWORKER_H
#define DEVICEWORKER_H

#include <QObject>
#include <QThread>
#include "LDB.h"
#include <QModbusClient>
#include <QSerialPort>
#include <QQueue>
#include <QMutex>
#include <QSemaphore>




class DeviceWorker : public QObject
{
    Q_OBJECT
public:
    enum Result
    {
        CreateFinished = 0,
        UnconnectedResult,
        ConnectedResult,
        FinishResult,
        StopResult,
    };

    enum Action
    {
        Create_action = 0,
        Connect_action,
        Send_action,
    };

    struct DeviceInfo
    {
        QString port;
        QSerialPort::Parity parity;
        QSerialPort::BaudRate baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::StopBits stopBits;
    };
public:
    explicit DeviceWorker(QObject *parent = nullptr);
    ~DeviceWorker();

    void start();
    void actionDataStop();
    void addIn(LOrder* order);

    void createDevice();
    void connectDevice(const DeviceWorker::DeviceInfo& info);

private:
    void write(LOrder* order);
    QModbusDataUnit WriteReadRequest(LOrder* order) const;
    void read(LOrder* order);
    LOrder* takeOut();

    void createAction();
    void connectAction(const DeviceWorker::DeviceInfo& info);
    void sendAction();
public slots:
    void on_errorOccurred(QModbusDevice::Error error);
    void on_stateChanged(QModbusDevice::State state);
    void on_read_ready();
    void on_start_work();
signals:
    void message(const QString& msg);
    void result(const int code);
    void writeFinished();
    void readFinished();
private:
    QThread *m_thread = nullptr;
    QModbusClient *m_modbusDevice = nullptr;

    bool m_stop = true;
    QQueue<LOrder*> m_queue;
    QMutex m_mutex;
    QSemaphore m_sem;
    Action m_action = Action::Send_action;
    DeviceWorker::DeviceInfo m_info;
};

#endif // DEVICEWORKER_H
