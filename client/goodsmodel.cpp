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
    qDebug() << Q_FUNC_INFO;

    beginResetModel();
    m_data = value;
    endResetModel();
}

void GoodsModel::appendItem(const GoodsInfo& value)
{
    qDebug() << Q_FUNC_INFO;

    int idx = m_data.size();

    beginInsertRows({}, idx, idx);
    m_data.append(value);
    endInsertRows();
}

void GoodsModel::removeItem(int index)
{
    qDebug() << Q_FUNC_INFO;

    beginRemoveRows({}, index, index);
    m_data.removeAt(index);
    endRemoveRows();
}

void GoodsModel::changeItem(int idx, const GoodsInfo& value)
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << "Idx:" << idx << "Name:" << value.name() << "Count:" << value.count();

    m_data[idx] = value;
    emit dataChanged(index(idx), index(idx));

    if (value.count() < 5)
        emit lowCountWarning(value.name(), value.vendorCode(), value.count());
}

const QList<GoodsInfo>& GoodsModel::modelData() const
{
    return m_data;
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
