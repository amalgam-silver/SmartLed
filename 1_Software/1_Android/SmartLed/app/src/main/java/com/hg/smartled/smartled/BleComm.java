package com.hg.smartled.smartled;

import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCharacteristic;

import java.util.Date;

/**
 * Created by hg on 2016-1-10.
 */
public class BleComm {

    /* 发送设置时间报文 */
    public void bleSetTime(BluetoothGattCharacteristic bleSend, BluetoothGatt ble)
    {
        Date curDate = new Date(System.currentTimeMillis());//获取当前时间

        byte[] dataSend = {0x7f,0x01,0x01,12,15,2,20,0,24,30,5,0};
        int year = curDate.getYear();
        year -= 100;
        dataSend[4] = (byte)(year);
        dataSend[5] = (byte)(curDate.getMonth() + 1);
        dataSend[6] = (byte)(curDate.getDate());
        dataSend[7] = (byte)(curDate.getHours());
        dataSend[8] = (byte)(curDate.getMinutes());
        dataSend[9] = (byte)(curDate.getSeconds());
        dataSend[10] = (byte)(curDate.getDay());
        dataSend[11] = makeCheckSum(dataSend, dataSend[3]);

        bleSend.setValue(dataSend);
        ble.writeCharacteristic(bleSend);
    }

    /* 发送请求锁屏时间报文 */
    public void bleGetLockTime(BluetoothGattCharacteristic bleSend, BluetoothGatt ble)
    {
        byte[] dataSend = {0x7f, 0x02, 0x00, 6, 0, 0x08};
        bleSend.setValue(dataSend);
        ble.writeCharacteristic(bleSend);
    }

    /* 发送设置锁屏时间报文 */
    public void bleSetLockTime(BluetoothGattCharacteristic bleSend, BluetoothGatt ble, byte time)
    {
        byte[] dataSend = {0x7f, 0x02, 0x01, 6, 10, 0};

        if (time < 10) {
            time = 10;
        }else if (time > 70){
            time = 70;
        }
        dataSend[4] = time;
        dataSend[5] = makeCheckSum(dataSend, dataSend[3]);

        bleSend.setValue(dataSend);
        ble.writeCharacteristic(bleSend);
    }

    /* 发送获取系统状态报文 */
    public void bleGetSysState(BluetoothGattCharacteristic bleSend, BluetoothGatt ble)
    {
        byte[] dataSend = {0x7f, 0x03, 0x00, 0x0C, 0, 0, 0, 0, 0, 0, 0, 0x0F};
        bleSend.setValue(dataSend);
        ble.writeCharacteristic(bleSend);
    }

    /* 发送获取Led状态报文 */
    public void bleGetLedState(BluetoothGattCharacteristic bleSend, BluetoothGatt ble)
    {
        byte[] dataSend = {0x7f, 0x04, 0x00, 0x0C, 0, 0, 0, 0, 0, 0, 0, 0x10};
        bleSend.setValue(dataSend);
        ble.writeCharacteristic(bleSend);
    }

    /* 发送设置Led状态报文 */
    public void bleSetLedState(BluetoothGattCharacteristic bleSend, BluetoothGatt ble, byte pattern, byte delayTime, byte light, byte breath, byte r, byte g, byte b)
    {
        pattern = adjustLmt(pattern, (byte)0, (byte)1);
        delayTime = adjustLmt(delayTime, (byte)0, (byte)120);
        light = adjustLmt(light, (byte)0, (byte)100);
        breath = adjustLmt(breath, (byte)0, (byte)1);
        r = adjustLmt(r, (byte)0, (byte)100);
        g = adjustLmt(g, (byte)0, (byte)100);
        b = adjustLmt(b, (byte)0, (byte)100);

        byte[] dataSend = {0x7f, 0x04, 0x01, 0x0C, 0, 0, 0, 0, 0, 0, 0, 0};
        dataSend[4] = pattern;
        dataSend[5] = delayTime;
        dataSend[6] = light;
        dataSend[7] = breath;
        dataSend[8] = r;
        dataSend[9] = g;
        dataSend[10] = b;
        dataSend[11] = makeCheckSum(dataSend, dataSend[3]);
        bleSend.setValue(dataSend);
        ble.writeCharacteristic(bleSend);
    }

    /* 限制上下限 */
    private byte adjustLmt(byte value, byte min, byte max)
    {
        if (value > max)    return max;
        if (value < min)    return min;
        return  value;
    }

    /* 计算和校验 */
    private byte makeCheckSum(byte data[], int length)
    {
        byte ret = 0;
        int sum = 0;

        for (int i = 1; i < length - 1; i++)
        {
            sum += data[i];
        }

        ret = (byte)sum;
        return ret;
    }
}
