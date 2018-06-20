package com.example.nguyenduytam.i_global;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

public class man_hinh_thong_tin extends AppCompatActivity {
    ImageView icBackView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_man_hinh_thong_tin);
        icBackView = (ImageView) findViewById(R.id.ic_back_view);
        icBackView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent screen_main = new Intent(man_hinh_thong_tin.this, MainActivity.class);
                startActivity(screen_main);
             }
        });
    }
}
