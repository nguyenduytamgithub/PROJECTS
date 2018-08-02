import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;

import com.google.android.gms.maps.OnMapReadyCallback;

public class mergeUrl extends AppCompatActivity  {
    String url_country = "https://restcountries.eu/rest/v2/";
    protected void merger_country (String country){
        url_country += country;
    }
}
