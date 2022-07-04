#ifndef IDATACENTER_H
#define IDATACENTER_H
#define DATACENTER_UUID "{26415f20-f357-44fa-a6fc-b686f00dfc39}"
#include "IPlugin.h"
#include "LDB.h"


class IDataCenter
{
public:
    virtual LPoint* getPoint(const int id) = 0;
    virtual QList<LChart*> getChartList(const int id) = 0;
    virtual QList<LPoint*> getPointList(const int chartId) = 0;
    virtual bool insertChart(LChart* p) = 0;
    virtual bool updateChart(LChart* p) = 0;
    virtual bool removeChart(LChart* p) = 0;

    virtual bool insertPoint(LPoint* p) = 0;
    virtual bool updatePoint(LPoint* p) = 0;
    virtual bool removePoint(LPoint* p) = 0;



    virtual void execute(LOrder* p) = 0; //执行
    virtual QMap<LOrder::Type, QList<LOrder*>>& getOrderList() = 0;
    virtual LOrder* newOrder(LOrder* p) = 0;
    virtual LOrder* getOrder(const int id, const int type) = 0;

    virtual void registerDeviceState(std::function<void()> cb) = 0;
    virtual void noticeDeviceState() = 0;
    virtual QMap<LDevice::DeviceState, int> deviceStateList() = 0;
    virtual QMap<LDevice::DeviceState, int> deviceStateListByType(const int type) = 0;
};
Q_DECLARE_INTERFACE(IDataCenter,"LZ.IDataCenter/1.0")

#endif // IDATACENTER_H
