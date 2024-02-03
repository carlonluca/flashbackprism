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

#ifndef FPREQUESTPHOTOACTION_H
#define FPREQUESTPHOTOACTION_H

#include <QObject>
#include <QNetworkReply>

#include <lqtutils_prop.h>

#include "fprequest.h"

L_BEGIN_CLASS(FPRequestPhotoActionList)
L_RW_PROP_AS(QStringList, photos)
L_END_CLASS

class FPRequestPhotoAction : public FPRequest
{
    Q_OBJECT
    L_RW_PROP_AS(QStringList, photos)
public:
    explicit FPRequestPhotoAction(QObject* parent = nullptr);

protected:
    QJsonObject dataJson();
};

class FPRequestPhotoArchive : public FPRequestPhotoAction
{
    Q_OBJECT
public:
    explicit FPRequestPhotoArchive(QObject* parent = nullptr);

    Q_INVOKABLE bool request();

private:
    void handleResponse(const QByteArray& data);

signals:
    void errorOccurred(QNetworkReply::NetworkError error);
    void succeeded();
};

#endif // FPREQUESTPHOTOACTION_H
