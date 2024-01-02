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
 * Date:    2023.12.31
 */

#ifndef FPPHOTOPROVIDER_H
#define FPPHOTOPROVIDER_H

#include "lqtutils_net.h"
#include <QObject>
#include <QQuickAsyncImageProvider>

class FPPhotoResponse : public QQuickImageResponse
{
    Q_OBJECT
public:
    FPPhotoResponse(const QString& hash, const QSize& requestedSize);
    ~FPPhotoResponse();

    QQuickTextureFactory* textureFactory() const override;
    void cancel() override;

signals:
    void imageDownloaded(const QImage& image);

private:
    lqt::Downloader* m_downloader;
    QImage m_image;
    QByteArray m_data;
};

class FPPhotoProvider : public QQuickAsyncImageProvider
{
    Q_OBJECT
public:
    QQuickImageResponse* requestImageResponse(const QString& id,
                                              const QSize& requestedSize) override;

signals:
    void imageDownloaded(const QImage& image);
};

class FPPhotoViewStore : public QObject
{
    Q_OBJECT
    L_RW_PROP_AS(QImage, lastPhoto)
public:
    FPPhotoViewStore(QObject* parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void copyToClipboard();
    Q_INVOKABLE bool share();
};

#endif // FPPHOTOPROVIDER_H
