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

bool Controller::connected() const
{
    return m_connected;
}

bool Controller::loggedIn() const
{
    return m_loggedIn;
}

void Controller::setConnected(bool connected)
{
    if (m_connected == connected)
        return;

    m_connected = connected;
    emit connectedChanged(m_connected);
}

void Controller::setLoggedIn(bool loggedIn)
{
    if (m_loggedIn == loggedIn)
        return;

    m_loggedIn = loggedIn;
    emit loggedInChanged(m_loggedIn);
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
