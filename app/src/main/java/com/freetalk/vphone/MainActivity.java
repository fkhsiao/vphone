package com.freetalk.vphone;

import android.app.ActionBar;
import android.support.v4.text.util.LinkifyCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.util.Linkify;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.freetalk.pjinterface.PJ;

import java.util.concurrent.TimeUnit;

import static com.freetalk.pjinterface.PJ.createSipEngine;

public class MainActivity extends AppCompatActivity {
    // Class references


    // Constants
    public final static String TAG = "VP_MainActivity.java";
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

        hideStatusBar();
        showAppIconInActionBar();

        show();

    }

    private void hideStatusBar() {
        View decorView = getWindow().getDecorView();
        // Hide the status bar.
        int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN;
        decorView.setSystemUiVisibility(uiOptions);
        // Remember that you should never show the action bar if the
        // status bar is hidden, so hide that too if necessary.
        ActionBar actionBar = getActionBar();
        if (actionBar != null) {
            actionBar.hide();
        }
    }

    private void showAppIconInActionBar() {
        getSupportActionBar().setDisplayShowHomeEnabled(true);
        getSupportActionBar().setLogo(R.mipmap.ic_launcher);
        getSupportActionBar().setDisplayUseLogoEnabled(true);
    }

    private void show() {

        TextView tv = (TextView) findViewById(R.id.hello_text);
        tv.setText(PJ.helloJNI());

    }
}
