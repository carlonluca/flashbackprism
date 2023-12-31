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

#include <lqtutils/lqtutils_qsl.h>

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
#include <lightlogger/lc_logging.h>
lightlogger::custom_log_func lightlogger::global_log_func = log_to_stdout;

int main(int argc, char** argv)
{
    qInstallMessageHandler(lightlogger::log_handler);

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
    auto notProc = new FPNotificationProcessor(photoMonitor, qApp);
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
    engine.loadFromModule("FlashbackPrism", "Main");

    return app.exec();
}
