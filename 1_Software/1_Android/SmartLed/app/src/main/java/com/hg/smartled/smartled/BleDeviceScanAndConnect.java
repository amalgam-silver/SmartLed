package com.hg.smartled.smartled;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Toast;
import java.util.ArrayList;
import java.util.List;

public class BleDeviceScanAndConnect extends ActionBarActivity {

    private boolean successBle = false;
    private BluetoothGatt mBluetoothGatt;
    private BluetoothAdapter mBluetoothAdapter;
    private ArrayList<BluetoothDevice> mLeDevice = new ArrayList<>();
    private ArrayList<String>mNameLeDevice = new ArrayList<>();
    private Handler mHandler;
    private boolean mScanning;
    private static final long SCAN_PERIOD = 10000;
    ArrayAdapter<String> aa;
    EditText editTextLog;
    byte[] dataReceive;
    BluetoothGattCharacteristic characteristicDataReceive,characteristicDataSend;
    private BluetoothDevice bluetoothDevice = null;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ble_device_scan_and_connect);

//        getActionBar().setTitle(R.string.device_search);

        final BluetoothManager bluetoothManager =
                (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        mBluetoothAdapter = bluetoothManager.getAdapter();

        if(mBluetoothAdapter == null){
            Toast.makeText(this, R.string.ble_not_supported, Toast.LENGTH_SHORT).show();
            finish();
        }

        mHandler = new Handler();

        ListView listViewLeDevice = (ListView) findViewById(R.id.listViewBluetoothDevice);

        aa = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, mNameLeDevice);

        listViewLeDevice.setAdapter(aa);
        listViewLeDevice.setOnItemClickListener(listener);

        editTextLog = (EditText) findViewById(R.id.editTextLog);
        editTextLog.setEnabled(false);
        editTextLog.setFocusable(false);

        handlerDisplay.postDelayed(runnable, 500);
    }

    AdapterView.OnItemClickListener listener = new AdapterView.OnItemClickListener(){

        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            final BluetoothDevice device = mLeDevice.get(position);
            bluetoothDevice = device;
            if(device == null) return;

            Toast.makeText(BleDeviceScanAndConnect.this,"GATT server connectting",Toast.LENGTH_SHORT).show();
            scanLeDevice(false);

            mBluetoothGatt = device.connectGatt(BleDeviceScanAndConnect.this, false, mGattCallback);
            mLeDevice.clear();
            mNameLeDevice.clear();
            aa.notifyDataSetChanged();
        }
    };

//    @Override
//    public boolean onKeyDown(int keyCode, KeyEvent event) {
//
//        if (keyCode==KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0)
//        {
//
//        }
//        return false;
//    }

    private int mConnectionState = STATE_DISCONNECTED;

    private static final int STATE_DISCONNECTED = 0;
    private static final int STATE_CONNECTING = 1;
    private static final int STATE_CONNECTED = 2;

    BluetoothGattCallback mGattCallback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            super.onConnectionStateChange(gatt, status, newState);

            if (newState == BluetoothProfile.STATE_CONNECTED) {
                mConnectionState = STATE_CONNECTED;
                mBluetoothGatt.discoverServices();
            } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                mConnectionState = STATE_DISCONNECTED;
            }
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            super.onServicesDiscovered(gatt, status);
            displayGattServices(mBluetoothGatt.getServices());
            if(successBle)
            {
                String bleAddr = mBluetoothGatt.getDevice().getAddress();
                Intent intent = new Intent();
                intent.putExtra("ble", bleAddr);
                setResult(14159, intent);
                finish();
            }
        }

        @Override
        public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
            super.onCharacteristicChanged(gatt, characteristic);
            mBluetoothGatt.readCharacteristic(characteristic);
            dataReceive = characteristic.getValue();
            // String Log = new String(dataReceive);
        }
    };

    Handler handlerDisplay=new Handler();
    Runnable runnable=new Runnable() {
        @Override
        public void run() {
            if(dataReceive != null) {
                String Log = new String(dataReceive);
                editTextLog.setText(Log);
            }

//            if (!successBle)
//            {
//                if (mBluetoothGatt != null) {
//                    mBluetoothGatt.disconnect();
//                }
//                if (bluetoothDevice != null) {
//                    mBluetoothGatt = bluetoothDevice.connectGatt(BleDeviceScanAndConnect.this, false, mGattCallback);
//                }
//            }
            handlerDisplay.postDelayed(this, 300);
        }
    };

    private void displayGattServices(List<BluetoothGattService> gattServices) {
        if (gattServices == null) return;

        // Loops through available GATT Services.
        for (BluetoothGattService gattService : gattServices) {
            List<BluetoothGattCharacteristic> gattCharacteristics = gattService.getCharacteristics();

            // Loops through available Characteristics.
            for (BluetoothGattCharacteristic gattCharacteristic : gattCharacteristics) {
                String uuid = gattCharacteristic.getUuid().toString();
                if(uuid.contains("ffe9")){
                    characteristicDataSend = gattCharacteristic;
                    BleComm bleComm = new BleComm();
                    /* 设置时间 */
                    bleComm.bleSetTime(characteristicDataSend, mBluetoothGatt);
                    successBle = true;
                }
                else if(uuid.contains("ffe4")){
                    characteristicDataReceive = gattCharacteristic;
                    mBluetoothGatt.setCharacteristicNotification(characteristicDataReceive, true);
                }
            }
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_ble_device_scan_and_connect, menu);

        if (!mScanning) {
            menu.findItem(R.id.menu_searching).setVisible(true);
            menu.findItem(R.id.menu_stop).setVisible(false);
            menu.findItem(R.id.menu_refresh).setActionView(null);
        } else {
            menu.findItem(R.id.menu_searching).setVisible(false);
            menu.findItem(R.id.menu_stop).setVisible(true);
            menu.findItem(R.id.menu_refresh).setActionView(
                    R.layout.ble_scanning_progress);
        }

        return true;
    }

    @Override
    protected void onStop() {
        super.onStop();

        mLeDevice.clear();
        mNameLeDevice.clear();
        if(mBluetoothGatt != null)
            mBluetoothGatt.close();
        handlerDisplay.removeCallbacks(runnable);
    }

    @Override
    protected void onResume() {
        super.onResume();

        if(!mBluetoothAdapter.isEnabled()){
            Intent enableBluetoothIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBluetoothIntent, 1);
        }
        scanLeDevice(true);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        if(id == R.id.menu_searching){
            mLeDevice.clear();
            mNameLeDevice.clear();
            aa.notifyDataSetChanged();
            scanLeDevice(true);
        }else if(id == R.id.menu_stop){
            scanLeDevice(false);
        }

        return super.onOptionsItemSelected(item);
    }

    private void scanLeDevice (final boolean enable){
        if(enable){
            mHandler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    mScanning = false;
                    mBluetoothAdapter.stopLeScan(mLeScanCallBack);
                    invalidateOptionsMenu();
                }
            },SCAN_PERIOD);

            mScanning = true;
            mBluetoothAdapter.startLeScan(mLeScanCallBack);
        }else{
            mScanning = false;
            mBluetoothAdapter.stopLeScan(mLeScanCallBack);
        }
        invalidateOptionsMenu();
    }

    private BluetoothAdapter.LeScanCallback mLeScanCallBack =
            new BluetoothAdapter.LeScanCallback(){

                public void onLeScan(final BluetoothDevice device, int rssi, byte[] scanRecord){
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            if(!mLeDevice.contains(device)){
                                mLeDevice.add(device);
                                mNameLeDevice.add(device.getName());
                                aa.notifyDataSetChanged();
                            }
                        }
                    });
                }
            };
}