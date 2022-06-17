#ifndef ORDERMODEL_H
#define ORDERMODEL_H
#include "LOrderModel.h"
#include "LDB.h"
#include <QModbusDataUnit>

class OrderModel : public LOrderModel
{
    Q_OBJECT
private:
    enum Type
    {
        id = 0,
        name,
        deviceId,
        value,
        byteType,
        rwType,
        registerType,
        serverAddress,
        startAddress,
        numberOfValues,
        mark,
    };

    QMap<Type, QString> TypeM = {
        {id                 , "ID"},
        {name               , "名称"},
        {deviceId           , "设备"},
        {value              , "值"},
        {byteType           , "字节类型"},
        {rwType             , "读写"},
        {registerType       , "功能码"},
        {serverAddress      , "服务器地址"},
        {startAddress       , "起始地址"},
        {numberOfValues     , "读写个数"},
        {mark               , "注释"},
    };

    QMap<QModbusDataUnit::RegisterType, QString> RegisterTypeM = {
        {QModbusDataUnit::DiscreteInputs    , "DiscreteInputs"  },
        {QModbusDataUnit::Coils             , "Coils"           },
        {QModbusDataUnit::InputRegisters    , "InputRegisters"  },
        {QModbusDataUnit::HoldingRegisters  , "HoldingRegisters"},
    };
    QVector<QString> RegisterTypeOnlyWriteV = {
        {"Coils"},
        {"HoldingRegisters"},
    };

public:
    explicit OrderModel(QObject *parent = nullptr);

    void pressIndex(const QModelIndex &index);
    void updateDevice();
    virtual void remove(LOrder *order) override;
protected:
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
private:
    void init();
signals:
    void testSignel(LOrder *order);
private:
    QMap<int, QString> m_deviceIdMap;

};

#endif // ORDERMODEL_H
