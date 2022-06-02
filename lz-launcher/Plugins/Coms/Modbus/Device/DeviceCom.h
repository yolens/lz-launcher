#ifndef DEVICECOM_H
#define DEVICECOM_H

#include <QWidget>
#include "LZLib.h"
#include <QModbusClient>
#include <QSerialPort>

const QString Table_Device_Modbus = "Device_Modbus";
const auto CREATE_SQL_Device_Modbus = QLatin1String(R"(
    create table IF NOT EXISTS %1(
                                    id integer primary key,
                                    type integer        ,
                                    name varchar        ,
                                    mark varchar        ,
                                    port varchar        ,
                                    parity varchar      ,
                                    baudRate varchar    ,
                                    dataBits varchar    ,
                                    stopBits varchar
    ))").arg(Table_Device_Modbus);

const auto INSERT_SQL_Device_Modbus = QLatin1String(R"(
    insert into %1(
                    type        ,
                    name        ,
                    mark        ,
                    port        ,
                    parity      ,
                    baudRate    ,
                    dataBits    ,
                    stopBits
                  )
            values(
                    :type       ,
                    :name       ,
                    :mark       ,
                    :port       ,
                    :parity     ,
                    :baudRate   ,
                    :dataBits   ,
                    :stopBits
                   )
    )").arg(Table_Device_Modbus);

const auto UPDATE_SQL_Device_Modbus = QLatin1String(R"(
    update %1 set
                    type=:type          ,
                    name=:name          ,
                    mark=:mark          ,
                    port=:port          ,
                    parity=:parity      ,
                    baudRate=:baudRate  ,
                    dataBits=:dataBits  ,
                    stopBits=:stopBits
              where id=:id
    )").arg(Table_Device_Modbus);
const auto DELETE_SQL_Device_Modbus = QLatin1String(R"(
    delete from %1 where id=:id
    )").arg(Table_Device_Modbus);

const auto SELECT_SQL_Device_Modbus = QLatin1String(R"(
    select * from %1
    )").arg(Table_Device_Modbus);

namespace Ui {
class DeviceCom;
}

class DeviceCom : public QWidget, public LDevice
{
    Q_OBJECT
private:
    QHash<QString, QSerialPort::Parity> ParityM = {
        {"No"       , QSerialPort::NoParity},
        {"Even"     , QSerialPort::EvenParity},
        {"Odd"      , QSerialPort::OddParity},
        {"Space"    , QSerialPort::SpaceParity},
        {"Mark"     , QSerialPort::MarkParity},
    };
    QStringList ParityL = {"No", "Even", "Odd", "Space", "Mark"};

    QHash<QString, QSerialPort::BaudRate> BaudRateM = {
        {"1200"     , QSerialPort::Baud1200},
        {"2400"     , QSerialPort::Baud2400},
        {"4800"     , QSerialPort::Baud4800},
        {"9600"     , QSerialPort::Baud9600},
        {"19200"    , QSerialPort::Baud19200},
        {"38400"    , QSerialPort::Baud38400},
        {"57600"    , QSerialPort::Baud57600},
        {"115200"   , QSerialPort::Baud115200},
    };
    QStringList BaudRateL = {"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"};

    QHash<QString, QSerialPort::DataBits> DataBitsM = {
        {"5"        , QSerialPort::Data5},
        {"6"        , QSerialPort::Data6},
        {"7"        , QSerialPort::Data7},
        {"8"        , QSerialPort::Data8},
    };
    QStringList DataBitsL = {"5", "6", "7", "8"};

    QHash<QString, QSerialPort::StopBits> StopBitsM = {
        {"1"        , QSerialPort::OneStop},
        {"2"        , QSerialPort::TwoStop},
        {"3"        , QSerialPort::OneAndHalfStop},
    };
    QStringList StopBitsL = {"1", "2", "3"};
public:
    explicit DeviceCom(QWidget *parent = nullptr);
    ~DeviceCom();

    void init();
    void action(const LOrder* order);

    static bool loadDb();
    virtual bool updateDb() override;
    virtual bool insertDb() override;
    virtual bool removeDb() override;

    virtual QString port() override;
    virtual QString parity() override;
    virtual QString baudRate() override;
    virtual QString dataBits() override;
    virtual QString stopBits() override;
    virtual void setPort(const QString& value) override;
    virtual void setParity(const QString& value) override;
    virtual void setBaudRate(const QString& value) override;
    virtual void setDataBits(const QString& value) override;
    virtual void setStopBits(const QString& value) override;
private:
    bool bindValue(QSqlQuery& query);
    bool connectDevice();

    void write(const LOrder* order);
    QModbusDataUnit writeRequest(const LOrder* order) const;
protected:
    virtual void paintEvent(QPaintEvent * event) override;
signals:
    void updateView();
private slots:
    void on_errorOccurred(QModbusDevice::Error error);
    void on_stateChanged(QModbusDevice::State state);

    void on_pushButton_connect_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_delete_clicked();

private:
    Ui::DeviceCom *ui;

    QModbusClient *m_modbusDevice = nullptr;

    QString m_port;
    QString m_parity = "No";
    QString m_baudRate = "9600";
    QString m_dataBits = "5";
    QString m_stopBits = "1";

};

#endif // DEVICECOM_H
