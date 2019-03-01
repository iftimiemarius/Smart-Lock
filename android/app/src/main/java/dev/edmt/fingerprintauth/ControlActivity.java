package dev.edmt.fingerprintauth;

import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Switch;
import android.widget.TextView;

import java.io.File;
import java.io.IOException;

import static android.R.id.text1;
import static dev.edmt.fingerprintauth.HomeActivity.dOut;
import static dev.edmt.fingerprintauth.HomeActivity.reader;

public class ControlActivity extends AppCompatActivity {
    TextView textView;
    public static ImageView imageView2;
    public static ImageView imageView3;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_control);
        /*Button button;
        button = (Button) findViewById(R.id.button2);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    String abc="123456";
                    dOut.writeBytes(abc);
                    dOut.flush();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }*/
        final TextView switchStatus;
        Switch mySwitch;
        mySwitch = (Switch) findViewById(R.id.switch1);
        mySwitch.setChecked(false);
        final String path_on,path_off,templo,temphi,tempmed;
        path_on = "storage/emulated/0/on.png";
        path_off = "storage/emulated/0/off.png";
        templo = "storage/emulated/0/templo.png";
        tempmed = "storage/emulated/0/tempmed.png";
        temphi = "storage/emulated/0/temphi.png";
        imageView2 = (ImageView) findViewById(R.id.imageView2);
        imageView2.setImageDrawable(Drawable.createFromPath(path_off));
        imageView3 = (ImageView) findViewById(R.id.imageView3);
        final String on, off;
        on = "129876";
        off = "123987";
        //set the switch to ON
        //mySwitch.setChecked(true);
        //attach a listener to check for changes in state
        mySwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView,
                                         boolean isChecked) {

                if (isChecked) {
                    try {
                        dOut.writeBytes(on);
                        imageView2.setImageDrawable(Drawable.createFromPath(path_on));
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    try {
                        dOut.flush();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                } else {
                    try {
                        dOut.writeBytes(off);
                        imageView2.setImageDrawable(Drawable.createFromPath(path_off));
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    try {
                        dOut.flush();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }

            }
        });

        //check the current state before we display the screen
        /*if (mySwitch.isChecked()) {
            try {
                dOut.writeBytes(on);
            } catch (IOException e) {
                e.printStackTrace();
            }
            try {
                dOut.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
        } else {
            try {
                dOut.writeBytes(off);
            } catch (IOException e) {
                e.printStackTrace();
            }
            try {
                dOut.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }*/
        Button button;
        button = (Button) findViewById(R.id.button3);
        textView = (TextView)findViewById(R.id.textView);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    String abc="123456";
                    dOut.writeBytes(abc);
                    dOut.flush();
                    //
                    String linie = reader.readLine();
                    String temperatura = linie + "°C";
                    Log.d("aa ",linie);
                    textView.setText(temperatura);
                    int temperature=Integer.parseInt(linie);
                    if(temperature<10)
                        imageView3.setImageDrawable(Drawable.createFromPath(templo));
                    else if(temperature>=10&&temperature<25)
                        imageView3.setImageDrawable(Drawable.createFromPath(tempmed));
                    else
                        if(temperature>=25)
                            imageView3.setImageDrawable(Drawable.createFromPath(temphi));
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        Button button2;
        button2 = (Button) findViewById(R.id.button2);
        button2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    String abc="985123";
                    dOut.writeBytes(abc);
                    dOut.flush();
                    Intent homeIntent = new Intent(Intent.ACTION_MAIN);
                    homeIntent.addCategory( Intent.CATEGORY_HOME );
                    homeIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                    startActivity(homeIntent);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    protected void onStop()
    {
        super.onStop();
        try {
            dOut.writeBytes("128932");
        } catch (IOException e) {
            e.printStackTrace();
        }
        // insert here your instructions
    }
}

