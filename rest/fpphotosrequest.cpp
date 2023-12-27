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

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <lqtutils/lqtutils_qsl.h>
#include <lqtutils/lqtutils_autoexec.h>
#include <lserializer.h>

#include <data/fpqueryresultitem.h>
#include <data/fppersistentsetup.h>

#include "fpphotosrequest.h"

// curl -X GET -H "X-Session-ID: ..." "http://.../api/v1/photos?count=100&year=&month=12&day=25"

FPPhotosRequest::FPPhotosRequest(QObject* parent)
    : FPRequest{parent}
{}

void FPPhotosRequest::request(std::optional<int> count,
                              std::optional<int> year,
                              std::optional<int> month,
                              std::optional<int> day)
{
    set_working(true);

    QUrlQuery query;
    if (count)
        query.addQueryItem(QSL("count"), QString::number(*count));
    if (year)
        query.addQueryItem(QSL("year"), QString::number(*year));
    if (month)
        query.addQueryItem(QSL("month"), QString::number(*month));
    if (day)
        query.addQueryItem(QSL("day"), QString::number(*day));

    if (url().isEmpty()) {
        qWarning() << "Missing URL";
        return;
    }

    if (token().isNull()) {
        qWarning() << "Missing token";
        return;
    }

    QUrl _url = url();
    _url.setPath(QSL("/api/v1/photos"));
    _url.setQuery(query);

    QNetworkRequest req(_url);
    req.setRawHeader("X-Session-ID", token().toUtf8());

    QNetworkReply* reply = m_man->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        lqt::AutoExec exec([reply, this] {
            set_working(false);
            reply->deleteLater();
        });

        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "Photos request failed:" << reply->errorString();
            emit requestFailed();
            return;
        }

        handleResponse(reply->readAll());
    });
}

QUrl FPPhotosRequest::thumbnailUrl(FPQueryResultItem* item, int size)
{
    Q_ASSERT(item);
    if (!item)
        return QUrl();

    FPPersistentSetup setup;
    QUrl baseUrl = setup.photoprismUrl();
    baseUrl.setPath(QSL("/api/v1/t/%1/%2/tile_500").arg(item->Hash(), setup.previewToken()));

    return baseUrl;
}

QUrl FPPhotosRequest::photoUrl(FPQueryResultItem *item)
{
    Q_ASSERT(item);
    if (!item)
        return QUrl();

    FPPersistentSetup setup;
    QUrlQuery query;
    query.addQueryItem(QSL("t"), setup.downloadToken());

    QUrl baseUrl = setup.photoprismUrl();
    baseUrl.setPath(QSL("/api/v1/dl/%1").arg(item->Hash()));
    baseUrl.setQuery(query);

    return baseUrl;
}

void FPPhotosRequest::request(int count, int month, int day)
{
    request(count, std::nullopt, month, day);
}

void FPPhotosRequest::handleResponse(const QByteArray& data)
{
    qDebug() << "Downloaded:" << data.size();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qWarning() << "Failed to decode json:" << data;
        return;
    }

    QJsonArray json = doc.array();

    lqo::Deserializer<FPQueryResultItem> des;
    const QList<FPQueryResultItem*> items = des.deserializeObjectArray(json);

    emit requestSucceeded(items);
}
