/**
 * 执行指令:node TcpServer.js
 */
const net = require('net');
const server = net.createServer();

/**
 * 状态说明
 * 0发送自锁吸合指令
 * 1发送查询输出状态指令
 * 2发送自锁断开指令
 * 3发送查询输出状态指令
 * 4发送点动开指令
 * 5发送查询输出状态指令
 * 6发送查询输入状态指令
 */
var exeStatus = 0;

server.on('error',(err) => {
    console.log('server error:',err);
});

server.on('close',() => {
    console.log('server closed!');
});

server.listen(6000,() => {
    console.log('listen port 6000');
});


var clientSock = null;
var intervalObj = null;
console.log('本机作为服务端');
server.on('connection',(client) => {

    clientSock = client;
    client.on('data',function(data) {
        console.log('网络继电器应答:' + data);
        console.log('指令执行成功');
    });

    client.on('error',(err) => {
        console.log(err);
    });

    client.on('end',() => {
        console.log('client discommected!');
    });

    client.on('close',function() {
        //server.close();
    });

    
    
    intervalObj = setInterval(IntervalFunc,1250);
});


function IntervalFunc() {

    if(clientSock == null) {
        clearInterval(intervalObj);
        process.exit(0);
    }

    if(0 == exeStatus) {
        console.log('控制继电器通道1常开吸合发送：AT+STACH1=1\\r\\n');
        clientSock.write("AT+STACH1=1\r\n");
    } else if(1 == exeStatus) {
        console.log('发送读继电器通道1状态指令：AT+STACH1=?\\r\\n');
        clientSock.write("AT+STACH1=?\r\n");
    } else if(2 == exeStatus) {
        console.log('控制继电器通道1常开断开发送：AT+STACH1=0\\r\\n');
        clientSock.write("AT+STACH1=0\r\n");
    } else if(3 == exeStatus) {
        console.log('发送读继电器通道1状态指令：AT+STACH1=?\\r\\n');
        clientSock.write("AT+STACH1=?\r\n");
    } else if(4 == exeStatus) {
        console.log('控制继电器通道1常开接口吸合2秒后断开发送：AT+STACH1=1,2\\r\\n');
        clientSock.write("AT+STACH1=1,2\r\n");
    } else if(5 == exeStatus) {
        console.log('发送读继电器通道1状态指令：AT+STACH1=?\\r\\n');
        clientSock.write("AT+STACH1=?\r\n");
    } else if(6 == exeStatus) {
        console.log('获取开关量输入接口1的状态发送：AT+OCCH1=?\\r\\n');
        clientSock.write("AT+OCCH1=?\r\n");
    }

    exeStatus ++;
    if(exeStatus > 7) {
        console.log('更多控制指令的详细解释请查阅《TCP-KP型网络继电器AT指令表.PDF》');
        console.log('本Demo仅用于开发参考，不能直接用于商业项目实施！');
        clearInterval(intervalObj);
        process.exit(0);//程序执行完成
    }else{
        clientSock.pipe(clientSock);
    }
}
