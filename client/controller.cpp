#include "controller.h"

#include <QQmlEngine>


Controller::Controller(QObject *parent)
    : QObject(parent)
{
}

void Controller::registerTypes()
{
    qmlRegisterSingletonType<Controller>("Accounting.Clients", 1, 0, "Controller", [] (QQmlEngine* engine, QJSEngine*) -> QObject* {
        auto ret = Controller::instance();
        engine->setObjectOwnership(ret, QQmlEngine::CppOwnership);
        return ret;
    });

     qRegisterMetaType<GoodsInfo>();
     qRegisterMetaType<Order>();
}

void Controller::loginAsEmployee(const QString &login, const QString &password)
{
    qDebug() << "Login with credentials: " << login << password;

    auto result = m_replica->loginAsEmployee(login, password);
    result.waitForFinished();
    m_uid = result.returnValue();

    qDebug() << "Login UID: " << m_uid;

    setLoggedIn(!m_uid.isNull());
}

void Controller::orderGood(const GoodsInfo &sourceGood, const PersonalInfo &custumerInfo)
{
    m_replica->orderGood(sourceGood, custumerInfo);
}

void Controller::orderGood2(const QString& vendorCode, int count)
{
    qDebug() << "Order good with vendor code & count:" << vendorCode << count;

    // ...
}

void Controller::addGood(const GoodsInfo& sourceGood)
{
    m_replica->addGood(sourceGood, m_uid);
}

void Controller::removeGood(const GoodsInfo& sourceGood)
{
    m_replica->removeGood(sourceGood, m_uid);
}

void Controller::logOff()
{
    m_replica->logOff(m_uid);
    m_uid = QUuid();

    setLoggedIn(false);
}

QVariantList Controller::allGoods() const
{
    // DEBUG!!!
    QVariantList list;

    GoodsInfo info1, info2, info3;
    info1.setName("Xyi 1");
    info2.setName("Xyi 2");
    info3.setName("Xyi 3");
    info1.setVendorCode("code1");
    info2.setVendorCode("code2");
    info3.setVendorCode("code3");

    list.append(QVariant::fromValue(info1));
    list.append(QVariant::fromValue(info2));
    list.append(QVariant::fromValue(info3));

    for (const auto& x : list)
        qDebug() << x.value<GoodsInfo>().name() << x.value<GoodsInfo>().vendorCode();

    return list;

    // DEBUG!!!
    //return m_replica->allGoods();
}

QVariantList Controller::orders() const
{
    // DEBUG!!!
    QVariantList list;

    // ...

    return list;

    // DEBUG!!!
    // return m_replica->orders();
}

void Controller::connectToHost(const QUrl& hostUrl)
{
    if (!m_remoteNode.connectToNode(hostUrl)) {
        qDebug() << "Unable to connect";
        setConnected(false);
    }

    if (m_replica) {
        m_replica->deleteLater();
    }

    m_replica = m_remoteNode.acquire<AccountingServerReplica>();

    connect(m_replica, &AccountingServerReplica::stateChanged, [this] (QRemoteObjectReplica::State newState, QRemoteObjectReplica::State) {
        setConnected(newState == QRemoteObjectReplica::State::Valid);

        if (m_connected) {
            qDebug() << "Connected.";
            //connect(m_replica, &AccountingServerReplica::allGoodsChanged, this, &Controller::allGoodsChanged); // DEBUG!!!
            //connect(m_replica, &AccountingServerReplica::ordersChanged, this, &Controller::ordersChanged); // DEBUG!!!
        }
    });
}
