var net = require('net')

function getConnection(connName) {
    console.log('本机作为客户端');
    console.log('主动连接IP：192.168.1.199:12345');
    var client = net.connect({port:12345,host:'192.168.1.199'},function(){

        this.on('data',function(data){
            console.log(connName + " 网络继电器应答:" + data);
            console.log('指令执行成功');
        });

        this.on('end',function(){
            console.log(connName + 'Client disconnection');
        });

        this.on('error',function(err){
            console.log('Socket Error:',JSON.stringify(err));
        });

        this.on('timeout',function(){
            console.log('Socket Timed out');
        });

        this.on('close',function(){
            console.log('Socket Closed');
        });
    });
    return client;
};

function writeData(socket,data){
    var success = !socket.write(data);
    if(!success){
        (function(socket,data){
            socket.once('drain',function(){
                writeData(socket,data);
            });
        })(socket,data);
    }
}

var relay = getConnection('relay');
console.log('TCP-KP系列网络继电器功能测试：');
var intervalObj = setInterval(intervalFunc,1250);

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

function intervalFunc(){
    if(0 == exeStatus){
        console.log('控制继电器通道1常开吸合发送：AT+STACH1=1\\r\\n');
        writeData(relay,"AT+STACH1=1\r\n");
    } else if(1 == exeStatus){
        console.log('发送读继电器通道1状态指令：AT+STACH1=?\\r\\n');
        writeData(relay,"AT+STACH1=?\r\n");
    } else if(2 == exeStatus){
        console.log('控制继电器通道1常开断开发送：AT+STACH1=0\\r\\n');
        writeData(relay,"AT+STACH1=0\r\n");
    } else if(3 == exeStatus){
        console.log('发送读继电器通道1状态指令：AT+STACH1=?\\r\\n');
        writeData(relay,"AT+STACH1=?\r\n");
    } else if(4 == exeStatus){
        console.log('控制继电器通道1常开接口吸合2秒后断开发送：AT+STACH1=1,2\\r\\n');
        writeData(relay,"AT+STACH1=1,2\r\n");
    } else if(5 == exeStatus){
        console.log('发送读继电器通道1状态指令：AT+STACH1=?\\r\\n');
        writeData(relay,"AT+STACH1=?\r\n");
    } else if(6 == exeStatus){
        console.log('获取开关量输入接口1的状态发送：AT+OCCH1=?\\r\\n');
        writeData(relay,"AT+OCCH1=?\r\n");
    }
    exeStatus ++;
    if(exeStatus > 7){
        console.log('更多控制指令的详细解释请查阅《TCP-KP型网络继电器AT指令表.PDF》');
        console.log('本Demo仅用于开发参考，不能直接用于商业项目实施！');
        clearInterval(intervalObj);
        process.exit(0);//程序执行完成，退出
    }
}


