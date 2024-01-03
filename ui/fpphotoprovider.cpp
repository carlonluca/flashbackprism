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
#include <QTemporaryFile>
#include <QDir>
#include <QDesktopServices>
#include <QStandardPaths>
#ifdef Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>
#endif

#include <lqtutils_net.h>
#include <lqtutils_ui.h>
#include <lqtutils_qsl.h>
#include <lqtutils_string.h>

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
            emit imageDownloaded(m_image, m_data);
            return;
        case LQTDownloaderState::S_NETWORK_FAILURE:
        case LQTDownloaderState::S_IO_FAILURE:
        case LQTDownloaderState::S_ABORTED:
            qWarning() << "Failed to download photo:" << m_downloader->state();
            emit finished();
            emit imageDownloaded(QImage(), QByteArray());
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

bool FPPhotoViewStore::share(FPQueryResultItem* item)
{
    const QString filePath = saveToTempFile(item);
    if (filePath.isNull())
        return false;

    return lqt::QmlUtils::shareResource(QUrl::fromLocalFile(filePath),
                                        QSL("image/png"),
                                        QSL("luke.flashbackprism.qtprovider"));
}

bool FPPhotoViewStore::open(FPQueryResultItem* item)
{
    const QString filePath = saveToTempFile(item);
    if (filePath.isNull())
        return false;

    return QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void FPPhotoViewStore::download(FPQueryResultItem* item, QJSValue callback)
{
#ifdef Q_OS_ANDROID
    auto future = QtAndroidPrivate::requestPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    future.then(this, [item, callback, this] (QtAndroidPrivate::PermissionResult result) {
        if (result != QtAndroidPrivate::PermissionResult::Authorized) {
            if (!callback.isNull() && callback.isCallable()) {
                callback.call(QJSValueList());
                return;
            }

            return;
        }
#endif

        const QString fileName = getFileName(item);
        if (fileName.isEmpty()) {
            qWarning() << "PhotoPrism does not seem to have provided a file name";
            callback.call(QJSValueList());
            return;
        }

        const QString filePath = lqt::path_combine({
            QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
            fileName
        });

        qDebug() << "Write to file:" << filePath;

        QFile f(filePath);
        if (!f.open(QIODevice::WriteOnly)) {
            qWarning() << "Cannot open file" << filePath << "for writing";
            callback.call(QJSValueList());
            return;
        }

        if (f.write(m_lastPhotoData) != m_lastPhotoData.size()) {
            qWarning() << "Incomplete write to file" << filePath;
            callback.call(QJSValueList());
            return;
        }

        callback.call({ filePath });

#ifdef Q_OS_ANDROID
    });
#endif
}

bool FPPhotoViewStore::cleanTempFile(FPQueryResultItem* item)
{
    const QString filePath = getTempFilePath(item);
    if (filePath.isEmpty()) {
        qWarning() << "PhotoPrism does not seem to have provided a file name";
        return false;
    }

    return QFile(filePath).remove();
}

QString FPPhotoViewStore::saveToTempFile(FPQueryResultItem* item)
{
    const QString filePath = getTempFilePath(item);
    if (filePath.isEmpty()) {
        qWarning() << "PhotoPrism does not seem to have provided a file name";
        return QString();
    }

    QFile tempFile(filePath);
    if (!tempFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open temporary file";
        return QString();
    }

    if (tempFile.write(m_lastPhotoData) != m_lastPhotoData.size()) {
        qWarning() << "Failed to save image to temporary location";
        return QString();
    }

    qDebug() << "File saved" << tempFile.fileName();
    return QFileInfo(tempFile).absoluteFilePath();
}

QString FPPhotoViewStore::getFileName(FPQueryResultItem* item)
{
    if (!item)
        return QString();

    const QFileInfo fileInfo(item->FileName());
    const QString fileBaseName = fileInfo.completeBaseName();
    const QString fileExt = fileInfo.completeSuffix();

    return fileBaseName + "." + fileExt;
}

QString FPPhotoViewStore::getTempFilePath(FPQueryResultItem *item)
{
    const QString fileName = getFileName(item);
    if (fileName.isEmpty()) {
        qWarning() << "PhotoPrism does not seem to have provided a file name";
        return QString();
    }

    return lqt::path_combine({
        QDir::tempPath(),
        fileName
    });
}
