package com.example.nckh;

import android.app.Service;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.IBinder;
import android.support.annotation.Nullable;

public class Music extends Service
{
    @Nullable
    @Override
    public IBinder onBind(Intent intent)
    {
        return null;
    }

     public MediaPlayer song;
     @Override
    public void onCreate()
    {
        super.onCreate();
        song = MediaPlayer.create(this,R.raw.canhbao);
        song.start();
    }


    public int onStartCommand(Intent intent, int flags, int startId)
    {
        return super.onStartCommand(intent,flags,startId);
    }


    @Override
    public void onDestroy()
    {
        super.onDestroy();
        song.stop();
    }
}
