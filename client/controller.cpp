#include "controller.h"

#include <QQmlEngine>


Controller::Controller(QObject *parent)
    : QObject(parent)
{
}

Controller* Controller::instance()
{
    static Controller c;
    return &c;
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

void Controller::orderGood(int index, int count)
{
    qDebug() << "Order good with index & count:" << index << count;

    GoodsInfo info = m_goodsModel.modelData().at(index);
    info.setCount(count);

    qDebug() << "Good:" << info.vendorCode();

    m_replica->orderGood(info, {});
}

void Controller::addGood(const GoodsInfo& sourceGood)
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << "Vendor code:" << sourceGood.vendorCode() << "Count:" << sourceGood.count();

    m_replica->addGood(sourceGood, m_uid);
}

void Controller::addGood2(QString name, QString vendorCode, int count, QString vendorName, QString productType, QString capacity, int cores)
{
    GoodsInfo info;
    info.setName(name);
    info.setVendorCode(vendorCode);
    info.setCount(count);
    info.setVendorName(vendorName);
    info.setProductType(productType);
    info.setCapacity(capacity);
    info.setCores(cores);

    addGood(info);
}

void Controller::updateExistingGood(const QString& vendorCode, int count)
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << "Vendor code:" << vendorCode << "Count:" << count;

    GoodsInfo info;
    info.setVendorCode(vendorCode);
    info.setCount(count);

    m_replica->addGood(info, m_uid);
}

void Controller::removeGood(const GoodsInfo& sourceGood)
{
    m_replica->removeGood(sourceGood, m_uid);
}

void Controller::removeGood2(int index)
{
    removeGood(m_goodsModel.modelData().at(index));
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

QAbstractListModel* Controller::goodsModel()
{
    return &m_goodsModel;
}

QAbstractListModel* Controller::ordersModel()
{
    return &m_ordersModel;
}

void Controller::setConnected(bool connected)
{
    if (m_connected == connected)
        return;

    m_connected = connected;
    emit connectedChanged(m_connected);

    if (!m_connected) {
        m_uid = QUuid();
        setLoggedIn(false);
    }
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

            m_goodsModel.setModelData(m_replica->allGoods());
            m_ordersModel.setModelData(m_replica->orders());

            connect(m_replica, &AccountingServerReplica::goodAdded, &m_goodsModel, &GoodsModel::appendItem);
            connect(m_replica, &AccountingServerReplica::goodRemoved, &m_goodsModel, &GoodsModel::removeItem);
            connect(m_replica, &AccountingServerReplica::goodChanged, &m_goodsModel, &GoodsModel::changeItem);
            // Also: reset...

            connect(m_replica, &AccountingServerReplica::orderAdded, &m_ordersModel, &OrdersModel::appendItem);
            // Also: changed, removed, reset...
        }
    });
}
