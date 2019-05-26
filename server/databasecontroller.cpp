#include "databasecontroller.h"

#include "accounting_server.h"

#include <QString>
#include <QSqlField>

namespace Constants {

namespace Placeholders {

static const QString name                   { ":name" };
static const QString vendorCode             { ":vendor_code" };
static const QString productName            { ":product_name" };
static const QString count                  { ":count" };
static const QString vendorName             { ":vendor_name" };
static const QString productType            { ":product_type" };
static const QString capacity               { ":capacity" };
static const QString cores                  { ":cores" };
static const QString customerName           { ":customer_name" };
static const QString custumerPhoneNumber    { ":custumer_phone_numbe" };
static const QString customerAddress         { ":customer_adress" };
static const QString deliverTime            { ":deliver_time" };
static const QString retrievePlace           { ":retrive_place" };

}

namespace Queries {

static const QString createGoodsTable   { "create table goods(id integer primary key, name varchar, vendor_code varchar,"
                                          "count integer, vendor_name varchar, product_type varchar, capacity varchar, cores integer)" };

static const QString createOrdersTable  { "create table orders(id integer primary key, product_name varchar, vendor_code varchar,"
                                          "count integer, vendor_name varchar, product_type varchar, capacity varchar, cores integer,"
                                          "customer_name varchar, custumer_phone_number varchar, customer_adress varchar,"
                                          "deliver_time varchar, retrive_place varchar)" };

static const QString insertGood         { "INSERT INTO goods (name, vendor_code, count, vendor_name, product_type, capacity, cores)"
                                          "VALUES(:name, :vendor_code, :vendor_name, :product_type, :capacity, :cores)" };
static const QString removeGood         { "DELETE FROM goods WHERE vendor_code = :vendor_code" };


static const QString updateGoods        { "UPDATE goods SET count = :count WHERE vendor_code = :vendor_code;" };
static const QString updateOrders       { "INSERT INTO orders (product_name, vendor_code,"
                                          "count, vendor_name, product_type, capacity, cores,"
                                          "customer_name, custumer_phone_number, customer_adress,"
                                          "deliver_time, retrive_place)"
                                          "VALUES (:name, :vendor_code, :product_name,"
                                          ":count, :vendor_name, :product_type, :capacity, :cores,"
                                          ":customer_name, :custumer_phone_number, :customer_adress,"
                                          ":deliver_time, :retrive_place)" };

static const QString getGoods           { "SELECT * FROM goods" };
static const QString getOrders          { "SELECT * FROM orders" };

}

static const QString goodsTable         { "goods" };
static const QString ordersTable        { "orders" };

static const QString dbName             { "goods.db" };

QString formatPlaceholder(const QString& placeholder)
{
    return placeholder.right(placeholder.size() - 1);
}

}

QSqlError DatabaseController::getAllGoods(QList<GoodsInfo>& goodsList) const
{
    QSqlQuery q;

    if (!q.prepare(Constants::Queries::getGoods)) {
        return q.lastError();
    }

    if (!q.exec())
        return q.lastError();

    while (q.next()) {
        GoodsInfo good;

        good.setName(q.value(Constants::formatPlaceholder(Constants::Placeholders::name)).toString());
        good.setVendorCode(q.value(Constants::formatPlaceholder(Constants::Placeholders::vendorCode)).toString());
        good.setCount(q.value(Constants::formatPlaceholder(Constants::Placeholders::count)).toInt());
        good.setVendorName(q.value(Constants::formatPlaceholder(Constants::Placeholders::vendorName)).toString());
        good.setProductType(q.value(Constants::formatPlaceholder(Constants::Placeholders::productType)).toString());
        good.setCapacity(q.value(Constants::formatPlaceholder(Constants::Placeholders::capacity)).toString());
        good.setCores(q.value(Constants::formatPlaceholder(Constants::Placeholders::cores)).toInt());

        goodsList.push_back(good);
    }

    return { };
}

QSqlError DatabaseController::getAllOrders(QList<Order>& orderList) const
{
    QSqlQuery q;

    if (!q.prepare(Constants::Queries::getOrders))
        return q.lastError();

    if (!q.exec())
        return q.lastError();

    while (q.next()) {
        GoodsInfo good;

        good.setName(q.value(Constants::formatPlaceholder(Constants::Placeholders::name)).toString());
        good.setVendorCode(q.value(Constants::formatPlaceholder(Constants::Placeholders::vendorCode)).toString());
        good.setCount(q.value(Constants::formatPlaceholder(Constants::Placeholders::count)).toInt());
        good.setVendorName(q.value(Constants::formatPlaceholder(Constants::Placeholders::vendorName)).toString());
        good.setProductType(q.value(Constants::formatPlaceholder(Constants::Placeholders::productType)).toString());
        good.setCapacity(q.value(Constants::formatPlaceholder(Constants::Placeholders::capacity)).toString());
        good.setCores(q.value(Constants::formatPlaceholder(Constants::Placeholders::cores)).toInt());

        PersonalInfo customer;
        customer.setAddress(q.value(Constants::formatPlaceholder(Constants::Placeholders::customerAddress)).toString());
        customer.setName(q.value(Constants::formatPlaceholder(Constants::Placeholders::customerName)).toString());
        customer.setPhoneNumber(q.value(Constants::formatPlaceholder(Constants::Placeholders::custumerPhoneNumber)).toString());
        customer.setRetrievePlace(q.value(Constants::formatPlaceholder(Constants::Placeholders::retrievePlace)).toString());
        customer.setTime(q.value(Constants::formatPlaceholder(Constants::Placeholders::deliverTime)).toString());

        Order order;
        order.setGood(good);
        order.setCustomer(customer);

        orderList.push_back(order);
    }

    return { };
}

QSqlError DatabaseController::initDb()
{
    if (m_initialized) {
        return { };
    }

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(Constants::dbName);

    if (!m_db.open())
        return m_db.lastError();

    QStringList tables = m_db.tables();
    if (tables.contains(Constants::goodsTable, Qt::CaseInsensitive)
        && tables.contains(Constants::ordersTable, Qt::CaseInsensitive))
    {
        m_initialized = true;
        return { };
    }

    QSqlQuery q;
    if (!q.exec(Constants::Queries::createGoodsTable))
        return q.lastError();

    if (!q.exec(Constants::Queries::createOrdersTable)) {
        m_db.close();
        return q.lastError();
    }

    m_initialized = true;
    return { };
}

QSqlError DatabaseController::orderGood(const GoodsInfo &sourceGood, const PersonalInfo &custumerInfo, int countInDb)
{
    QSqlError updateGoodsResult = updateGoods(sourceGood.vendorCode(), countInDb);

    if (updateGoodsResult.type() != QSqlError::NoError)
        return updateGoodsResult;

    // TODO: here we probably should rollback commit if we have some issues in update order
    // but i think that this issue will no reproduce with SQLITE db driver
    // Or probably we should use m_db.transaction(); and m_db.commit(); for atomic
    // but i dunno if QSQLITE supports this operations

    return updateOrders(sourceGood, custumerInfo);
}

QSqlError DatabaseController::addGood(const QString& vendorCode, int countInDb)
{
    return updateGoods(vendorCode, countInDb);
}

QSqlError DatabaseController::addNewGood(const GoodsInfo& sourceGood)
{
    QSqlQuery q;

    if (!q.prepare(Constants::Queries::insertGood))
        return q.lastError();

    q.bindValue(Constants::Placeholders::name, sourceGood.name());
    q.bindValue(Constants::Placeholders::vendorCode, sourceGood.vendorCode());
    q.bindValue(Constants::Placeholders::vendorName, sourceGood.vendorName());
    q.bindValue(Constants::Placeholders::productType, sourceGood.productType());
    q.bindValue(Constants::Placeholders::capacity, sourceGood.capacity());
    q.bindValue(Constants::Placeholders::cores, sourceGood.cores());

    if (!q.exec())
        return q.lastError();

    return { };
}

QSqlError DatabaseController::removeGood(const QString& vendorCode)
{
    QSqlQuery q;

    if (!q.prepare(Constants::Queries::removeGood))
        return q.lastError();

    q.bindValue(Constants::Placeholders::vendorCode, vendorCode);

    if (!q.exec())
        return q.lastError();

    return { };
}

QSqlError DatabaseController::updateGoods(const QString& vendorCode, int countInDb)
{
    if (countInDb == 0) {
        return removeGood(vendorCode);
    }

    QSqlQuery q;
    if (!q.prepare(Constants::Queries::updateGoods))
        return q.lastError();

    q.bindValue(Constants::Placeholders::count, countInDb);
    q.bindValue(Constants::Placeholders::vendorCode, vendorCode);

    if (!q.exec())
        return q.lastError();

    return { };
}

QSqlError DatabaseController::updateOrders(const GoodsInfo& sourceGood, const PersonalInfo& custumerInfo)
{
    QSqlQuery q;
    if (!q.prepare(Constants::Queries::updateOrders))
        return q.lastError();

    q.bindValue(Constants::Placeholders::productName, sourceGood.name());
    q.bindValue(Constants::Placeholders::vendorCode, sourceGood.vendorCode());
    q.bindValue(Constants::Placeholders::count, sourceGood.count());
    q.bindValue(Constants::Placeholders::vendorName, sourceGood.vendorName());
    q.bindValue(Constants::Placeholders::productType, sourceGood.productType());
    q.bindValue(Constants::Placeholders::capacity, sourceGood.capacity());
    q.bindValue(Constants::Placeholders::cores, sourceGood.cores());
    q.bindValue(Constants::Placeholders::customerName, custumerInfo.name());
    q.bindValue(Constants::Placeholders::custumerPhoneNumber, custumerInfo.phoneNumber());
    q.bindValue(Constants::Placeholders::customerAddress, custumerInfo.address());
    q.bindValue(Constants::Placeholders::deliverTime, custumerInfo.time());
    q.bindValue(Constants::Placeholders::retrievePlace, custumerInfo.retrievePlace());

    if (!q.exec())
        return q.lastError();

    return { };
}


