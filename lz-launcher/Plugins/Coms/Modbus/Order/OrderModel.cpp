#include "OrderModel.h"
#include "ModbusData.h"

OrderModel::OrderModel(QObject *parent) : LOrderModel(parent)
{
    init();
}

void OrderModel::init()
{
    foreach (const Type type, TypeM.keys())
    {
        appendHeadTitle(TypeM.value(type));
    }


    QStringList stringList;
    updateDevice();

    stringList.clear();
    for (auto a : RWTypeM.toStdMap())
    {
        stringList.append(a.second);
    }
    addComboBoxDelegate(Type::rwType, stringList);

    stringList.clear();
    for (auto a : ByteTypeM.toStdMap())
    {
        stringList.append(a.second);
    }
    addComboBoxDelegate(Type::byteType, stringList);

    stringList.clear();
    for (auto a : RegisterTypeM.toStdMap())
    {
        stringList.append(a.second);
    }
    addComboBoxDelegate(Type::registerType, stringList);

}

void OrderModel::pressIndex(const QModelIndex &index)
{
    if (index.isValid()) {
        LOrder *order = dataList()->at(index.row());
        QStringList stringList;
        if (LOrder::Write == order->rwType())
        {
            stringList.append(RegisterTypeM.value(QModbusDataUnit::DiscreteInputs));
            stringList.append(RegisterTypeM.value(QModbusDataUnit::InputRegisters));
            disenableComboBoxItem(Type::registerType, stringList);
        }
        else
        {
            disenableComboBoxItem(Type::registerType, stringList);
        }
    }
}

void OrderModel::updateDevice()
{
    QStringList stringList;
    QList<LDevice*>* list = ModbusData::instance()->deviceList();
    for (int i = 0; i<list->count(); i++)
    {
        QString name = QString("%1(%2)").arg(list->value(i)->name()).arg(list->value(i)->id);
        stringList.append(name);
        m_deviceIdMap.insert(list->value(i)->id, name);
    }
    addComboBoxDelegate(Type::deviceId, stringList);
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
            if (LOrder::HEX == order->byteType())
            {
                return QString("0x%1").arg(QString::number(order->value().toULongLong(), 16));
            }
            else if (LOrder::Binary == order->byteType())
            {
                return QString::number(order->value().toULongLong(), 2);
            }
            else if (LOrder::Float == order->byteType())
            {
                long temp = order->value().toString().toLong();
                return *(float*)&temp;
            }
            else
            {
                return order->value().toULongLong();
            }
            break;
        case Type::deviceId:
            return m_deviceIdMap.value(order->deviceId());
            break;
        case Type::rwType:
        {

            return RWTypeM.value(order->rwType());
        }
            break;
        case Type::byteType:
            return ByteTypeM.value(order->byteType());
            break;
        case Type::registerType:
            return RegisterTypeM.value((QModbusDataUnit::RegisterType)order->registerType());
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
#include <QtDebug>
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
            if (LOrder::HEX == order->byteType())
            {
                QString temp = value.toString();
                temp = temp.replace("0x", "");
                quint64 u64 = temp.toULongLong(nullptr, 16);
                order->setValue(u64);
            }
            else if (LOrder::Binary == order->byteType())
            {
                quint64 u64 = value.toString().toULongLong(nullptr, 2);
                order->setValue(u64);
            }
            else if (LOrder::Float == order->byteType())
            {
                float f = value.toFloat();
                quint32 temp = *(quint32*)&f;
                order->setValue(temp);
            }
            else
            {
                order->setValue(value.toULongLong());
            }
            break;
        case Type::deviceId:
            for (auto a : m_deviceIdMap.toStdMap())
            {
                if (a.second == value.toString())
                {
                    order->setDeviceId(a.first);
                    break;
                }
            }
            break;
        case Type::rwType:
        {
            for (auto a : RWTypeM.toStdMap())
            {
                if (a.second == value.toString())
                {
                    order->setRWType(a.first);
                    break;
                }
            }
        }
            break;
        case Type::byteType:
            for (auto a : ByteTypeM.toStdMap())
            {
                if (a.second == value.toString())
                {
                    order->setByteType(a.first);
                    break;
                }
            }
            break;
        case Type::registerType:
            for (auto a : RegisterTypeM.toStdMap())
            {
                if (a.second == value.toString())
                {
                    order->setRegisterType(a.first);
                    break;
                }
            }
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
        ModbusData::instance()->update(order);
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags OrderModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QAbstractItemModel::flags(index);
    }
    Qt::ItemFlags flag = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    if (index.column() == Type::id)
    {
        flag &= ~Qt::ItemIsEnabled;
    }
    /*else if (index.column() == Type::id)
    {
        flag |= Qt::ItemIsEditable;
    }*/

    return flag;
}


void OrderModel::remove(LOrder *order)
{
    ModbusData::instance()->remove(order);
}
