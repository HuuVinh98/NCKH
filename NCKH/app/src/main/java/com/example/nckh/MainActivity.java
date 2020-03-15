
package com.example.nckh;

import android.app.AlarmManager;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.media.MediaPlayer;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.view.Window;
import android.widget.Toast;
import com.example.nckh.R;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.concurrent.Delayed;
import java.util.logging.Handler;

public class MainActivity<p> extends AppCompatActivity
{
   public float limitTop;
   public float limitBottom;
   public float p;
    DatabaseReference mData;
    TextView PH;
    TextView Temp;
    TextView NH3;
    TextView Status;
    Button back;
    ConnectionDetector check; //Biến kiểm tra trạng thái wifi
    private Object MainActivity;
    private Object MainActivity3;


    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);
        //Khai báo biến
        Temp   = (TextView) findViewById(R.id.temp);
        PH     = (TextView) findViewById(R.id.ph);
        NH3    = (TextView) findViewById(R.id.nh3);
        Status = (TextView) findViewById(R.id.status);
        back   = (Button)   findViewById(R.id.back) ;
        check = new ConnectionDetector(this);
        mData = FirebaseDatabase.getInstance().getReference();

        // Các hàm làm việc
        checkWifi();
        getData();
        back.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                Intent main = new Intent(MainActivity.this,MainActivity3.class );
                startActivity(main);

            }
        });
    }

    // Hàm Nhận dữ liệu
    public void getData()
    {
        // Nhận giới hạn trên
        mData.child("Top").addValueEventListener(new ValueEventListener()
        {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot)
            {
                limitTop = Float.parseFloat(dataSnapshot.getValue().toString());
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError)
            {

            }
        });
        // Nhận giới hạn dưới

        mData.child("Bottom").addValueEventListener(new ValueEventListener()
        {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot)
            {
                 limitBottom = Float.parseFloat(dataSnapshot.getValue().toString());
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError)
            {

            }
        });

        //Nhận giá trị nhiệt độ
        mData.child("Temperature").addValueEventListener(new ValueEventListener()
        {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot)
            {

                Temp.setText(dataSnapshot.getValue().toString() +" ℃");
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError)
            {

            }
        });

        //Nhận giá trị pH
        mData.child("pH").addValueEventListener(new ValueEventListener()
        {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot)
            {
                PH.setText(dataSnapshot.getValue().toString());
                p = Float.parseFloat(dataSnapshot.getValue().toString());

                //pH cao
                if (p > limitTop)
                {
                    Status.setText("pH CAO");
                    _startService();
                    AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
                    builder.setTitle(" NGUY HIỂM pH CAO ");
                    builder.setMessage("Sử dụng mật đường 3kg/1000 m3 kết hợp sử dụng vi sinh hoặc dùng Acid acetic 3lít/1000 m3");
                    builder.setIcon(android.R.drawable.ic_delete);
                    builder.setPositiveButton("Đã Hiểu!", new DialogInterface.OnClickListener()
                    {
                        @Override
                        public void onClick(DialogInterface dialog, int which)
                        {
                            dialog.cancel();
                            _stopService();

                        }
                    });
                    builder.create().show();
                }

                //pH thấp
                else if ((p <limitBottom)&&(p>=1))
                {
                    Status.setText("pH THẤP");
                    _startService();
                    AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
                    builder.setTitle("NGUY HIỂM: pH THẤP");
                    builder.setMessage("Cần bón vôi (CaCO3, Dolomite) với liều 10 – 20 kg/1000 m3 nước.");
                    builder.setIcon(android.R.drawable.ic_delete);
                    builder.setPositiveButton("Đã Hiểu!", new DialogInterface.OnClickListener()
                    {
                        @Override
                        public void onClick(DialogInterface dialog, int which)
                        {
                            dialog.cancel();
                            _stopService();
                        }
                    });
                    builder.create().show();
                }

                // Lỗi cảm biến
                else if (p < 1)
                {
                    _startService();
                    AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
                    builder.setTitle("Cảnh Báo:LỖI THIẾT BỊ");
                    builder.setMessage("Dữ liệu có sự sai xót,kiểm tra lại cảm biến hoặc kết nối");
                    builder.setIcon(android.R.drawable.ic_delete);
                    builder.setPositiveButton("Đã Hiểu!", new DialogInterface.OnClickListener()
                    {
                        @Override
                        public void onClick(DialogInterface dialog, int which)
                        {
                            dialog.cancel();
                            _stopService();
                        }
                    });
                    builder.create().show();
                }

                // an toàn
                else if ((p>= limitBottom) && (p <= limitTop))
                {
                    Status.setText("AN TOÀN");
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError)
            {

            }


        });


        //Nhận giá trị NH3
        mData.child("NH3").addValueEventListener(new ValueEventListener()
        {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot)
            {
                NH3.setText(dataSnapshot.getValue().toString());
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError)
            {

            }
        });
    }


    //Hàm kiểm tra kết nối wifi
    public void checkWifi()
    {
        //kiểm tra wifi
        if (!
                check.Connected())
        {


            Toast.makeText(MainActivity.this, "Chưa Kết Nối Wifi", Toast.LENGTH_SHORT).show();
            AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
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
    public boolean isConnected(Context context)
    {
        ConnectivityManager cm = (ConnectivityManager) context.getSystemService(Service.CONNECTIVITY_SERVICE);
        NetworkInfo info = cm.getActiveNetworkInfo();
        if (cm !=null && info.isConnectedOrConnecting())
            {
                android.net.NetworkInfo wifi = cm.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
                android.net.NetworkInfo mobile = cm.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);

                if ((mobile !=null && mobile.isConnectedOrConnecting()) || (wifi !=null && wifi.isConnectedOrConnecting()))
                    return true;
                else return false;
            }
            else
                return false;

    }
    public AlertDialog.Builder buildDialog(Context c)
    {
        AlertDialog.Builder builder = new AlertDialog.Builder(c);
        builder.setTitle("MẤT KẾT NỐI WIFI");
        builder.setMessage("Vui lòng bật kết nối Wifi hoặc kiểm tra lại đường truyền ");
        builder.setIcon(android.R.drawable.ic_delete);
        builder.setPositiveButton("OK!", new DialogInterface.OnClickListener()
        {
            @Override
            public void onClick(DialogInterface dialog, int which)
            {
                finish();
            }
        });
      return builder;
    }
    // Hàm chạy Service
    public void _startService()
    {
        Intent intent = new Intent(this,Music.class);
        startService(intent);
    }
    // Hàm tắt Service
    public void _stopService()
    {
        Intent intent = new Intent(this,Music.class);
        stopService(intent);
    }


}


