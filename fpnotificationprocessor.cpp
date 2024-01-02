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

#include <QGuiApplication>

#include <data/fppersistentsetup.h>

#include <lqtutils_ui.h>
#include <lqtutils_qsl.h>

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
    if (now.date() > last && now.time().hour() >= 8) {
        sendNotificationIfNeeded();
        return;
    }

    qDebug() << "No notification:" << last.toString() << now.date().toString() << m_photoMonitor->flashbackYears()->rowCount();
}

void FPNotificationProcessor::sendNotificationIfNeeded()
{
    qDebug() << "Not:" << m_photoMonitor->flashbackYears()->rowCount();
    if (m_photoMonitor->flashbackYears()->rowCount() <= 0)
        return;



    qDebug() << "Send notification";
    lqt::SystemNotification notification;
    notification.set_appName(qApp->applicationName());
    notification.set_message(QSL("Message"));
#ifdef Q_OS_ANDROID
    notification.set_icon(QImage(":/qt/qml/FlashbackPrism/assets/icon_96.png"));
#endif
    notification.send();

    FPPersistentSetup().set_lastNotification(QDateTime::currentDateTime().date());
}
