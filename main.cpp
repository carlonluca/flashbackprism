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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDirIterator>
#include <QFileInfo>
#ifdef Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>
#include <QJniObject>
#endif

#include <lqtutils/lqtutils_qsl.h>
#include <lqtutils/lqtutils_ui.h>

#include "fpnotificationprocessor.h"
#include "fpphotomonitor.h"
#include "fpqmlutils.h"
#include "lqtutils_fa.h"
#include "rest/fploginrequest.h"
#include "rest/fpphotosrequest.h"
#include "data/fppersistentsetup.h"
#include "ui/fpphotoprovider.h"

#include <QtQml/QQmlExtensionPlugin>
Q_IMPORT_QML_PLUGIN(lqtutilsPlugin)

#define COLORING_ENABLED
#define LOG_TAG "FlashbackPrism"
#include <lightlogger/lc_logging.h>
lightlogger::custom_log_func lightlogger::global_log_func = log_to_default;

#ifdef Q_OS_ANDROID
void start_service()
{
    QJniObject ctx = QNativeInterface::QAndroidApplication::context();
    if (!ctx.isValid()) {
        qWarning() << "Could not get the app context";
        return;
    }

    const jclass clazz = QJniEnvironment().findClass("luke/flashbackprism/FlashbackPrismMonitor");
    QJniObject intent("android/content/Intent",
                      "(Landroid/content/Context;Ljava/lang/Class;)V",
                      ctx.object(),
                      clazz);
    if (!intent.isValid()) {
        qWarning() << "Failed to create intent";
        return;
    }

    intent.callObjectMethod("addFlags", "(I)Landroid/content/Intent;",
                            QJniObject::getStaticField<jint>("android/content/Intent", "FLAG_ACTIVITY_NEW_TASK"));
    ctx.callObjectMethod("startService",
                         "(Landroid/content/Intent;)Landroid/content/ComponentName;",
                         intent.object());
}
#endif

int main(int argc, char** argv)
{
    qInstallMessageHandler(lightlogger::log_handler);

#ifdef Q_OS_ANDROID
    if (argc > 1 && strcmp(argv[1], "-service") == 0) {
        QAndroidService app(argc, argv);
        app.setOrganizationName(QSL("Luca Carlon"));
        app.setOrganizationDomain(QSL("org.duckdns.bugfreeblog"));
        app.setApplicationName(QSL("FlashbackPrism"));
        app.setApplicationVersion(APP_VERSION);
        QObject::connect(&app, &QAndroidService::aboutToQuit, [] {
            qInfo() << "Service stopped";
        });

        auto photoMonitor = new FPPhotoMonitor(qApp);
        new FPNotificationProcessor(photoMonitor, qApp);

        photoMonitor->start();

        qInfo() << "Service is starting...";
        return app.exec();
    }
#endif

    QGuiApplication app(argc, argv);
    app.setOrganizationName(QSL("Luca Carlon"));
    app.setOrganizationDomain(QSL("org.duckdns.bugfreeblog"));
    app.setApplicationName(QSL("FlashbackPrism"));
    app.setApplicationVersion(APP_VERSION);

    qInfo() << "App version:" << app.applicationVersion();
    qInfo() << "App config:" << QSettings().fileName();

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    qmlRegisterType<FPLoginRequest>("FlashbackPrism", 1, 0, "FPLoginRequest");
    qmlRegisterType<FPPhotosRequest>("FlashbackPrism", 1, 0, "FPPhotosRequest");
    qmlRegisterType<FPPhotoMonitor>("FlashbackPrism", 1, 0, "FPPhotoMonitor");

    auto photoMonitor = new FPPhotoMonitor(qApp);
#ifndef Q_OS_ANDROID
    new FPNotificationProcessor(photoMonitor, qApp);
#endif
    auto photoProvider = new FPPhotoProvider;
    auto photoStore = new FPPhotoViewStore(qApp);
    QObject::connect(photoProvider, &FPPhotoProvider::imageDownloaded, photoStore, [photoStore] (const QImage& photo) {
        photoStore->set_lastPhoto(photo);
    });

    lqt::embed_font_awesome(engine.rootContext());
    engine.addImageProvider("photo", photoProvider);
    engine.rootContext()->setContextProperty("photoViewStore",
                                             photoStore);
    engine.rootContext()->setContextProperty("photoProvider",
                                             photoProvider);
    engine.rootContext()->setContextProperty("photoMonitor",
                                             photoMonitor);
    engine.rootContext()->setContextProperty("settingsNotifier",
                                             new FPPersistentSetup(qApp));
    engine.rootContext()->setContextProperty("qmlUtils",
                                             new FPQmlUtils(qApp));
    engine.rootContext()->setContextProperty("lqtQmlUtils",
                                             new lqt::QmlUtils(qApp));
    engine.loadFromModule("FlashbackPrism", "Main");

#ifdef Q_OS_ANDROID
    start_service();
#endif

    return app.exec();
}
