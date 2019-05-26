#include "OrdersModel.h"

enum Roles {
    Record = Qt::UserRole,

    Name,
    VendorCode,
    Count,
    VendorName,
    ProductType,
    Capacity,
    Cores
};


OrdersModel::OrdersModel(QObject *parent) : QAbstractListModel(parent)
{
}

void OrdersModel::setModelData(const QList<Order>& value)
{
    beginResetModel();
    m_data = value;
    endResetModel();
}

void OrdersModel::appendItem(const Order& value)
{
    int idx = m_data.size();

    beginInsertRows({}, idx, idx);
    m_data.append(value);
    endInsertRows();
}

void OrdersModel::removeItem(int index)
{
    beginRemoveRows({}, index, index);
    m_data.removeAt(index);
    endRemoveRows();
}

void OrdersModel::changeItem(int idx, const Order& value)
{
    m_data[idx] = value;
    emit dataChanged(index(idx), index(idx));
}


int OrdersModel::rowCount(const QModelIndex&) const
{
    return m_data.size();
}

QVariant OrdersModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    switch (role) {
        case Record: return QVariant::fromValue(m_data.at(index.row()));

        default:
            qCritical() << "Unsupported role!";
            return {};
    }
}

QHash<int, QByteArray> OrdersModel::roleNames() const
{
    return {
        {Record, "record"},
        {Name, "name"},
        {VendorCode, "vendorCode"},
        {Count, "count"},
        {VendorName, "vendorName"},
        {ProductType, "productType"},
        {Capacity, "capacity"},
        {Cores, "cores"}
    };
}
