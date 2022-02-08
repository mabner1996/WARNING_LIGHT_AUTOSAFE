package com.aliyun.sample.alicloud;

import com.aliyun.iot20180120.models.BatchPubRequest;
import com.aliyun.iot20180120.models.BatchPubResponse;
import com.aliyun.tea.TeaModel;
import com.aliyun.teaopenapi.models.Config;

import java.util.Base64;

/**
 * \* @author: wky
 * \* Date: 2021/11/17
 * \* Time: 17:13
 * \* To change this template use File | Settings | File Templates.
 * \* Description:源码来自阿里云官网
 * \
 */
public class SendService {

    public SendService() {

    }

    /**
     * 使用AK&SK初始化账号Client
     * @param accessKeyId
     * @param accessKeySecret
     * @return Client
     * @throws Exception
     */
    public static com.aliyun.iot20180120.Client createClient(String accessKeyId, String accessKeySecret) throws Exception {
        Config config = new Config()
                // 您的AccessKey ID
                .setAccessKeyId(accessKeyId)
                // 您的AccessKey Secret
                .setAccessKeySecret(accessKeySecret);
        // 访问的域名
        config.endpoint = "iot.cn-shanghai.aliyuncs.com";//进入物联网平台，如果左上角为  华东2(上海) ，访问域名才是这个，其他的域名请打阿里云技术支持电话 400 801 3260
        return new com.aliyun.iot20180120.Client(config);
    }

    public static void SendCmd(String cmd) throws Exception {

        //accessKey、accessSecret这两个参数在登录阿里云后，进入控制台，鼠标移到右上角的头像图标上，在展开的菜单中，点击AccessKey管理，进入查看
        String accessKey = "YourAccessKey";
        String accessSecret = "YourAccessSecret";
        String productKey = "YourProductKey";//产品Id
        String deviceName = "YourDeviceName";//设备名

        com.aliyun.iot20180120.Client client = createClient(accessKey, accessSecret);
        BatchPubRequest batchPubRequest = new BatchPubRequest()
                // Array, 必填, 要接收消息的设备名称。  > - 与**ProductKey**结合使用，传入设备必须属于同一产品。 - 单次调用，最多传入100个设备名称，且设备名称不可重复。
                .setDeviceName(java.util.Arrays.asList(
                        deviceName
                ))
                .setProductKey(productKey)
                .setQos(0)
                .setTopicShortName("get")//给设备下发指令，用的是设备自身的订阅主题，目前使用的订阅主题为 /a18pRYzxxxx/test_device/user/get ， 所以填 get
                .setMessageContent(Base64.getEncoder().encodeToString(cmd.getBytes("utf-8")));
        BatchPubResponse resp = client.batchPub(batchPubRequest);
        com.aliyun.teaconsole.Client.log(com.aliyun.teautil.Common.toJSONString(TeaModel.buildMap(resp)));
    }
}