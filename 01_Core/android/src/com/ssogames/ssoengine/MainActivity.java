package com.ssogames.ssoengine;

import android.app.NativeActivity;
import android.os.Bundle;
import android.widget.Toast;

public class MainActivity extends NativeActivity {
    
    static {
        System.loadLibrary("SSOEngine");
    }
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // Show a toast to confirm loading
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(MainActivity.this, "SSOEngine Loaded!", Toast.LENGTH_SHORT).show();
            }
        });
    }
}
