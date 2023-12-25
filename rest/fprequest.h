/*
 * This file is part of mldonket-next.
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
 * Date:    2023.25.13
 */

#ifndef FPREQUEST_H
#define FPREQUEST_H

#include <QObject>

#include <lqtutils_prop.h>

class QNetworkAccessManager;

class FPRequest : public QObject
{
    Q_OBJECT
    L_RW_PROP_AS(bool, working, false)
public:
    explicit FPRequest(QObject *parent = nullptr);

protected:
    QNetworkAccessManager* m_man;
};

#endif // FPREQUEST_H
