
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
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity2 extends AppCompatActivity
{
    DatabaseReference mData;
    TextView Limit;
    TextView Ltop;
    TextView Lbottom;
    EditText Top;
    EditText Bottom;
    Button Setup;
    Button Default;
    Button back2;
    ConnectionDetector check; //Biến kiểm tra trạng thái wifi
    private Object MainActivity;
    private Object MainActivity3;
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main2);
        //Khai báo biến
        Top     = (EditText)  findViewById(R.id.gioihantren);
        Bottom  = (EditText) findViewById(R.id.gioihanduoi);
        Default = (Button) findViewById(R.id.macdinh);
        Setup   = (Button) findViewById(R.id.thietlap);
        back2   = (Button)    findViewById(R.id.back2);
        Limit   = (TextView) findViewById(R.id.tvGioihan);
        Ltop    = (TextView) findViewById(R.id.tvTop);
        Lbottom = (TextView) findViewById(R.id.tvBottom) ;
        mData   = FirebaseDatabase.getInstance().getReference();

        // Các hàm làm việc

       Setup.setOnClickListener(new View.OnClickListener()
       {
           @Override
           public void onClick(View v)
           {
               mData.child("Top").setValue(Top.getText().toString());
               mData.child("Bottom").setValue(Bottom.getText().toString());
           }
       });
       Default.setOnClickListener(new View.OnClickListener()
       {
           @Override
           public void onClick(View v)
           {
               mData.child("Top").setValue(9);
               mData.child("Bottom").setValue(7);
           }
       });
        back2.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
             Intent main = new Intent(MainActivity2.this, MainActivity3.class);
             startActivity(main);
            }
        });
        mData.child("Top").addValueEventListener(new ValueEventListener()
        {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot)
            {
                Ltop.setText(dataSnapshot.getValue().toString());
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError)
            {

            }
        });
        mData.child("Bottom").addValueEventListener(new ValueEventListener()
        {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot)
            {
                Lbottom.setText(dataSnapshot.getValue().toString());
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError)
            {

            }
        });
    }
}


