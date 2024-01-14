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

#ifndef FPQUERYRESULTITEM_H
#define FPQUERYRESULTITEM_H

#include <QString>
#include <QDateTime>

#include <lqtutils_prop.h>

Q_NAMESPACE

L_BEGIN_CLASS(FPQueryResultItem)
L_RW_PROP_AS(QString, ID)
L_RW_PROP_AS(QString, UID)
L_RW_PROP_AS(QDateTime, TakenAt)
L_RW_PROP_AS(int, Year, -1)
L_RW_PROP_AS(int, Month, -1)
L_RW_PROP_AS(int, Day, -1)
L_RW_PROP_AS(QString, FileName)
L_RW_PROP_AS(QString, FileRoot)
L_RW_PROP_AS(QString, Name)
L_RW_PROP_AS(QString, Path)
L_RW_PROP_AS(QString, Hash)
L_RW_PROP_AS(QString, Type)
public:
    Q_INVOKABLE bool isSidecar() { return m_FileRoot == QStringLiteral("sidecar"); }
    Q_INVOKABLE bool isVideo() { return m_Type == QStringLiteral("video"); }
    Q_INVOKABLE bool isImage() { return m_Type == QStringLiteral("image"); }
L_END_CLASS

#endif // FPQUERYRESULTITEM_H
