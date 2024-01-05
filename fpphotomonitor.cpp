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

FPPhotoMonitor::FPPhotoMonitor(QObject *parent)
    : QObject { parent }
{
    m_refreshModel.setInterval(FPPersistentSetupNot().modelRefreshInterval(true));
    m_refreshModel.setSingleShot(false);
    connect(&m_refreshModel, &QTimer::timeout,
            this, &FPPhotoMonitor::refreshModel);
}

FPPhotoMonitor::~FPPhotoMonitor() {}

void FPPhotoMonitor::start()
{
    m_refreshModel.start();
}

void FPPhotoMonitor::stop()
{
    m_refreshModel.stop();
}

void FPPhotoMonitor::refreshModel()
{
    set_working(true);

    FPFlashbackYearsRequest* request = new FPFlashbackYearsRequest(this);
    connect(request, &FPFlashbackYearsRequest::requestFailed,
            this, &FPPhotoMonitor::errorOccurred);
    connect(request, &FPFlashbackYearsRequest::requestSucceeded,
            this, &FPPhotoMonitor::handleResult);
    connect(request, &FPFlashbackYearsRequest::requestFailed,
            request, &FPFlashbackYearsRequest::deleteLater);
    connect(request, &FPFlashbackYearsRequest::requestSucceeded,
            request, &FPFlashbackYearsRequest::deleteLater);
    request->request();
}

void FPPhotoMonitor::handleResult(const FPFlashbackYearList& items)
{
    set_working(false);
    if (!m_refreshModel.isActive())
        return;
    resetModel(items);
}

void FPPhotoMonitor::handleFailure(QNetworkReply::NetworkError /* error */)
{
    set_working(false);
    if (!m_refreshModel.isActive())
        return;
    resetModel(FPFlashbackYearList());
}

void FPPhotoMonitor::resetModel(const FPFlashbackYearList& model)
{
    m_flashbackYears->refreshModel(model);
    m_flashbackYearsBackend = model;
    qDebug() << "Model size:" << m_flashbackYearsBackend.size();
}
