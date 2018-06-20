package com.example.nguyenduytam.i_global;

import android.content.Intent;
import android.location.Address;
import android.location.Geocoder;
import android.media.Image;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.maps.CameraUpdate;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.MapFragment;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;

import java.io.IOError;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity implements OnMapReadyCallback {
    private EditText mSearchText;
    GoogleMap map;
    double lat = 10.762636;
    double lng = 106.681219;
    LatLng myLocal = new LatLng(lat, lng);
    ImageView icView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mSearchText = (EditText) findViewById(R.id.input_search);
        MapFragment mapFragment = (MapFragment) getFragmentManager().findFragmentById(R.id.idMap);
        mapFragment.getMapAsync(this);
        icView = (ImageView)findViewById(R.id.ic_view);
        icView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent screen_info = new Intent(MainActivity.this, man_hinh_thong_tin.class);
                startActivity(screen_info);
            }
        });
    }
    private static final String TAG = "MainActivity";
    private void init(){
        Log.d(TAG, "init: initializing");
        mSearchText.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int actionId, KeyEvent keyEvent) {
                if (actionId == EditorInfo.IME_ACTION_SEARCH
                        || actionId == EditorInfo.IME_ACTION_DONE
                        || keyEvent.getAction() == keyEvent.KEYCODE_ENTER
                        || keyEvent.getAction() == keyEvent.ACTION_DOWN){
                geoLocate();

            }
                return false;
            }


        });
    }

    private void geoLocate() {
            Log.d(TAG, "geoLocate: geoLocating");
            String searchString = mSearchText.getText().toString();
        Geocoder geocoder = new Geocoder(this);
        List<Address> list = new ArrayList<>();
        try {
            list = geocoder.getFromLocationName(searchString, 1);
        }catch (IOException e){
            Log.e(TAG, "geoLocate: IOException: "+ e.getMessage());
        }
        if (list.size() > 0){
            Address address = list.get(0);
           // Log.d(TAG, "geoLocate: found a location: " + address.getLongitude() + "  " + address.getLatitude());
             Log.d(TAG, "geoLocate: found a location: " + address.getAddressLine(0).toString());
            //Toast.makeText(this, address.toString(), Toast.LENGTH_SHORT).show();
            lng = address.getLongitude();
            lat = address.getLatitude();
            Log.d(TAG, "geoLocate: found a location: " + lng + "  " + lat);

            map.addMarker(new MarkerOptions().position(new LatLng(lat,lng))
                    .title(address.getAddressLine(0).toString()));
            map.moveCamera(CameraUpdateFactory.newLatLng(new LatLng(lat,lng)));
        }

    }

    @Override
    public void onMapReady(GoogleMap googleMap) {
        map = googleMap;
        map.moveCamera(CameraUpdateFactory.newLatLngZoom(myLocal,13));
        map.addMarker(new MarkerOptions().position(myLocal)
                .title("University of Science"));
        map.moveCamera(CameraUpdateFactory.newLatLng(myLocal));
        init();


    }
}
