#pragma once
#include <QAbstractListModel>
#include "rep_accounting_protocol_replica.h"


class OrdersModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit OrdersModel(QObject *parent = nullptr);

    void setModelData(const QList<Order>& value);
    void appendItem(const Order& value);
    void removeItem(int index);
    void changeItem(int idx, const Order& value);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Order> m_data;
};
