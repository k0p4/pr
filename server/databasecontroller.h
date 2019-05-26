#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QtSql>

class GoodsInfo;
class PersonalInfo;
class Order;

#define database DatabaseController::instance()

class DatabaseController
{
public:
    static DatabaseController* instance()
    {
        static DatabaseController controller;
        return &controller;
    }

    QSqlError getAllGoods(QList<GoodsInfo>& goodsList) const;
    QSqlError getAllOrders(QList<Order>& orderList) const;

    QSqlError initDb();

    QSqlError orderGood(const GoodsInfo& sourceGood, const PersonalInfo& custumerInfo, int countInDb);
    QSqlError addNewGood(const GoodsInfo& sourceGood);
    QSqlError addGood(const QString& vendorCode, int countInDb);
    QSqlError removeGood(const QString& vendorCode);

private:
    DatabaseController() = default;

    QSqlError updateGoods(const QString& vendorCode, int countInDb);
    QSqlError updateOrders(const GoodsInfo& sourceGood, const PersonalInfo& custumerInfo);

    bool m_initialized { false };

    QSqlDatabase m_db;
};

#endif // DATABASECONTROLLER_H
