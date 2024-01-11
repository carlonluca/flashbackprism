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
 * Date:    2023.12.24
 */

#include <QUrl>
#include <QUrlQuery>
#include <QSslSocket>

#include <lqtutils_qsl.h>

#include "fpqmlutils.h"
#include "data/fppersistentsetup.h"

FPQmlUtils::FPQmlUtils(QObject *parent)
    : QObject{parent}
{}

QString FPQmlUtils::formatDateForPhoto(const QDateTime& dateTime)
{
    if (dateTime.isNull())
        return QSL("-");
    return dateTime.toString("MM/dd");
}

QString FPQmlUtils::formatDateTimeForPhoto(const QDateTime &dateTime)
{
    if (dateTime.isNull())
        return QSL("-");
    return dateTime.toString("yyyy/MM/dd hh:mm");
}

QString FPQmlUtils::sslRuntimeVersion()
{
    return QSslSocket::sslLibraryVersionString();
}

QString FPQmlUtils::sslBuildVersion()
{
    return QSslSocket::sslLibraryBuildVersionString();
}

QString FPQmlUtils::qtVersion()
{
    return qVersion();
}

bool FPQmlUtils::sslSupported()
{
    return QSslSocket::supportsSsl();
}

QUrl FPQmlUtils::thumbnailUrl(FPQueryResultItem* item, int size)
{
    Q_ASSERT(item);
    if (!item)
        return QUrl();

    FPPersistentSetup setup;
    QUrl baseUrl = setup.photoprismUrl();
    baseUrl.setPath(QSL("/api/v1/t/%1/%2/tile_500").arg(item->Hash(), setup.previewToken()));

    return baseUrl;
}

QUrl FPQmlUtils::photoUrl(FPQueryResultItem* item)
{
    Q_ASSERT(item);
    if (!item)
        return QUrl();

    return photoUrl(item->Hash());
}

QUrl FPQmlUtils::photoUrl(const QString& hash)
{
    FPPersistentSetup setup;
    QUrlQuery query;
    query.addQueryItem(QSL("t"), setup.downloadToken());

    QUrl baseUrl = setup.photoprismUrl();
    baseUrl.setPath(QSL("/api/v1/dl/%1").arg(hash));
    baseUrl.setQuery(query);

    return baseUrl;
}
