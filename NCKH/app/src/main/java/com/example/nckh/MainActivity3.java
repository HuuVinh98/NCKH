
package com.example.nckh;

import android.app.AlertDialog;
import android.app.Service;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity3 extends AppCompatActivity
{
    TextView menu;
    Button wifi;
    Button tom;
    Button thietlap;
    Button thoat;
    ConnectionDetector check; //Biến kiểm tra trạng thái wifi
    private Object MainActivity;
    private Object MainActivity2;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main3);
        //Khai báo biến
        menu = (TextView) findViewById(R.id.menu3);
        tom  = (Button)   findViewById(R.id.button1);
        thietlap   = (Button)   findViewById(R.id.button2);
        wifi = (Button) findViewById(R.id.wifi);
        thoat = (Button)  findViewById(R.id.button3);
        check = new ConnectionDetector(this);
        checkWifi();
        tom.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                Intent tom = new Intent(MainActivity3.this,MainActivity.class);
                startActivity(tom);
            }
        });
        thietlap.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                Intent ca = new Intent(MainActivity3.this, MainActivity2.class);
                startActivity(ca);
            }
        });
        thoat.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                finishAffinity();
            }
        });
        wifi.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                Intent launchSmartConfig = getPackageManager().getLaunchIntentForPackage("com.cmmakerclub.iot.esptouch");
                startActivity(launchSmartConfig);
            }
        });

    }

    public void checkWifi()
    {
        //kiểm tra wifi
        if (check.Connected())
        {
            Toast.makeText(MainActivity3.this, "Đã Kết Nối Wifi", Toast.LENGTH_SHORT).show();

        }
        else
        {

            Toast.makeText(MainActivity3.this, "Chưa Kết Nối Wifi", Toast.LENGTH_SHORT).show();
            AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity3.this);
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
        }
    }
}


