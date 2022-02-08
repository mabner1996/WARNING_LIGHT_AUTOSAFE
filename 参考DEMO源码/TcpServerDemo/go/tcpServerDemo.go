package main

import (
	"fmt"
	"net"
	"strings"
	"time"
)

var buf []byte = make([]byte, 4096)

func main() {
	//参数连接 --> https://www.jianshu.com/p/325ac02fc31c
	fmt.Println("本机作为服务端")
	fmt.Println("本机IP：192.168.1.149 端口：6000")
	localAddress, _ := net.ResolveTCPAddr("tcp4", "192.168.1.149:6000") //定义一个本机IP和端口。
	var tcpListener, err = net.ListenTCP("tcp", localAddress)           //在刚定义好的地址上进监听请求。
	if err != nil {
		fmt.Println("监听出错：", err)
		return
	}
	defer func() { //担心return之前忘记关闭连接，因此在defer中先约定好关它。
		tcpListener.Close()
	}()

	fmt.Println("等待客户机的连接")
	var conn, err2 = tcpListener.AcceptTCP() //接受连接。
	if err2 != nil {
		fmt.Println("接受连接失败：", err2)
		return
	}
	fmt.Println("连接成功")

	var remoteAddr = conn.RemoteAddr() //获取连接到的对像的IP地址。
	fmt.Println("客户端IP与端口 --> ", remoteAddr)

	fmt.Println("TCP-KP系列网络继电器功能测试：")

	fmt.Println("控制继电器通道1常开吸合发送：AT+STACH1=1\\r\\n")
	conn.Write([]byte("AT+STACH1=1\r\n"))
	n, err := conn.Read(buf)
	fmt.Println("网络继电器应答：", strings.Replace(string(buf[:n]), "\r\n", "\\r\\n", 1))
	fmt.Println("指令执行成功！")
	time.Sleep(500000000) //0.5秒
	//================================================

	fmt.Println("控制继电器通道1常开断开发送：AT+STACH1=0\\r\\n")
	conn.Write([]byte("AT+STACH1=0\r\n"))
	y, err := conn.Read(buf)
	fmt.Println("网络继电器应答：", strings.Replace(string(buf[:y]), "\r\n", "\\r\\n", 1))
	fmt.Println("指令执行成功！")
	time.Sleep(500000000) //0.5秒
	//================================================

	fmt.Println("控制继电器通道1常开接口吸合15秒后断开发送：AT+STACH1=1,15\\r\\n")
	conn.Write([]byte("AT+STACH1=1,15\r\n"))
	k, err := conn.Read(buf)
	fmt.Println("网络继电器应答：", strings.Replace(string(buf[:k]), "\r\n", "\\r\\n", 1))
	fmt.Println("指令执行成功！")
	time.Sleep(500000000) //0.5秒
	//================================================

	fmt.Println("获取开关量输入接口1的状态发送：AT+OCCH1=?\\r\\n ")
	conn.Write([]byte("AT+OCCH1=?\r\n"))
	x, err := conn.Read(buf)
	fmt.Println("网络继电器应答：", strings.Replace(string(buf[:x]), "\r\n", "\\r\\n", 1))
	fmt.Println("指令执行成功！")

	fmt.Println("关闭客户端连接！")
	conn.Close()
	fmt.Println("测试完成！")

	fmt.Println("更多控制指令的详细解释请查阅《AT指令表.PDF》")
	fmt.Println("本Demo仅用于开发参考，不能直接用于商业项目实施！")
}
