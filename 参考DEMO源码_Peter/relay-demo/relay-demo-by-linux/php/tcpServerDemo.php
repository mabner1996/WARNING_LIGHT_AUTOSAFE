<?php
//windows  Linux系统通用
//启动指令
//php -q tcpServerDemo.php
$serverIp="192.168.1.229";//服务端IP
$serverPort=6000;//服务端端口
$READ_BUF_SIZE=1024;//

echo "本机作为服务端\r\n";
echo "本机IP：" . $serverIp . " 端口：" . $serverPort . "\r\n";
$server = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
socket_bind($server,$serverIp,$serverPort);//绑定端口与IP
socket_listen($server);//进入监听

echo "等待客户机的连接\r\n";
$client = socket_accept($server);//接受请求并建立连接
echo "连接成功\r\n";
socket_getpeername($client, $addr, $por);
echo "客户端IP：" .$addr . " 端口：" . $por . "\r\n";

echo "TCP-KP系列网络继电器功能测试：\r\n";

echo "控制继电器通道1常开吸合发送：AT+STACH1=1\\r\\n \r\n";
$msg = "AT+STACH1=1\r\n";
socket_write($client,$msg);
$data = socket_read($client,$READ_BUF_SIZE);
echo "网络继电器应答：" . str_replace("\r\n","\\r\\n",$data) . "\r\n";
echo "指令执行成功！\r\n";
usleep(500000);//0.5秒

//===========================================================
echo "控制继电器通道1常开断开发送：AT+STACH1=0\\r\\n \r\n";
$msg = "AT+STACH1=0\r\n";
socket_write($client,$msg);
$data = socket_read($client,$READ_BUF_SIZE);
echo "网络继电器应答：" . str_replace("\r\n","\\r\\n",$data) . "\r\n";
echo "指令执行成功！\r\n";
usleep(500000);//0.5秒

//===========================================================
echo "控制继电器通道1常开接口吸合15秒后断开发送：AT+STACH1=1,15\\r\\n \r\n";
$msg = "AT+STACH1=1,15\r\n";
socket_write($client,$msg);
$data = socket_read($client,$READ_BUF_SIZE);
echo "网络继电器应答：" . str_replace("\r\n","\\r\\n",$data) . "\r\n";
echo "指令执行成功！\r\n";
usleep(500000);//0.5秒

//===========================================================
echo "获取开关量输入接口1的状态发送：AT+OCCH1=?\\r\\n \r\n";
$msg = "AT+OCCH1=?\r\n";
socket_write($client,$msg);
$data = socket_read($client,$READ_BUF_SIZE);
echo "网络继电器应答：" . str_replace("\r\n","\\r\\n",$data) . "\r\n";
echo "指令执行成功！\r\n";
usleep(500000);//0.5秒

echo "关闭客户端连接！\r\n";
socket_close($client);
socket_close($server);
echo "测试完成！\r\n";

echo "更多控制指令的详细解释请查阅《TCP-KP型网络继电器AT指令表.PDF》\r\n";
echo "本Demo仅用于开发参考，不能直接用于商业项目实施！\r\n";

?>
