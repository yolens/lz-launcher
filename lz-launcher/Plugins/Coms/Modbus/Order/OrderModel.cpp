#include "OrderModel.h"
#include "ModbusData.h"

OrderModel::OrderModel(QObject *parent) : LOrderModel(parent)
{
    foreach (const Type type, TypeM.keys())
    {
        appendHeadTitle(TypeM.value(type));
    }
}


QVariant OrderModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        Type type = (Type)index.column();
        LOrder *order = dataList()->at(index.row());
        switch (type)
        {
        case Type::id:
            return order->id;
            break;
        case Type::name:
            return order->name();
            break;
        case Type::mark:
            return order->mark();
            break;
        case Type::value:
            return order->value();
            break;
        case Type::deviceId:
        {
            //order->deviceId();
            QString devices;
            QList<LDevice*>* list = ModbusData::instance()->deviceList();
            for (int i = 0; i<list->count(); i++)
            {
                devices += QString::number(list->value(i)->id) + ",";
            }
            return devices;
        }
            break;
        case Type::registerType:
            return order->registerType();
            break;
        case Type::serverAddress:
            return order->serverAddress();
            break;
        case Type::startAddress:
            return order->startAddress();
            break;
        case Type::numberOfValues:
            return order->numberOfValues();
            break;
        default:
            break;
        }
    }

    return QVariant();
}

bool OrderModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {

        Type type = (Type)index.column();
        LOrder *order = dataList()->at(index.row());
        switch (type)
        {
        case Type::id:
            //order->id = value.toInt();
            break;
        case Type::name:
            order->setName(value.toString());
            break;
        case Type::mark:
            order->setMark(value.toString());
            break;
        case Type::value:
            order->setValue(value);
            break;
        case Type::deviceId:
            order->setDeviceId(1);

            break;
        case Type::registerType:
            order->setRegisterType(value.toString());
            break;
        case Type::serverAddress:
            order->setServerAddress(value.toInt());
            break;
        case Type::startAddress:
            order->setStartAddress(value.toInt());
            break;
        case Type::numberOfValues:
            order->setNumberOfValues(value.toInt());
            break;
        default:
            break;
        }
        order->updateDb();
        emit dataChanged(index, index);
        return true;
    }
    return false;
}
