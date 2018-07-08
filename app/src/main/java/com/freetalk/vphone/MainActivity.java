package com.freetalk.vphone;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.freetalk.pjinterface.PJ;

public class MainActivity extends AppCompatActivity {
    // Class references


    // Constants
    public final static String TAG = "vphone";
    public static int logLevel = 4;   // Default value overwritten at runtime.

    // load JNI library
    static {
        System.loadLibrary("pjapi");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Log.i(TAG, "onCreate");

        TextView tv = (TextView) findViewById(R.id.hello_text);
        tv.setText(PJ.helloJNI());
    }
}
