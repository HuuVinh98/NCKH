
package com.example.nckh;

        import android.app.AlertDialog;
        import android.app.Service;
        import android.content.DialogInterface;
        import android.content.Intent;
        import android.media.MediaPlayer;
        import android.os.IBinder;
        import android.support.annotation.Nullable;

public class Connect extends Service
{
    private Object MainActivity;

    @Nullable
    @Override
    public IBinder onBind(Intent intent)
    {
        return null;
    }

    @Override
   /* public void onCreate()
    {
        super.onCreate();
        AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity);
        builder.setTitle("MẤT KẾT NỐI WIFI");
        builder.setMessage("Vui lòng bật kết nối Wifi hoặc kiểm tra lại đường truyền ");
        builder.setIcon(android.R.drawable.ic_delete);
        builder.setPositiveButton("OK!", new DialogInterface.OnClickListener()
        {
            @Override
            public void onClick(DialogInterface dialog, int which)
            {
                dialog.cancel();
            }
        });
        builder.show();

    }*/


    public int onStartCommand(Intent intent, int flags, int startId)
    {
        return super.onStartCommand(intent,flags,startId);
    }


    @Override
    public void onDestroy()
    {
        super.onDestroy();
    }
}
