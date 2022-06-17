#ifndef LORDERMODEL_H
#define LORDERMODEL_H
#include <QAbstractTableModel>
#include <QAbstractItemDelegate>
#include "LZControl_global.h"
#include "LZLib.h"
#include "LDB.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

class LZCONTROL_EXPORT LOrderModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    QMap<LOrder::RWType, QString> RWTypeM = {
        {LOrder::Write              , "写"},
        {LOrder::Read               , "读"},
    };
    QMap<LOrder::ByteType, QString> ByteTypeM = {
        {LOrder::HEX              , "16进制"},
        {LOrder::DEC              , "十进制"},
        {LOrder::Binary           , "二进制"},
        {LOrder::Float            , "浮点型"},
    };
public:
    explicit LOrderModel(QObject *parent = nullptr);

    void appendHeadTitle(const QString& title);
    QList<LOrder*>* dataList() const;
    void setOrderData(QList<LOrder*>* data);
    void reset();

    void disenableComboBoxItem(int col, QStringList list) const;
    void addComboBoxDelegate(int col, QStringList list);
    QMap<int, QAbstractItemDelegate*>& itemDelegates();
    bool removeRows(QModelIndexList indexes);
    LOrder *itemFromIndex(const QModelIndex &index) const;

    virtual void remove(LOrder *order) = 0;
protected:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    //virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex()) override;

private:
    QList<LOrder*>* m_dataList;
    QStringList m_headList;
    QMap<int, QAbstractItemDelegate*> m_delegateMap;
};

#endif // LORDERMODEL_H
