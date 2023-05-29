package com.example.firebasenodemcuandroid;

import static android.content.ContentValues.TAG;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends Activity {
    DatabaseReference mydb, ledRef;
    TextView temp, hum;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        temp = (TextView)findViewById(R.id.suhu);
        hum = (TextView)findViewById(R.id.kelembapan);
        mydb = FirebaseDatabase.getInstance().getReference().child("sensor");
        ledRef = FirebaseDatabase.getInstance().getReference().child("led");

        try {
            mydb.addValueEventListener(new ValueEventListener() {
                @Override
                public void onDataChange(DataSnapshot dataSnapshot) {
                    String tempdata = "";
                    String humdata = "";
                    if (dataSnapshot.child("suhu").getValue() != null) {
                        tempdata = dataSnapshot.child("suhu").getValue().toString();
                    }
                    if (dataSnapshot.child("kelembapan").getValue() != null) {
                        humdata = dataSnapshot.child("kelembapan").getValue().toString();
                    }
                    temp.setText(tempdata);
                    hum.setText(humdata);
                }
                @Override
                public void onCancelled(DatabaseError error) {
                    // Failed to read value
                    Log.w(TAG, "Failed to read value.", error.toException());
                }
            });
        } catch(Exception e) {
            // Handle exceptions here
        }
        Button btnhidup = findViewById(R.id.btn_hidup);
        Button btnmati = findViewById(R.id.btn_mati);

        btnhidup.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //Mengirimkan status led "1" = Hidup
                ledRef.setValue(1); // modifikasi
            }
        });

        btnmati.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //Mengirimkan status led "0" = Mati
                ledRef.setValue(0); // modifikasi
            }
        });
    }

}
