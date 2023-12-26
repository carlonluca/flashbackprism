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

#include <QDateTime>

#include "fpphotomonitor.h"
#include "data/fppersistentsetup.h"
#include "rest/fpphotosrequest.h"

FPPhotoMonitor::FPPhotoMonitor(QObject *parent)
    : QObject { parent }
{}

FPPhotoMonitor::~FPPhotoMonitor()
{
    qDeleteAll(m_flashbackYears);
}

void FPPhotoMonitor::start()
{
    set_working(true);

    const QDateTime now = QDateTime::currentDateTime();
    const int count = std::numeric_limits<int>::max();
    const int month = now.date().month();
    const int day = now.date().day();

    // TODO: handle failure here.
    FPPhotosRequest* request = new FPPhotosRequest(this);
    request->set_url(FPPersistentSetup().photoprismUrl());
    request->set_token(FPPersistentSetup().token());
    request->request(count, month, day);
    connect(request, &FPPhotosRequest::requestSucceeded,
            this, &FPPhotoMonitor::handleResult);
    connect(request, &FPPhotosRequest::requestSucceeded,
            request, &FPPhotosRequest::deleteLater);
}

void FPPhotoMonitor::stop()
{

}

void FPPhotoMonitor::handleResult(const QList<FPQueryResultItem*>& items)
{
    QHash<int, FPFlashbackYear*> flashbackYearsMap;
    for (FPQueryResultItem* item : items) {
        if (!item)
            continue;

        const int year = item->TakenAt().date().year();
        FPFlashbackYear* flashbackYear = nullptr;
        if (flashbackYearsMap.contains(year))
            flashbackYear = flashbackYearsMap[year];
        if (!flashbackYear) {
            flashbackYear = new FPFlashbackYear;
            flashbackYear->set_year(year);
            flashbackYearsMap.insert(year, flashbackYear);
        }
        flashbackYear->items().append(item);


    }

    QList<FPFlashbackYear*> flashbackYears = flashbackYearsMap.values();
    std::sort(flashbackYears.begin(), flashbackYears.end(), [] (FPFlashbackYear* y1, FPFlashbackYear* y2) {
        return y1->year() > y2->year();
    });

    resetModel(flashbackYears);

    set_working(false);
}

void FPPhotoMonitor::resetModel(const QList<FPFlashbackYear*>& model)
{
    qDeleteAll(m_flashbackYears);
    set_flashbackYears(model);
}
