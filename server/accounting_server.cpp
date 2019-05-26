/****************************************************************************
**
** Copyright (C) 2017 Ford Motor Company
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtRemoteObjects module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "accounting_server.h"

#include "databasecontroller.h"

namespace Constants {

static const QString login  { "admin" };
static const QString pwd    { "1" };

}

#define CHECK_EMPLOYEE                                      \
    do {                                                    \
        qDebug() << Q_FUNC_INFO;                            \
        if (!m_loggedInEmployees.contains(employeeId)) {    \
            qDebug() << Q_FUNC_INFO << " Unknown user";     \
            return;                                         \
        }                                                   \
    } while(0)

#define CHECK_SQL_ERR                                               \
    do {                                                            \
        if (err.type() != QSqlError::NoError) {                     \
            qDebug() << Q_FUNC_INFO << " Database error " << err;   \
            return;                                                 \
        }                                                           \
     } while(0)

AccountingServer::AccountingServer(QObject*)
{

}

AccountingServer::~AccountingServer()
{

}

bool AccountingServer::initialize()
{
    QSqlError err = database->getAllGoods(m_goods);

    if (err.type() != QSqlError::NoError) {
        qDebug() << Q_FUNC_INFO << " Database error " << err;
        return false;
    }

    err = database->getAllOrders(m_orders);

    if (err.type() != QSqlError::NoError) {
        qDebug() << Q_FUNC_INFO << " Database error " << err;
        return false;
    }

    setAllGoods(m_goods);
    setOrders(m_orders);

    return true;
}

void AccountingServer::orderGood(const GoodsInfo& sourceGood, const PersonalInfo& custumerInfo)
{
    QList<GoodsInfo>::iterator neededGood = std::find_if(m_goods.begin(), m_goods.end(), [&sourceGood] (const GoodsInfo& inf) {
        return inf.vendorCode() == sourceGood.vendorCode();
    });

    if (neededGood == m_goods.end()) {
        qDebug() << "No such good";
        return;
    }

    int count = neededGood->count() - sourceGood.count();

    if (count < 0) {
        qDebug() << "Incorrect product count";
        return;
    }

    QSqlError err = database->orderGood(sourceGood, custumerInfo, count);

    CHECK_SQL_ERR;

    if (count == 0) {
        emit goodRemoved(m_goods.indexOf(*neededGood));
        m_goods.erase(neededGood);
    } else {
        emit goodChanged(m_goods.indexOf(*neededGood), *neededGood);
    }

    neededGood->setCount(count);
    setAllGoods(m_goods);
    emit allGoodsChanged(m_goods);

    Order o;
    o.setCustomer(custumerInfo);
    o.setGood(sourceGood);

    m_orders.push_back(o);

    emit orderAdded(o);

    setOrders(m_orders);
}

QUuid AccountingServer::loginAsEmployee(const QString& login, const QString& password)
{
    if (login != Constants::login || password != Constants::pwd) {
        return { };
    }

    QUuid id = QUuid::createUuid();
    m_loggedInEmployees.push_back(id);

    return id;
}

void AccountingServer::addGood(const GoodsInfo& sourceGood, const QUuid& employeeId)
{
    CHECK_EMPLOYEE;

    QList<GoodsInfo>::iterator neededGood = std::find_if(m_goods.begin(), m_goods.end(), [&sourceGood] (const GoodsInfo& inf) {
        return inf.vendorCode() == sourceGood.vendorCode();
    });

    if (neededGood == m_goods.end()) {
        QSqlError err = database->addNewGood(sourceGood);

        CHECK_SQL_ERR;

        m_goods.push_back(sourceGood);

        emit goodAdded(sourceGood);
    } else {
        if (sourceGood.count() <= 0) {
            qDebug() << "Incorrect good count";
            return;
        }

        QSqlError err = database->addGood(sourceGood.vendorCode(), sourceGood.count());

        CHECK_SQL_ERR;

        neededGood->setCount(sourceGood.count());

        emit goodChanged(m_goods.indexOf(*neededGood), *neededGood);
    }

    setAllGoods(m_goods);
    emit allGoodsChanged(m_goods);
}

void AccountingServer::removeGood(const GoodsInfo& sourceGood, const QUuid& employeeId)
{
    CHECK_EMPLOYEE;

    QList<GoodsInfo>::iterator neededGood = std::find_if(m_goods.begin(), m_goods.end(), [&sourceGood] (const GoodsInfo& inf) {
        return inf.vendorCode() == sourceGood.vendorCode();
    });

    if (neededGood == m_goods.end()) {
        qDebug() << "No such good to remove";
        return;
    }

    auto err = database->removeGood(sourceGood.vendorCode());

    CHECK_SQL_ERR;

    emit goodRemoved(m_goods.indexOf(*neededGood));

    m_goods.removeOne(*neededGood);

    setAllGoods(m_goods);
}

void AccountingServer::logOff(const QUuid& employeeId)
{
    CHECK_EMPLOYEE;
    m_loggedInEmployees.removeOne(employeeId);
}


