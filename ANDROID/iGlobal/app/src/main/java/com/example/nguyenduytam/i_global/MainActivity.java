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
<<<<<<< HEAD
    public static String nation = "";
    public static String flag = "";
    public static String capital = "";
    public static String area = "";
    public static String population = "";
    public static String subregion = "";
    public static String data = "";
    public static String code = "";
    LatLng myLocal = new LatLng(lat, lng);
    ImageView icView;
    private static final String TAG = "MainActivity";
    private Address address;
    public  static String urlCountry = "https://restcountries.eu/rest/v2/name/";
    public  static String urlFlag = "http://www.countryflags.io/";
    public  static String partUrlFlag = "/flat/64.png";
=======
    LatLng myLocal = new LatLng(lat, lng);
    ImageView icView;
>>>>>>> adc6d2d864a712b5d1cd1dc43346af6fbc63e25c
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mSearchText = (EditText) findViewById(R.id.input_search);
        MapFragment mapFragment = (MapFragment) getFragmentManager().findFragmentById(R.id.idMap);
        mapFragment.getMapAsync(this);
        icView = (ImageView)findViewById(R.id.ic_view);
<<<<<<< HEAD

        icView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {


                Intent screen_info = new Intent(MainActivity.this, man_hinh_thong_tin.class);
                if (address != null){

                    screen_info.putExtra("quocgia", nation);
                    screen_info.putExtra("vitri", address.getFeatureName().toString());
                    screen_info.putExtra("thudo", capital);
                    screen_info.putExtra("dientich", area +" km²");
                    screen_info.putExtra("khihau", subregion);
                    screen_info.putExtra("danso", population +" người");
                    screen_info.putExtra("gioithieu","Bài viết");
                }else{
                    screen_info.putExtra("quocgia", "Việt Nam");
                    screen_info.putExtra("vitri", "University of Science");
                    screen_info.putExtra("thudo", "Hà Nội");
                    screen_info.putExtra("dientich", "331,210 km²");
                    screen_info.putExtra("khihau", "Nhiệt đới gió mùa");
                    screen_info.putExtra("danso", "92.7 triệu người");
                    screen_info.putExtra("gioithieu","Bài viết");
                }
=======
        icView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent screen_info = new Intent(MainActivity.this, man_hinh_thong_tin.class);
>>>>>>> adc6d2d864a712b5d1cd1dc43346af6fbc63e25c
                startActivity(screen_info);
            }
        });
    }
<<<<<<< HEAD

=======
    private static final String TAG = "MainActivity";
>>>>>>> adc6d2d864a712b5d1cd1dc43346af6fbc63e25c
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
<<<<<<< HEAD
        urlCountry = "https://restcountries.eu/rest/v2/name/";
        urlFlag = "http://www.countryflags.io/";
        nation = "";
        flag = "";
        capital = "";
        area = "";
        population = "";
        subregion = "";
        data = "";
        code = "";
=======
>>>>>>> adc6d2d864a712b5d1cd1dc43346af6fbc63e25c
            String searchString = mSearchText.getText().toString();
        Geocoder geocoder = new Geocoder(this);
        List<Address> list = new ArrayList<>();
        try {
            list = geocoder.getFromLocationName(searchString, 1);
        }catch (IOException e){
            Log.e(TAG, "geoLocate: IOException: "+ e.getMessage());
        }
        if (list.size() > 0){
<<<<<<< HEAD
            address = list.get(0);
           // Log.d(TAG, "geoLocate: found a location: " + address.getLongitude() + "  " + address.getLatitude());
             Log.d(TAG, "geoLocate: found a location: " + address.toString());

            //Toast.makeText(this, address.toString(), Toast.LENGTH_SHORT).show();
            lng = address.getLongitude();
            lat = address.getLatitude();
            urlCountry += address.getCountryName();
            Log.d(TAG, "geoLocate: found a location: " + lng + "  " + lat);
            fetchData process = new fetchData();
            process.execute();
            map.moveCamera(CameraUpdateFactory.newLatLngZoom(myLocal,13));
=======
            Address address = list.get(0);
           // Log.d(TAG, "geoLocate: found a location: " + address.getLongitude() + "  " + address.getLatitude());
             Log.d(TAG, "geoLocate: found a location: " + address.getAddressLine(0).toString());
            //Toast.makeText(this, address.toString(), Toast.LENGTH_SHORT).show();
            lng = address.getLongitude();
            lat = address.getLatitude();
            Log.d(TAG, "geoLocate: found a location: " + lng + "  " + lat);

>>>>>>> adc6d2d864a712b5d1cd1dc43346af6fbc63e25c
            map.addMarker(new MarkerOptions().position(new LatLng(lat,lng))
                    .title(address.getAddressLine(0).toString()));
            map.moveCamera(CameraUpdateFactory.newLatLng(new LatLng(lat,lng)));
        }
<<<<<<< HEAD
=======

>>>>>>> adc6d2d864a712b5d1cd1dc43346af6fbc63e25c
    }

    @Override
    public void onMapReady(GoogleMap googleMap) {
        map = googleMap;
<<<<<<< HEAD

        map.moveCamera(CameraUpdateFactory.newLatLngZoom(myLocal,16));
=======
        map.moveCamera(CameraUpdateFactory.newLatLngZoom(myLocal,13));
>>>>>>> adc6d2d864a712b5d1cd1dc43346af6fbc63e25c
        map.addMarker(new MarkerOptions().position(myLocal)
                .title("University of Science"));
        map.moveCamera(CameraUpdateFactory.newLatLng(myLocal));
        init();


    }
}
