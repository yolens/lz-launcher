#ifndef ORDERMODEL_H
#define ORDERMODEL_H
#include "LOrderModel.h"

class OrderModel : public LOrderModel
{
    Q_OBJECT
private:
    enum Type
    {
        id = 0,
        name,
        mark,
        value,
        deviceId,
        registerType,
        serverAddress,
        startAddress,
        numberOfValues
    };

    QMap<Type, QString> TypeM = {
        {id                 , "ID"},
        {name               , "名称"},
        {mark               , "注释"},
        {value              , "值"},
        {deviceId           , "设备"},
        {registerType       , "功能码"},
        {serverAddress      , "服务器地址"},
        {startAddress       , "起始地址"},
        {numberOfValues     , "写入个数"},
    };
public:
    explicit OrderModel(QObject *parent = nullptr);

protected:
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

};

#endif // ORDERMODEL_H
