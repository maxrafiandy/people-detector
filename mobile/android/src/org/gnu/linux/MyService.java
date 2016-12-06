/* * *
 * This code is based on Android Developer example.
 * For more details, see https://developer.android.com/guide/components/services.html
 * * */

package org.gnu.linux;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.os.Process;
import android.util.Log;
import android.widget.Toast;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;

public class MyService extends Service {
    private Looper mServiceLooper;
    private ServiceHandler mServiceHandler;
    private MyService m_service = this;

    // Handler that receives messages from the thread
    private final class ServiceHandler extends Handler {
        public ServiceHandler(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(Message msg) {

            /* While service active ignore InterruptedException and IOException */
            for(;;) {
                // get data from server every 1 second
                try {
                    Thread.sleep(1000);

                    byte[] data = new byte[20];
                    Socket client = new Socket("192.168.100.76", 3212);
                    InputStream inFromServer = client.getInputStream();
                    DataInputStream in = new DataInputStream(inFromServer);
                    int report = in.read(data);
                    String _msg = new String(data);
                    Log.i("MyService.class", _msg);
                    if (_msg.startsWith("DETECTED"))
                    {
                        Uri notif_sound = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
                        Intent intent = new Intent(m_service, MyService.class);
                        // use System.currentTimeMillis() to have a unique ID for the pending intent
                        PendingIntent pIntent = PendingIntent.getActivity(m_service, (int) System.currentTimeMillis(), intent, 0);

                        // build notification
                        // the addAction re-use the same intent to keep the example short
                        Notification n  = new Notification.Builder(m_service)
                                .setContentTitle("SmartHome's Warning!")
                                .setContentText("Someone has been detected.")
                                .setSmallIcon(R.drawable.warning)
                                .setContentIntent(pIntent)
                                .setAutoCancel(true)
                                .setSound(notif_sound)
                                .build();

                        NotificationManager notificationManager =
                                (NotificationManager) getSystemService(NOTIFICATION_SERVICE);

                        notificationManager.notify(0, n);
                    }

                } catch (InterruptedException e) {
                    Log.i("MyService.class:", "InterruptedException");
                    Thread.currentThread().interrupt();
                } catch (IOException e) {
                    Log.i("MyService.class", "IOException");
                    Thread.currentThread().interrupt();
                } catch (Exception e) {
                    Log.i("MyService.class", "Exception");
                    Thread.currentThread().interrupt();
                    break;
                }
            }
            // Stop the service using the startId, so that we don't stop
            // the service in the middle of handling another job
            stopSelf(msg.arg1);
        }
    }

    @Override
    public void onCreate() {
        // Start up the thread running the service.  Note that we create a
        // separate thread because the service normally runs in the process's
        // main thread, which we don't want to block.  We also make it
        // background priority so CPU-intensive work will not disrupt our UI.
        HandlerThread thread = new HandlerThread("ServiceStartArguments",
                Process.THREAD_PRIORITY_BACKGROUND);
        thread.start();

        // Get the HandlerThread's Looper and use it for our Handler
        mServiceLooper = thread.getLooper();
        mServiceHandler = new ServiceHandler(mServiceLooper);
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Toast.makeText(this, "Monitor service started!", Toast.LENGTH_SHORT).show();

        // For each start request, send a message to start a job and deliver the
        // start ID so we know which request we're stopping when we finish the job
        Message msg = mServiceHandler.obtainMessage();
        msg.arg1 = startId;
        mServiceHandler.sendMessage(msg);

        // If we get killed, after returning from here, restart
        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        // We don't provide binding, so return null
        return null;
    }

    @Override
    public void onDestroy() {
        Toast.makeText(this, "Service done", Toast.LENGTH_SHORT).show();
    }
}
