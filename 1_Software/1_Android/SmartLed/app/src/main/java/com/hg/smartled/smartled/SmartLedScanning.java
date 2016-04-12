package com.hg.smartled.smartled;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.Context;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothProfile;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;


public class SmartLedScanning extends ActionBarActivity {

    private BluetoothDevice bleDevice;                  // 蓝牙设备,即需要连接的远程蓝牙设备
    private BluetoothAdapter mBluetoothAdapter;        // 蓝牙设配器,即手机内置的蓝牙模块
    private BluetoothGatt mBluetoothGatt;              // 连接后的远程蓝牙设备对象
    private byte[] dataReceive;                        // 接收缓存区
    private BluetoothGattCharacteristic characteristicDataReceive,characteristicDataSend;   //接收和发送特征字
    private BleComm bleComm = new BleComm();
    private boolean reqUpdateUI = false;              // 更新UI请求
    private boolean bleConnected = false;             // 设备连接成功标志
    private int getBleStateCnt = 0;                    // 设备连接成功后，定时查询设备的计数器，0~2循环，0时查询睡眠时间，1时查询系统状态，2时查询LED状态
    /* 控件声明 */
    private LinearLayout linearLayoutLight = null, linearLayoutColor = null, linearLayoutInfo = null;
    private TextView textViewSleepTime = null, textViewDelayOff = null, textViewInfo = null, textViewMode = null;
    private RadioGroup radioGroupMode = null;
    private RadioButton radioButtonLight = null, radioButtonColor = null;
    private CheckBox checkBoxBreath = null;
    private SeekBar seekBarSleepTime = null, seekBarDelayOff = null;
    private SeekBar seekBarRed = null, seekBarGreen = null, seekBarBlue = null;
    private SeekBar seekBarLight = null;
    private Button buttonInfo = null;

    /* UI更新需要的全局变量 */
    private String sleepTime = null, delayOff = null;
    private int progressSleepTime = 0;
    private int mode = 0, delayTime = 0, light = 0, red = 0, green = 0, blue = 0, breathEn = 0;
    private String info;
    private boolean showInfoEn = false;
    private int charging = 0, voltage = 0, version = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_smart_led_scanning);

        /* 手机无BLE的情况 */
        if(!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)){
            Toast.makeText(this, R.string.ble_not_supported, Toast.LENGTH_SHORT).show();
            finish();
        }

        /* 获取手机的蓝牙适配器 */
        final BluetoothManager bluetoothManager =
                (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        mBluetoothAdapter = bluetoothManager.getAdapter();

        /* 关联控件 */
        linearLayoutColor = (LinearLayout)findViewById(R.id.linearLayoutColor);
        linearLayoutLight = (LinearLayout)findViewById(R.id.linearLayoutLight);
        linearLayoutInfo = (LinearLayout)findViewById(R.id.linearLayoutInfo);
        textViewSleepTime = (TextView)findViewById(R.id.textViewSleepTime);
        textViewDelayOff = (TextView)findViewById(R.id.textViewDelay);
        textViewInfo = (TextView)findViewById(R.id.textViewInfo);
        radioGroupMode = (RadioGroup)findViewById(R.id.radioGroup);
        radioButtonColor = (RadioButton)findViewById(R.id.radioButtonColorMode);
        radioButtonLight = (RadioButton)findViewById(R.id.radioButtonLightMode);
        checkBoxBreath = (CheckBox)findViewById(R.id.checkBoxBreath);
        seekBarSleepTime = (SeekBar)findViewById(R.id.seekBarSleep);
        seekBarDelayOff = (SeekBar)findViewById(R.id.seekBarDelay);
        seekBarRed = (SeekBar)findViewById(R.id.seekBarRed);
        seekBarGreen = (SeekBar)findViewById(R.id.seekBarGreen);
        seekBarBlue = (SeekBar)findViewById(R.id.seekBarBlue);
        seekBarLight = (SeekBar)findViewById(R.id.seekBarLight);
        buttonInfo = (Button)findViewById(R.id.buttonInfo);
        textViewMode = (TextView)findViewById(R.id.textViewMode);

        info = "SmartLed\n";
        sleepTime = getString(R.string.sleep_time);
        delayOff = getString(R.string.delayOff);

        /* 模式单选按钮发生变化时,切换不同模式的显示 */
        radioGroupMode.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                if (checkedId == R.id.radioButtonLightMode) {
                    linearLayoutLight.setVisibility(View.VISIBLE);
                    linearLayoutColor.setVisibility(View.INVISIBLE);
                } else if (checkedId == R.id.radioButtonColorMode) {
                    linearLayoutLight.setVisibility(View.INVISIBLE);
                    linearLayoutColor.setVisibility(View.VISIBLE);
                }
            }
        });

        /* 照明模式按钮按下 */
        radioButtonLight.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (characteristicDataSend != null) {
                    mode = 0;
                    bleComm.bleSetLedState(characteristicDataSend, mBluetoothGatt, (byte) mode, (byte) delayTime, (byte) light, (byte) breathEn, (byte) red, (byte) green, (byte) blue);
                }
            }
        });

        /* 彩灯模式按钮按下 */
        radioButtonColor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (characteristicDataSend != null) {
                    mode = 1;
                    bleComm.bleSetLedState(characteristicDataSend, mBluetoothGatt, (byte) mode, (byte) delayTime, (byte) light, (byte) breathEn, (byte) red, (byte) green, (byte) blue);
                }
            }
        });

        /* 关屏时间滑动条监听 */
        seekBarSleepTime.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            /* 实时更新文字显示 */
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (characteristicDataSend != null) {
                    if (progress < 10) {
                        sleepTime = getString(R.string.sleep_time) + "10 s";
                    } else if (progress <= 60) {
                        sleepTime = getString(R.string.sleep_time) + progress + " s";
                    } else {
                        sleepTime = getString(R.string.sleep_time) + getString(R.string.never);
                    }
                    textViewSleepTime.setText(sleepTime);
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            /* 滑块停下来时发送蓝牙报文，设置LED */
            public void onStopTrackingTouch(SeekBar seekBar) {
                if (characteristicDataSend != null) {
                    progressSleepTime = seekBar.getProgress();
                    if (progressSleepTime > 60) {
                        progressSleepTime = 70;
                        seekBar.setProgress(progressSleepTime);
                    } else if (progressSleepTime < 10) {
                        progressSleepTime = 10;
                        seekBar.setProgress(progressSleepTime);
                    }
                    bleComm.bleSetLockTime(characteristicDataSend, mBluetoothGatt, (byte) progressSleepTime);
                }
            }
        });

        /* 延时关断滑动条监听 */
        seekBarDelayOff.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            /* 实时更新文字显示 */
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (characteristicDataSend != null) {
                    delayOff = getString(R.string.delayOff) + progress + " min";
                    textViewDelayOff.setText(delayOff);
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            /* 滑块停下来时发送蓝牙报文，设置LED */
            public void onStopTrackingTouch(SeekBar seekBar) {
                if (characteristicDataSend != null) {
                    delayTime = seekBar.getProgress();
                    bleComm.bleSetLedState(characteristicDataSend, mBluetoothGatt, (byte) mode, (byte) delayTime, (byte) light, (byte) breathEn, (byte) red, (byte) green, (byte) blue);
                }
            }
        });

        /* 亮度滑动条监听 */
        seekBarLight.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            /* 滑块变化时发送蓝牙报文，设置LED */
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (characteristicDataSend != null) {
                    light = seekBar.getProgress();
                    bleComm.bleSetLedState(characteristicDataSend, mBluetoothGatt, (byte) mode, (byte) delayTime, (byte) light, (byte) breathEn, (byte) red, (byte) green, (byte) blue);
                }
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });

        /* 红色滑动条监听 */
        seekBarRed.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            /* 滑块变化时发送蓝牙报文，设置LED */
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (characteristicDataSend != null) {
                    red = seekBar.getProgress();
                    bleComm.bleSetLedState(characteristicDataSend, mBluetoothGatt, (byte) mode, (byte) delayTime, (byte) light, (byte) breathEn, (byte) red, (byte) green, (byte) blue);
                }
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });

        /* 绿色滑动条监听 */
        seekBarGreen.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            /* 滑块变化时发送蓝牙报文，设置LED */
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (characteristicDataSend != null) {
                    green = seekBar.getProgress();
                    bleComm.bleSetLedState(characteristicDataSend, mBluetoothGatt, (byte) mode, (byte) delayTime, (byte) light, (byte) breathEn, (byte) red, (byte) green, (byte) blue);
                }
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });

        /* 蓝色滑动条监听 */
        seekBarBlue.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            /* 滑块变化时发送蓝牙报文，设置LED */
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (characteristicDataSend != null) {
                    blue = seekBar.getProgress();
                    bleComm.bleSetLedState(characteristicDataSend, mBluetoothGatt, (byte) mode, (byte) delayTime, (byte) light, (byte) breathEn, (byte) red, (byte) green, (byte) blue);
                }            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });

        /* 呼吸灯复选框监听 */
        checkBoxBreath.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged (CompoundButton buttonView,boolean isChecked) {
                if (characteristicDataSend != null) {
                    if (isChecked) {        // 呼吸灯使能
                        breathEn = 1;
                    } else {                 // 呼吸灯失能
                        breathEn = 0;
                    }
                    bleComm.bleSetLedState(characteristicDataSend, mBluetoothGatt, (byte) mode, (byte) delayTime, (byte) light, (byte) breathEn, (byte) red, (byte) green, (byte) blue);
                }
            }
        });

        buttonInfo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (showInfoEn) {
                    showInfoEn = false;
                    linearLayoutInfo.setVisibility(View.INVISIBLE);
                    textViewSleepTime.setVisibility(View.VISIBLE);
                    seekBarSleepTime.setVisibility(View.VISIBLE);
                    radioGroupMode.setVisibility(View.VISIBLE);
                    textViewMode.setVisibility(View.VISIBLE);
                    if (mode == 0) {
                        linearLayoutLight.setVisibility(View.VISIBLE);
                        linearLayoutColor.setVisibility(View.INVISIBLE);
                    }else {
                        linearLayoutColor.setVisibility(View.VISIBLE);
                        linearLayoutLight.setVisibility(View.INVISIBLE);
                    }
                }else {
                    showInfoEn = true;
                    linearLayoutColor.setVisibility(View.INVISIBLE);
                    linearLayoutLight.setVisibility(View.INVISIBLE);
                    textViewSleepTime.setVisibility(View.INVISIBLE);
                    seekBarSleepTime.setVisibility(View.INVISIBLE);
                    linearLayoutInfo.setVisibility(View.VISIBLE);
                    textViewMode.setVisibility(View.INVISIBLE);
                    radioGroupMode.setVisibility(View.INVISIBLE);
                    textViewInfo.setText(info);
                }
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_smart_led_scanning, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        if(id == R.id.menu_scan)
        {
            Intent intent = new Intent(this, BleDeviceScanAndConnect.class);
            startActivityForResult(intent, 31415);
        }
        //noinspection SimplifiableIfStatement

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if ((requestCode == 31415) && (resultCode == 14159))
        {
            /* 根据从蓝牙设备搜索界面返回的蓝牙设备MAC地址获取蓝牙设备 */
            String bleAddr = data.getStringExtra("ble");
            bleDevice = mBluetoothAdapter.getRemoteDevice(bleAddr);

            /* 连接蓝牙设备 */
            mBluetoothGatt = bleDevice.connectGatt(SmartLedScanning.this, false, mGattCallback);
            Log.i("Hg", "BLE connecting...");
        }
    }

    /* 蓝牙设备连接回调 */
    BluetoothGattCallback mGattCallback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            super.onConnectionStateChange(gatt, status, newState);
            if (newState == BluetoothProfile.STATE_CONNECTED) {
                Log.i("Hg", "BLE connected.");
                mBluetoothGatt.discoverServices();      // 寻找蓝牙服务
                Log.i("Hg", "BLE searching services...");
            } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                Log.i("Hg", "BLE disconnected.");
                bleConnected = false;
                characteristicDataReceive = null;
                characteristicDataSend = null;
            }
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            int serviceNum = 0;
            super.onServicesDiscovered(gatt, status);
            List<BluetoothGattService> gattServices = mBluetoothGatt.getServices();     // 获取蓝牙服务

            if (gattServices == null) return;       // 若为null直接返回

            // Loops through available GATT Services.
            for (BluetoothGattService gattService : gattServices) {
                List<BluetoothGattCharacteristic> gattCharacteristics = gattService.getCharacteristics();

                // Loops through available Characteristics.
                for (BluetoothGattCharacteristic gattCharacteristic : gattCharacteristics) {
                    String uuid = gattCharacteristic.getUuid().toString();

                    if(uuid.contains("ffe9")) {                 // 蓝牙串口发送 特征字
                        Log.i("Hg", "BLE find send service.");
                        characteristicDataSend = gattCharacteristic;
                        serviceNum++;
                    }

                    if(uuid.contains("ffe4")){                  // 蓝牙串口接收 特征字
                        Log.i("Hg", "BLE find receive service.");
                        characteristicDataReceive = gattCharacteristic;
                        mBluetoothGatt.setCharacteristicNotification(characteristicDataReceive, true);
                        serviceNum++;
                    }
                }
            }

            bleConnected = (serviceNum >= 2);
            Log.i("Hg", "serviceNum = " + serviceNum);

            handlerDisplay.postDelayed(runnable, 500);
        }

        @Override
        public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
            super.onCharacteristicChanged(gatt, characteristic);
            mBluetoothGatt.readCharacteristic(characteristic);
            dataReceive = characteristic.getValue();

            String receiveHex = byte2HexString(dataReceive);
            Log.i("Hg", "BLE received data : " + receiveHex);

            if ((dataReceive[0] != 0x7f) || (dataReceive[2] != 0))
                return;

            if (!checkSum(dataReceive, dataReceive[3]))     // 校验和不通过，直接返回
                return;

            switch (dataReceive[1])
            {
                case 2:     // 关屏时间
                    /* 静态文本显示 */
                    if (dataReceive[4] != progressSleepTime) {
                        if (dataReceive[4] <= 60) {
                            sleepTime = getString(R.string.sleep_time) + dataReceive[4] + " s";
                        } else {
                            sleepTime = getString(R.string.sleep_time) + getString(R.string.never);
                        }
                    /* 进度条进度 */
                        progressSleepTime = dataReceive[4];
                    /* 更新UI请求 */
                        reqUpdateUI = true;
                    }
                    break;
                case 3:     // 系统状态
                    if ((dataReceive[4] != charging) || (dataReceive[5] != voltage) || (dataReceive[8] != version)) {
                        charging = dataReceive[4];
                        voltage = dataReceive[5];
                        version = dataReceive[8];

                        int tempVol = voltage;
                        if (tempVol < 0)    tempVol += 256;
                        int temperature = dataReceive[9] * 256 + dataReceive[10];
                        info = "SmartLed\n\n";
                        info = "环境温度: " + ((float)temperature / 100) + " ℃\n";
                        info += (dataReceive[4] == 1) ? "充电中...\n" : "未充电.\n";
                        info += "电池电压: " + (((float) tempVol + 200) / 100) + " V\n";
                        info += "软件版本: V" + ((float) dataReceive[8] / 100);
                        reqUpdateUI = true;
                    }
                    break;
                case 4:     // LED状态
                    if (dataReceive[4] != mode || dataReceive[5] != delayTime ||
                            dataReceive[6] != light || dataReceive[7] != breathEn ||
                            dataReceive[8] != red || dataReceive[9] != green ||
                            dataReceive[10] != blue)
                    {
                        mode = dataReceive[4];
                        delayTime = dataReceive[5];
                        light = dataReceive[6];
                        breathEn = dataReceive[7];
                        red = dataReceive[8];
                        green = dataReceive[9];
                        blue = dataReceive[10];

                        delayOff = getString(R.string.delayOff) + delayTime + " min";

                        reqUpdateUI = true;
                    }
                    break;
            }
        }
    };

    Handler handlerDisplay=new Handler();
    /* 用于定时更新UI */
    Runnable runnable=new Runnable() {
        @Override
        public void run() {
            Log.i("Hg", "reqUpdate: " + reqUpdateUI + "   ,bleConnected: " + bleConnected);

            /* 更新UI控件 */
            if(reqUpdateUI) {
                Log.i("Hg", "Updating UI...");
                /* 模式单选按钮 */
                if (mode == 0)
                {
                    radioButtonLight.setChecked(true);
                }
                else
                {
                    radioButtonColor.setChecked(true);
                }

                /* 休眠时间进度条 */
                textViewSleepTime.setText(sleepTime);
                seekBarSleepTime.setProgress(progressSleepTime);

                /* 延时关灯进度条 */
                textViewDelayOff.setText(delayOff);
                seekBarDelayOff.setProgress(delayTime);

                /* 亮度进度条 */
                seekBarLight.setProgress(light);

                /* 呼吸灯复选框 */
                checkBoxBreath.setChecked(breathEn == 1);

                /* RGB进度条 */
                seekBarRed.setProgress(red);
                seekBarGreen.setProgress(green);
                seekBarBlue.setProgress(blue);

                /* Info文本 */
                textViewInfo.setText(info);

                reqUpdateUI = false;
            }

            /* 定期请求设备状态 */
            if (bleConnected) {
                getBleStateCnt ++;
                getBleStateCnt %= 3;
                switch (getBleStateCnt)
                {
                    case 0:
                        Log.i("Hg", "BLE getting lock time...");
                        bleComm.bleGetLockTime(characteristicDataSend, mBluetoothGatt);
                        break;
                    case 1:
                        Log.i("Hg", "BLE getting system state...");
                        bleComm.bleGetSysState(characteristicDataSend, mBluetoothGatt);
                        break;
                    case 2:
                        Log.i("Hg", "BLE getting led state...");
                        bleComm.bleGetLedState(characteristicDataSend, mBluetoothGatt);
                        break;
                }
            }
            handlerDisplay.postDelayed(this, 500);
        }
    };

    protected void onStop() {
        super.onStop();

        if(mBluetoothGatt != null)
            mBluetoothGatt.close();
        handlerDisplay.removeCallbacks(runnable);
    }

    /* 检验校验和是否正确 */
    boolean checkSum(byte data[], int length)
    {
        byte checkSum = makeCheckSum(data, data[3]);
        return data[length - 1] == checkSum;
    }

    /* 计算和校验 */
    private byte makeCheckSum(byte data[], int length)
    {
        byte ret;
        int sum = 0;

        for (int i = 1; i < length - 1; i++)
        {
            sum += data[i];
        }

        ret = (byte)sum;
        return ret;
    }

    private String byte2HexString(byte data[])
    {
        String ret = " ";

        for (int i = 0; i < data.length; i++)
        {
            ret += Integer.toHexString(data[i] & 0xff) + " ";
        }

        return ret;
    }
}