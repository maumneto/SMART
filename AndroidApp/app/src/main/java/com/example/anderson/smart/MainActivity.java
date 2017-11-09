package com.example.anderson.smart;

import android.content.Intent;
import android.graphics.Color;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.io.UnsupportedEncodingException;

import static java.lang.Thread.sleep;

public class MainActivity extends AppCompatActivity {

    String host = "192.168.0.69";
    String port = "1883";
    String TAG = "Mqtt";
    byte[] encodedPayload;
    MqttAndroidClient client;
    IMqttToken token;

    Button btReal;
    Button btHistorical;
    TextView tvStatus;
    Handler handler;
    Runnable r;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btReal = (Button) findViewById(R.id.btReal);
        btHistorical = (Button) findViewById(R.id.btHistorical);
        tvStatus = (TextView) findViewById(R.id.tvStatus);

        btReal.setEnabled(false);

        final String clientId = MqttClient.generateClientId();
        client = new MqttAndroidClient(this.getApplicationContext(), "tcp://"+host+":"+port, clientId);

        handler = new Handler();
        r = new Runnable() {
            public void run() {
                checkConnect();
                handler.postDelayed(this, 2000);
            }
        };
        handler.postDelayed(r, 1000);

        btReal.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startActivity(new Intent(MainActivity.this, RealActivity.class));
            }
        });

        btHistorical.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startActivity(new Intent(MainActivity.this, HistoricalActivity.class));
            }
        });
    }

    public void checkConnect() {
        try {
            token = client.connect();
            token.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    // We are connected
                    Log.d(TAG, "onSuccess");
                    btReal.setEnabled(true);
                    tvStatus.setTextColor(Color.BLUE);
                    tvStatus.setText("Online");
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    // Something went wrong e.g. connection timeout or firewall problems
                    Log.d(TAG, "onFailure");
                    btReal.setEnabled(false);
                    tvStatus.setTextColor(Color.RED);
                    tvStatus.setText("Offline");
                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        handler.removeCallbacks(r);
        try {
            client.disconnect();
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }
}
