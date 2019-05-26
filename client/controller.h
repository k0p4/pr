#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QVariantList>
#include <QObject>
#include "rep_accounting_protocol_replica.h"


class Controller : public QObject
{
    Q_OBJECT
public:

    static Controller* instance()
    {
        static Controller c;
        return &c;
    }

    void registerTypes();

    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged)
    Q_PROPERTY(bool loggedIn READ loggedIn WRITE setLoggedIn NOTIFY loggedInChanged)

    Q_PROPERTY(QVariantList allGoods READ allGoods NOTIFY allGoodsChanged)
    Q_PROPERTY(QVariantList orders READ orders NOTIFY ordersChanged)

    Q_INVOKABLE void connectToHost(const QUrl& hostUrl);

    Q_INVOKABLE void loginAsEmployee(const QString& login, const QString& password);
    Q_INVOKABLE void orderGood(const GoodsInfo& sourceGood, const PersonalInfo& custumerInfo);
    Q_INVOKABLE void orderGood2(const QString& vendorCode, int count);
    Q_INVOKABLE void addGood(const GoodsInfo& sourceGood);
    Q_INVOKABLE void removeGood(const GoodsInfo& sourceGood);
    Q_INVOKABLE void logOff();

    bool connected() const
    {
        return m_connected;
    }

    QVariantList allGoods() const;
    QVariantList orders() const;

    bool loggedIn() const
    {
        return m_loggedIn;
    }

public slots:
    void setConnected(bool connected)
    {
        if (m_connected == connected)
            return;

        m_connected = connected;
        emit connectedChanged(m_connected);
    }

    void setLoggedIn(bool loggedIn)
    {
        if (m_loggedIn == loggedIn)
            return;

        m_loggedIn = loggedIn;
        emit loggedInChanged(m_loggedIn);
    }

signals:
    void connectedChanged(bool connected);

    void allGoodsChanged(QVariantList allGoods);
    void ordersChanged(QVariantList orders);

    void loggedInChanged(bool loggedIn);

private:
    explicit Controller(QObject* parent = nullptr);

    AccountingServerReplica* m_replica { nullptr };
    QRemoteObjectNode m_remoteNode;

    bool m_connected { false };
    QList<GoodsInfo> m_allGoods;
    QList<Order> m_orders;

    QUuid m_uid;
    bool m_loggedIn { false };
};

#endif // CONTROLLER_H
