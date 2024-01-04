import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import luke.flashbackprism.FlashbackPrismMonitor;

public class FlashbackBootReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        final Intent serviceIntent = new Intent(context, FlashbackPrismMonitor.class);
        serviceIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startService(serviceIntent);
    }
}
