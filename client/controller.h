#pragma once
#include <QObject>
#include "rep_accounting_protocol_replica.h"
#include "goodsmodel.h"
#include "ordersmodel.h"


class Controller : public QObject
{
    Q_OBJECT
public:
    static Controller* instance();

    void registerTypes();

    Q_PROPERTY(QAbstractListModel* goodsModel READ goodsModel CONSTANT)
    Q_PROPERTY(QAbstractListModel* ordersModel READ ordersModel CONSTANT)
    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged)
    Q_PROPERTY(bool loggedIn READ loggedIn WRITE setLoggedIn NOTIFY loggedInChanged)

    Q_INVOKABLE void connectToHost(const QUrl& hostUrl);

    Q_INVOKABLE void loginAsEmployee(const QString& login, const QString& password);
    Q_INVOKABLE void orderGood(int index, int count);
    Q_INVOKABLE void addGood(const GoodsInfo& sourceGood);
    Q_INVOKABLE void addGood2(QString name,
                              QString vendorCode,
                              int count,
                              QString vendorName,
                              QString productType,
                              QString capacity,
                              int cores);
    Q_INVOKABLE void updateExistingGood(const QString& vendorCode, int count);
    Q_INVOKABLE void removeGood(const GoodsInfo& sourceGood);
    Q_INVOKABLE void removeGood2(int index);
    Q_INVOKABLE void logOff();

    bool connected() const;
    bool loggedIn() const;

    QAbstractListModel* goodsModel();
    QAbstractListModel* ordersModel();

public slots:
    void setConnected(bool connected);
    void setLoggedIn(bool loggedIn);

signals:
    void connectedChanged(bool connected);
    void loggedInChanged(bool loggedIn);

private:
    explicit Controller(QObject* parent = nullptr);

    AccountingServerReplica* m_replica { nullptr };
    QRemoteObjectNode m_remoteNode;

    GoodsModel m_goodsModel;
    OrdersModel m_ordersModel;

    bool m_connected { false };

    QUuid m_uid;
    bool m_loggedIn { false };
};
