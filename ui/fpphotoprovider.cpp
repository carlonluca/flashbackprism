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

#include <QGuiApplication>
#include <QClipboard>

#include <lqtutils_net.h>

#include "fpphotoprovider.h"
#include "fpqmlutils.h"

FPPhotoResponse::FPPhotoResponse(const QString& hash, const QSize& requestedSize) :
    QQuickImageResponse()
{
    m_downloader = new lqt::Downloader(FPQmlUtils::photoUrl(hash), &m_data);
    connect(m_downloader, &lqt::Downloader::stateChanged, this, [this] {
        switch (m_downloader->state()) {
        case LQTDownloaderState::S_IDLE:
        case LQTDownloaderState::S_DOWNLOADING:
            return;
        case LQTDownloaderState::S_DONE:
            m_image = QImage::fromData(m_data);
            if (m_image.isNull())
                qWarning() << "Failed to decode image";
            emit finished();
            emit imageDownloaded(m_image);
            return;
        case LQTDownloaderState::S_NETWORK_FAILURE:
        case LQTDownloaderState::S_IO_FAILURE:
        case LQTDownloaderState::S_ABORTED:
            qWarning() << "Failed to download photo:" << m_downloader->state();
            emit finished();
            emit imageDownloaded(QImage());
            return;
        }
    });
    m_downloader->download();
}

FPPhotoResponse::~FPPhotoResponse()
{
    m_downloader->deleteLater();
}

QQuickTextureFactory* FPPhotoResponse::textureFactory() const
{
    return QQuickTextureFactory::textureFactoryForImage(m_image);
}

void FPPhotoResponse::cancel()
{
    m_downloader->abort();
}

QQuickImageResponse* FPPhotoProvider::requestImageResponse(const QString& id, const QSize& requestedSize)
{
    auto imageResponse = new FPPhotoResponse(id, requestedSize);
    connect(imageResponse, &FPPhotoResponse::imageDownloaded,
            this, &FPPhotoProvider::imageDownloaded);
    return imageResponse;
}

void FPPhotoViewStore::copyToClipboard()
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    if (!clipboard) {
        qWarning() << "Failed to get clipboard";
        return;
    }

    clipboard->setImage(m_lastPhoto);
}