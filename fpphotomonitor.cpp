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

    // TODO: handle failure
    FPFlashbackYearsRequest* request = new FPFlashbackYearsRequest(this);
    connect(request, &FPFlashbackYearsRequest::requestFailed, this, [request] {

    });
    connect(request, &FPFlashbackYearsRequest::requestSucceeded,
            this, &FPPhotoMonitor::handleResult);
    request->request();
}

void FPPhotoMonitor::stop()
{

}

void FPPhotoMonitor::handleResult(const QList<FPFlashbackYear*>& items)
{
    resetModel(items);
    set_working(false);
}

void FPPhotoMonitor::resetModel(const QList<FPFlashbackYear*>& model)
{
    qDeleteAll(m_flashbackYears);
    set_flashbackYears(model);
}
