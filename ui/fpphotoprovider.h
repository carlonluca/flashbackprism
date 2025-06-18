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

#include <QObject>
#include <QQuickAsyncImageProvider>

#include <lqtutils_net.h>

#include <data/fpqueryresultitem.h>

class FPPhotoResponse : public QQuickImageResponse
{
    Q_OBJECT
public:
    FPPhotoResponse(const QString& hash, const QSize& requestedSize);
    ~FPPhotoResponse();

    QQuickTextureFactory* textureFactory() const override;
    QString errorString() const override;
    void cancel() override;

signals:
    void imageDownloaded(const QString& hash, const QImage& image, const QByteArray& data);
    void imageDownloadProgress(quint64 downloaded, quint64 total);

private:
    lqt::Downloader* m_downloader;
    QImage m_image;
    QByteArray m_data;
};

class FPPhotoViewStore;
class FPPhotoProvider : public QQuickAsyncImageProvider
{
    Q_OBJECT
public:
    QQuickImageResponse* requestImageResponse(const QString& id,
                                              const QSize& requestedSize) override;

signals:
    void imageDownloaded(const QString& hash, const QImage& image, const QByteArray& data);
    void imageDownloadProgress(quint64 downloaded, quint64 total);
};

class FPPhotoViewStore : public QObject
{
    Q_OBJECT
    L_RW_PROP_AS(FPQueryResultItem*, item, nullptr)
    L_RW_PROP_AS(FPPhotoProvider*, provider, nullptr)
    L_RW_PROP_AS(QImage, lastPhoto)
    L_RW_PROP_AS(QByteArray, lastPhotoData)
    L_RW_PROP_AS(qreal, progress, 0)
public:
    FPPhotoViewStore(QObject* parent = nullptr) : QObject(parent) {
        connect(this, &FPPhotoViewStore::providerChanged, this, [this] {
            if (m_provider) {
                this->disconnect();
                set_progress(0);
                connect(m_provider, &FPPhotoProvider::imageDownloaded,
                        this, &FPPhotoViewStore::onImageReceived);
                connect(m_provider, &FPPhotoProvider::imageDownloadProgress,
                        this, &FPPhotoViewStore::onImageDownloadProgress);
            }
        });
    }

    ~FPPhotoViewStore() {
        cleanTempFile();
    }

    Q_INVOKABLE void copyToClipboard();
    Q_INVOKABLE bool share();
    Q_INVOKABLE bool open();
    Q_INVOKABLE void download(QJSValue callback);
    Q_INVOKABLE bool cleanTempFile();

public slots:
    void onImageReceived(const QString& hash, const QImage& image, const QByteArray& data);
    void onImageDownloadProgress(quint64 downloaded, quint64 total);

private:
    QString saveToTempFile();
    QString getFileName();
    QString getTempFilePath();
};

#endif // FPPHOTOPROVIDER_H
