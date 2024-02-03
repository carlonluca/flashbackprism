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
 * Date:    2023.01.15
 */

#include <QNetworkRequest>

#include <lqtutils/lqtutils_qsl.h>
#include <lqtutils/lqtutils_autoexec.h>
#include <lserializer.h>

#include "fprequestphotoaction.h"

FPRequestPhotoAction::FPRequestPhotoAction(QObject* parent) :
    FPRequest{parent} {}

QJsonObject FPRequestPhotoAction::dataJson()
{
    FPRequestPhotoActionList list;
    list.set_photos(m_photos);

    lqo::Serializer serializer;
    return serializer.serialize(&list);
}

FPRequestPhotoArchive::FPRequestPhotoArchive(QObject* parent) :
    FPRequestPhotoAction{parent} {}

[[nodiscard]]
bool FPRequestPhotoArchive::request()
{
    set_working(true);

    if (!validateSetup())
        return false;

    QUrl _url = url();
    _url.setPath(QSL("/api/v1/batch/photos/archive"));

    const QJsonObject json = dataJson();
    const QString jsonString = QJsonDocument(json).toJson(QJsonDocument::Compact);
    const QByteArray jsonData = jsonString.toUtf8();

    QNetworkRequest req(_url);
    req.setRawHeader("X-Session-ID", token().toUtf8());
    req.setHeader(QNetworkRequest::ContentTypeHeader, QSL("application/json"));
    req.setHeader(QNetworkRequest::ContentLengthHeader, jsonData.length());

    QNetworkReply* reply = m_man->post(req, jsonData);
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        lqt::AutoExec exec([reply, this] {
            set_working(false);
            reply->deleteLater();
        });

        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "Could not archive photo";
            emit errorOccurred(reply->error());
            return;
        }

        handleResponse(reply->readAll());
    });

    return true;
}

void FPRequestPhotoArchive::handleResponse(const QByteArray& data)
{
    if (data.isEmpty()) {
        emit errorOccurred(QNetworkReply::UnknownServerError);
        return;
    }

    const QJsonObject json = QJsonDocument::fromJson(data).object();
    if (json.isEmpty()) {
        emit errorOccurred(QNetworkReply::UnknownServerError);
        return;
    }

    FPRequestResponseRef res(lqo::Deserializer<FPRequestResponse>().deserialize(json));
    if (!res) {
        emit errorOccurred(QNetworkReply::UnknownServerError);
        return;
    }

    if (res->code() == 200) {
        emit succeeded();
        return;
    }

    emit errorOccurred(QNetworkReply::UnknownServerError);
}
