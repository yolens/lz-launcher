#ifndef IORDER_H
#define IORDER_H
#define MODBUS_UUID "{c16069d4-2677-4971-a0d4-cb915080ad6c}"
#include "IPlugin.h"
class IOrder
{
public:
    virtual void execute() = 0; //执行
    virtual void write(QVariant value) = 0; //写入值
    virtual QVariant read() = 0; //获取值
    virtual QWidget* getWidget() = 0; //获取编辑窗口
};
Q_DECLARE_INTERFACE(IOrder,"LZ.IOrder/1.0")

#endif // IORDER_H
