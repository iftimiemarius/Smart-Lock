package dev.edmt.fingerprintauth;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.StrictMode;
import android.provider.MediaStore;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.lang.Object;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.concurrent.TimeUnit;

import org.apache.commons.io.IOUtils;

import static android.provider.MediaStore.EXTRA_OUTPUT;

public class HomeActivity extends Activity {
    Button button;
    ImageView imageView;
    static final int CAM_REQUEST = 1;
    public static DataOutputStream dOut;
    public static Socket socket;
    public static BufferedReader reader;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_home);
        Button btn = (Button) findViewById(R.id.buttonxd);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int SDK_INT = android.os.Build.VERSION.SDK_INT;
                if (SDK_INT > 8) {
                    StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder()
                            .permitAll().build();
                    StrictMode.setThreadPolicy(policy);
                    String[] perms = {"android.permission.READ_INTERNAL_STORAGE", "android.permission.READ_EXTERNAL_STORAGE"};

                    int permsRequestCode = 200;

                    requestPermissions(perms, permsRequestCode);
                    try {
                        socket = new Socket("192.168.2.122", 1234);
                        File file = new File("/storage/emulated/0/poza/cam_image.jpg");
                        FileInputStream fis = new FileInputStream(file);
                        InputStream is = fis;
                        byte[] message = IOUtils.toByteArray(is);
                        dOut = new DataOutputStream(socket.getOutputStream());
                        InputStreamReader isr = new InputStreamReader(socket.getInputStream());
                        reader = new BufferedReader(isr);
                        int leng = message.length;
                        byte[] length = ByteBuffer.allocate(4).putInt(message.length).array(); //int 2 bytes
                        System.out.println(message.length);
                        byte[] newLen = new byte[4]; //little endian & big endian
                        for (int i = 0; i < 4; i++) {
                            System.out.println(length[i]); //original bytes
                            newLen[3 - i] = length[i]; //reversed orders
                        }
                        dOut.write(newLen); //send length
                        dOut.flush();
                        dOut.write(message);//send the image
                        String linie = reader.readLine();
                        Log.d("ia vezi boss ", linie);
                        int linieint = Integer.parseInt(linie);
                        if (linieint == 1) {
                            control();
                        }
                    } catch (IOException e) {
                        //Log.d("BHABLHABLHABLHA","DAFSDFASDFASDF");

                        e.printStackTrace();
                    }
                }
            }
        });
        button = (Button) findViewById(R.id.button);
        imageView = (ImageView) findViewById(R.id.imageofu);
        imageView.setRotation(270);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent camera_intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
                File file = getFile();
                camera_intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(file));
                startActivityForResult(camera_intent, CAM_REQUEST);


            }
        });
    }
    private void control() {
        Intent intent = new Intent(this, ControlActivity.class);
        startActivity(intent);
    }

    private File getFile() {
        File folder = new File("storage/emulated/0/poza");
        if (!folder.exists()) {
            folder.mkdir();
        }
        File image_file = new File(folder, "cam_image.jpg");
        return image_file;

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        String path = "storage/emulated/0/poza/cam_image.jpg";
        imageView.setImageDrawable(Drawable.createFromPath(path));
    }
}