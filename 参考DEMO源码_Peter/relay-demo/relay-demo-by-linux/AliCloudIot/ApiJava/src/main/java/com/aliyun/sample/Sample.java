/**
 * 声明：
 *      源代码来自阿里云，本公司只是将AMQP与调用API下发的代码整合为一份
 *      接收数据采用AMQP的方式，当然接收还有MNS，但MNS还需要二次收费，看用户个人需求，具体区别请咨询阿里云技术
 *      发送数据采用调用API下发的方式
 *
 */
package com.aliyun.sample;

import com.aliyun.sample.alicloud.ReceiveService;
import com.aliyun.sample.alicloud.SendService;

public class Sample {

    static Runnable recService = new ReceiveService();

    public static void main(String[] args_) throws Exception {
        //java.util.List<String> args = java.util.Arrays.asList(args_);

        startReceiveThread();//必须等待接收线程完全启动
        relayControl();

        delayFunc(10*1000);//等待10秒再退出程序
        stopReceiveThread();

    }


    static void startReceiveThread() {
        //Runnable recService = new ReceiveService();
        Thread recTh = new Thread(recService);
        recTh.start();
        delayFunc(10*1000);//必须等待接收线程进入接收状态，否则有些数据会接收不到
    }

    static void stopReceiveThread() {
        ((ReceiveService)recService).isRun = false;
        ((ReceiveService)recService).Distroy();
    }

    static void relayControl() throws Exception {
        int delayTime = 500;
        //发送第一路继电器吸合指令
        SendService.SendCmd("AT+STACH1=1\r\n");
        delayFunc(delayTime);
        //发送第一路继电器断开指令
        SendService.SendCmd("AT+STACH1=0\r\n");
        delayFunc(delayTime);
        //发送第一路继电器点动开指令，点动时间为5秒
        SendService.SendCmd("AT+STACH1=1,5\r\n");
        delayFunc(delayTime);
        //查询第一路继电器状态，冒号后面的第1位值为1表示吸合，为0表示断开
        SendService.SendCmd("AT+STACH1=?\r\n");
        delayFunc(delayTime);
        //查询第一路输入状态
        SendService.SendCmd("AT+OCCH1=?\r\n");
    }

    static void delayFunc(int millis){
        try{
            Thread.sleep(millis);
        }catch (Exception e){}
    }

}
