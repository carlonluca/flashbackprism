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
    m_timer.setInterval(std::chrono::milliseconds(std::chrono::minutes(30)).count());
    connect(&m_timer, &QTimer::timeout,
            this, &FPNotificationProcessor::process);
    m_timer.start();
}

void FPNotificationProcessor::process()
{
    const QDateTime now = QDateTime::currentDateTime();
    const QDate last = FPPersistentSetup().lastNotification();
    if (now.time().hour() < 8) {
        qDebug() << "Won't send a notification at" << now.time();
        return;
    }

    if (last.isNull()) {
        sendNotificationIfNeeded();
        return;
    }

    if (now.date() > last) {
        sendNotificationIfNeeded();
        return;
    }
}

void FPNotificationProcessor::sendNotificationIfNeeded()
{
    const int years = m_photoMonitor->flashbackYears()->rowCount();
    if (years <= 0) {
        qDebug() << "Won't send a notification, no memories for today";
        return;
    }

    int photos = 0;
    for (const FPFlashbackYearRef& year : m_photoMonitor->flashbackYearsBackend())
        if (year)
            photos += year->items().size();

    qDebug() << "Send notification";
#ifdef Q_OS_ANDROID
    lqt::AndroidSystemNotification notification;
    notification.set_icon(QImage(":/qt/qml/FlashbackPrism/assets/icon_notification.png"));
    notification.set_activityClass(QSL("org.qtproject.qt.android.bindings.QtActivity"));
#else
    lqt::SystemNotification notification;
#endif
    notification.set_appName(qApp->applicationName());
    notification.set_title(tr("Flashbacks available"));
    notification.set_message(tr("You have %1 memories taken in %2 years for today. Have a look!").arg(photos).arg(years));
    notification.set_openApp(true);
    notification.send();

    FPPersistentSetup().set_lastNotification(QDateTime::currentDateTime().date());
}
