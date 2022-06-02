#ifndef MODBUSDATA_H
#define MODBUSDATA_H
#include "LZLib.h"

class ModbusData
{
private:
    ModbusData();

public:
    static ModbusData* instance();

    void loadDb();

    bool insert(LOrder *p);
    bool update(LOrder *p);
    bool remove(LOrder *p);
    bool insert(LDevice *p);
    bool update(LDevice *p);
    bool remove(LDevice *p);
    QList<LOrder*>* orderList();
    void setOrderList(QList<LOrder*>& list);
    QList<LDevice*>* deviceList();
    void setDeviceList(QList<LDevice*>& list);
private:
    QList<LOrder*> m_orderList;
    QList<LDevice*> m_deviceList;
};

#endif // MODBUSDATA_H
