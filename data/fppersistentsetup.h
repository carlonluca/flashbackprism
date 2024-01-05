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
 * Date:    2023.12.24
 */

#ifndef FPPERSISTENTSETUP_H
#define FPPERSISTENTSETUP_H

#include <QObject>
#include <QUrl>
#include <QDate>

#include <lqtutils/lqtutils_settings.h>
#include <lqtutils/lqtutils_prop.h>

Q_NAMESPACE

inline qint64 mintomillis(int mins)
{
    return (qint64)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::minutes(mins)).count();
}

L_DECLARE_SETTINGS(FPPersistentSetup, new QSettings)
L_DEFINE_VALUE(QUrl, photoprismUrl, QUrl())
L_DEFINE_VALUE(QString, token, QString())
L_DEFINE_VALUE(QString, downloadToken, QString())
L_DEFINE_VALUE(QString, previewToken, QString())
L_DEFINE_VALUE(QDate, lastNotification, QDate())
L_END_CLASS

L_DECLARE_SETTINGS(FPPersistentSetupNot, new QSettings, "NOTIFICATIONS")
L_DEFINE_VALUE(qint64, modelRefreshInterval, mintomillis(30))
L_END_CLASS

#endif // FPPERSISTENTSETUP_H
