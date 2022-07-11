#include "LZLib.h"

LZLib::LZLib()
{
}

LZLib* LZLib::instance()
{
    static LZLib ins;
    return &ins;
}

QVariant LZLib::toLonglong(const LOrder::ByteType type, const QVariant& value)
{
    QVariant u64 = value;
    if (LOrder::HEX == type)
    {
        QString temp = u64.toString();
        temp = temp.replace("0x", "");
        u64 = temp.toULongLong(nullptr, 16);
    }
    else if (LOrder::Binary == type)
    {
        u64 = u64.toString().toULongLong(nullptr, 2);
    }
    else if (LOrder::Float == type)
    {
        float f = u64.toFloat();
        u64 = *(quint32*)&f;
    }
    else
    {
        u64 = u64.toULongLong();
    }
    return u64;
}

QVariant LZLib::fromLonglong(const LOrder::ByteType type, const QVariant& value)
{
    QVariant u64 = value;
    if (LOrder::HEX == type)
    {
        u64 = QString("0x%1").arg(QString::number(u64.toULongLong(), 16));
    }
    else if (LOrder::Binary == type)
    {
        u64 = QString::number(u64.toULongLong(), 2);
    }
    else if (LOrder::Float == type)
    {
        long temp = u64.toString().toLong();
        u64 = *(float*)&temp;
        //u64 = QString::number(u64.toFloat(), 'f', 6);
        u64 = QString::asprintf ("%.6f", u64.toFloat());
    }
    else
    {
        u64 = u64.toULongLong();
    }
    return u64;
}
