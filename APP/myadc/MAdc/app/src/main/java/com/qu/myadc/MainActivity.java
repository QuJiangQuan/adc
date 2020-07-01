package com.qu.myadc;

import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {


    TextView text = null;

    Handler handler=new Handler();

    Runnable runnable=new Runnable() {
        String val = null;
        @Override
        public void run() {
            // TODO Auto-generated method stub
            //读取adc值，显示

            val = HardControl.adcRead();
            Log.d("LOG",val);
            text.setText(val);
            handler.postDelayed(this, 2000);
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        text = (TextView)findViewById(R.id.edit);
        HardControl.adcOpen();
        handler.postDelayed(runnable, 2000);//每两秒执行一次runnable.
        Log.d("LOG","start");
    }
}


