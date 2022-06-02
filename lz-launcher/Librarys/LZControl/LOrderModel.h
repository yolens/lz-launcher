#ifndef LORDERMODEL_H
#define LORDERMODEL_H
#include <QAbstractTableModel>
#include "LZControl_global.h"
#include "LZLib.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

class LZCONTROL_EXPORT LOrderModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit LOrderModel(QObject *parent = nullptr);

    void appendHeadTitle(const QString& title);
    QList<LOrder*>* dataList() const;
    void setOrderData(QList<LOrder*>* data);
    void reset();
protected:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    QList<LOrder*>* m_dataList;
    QStringList m_headList;
};

#endif // LORDERMODEL_H
