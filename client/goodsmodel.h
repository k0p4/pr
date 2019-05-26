#pragma once
#include <QAbstractListModel>
#include "rep_accounting_protocol_replica.h"


class GoodsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit GoodsModel(QObject *parent = nullptr);

    void setModelData(const QList<GoodsInfo>& value);
    void appendItem(const GoodsInfo& value);
    void removeItem(int index);
    void changeItem(int idx, const GoodsInfo& value);

    const QList<GoodsInfo>& modelData() const;

signals:
    void lowCountWarning(const QString& name, const QString& vendorCode, int count);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<GoodsInfo> m_data;
};
