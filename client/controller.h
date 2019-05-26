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

    Q_INVOKABLE void connectToHost(const QUrl& hostUrl);

    Q_INVOKABLE void loginAsEmployee(const QString& login, const QString& password);
    Q_INVOKABLE void orderGood(const GoodsInfo& sourceGood, const PersonalInfo& custumerInfo);
    Q_INVOKABLE void orderGood2(const QString& vendorCode, int count);
    Q_INVOKABLE void addGood(const GoodsInfo& sourceGood);
    Q_INVOKABLE void removeGood(const GoodsInfo& sourceGood);
    Q_INVOKABLE void logOff();

    bool connected() const;
    bool loggedIn() const;

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

    bool m_connected { false };

    QUuid m_uid;
    bool m_loggedIn { false };
};

#endif // CONTROLLER_H
