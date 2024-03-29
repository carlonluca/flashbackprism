/*
 * This file is part of FlashbackPrism.
 *
 * Copyright (c) 2023 Luca Carlon
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Author:  Luca Carlon
 * Company: -
 * Date:    2023.12.25
 */

#include <QJsonDocument>
#include <QJsonObject>

#include <lqtutils/lqtutils_net.h>
#include <lqtutils/lqtutils_qsl.h>
#include <lqtutils/lqtutils_autoexec.h>
#include <lserializer.h>

#include <data/fplogindata.h>

#include "fploginrequest.h"

// curl -X POST -H "Content-Type: application/json" -d '{"username": "luca", "password": "..."}' http://.../api/v1/session

FPLoginRequest::FPLoginRequest(QObject *parent)
    : FPRequest { parent }
{}

void FPLoginRequest::login(const QUrl& url, const QString& uname, const QString& pwd)
{
    set_working(true);

    QUrl _url = url;
    _url.setPath(QSL("/api/v1/session"));

    FPLoginCredentials credentials;
    credentials.set_username(uname);
    credentials.set_password(pwd);

    lqo::Serializer ser;
    QJsonObject loginInput = ser.serialize<FPLoginCredentials>(&credentials);

    QByteArray inputData = QJsonDocument(loginInput).toJson(QJsonDocument::Compact);
    QNetworkRequest req(_url);
    req.setTransferTimeout(10*1000);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QSL("application/json"));
    req.setHeader(QNetworkRequest::ContentLengthHeader, inputData.length());

    QNetworkReply* reply = m_man->post(req, inputData);
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        lqt::AutoExec exec([reply, this] {
            set_working(false);
            reply->deleteLater();
        });

        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "Login request failed:" << reply->errorString();
            emit loginFailed();
            return;
        }

        handleResponse(reply->readAll());
    });
}

void FPLoginRequest::handleResponse(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qWarning() << "Failed to parse json";
        emit loginFailed();
        return;
    }

    const QJsonObject obj = doc.object();
    if (obj.contains(QSL("error"))) {
        qWarning() << "Service returned error:" << obj["error"];
        emit loginFailed();
        return;
    }

    lqo::Deserializer<FPLoginResponse> des;
    QScopedPointer<FPLoginResponse> loginData(des.deserialize(obj));
    if (!loginData) {
        qDebug() << "Failed to parse login data";
        emit loginFailed();
        return;
    }

    qDebug() << "Token received:" << loginData->id();

    const QString token = loginData->id();
    const QString downloadToken = loginData->config() ? loginData->config()->downloadToken() : QString();
    const QString previewToken = loginData->config() ? loginData->config()->previewToken() : QString();

    emit loginSucceeded(token, downloadToken, previewToken);
}
