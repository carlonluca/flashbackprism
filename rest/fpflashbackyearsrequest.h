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

#ifndef FPFLASHBACKYEARSREQUEST_H
#define FPFLASHBACKYEARSREQUEST_H

#include <QObject>
#include <QSharedPointer>

#include <lqtutils_prop.h>

#include <data/fpqueryresultitem.h>
#include <rest/fprequest.h>

L_BEGIN_CLASS(FPFlashbackYear)
L_RW_PROP_AS(int, year, 0)
L_RW_PROP_REF_AS(QList<FPQueryResultItem*>, items)
public:
    ~FPFlashbackYear() { qDebug() << Q_FUNC_INFO; qDeleteAll(m_items); }
L_END_CLASS
typedef QSharedPointer<FPFlashbackYear> FPFlashbackYearRef;
typedef QList<FPFlashbackYearRef> FPFlashbackYearList;

class FPFlashbackYearsRequest : public FPRequest
{
    Q_OBJECT
public:
    explicit FPFlashbackYearsRequest(QObject* parent = nullptr);

    void request();

signals:
    void requestFailed();
    void requestSucceeded(FPFlashbackYearList years);

private:
    void handleResult(QList<FPQueryResultItem*> items);
};

#endif // FPFLASHBACKYEARSREQUEST_H
