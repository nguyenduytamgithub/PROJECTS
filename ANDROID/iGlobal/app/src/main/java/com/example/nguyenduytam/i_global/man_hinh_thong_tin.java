package com.example.nguyenduytam.i_global;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
<<<<<<< HEAD
import android.text.style.LeadingMarginSpan;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.caverock.androidsvg.SVGImageView;
import com.squareup.picasso.Picasso;

public class man_hinh_thong_tin extends AppCompatActivity {
    ImageView icBackView;
    TextView textQgInput;
    TextView textVtInput;
    TextView textTdInput;
    TextView textDsInput;
    TextView textKhInput;
    TextView textGtInput;
    TextView textDtInput;
    ImageView imageFlag;
    LinearLayout image_from_url;

    private static final String TAG = "man_hinh_2";
    @Override
    protected void onCreate(Bundle savedInstanceState) {


        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_man_hinh_thong_tin);
        icBackView = (ImageView) findViewById(R.id.ic_back_view);
        textQgInput = (TextView)findViewById(R.id.text_qg_input);
        textVtInput = (TextView)findViewById(R.id.text_vt_input);
        textDsInput = (TextView)findViewById(R.id.text_ds_input);
        textTdInput = (TextView)findViewById(R.id.text_td_input);
        textKhInput = (TextView)findViewById(R.id.text_kh_input);
        textGtInput = (TextView)findViewById(R.id.text_gt_input);
        textDtInput = (TextView)findViewById(R.id.text_dt_input);
        imageFlag = (ImageView)findViewById(R.id.image_flag);

=======
import android.view.View;
import android.widget.ImageView;

public class man_hinh_thong_tin extends AppCompatActivity {
    ImageView icBackView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_man_hinh_thong_tin);
        icBackView = (ImageView) findViewById(R.id.ic_back_view);
>>>>>>> adc6d2d864a712b5d1cd1dc43346af6fbc63e25c
        icBackView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent screen_main = new Intent(man_hinh_thong_tin.this, MainActivity.class);
                startActivity(screen_main);
             }
        });
<<<<<<< HEAD
        Bundle bd = getIntent().getExtras();
        if (bd != null){

            String text_qg_input = bd.getString("quocgia");
            String text_ds_input = bd.getString("danso");
            String text_kh_input = bd.getString("khihau");
            String text_dt_input = bd.getString("dientich");
            String text_td_input = bd.getString("thudo");
            String text_gt_input = bd.getString("gioithieu");
            String text_vt_input = "Hình ảnh ";
                    text_vt_input = text_vt_input + bd.getString("vitri");
            Log.d(TAG, MainActivity.flag);

            Picasso.with(this).load(MainActivity.urlFlag).into(imageFlag);
            textQgInput.setText(text_qg_input);
            textVtInput.setText(text_vt_input);
            textDsInput.setText(text_ds_input);
            textDtInput.setText(text_dt_input);
            textKhInput.setText(text_kh_input);
            textTdInput.setText(text_td_input);
            textGtInput.setText(text_gt_input);

        }else {
            String text_qg_input = "Khong co du lieu";
            textQgInput.setText(text_qg_input);
            String text_vt_input = "Khong co du lieu";
            textQgInput.setText(text_vt_input);
        }

    }



=======
    }
>>>>>>> adc6d2d864a712b5d1cd1dc43346af6fbc63e25c
}
