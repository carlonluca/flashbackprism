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
 * Date:    2023.12.28
 */

#ifndef FPQMLUTILS_H
#define FPQMLUTILS_H

#include <QObject>
#include <QDateTime>

#include <data/fpqueryresultitem.h>

class FPQmlUtils : public QObject
{
    Q_OBJECT
public:
    explicit FPQmlUtils(QObject* parent = nullptr);

    Q_INVOKABLE QString formatDateForPhoto(const QDateTime& dateTime);

    Q_INVOKABLE QUrl thumbnailUrl(FPQueryResultItem* item, int size);
    Q_INVOKABLE QUrl photoUrl(FPQueryResultItem* item);
};

#endif // FPQMLUTILS_H
