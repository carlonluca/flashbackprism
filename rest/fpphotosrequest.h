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

#ifndef FPPHOTOSREQUEST_H
#define FPPHOTOSREQUEST_H

#include <QObject>
#include <QNetworkReply>

#include "fprequest.h"

class FPQueryResultItem;

class FPPhotosRequest : public FPRequest
{
    Q_OBJECT
public:
    explicit FPPhotosRequest(QObject* parent = nullptr);

    bool request(std::optional<int> count,
                 std::optional<int> year,
                 std::optional<int> month,
                 std::optional<int> day);

public slots:
    bool request(int count,
                 int month,
                 int day);

private:
    void handleResponse(const QByteArray& data);

signals:
    void requestFailed(QNetworkReply::NetworkError error);
    void requestSucceeded(QList<FPQueryResultItem*> items);
};

#endif // FPPHOTOSREQUEST_H
