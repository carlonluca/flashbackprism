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

#ifndef FPPHOTOMONITOR_H
#define FPPHOTOMONITOR_H

#include <QObject>
#include <QTimer>
#include <QAbstractListModel>

#include <lqtutils_prop.h>

#include "rest/fpflashbackyearsrequest.h"

template<typename T>
class QmlSharedPointerList : public QAbstractListModel
{
public:
    QmlSharedPointerList<T>(const QList<QSharedPointer<T>>& list = QList<QSharedPointer<T>>(), QObject* parent = nullptr) :
        QAbstractListModel(parent), m_list(list) {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        return static_cast<int>(m_list.count());
    }

    QVariant data(const QModelIndex& index, int role = Qt::UserRole) const override {
        if (role != Qt::UserRole)
            return QVariant();
        if (index.row() < 0 || index.row() >= m_list.size())
            return QVariant();

        return QVariant::fromValue<T*>(m_list[index.row()].data());
    }

    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles.insert(Qt::UserRole, "data");
        return roles;
    }

    void refreshModel(const QList<QSharedPointer<T>>& list) {
        beginResetModel();
        m_list = list;
        endResetModel();
    }

private:
    QList<QSharedPointer<T>> m_list;
};

class FPPhotoMonitor : public QObject
{
    Q_OBJECT
    L_RW_PROP_AS(QmlSharedPointerList<FPFlashbackYear>*, flashbackYears, new QmlSharedPointerList(QList<QSharedPointer<FPFlashbackYear>>(), this))
    L_RW_PROP_AS(bool, working, false)
public:
    explicit FPPhotoMonitor(QObject* parent = nullptr);
    ~FPPhotoMonitor();

public slots:
    void start();
    void stop();
    void refreshModel();

private slots:
    void handleResult(const FPFlashbackYearList& items);
    void resetModel(const FPFlashbackYearList& model);

private:
    QTimer m_refreshModel;
};

#endif // FPPHOTOMONITOR_H
