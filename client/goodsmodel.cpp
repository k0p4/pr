#include "goodsmodel.h"

enum Roles {
    Record = Qt::UserRole,
    Name,
    Phone,
    Address,
    Time,
    RetrievePlace
};


GoodsModel::GoodsModel(QObject *parent) : QAbstractListModel(parent)
{
}

void GoodsModel::setModelData(const QList<GoodsInfo>& value)
{
    beginResetModel();
    m_data = value;
    endResetModel();
}

void GoodsModel::appendItem(const GoodsInfo& value)
{
    int idx = m_data.size();

    beginInsertRows({}, idx, idx);
    m_data.append(value);
    endInsertRows();
}

void GoodsModel::removeItem(int index)
{
    beginRemoveRows({}, index, index);
    m_data.removeAt(index);
    endRemoveRows();
}

void GoodsModel::changeItem(int idx, const GoodsInfo& value)
{
    m_data[idx] = value;
    emit dataChanged(index(idx), index(idx));
}


int GoodsModel::rowCount(const QModelIndex&) const
{
    return m_data.size();
}

QVariant GoodsModel::data(const QModelIndex& index, int role) const
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

QHash<int, QByteArray> GoodsModel::roleNames() const
{
    return {
        {Record, "record"},
        {Name, "name"},
        {Phone, "phone"},
        {Address, "address"},
        {Time, "time"},
        {RetrievePlace, "retrievePlace"}
    };
}
