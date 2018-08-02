package com.example.nguyenduytam.i_global;


import android.os.AsyncTask;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;


public class fetchData extends AsyncTask<Void,Void,Void> {
    String data ="";
    String dataParsed = "";
    String singleParsed ="";
    private static final String TAG = "fetchData";
    @Override
    protected Void doInBackground(Void... voids) {
        try {
            URL url = new URL(MainActivity.urlCountry);
            HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();
            InputStream inputStream = httpURLConnection.getInputStream();
            BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
            String line = "";
            while(line != null){
                line = bufferedReader.readLine();
                data = data + line;
            }

            JSONArray JA = new JSONArray(data);
            for(int i =0 ;i <JA.length(); i++){
                JSONObject JO = (JSONObject) JA.get(i);
                MainActivity.code += JO.get("alpha2Code");
                MainActivity.capital += JO.get("capital");
                MainActivity.subregion += JO.get("subregion");
                MainActivity.population += JO.get("population");
                MainActivity.area += JO.get("area");
                MainActivity.nation += JO.get("nativeName");
//                MainActivity.flag += JO.get("flag");
               //
               // singleParsed += JO.get("nativeName");


                dataParsed = dataParsed + singleParsed +"\n" ;


            }

        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (JSONException e) {
            e.printStackTrace();
        }

        return null;
    }

    @Override
    protected void onPostExecute(Void aVoid) {
        super.onPostExecute(aVoid);
        MainActivity.urlFlag += MainActivity.code;
        MainActivity.urlFlag += MainActivity.partUrlFlag;
        //   MainActivity.nation = this.dataParsed;
        Log.d(TAG, MainActivity.code);

    }
}