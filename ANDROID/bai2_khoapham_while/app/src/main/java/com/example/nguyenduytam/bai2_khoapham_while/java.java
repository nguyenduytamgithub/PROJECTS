package com.example.nguyenduytam.bai2_khoapham_while;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

public class java extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_java);
        for (int i = 0; i < 1000; i++){
           // Log.d("NDT", "" + i);
            System.out.println(i);
        }
    }
}
