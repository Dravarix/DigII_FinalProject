package edu.rowan.users.bluetooth_motordriver;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

/**
 * Main activity of Bluetooth_MotorController.
 *
 * @author James Ritchie III
 */
public class MainActivity extends Activity {
    public static final String DEVICE_NAME = "RN42-4373";
    public static final String STANDARD_SERIAL_PORT_SERVICE_ID = "00001101-0000-1000-8000-00805F9B34FB";
    public static final int REQUEST_ENABLE_BT = 1; // Arbitrary integer higher than zero.
    boolean connected = false;
    TextView statusText, speedText;
    SeekBar speedController;
    BluetoothAdapter mBluetoothAdapter;
    BluetoothSocket mmSocket;
    BluetoothDevice mmDevice;
    OutputStream mmOutputStream;
    InputStream mmInputStream;
    Thread workerThread;
    byte[] readBuffer;
    int readBufferPosition;
    volatile boolean stopWorker;

    /**
     * Initializes the buttons, text, and slidebar on the app, getting the info from the XML file.
     *
     * @param savedInstanceState savedInstanceState
     */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final Button connectBtn = (Button)findViewById(R.id.connectBtn);
        final Button brakeBtn = (Button)findViewById(R.id.brakeBtn);
        final Button leftBtn = (Button)findViewById(R.id.leftBtn);
        final Button straightBtn = (Button)findViewById(R.id.straightBtn);
        final Button rightBtn = (Button)findViewById(R.id.rightBtn);
        statusText = (TextView)findViewById(R.id.statusText);
        speedText = (TextView)findViewById(R.id.speedText);
        speedController = (SeekBar) findViewById(R.id.speedController);

        //Sets up connect button for connecting to/disconnecting from bluetooth module.
        connectBtn.setOnClickListener(new View.OnClickListener(){
            public void onClick(View v){
                //If not connected, connect to bluetooth and change button text to show DISCONNECT
                if(!connected) {
                    findBT();
                    if(connected) {
                        connectBtn.setText("DISCONNECT");
                    }
                }
                //If connected, then close connection and set button text to CONNECT
                else{
                    closeBT();
                    if(!connected) {
                        connectBtn.setText("CONNECT");
                    }
                }
            }
        });

        //Sets up brake button. This button moves slidebar to the middle.
        brakeBtn.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                //Set slidebar to middle position
                speedController.setProgress(50);
                //Send brake command over bluetooth connection
                byte speed = (byte) 50;
                byte[] bytesToSend = {speed, '\n'};
                try {
                    mmOutputStream.write(bytesToSend);
                }
                catch (IOException e) {
                    e.printStackTrace();
                }

            }
        });

        //Send signal to turn servo left
        leftBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //send an l character to set to the left
                byte direction = (byte) 'l';
                byte[] bytesToSend = {direction, '\n'};
                try {
                    mmOutputStream.write(bytesToSend);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        //Sends a signal to move servo to center position to move straight.
        straightBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //send an s character to set to straight
                byte direction = (byte) 's';
                byte[] bytesToSend = {direction, '\n'};
                try {
                    mmOutputStream.write(bytesToSend);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        //Send signal to turn servo right
        rightBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //Send an r characer to move servo to the right
                byte direction = (byte) 'r';
                byte[] bytesToSend = {direction, '\n'};
                try {
                    mmOutputStream.write(bytesToSend);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        //Sets up seekbar to send its current value to bluetooth controller.
        speedController.setOnSeekBarChangeListener(
            new SeekBar.OnSeekBarChangeListener() {
                @Override
                public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                    byte speed = (byte) speedController.getProgress();
                    byte[] bytesToSend = {speed, '\n'};
                    try {
                        mmOutputStream.write(bytesToSend);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }

                @Override
                public void onStartTrackingTouch(SeekBar seekBar) {
                    //Empty
                }

                @Override
                public void onStopTrackingTouch(SeekBar seekBar) {
                    //Empty
                }
            }
        );
    }

    /**
     * Looks for bluetooth adapter and attempts to connect to it. If there is no bluetooth adapater
     * available, an error message is shown and this method exits out. This method will prompt the
     * user to turn on bluetooth is the device has it and it is turned off.
     */
    void findBT(){
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(mBluetoothAdapter == null){
            statusText.setText("No bluetooth adapter available");
        }
        else { //If we have a bluetooth adapter
            if (!mBluetoothAdapter.isEnabled()) { //if it's not enabled, prompt user to enable
                Intent enableBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBluetooth, REQUEST_ENABLE_BT);
            }

            //Get the paired device list
            Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
            boolean deviceFound = false;
            if (pairedDevices.size() > 0) { //If there is paired devices in the saved list.
                for (BluetoothDevice device : pairedDevices) { //Search through paired devices list
                    if (device.getName().equals(DEVICE_NAME)) { //If our device is found, save name
                        mmDevice = device;
                        deviceFound = true;
                        break;
                    }
                }
            }
            if(deviceFound) { //If device is found in paired list, connect to it.
                statusText.setText("Connecting.");
                while (!connected) {
                    try { //Try to open bluetooth connection.
                        openBT();
                        connected = true;
                    }
                    catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }

    /**
     * Open the connection over the bluetooth line.
     *
     * @throws IOException Thrown if there is an error in opening the streams.
     */
    void openBT() throws IOException {
        UUID uuid = UUID.fromString(STANDARD_SERIAL_PORT_SERVICE_ID);
        mmSocket = mmDevice.createRfcommSocketToServiceRecord(uuid);
        mmSocket.connect();
        mmOutputStream = mmSocket.getOutputStream();
        mmInputStream = mmSocket.getInputStream();

        beginListenForData();

        statusText.setText("Bluetooth Connected");
    }

    /**
     * This function creates the thread that will listen for data sent to the device. The data will
     * then be checked to see if it is valid input. If so, it will display the result as the motor
     * speed percentage on the app.
     */
    void beginListenForData(){
        final Handler handler = new Handler();
        final byte delimiter = 10; //This is the ASCII code for a newline character

        stopWorker = false;
        readBufferPosition = 0;
        readBuffer = new byte[1024];
        //Create worker thread
        workerThread = new Thread(new Runnable() {
            public void run(){
            //continue running until stop command is given
            while(!Thread.currentThread().isInterrupted() && !stopWorker){
                try{
                    int bytesAvailable = mmInputStream.available();
                    if(bytesAvailable > 0){
                        byte[] packetBytes = new byte[bytesAvailable];
                        mmInputStream.read(packetBytes); //read in data
                        for(int i=0;i<bytesAvailable;i++){
                            byte b = packetBytes[i];
                            if(b == delimiter) {
                                byte[] encodedBytes = new byte[readBufferPosition];
                                //Receive and save the data
                                System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);

                                final String data = new String(encodedBytes, "US-ASCII");
                                int dataIntTemp = -80;
                                try {
                                    dataIntTemp = 2 * (encodedBytes[0] - 50);
                                }
                                catch(ArrayIndexOutOfBoundsException e){
                                    e.printStackTrace();
                                }
                                final int dataInt = dataIntTemp;
                                readBufferPosition = 0;

                                handler.post(new Runnable(){
                                    public void run(){
                                        //If the data is at or below 100, set the text to the value.
                                        if(dataInt <= 100) {
                                            speedText.setText(dataInt + " %");
                                        }
                                    }
                                });
                            }
                            else{
                                readBuffer[readBufferPosition++] = b;
                            }
                        }
                    }
                }
                catch (IOException ex){
                    stopWorker = true;
                }
            }
            }
        });
        //Begin worker thread.
        workerThread.start();
    }

    /**
     * Closes bluetooth connection. A message is shown if the device isn't connected.
     */
    void closeBT(){
        stopWorker = true;
        try {
            mmOutputStream.close();
            mmInputStream.close();
            mmSocket.close();
            connected = false;
        }
        catch(IOException e){
            e.printStackTrace();
        }
        statusText.setText("Not connected");
    }
}