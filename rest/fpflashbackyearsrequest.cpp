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

#include "fpflashbackyearsrequest.h"
#include "data/fppersistentsetup.h"
#include "rest/fpphotosrequest.h"

FPFlashbackYearsRequest::FPFlashbackYearsRequest(QObject *parent)
    : FPRequest{parent}
{}

void FPFlashbackYearsRequest::request()
{
    const QDateTime now = QDateTime::currentDateTime();
    const int count = std::numeric_limits<int>::max();
    const int month = now.date().month();
    const int day = now.date().day();

    FPPhotosRequest* request = new FPPhotosRequest(this);
    request->set_url(FPPersistentSetup().photoprismUrl());
    request->set_token(FPPersistentSetup().token());
    connect(request, &FPPhotosRequest::requestSucceeded,
            this, &FPFlashbackYearsRequest::handleResult);
    connect(request, &FPPhotosRequest::requestFailed,
            this, &FPFlashbackYearsRequest::requestFailed);
    connect(request, &FPPhotosRequest::requestSucceeded,
            request, &FPPhotosRequest::deleteLater);
    connect(request, &FPPhotosRequest::requestFailed,
            request, &FPPhotosRequest::deleteLater);
    if (!request->request(count, month, day))
        delete request;
}

void FPFlashbackYearsRequest::handleResult(QList<FPQueryResultItem*> items)
{
    QHash<int, FPFlashbackYearRef> flashbackYearsMap;
    for (FPQueryResultItem* item : items) {
        if (!item)
            continue;

        const int year = item->TakenAt().date().year();
        QSharedPointer<FPFlashbackYear> flashbackYear;
        if (flashbackYearsMap.contains(year))
            flashbackYear = flashbackYearsMap[year];
        if (!flashbackYear) {
            flashbackYear.reset(new FPFlashbackYear);
            flashbackYear->set_year(year);
            flashbackYearsMap.insert(year, flashbackYear);
        }
        flashbackYear->items().append(item);
    }

    FPFlashbackYearList flashbackYears = flashbackYearsMap.values();
    std::sort(flashbackYears.begin(), flashbackYears.end(), [] (const FPFlashbackYearRef& y1, FPFlashbackYearRef& y2) {
        return y1->year() > y2->year();
    });

    set_working(false);

    emit requestSucceeded(flashbackYears);
}
