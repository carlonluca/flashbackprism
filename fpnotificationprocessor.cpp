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
 * Date:    2023.12.30
 */

#include <data/fppersistentsetup.h>

#include "fpnotificationprocessor.h"

FPNotificationProcessor::FPNotificationProcessor(FPPhotoMonitor* photoMonitor, QObject* parent)
    : QObject{parent}
    , m_photoMonitor(photoMonitor)
{
    m_timer.setSingleShot(false);
    m_timer.setInterval(10000);
    connect(&m_timer, &QTimer::timeout,
            this, &FPNotificationProcessor::process);
    m_timer.start();
}

void FPNotificationProcessor::process()
{
    const QDate last = FPPersistentSetup().lastNotification();
    if (last.isNull()) {
        sendNotificationIfNeeded();
        return;
    }

    const QDateTime now = QDateTime::currentDateTime();
    if (now.date() > last) {
        sendNotificationIfNeeded();
        return;
    }
}

void FPNotificationProcessor::sendNotificationIfNeeded()
{
    if (m_photoMonitor->flashbackYears()->rowCount() <= 0)
        return;

    qDebug() << Q_FUNC_INFO;
}
