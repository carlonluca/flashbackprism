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
 * Date:    2023.01.02
 */

package luke.flashbackprism;

import android.util.Log;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ServiceInfo;
import android.os.Build;
import org.qtproject.qt.android.bindings.QtService;
import org.qtproject.qt.android.bindings.QtActivity;

public class FlashbackPrismMonitor extends QtService {
    private static final String CHANNEL_ID = "FlashbackPrismMonitor";

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        super.onStartCommand(intent, flags, startId);
        try {
            createNotificationChannel();
            final int SERVICE_ID = 1;
            final Intent openIntent = new Intent(this, QtActivity.class);
            openIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
            final PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, openIntent, PendingIntent.FLAG_IMMUTABLE);
            final Notification notification = new Notification.Builder(this, CHANNEL_ID)
                .setContentText("FlashbackPrism service is active")
                .setSmallIcon(R.drawable.icon_notification_24dp)
                .setContentIntent(pendingIntent)
                .build();
            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.TIRAMISU)
                startForeground(SERVICE_ID, notification);
            else
                startForeground(SERVICE_ID, notification, ServiceInfo.FOREGROUND_SERVICE_TYPE_DATA_SYNC);
        }
        catch (Exception e) {
            Log.w("FlashbackPrism", "Failed to start service", e);
        }
        return START_STICKY;
    }

    private void createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            final NotificationChannel serviceChannel = new NotificationChannel(
                    CHANNEL_ID,
                    "FlashbackPrism monitor",
                    NotificationManager.IMPORTANCE_DEFAULT
            );

            final NotificationManager manager = getSystemService(NotificationManager.class);
            if (manager != null)
                manager.createNotificationChannel(serviceChannel);
        }
    }
}
