using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Net.Sockets;
using System.Net;
using System.Threading;

namespace TCP_Server_Demo
{
    class Program
    {
        static void Main(string[] args)
        {
            TcpServerDemo();
            Console.WriteLine("更多控制指令的详细解释请查阅《TCP-KP型网络继电器AT指令表.PDF》");
            Console.WriteLine("本Demo仅用于开发参考，不能直接用于商业项目实施！");
            Console.ReadLine();
        }

        /// <summary>
        /// 通过Socket类进行TcpServer通讯演示
        /// 步骤：
        /// 1、Bind IP与端口
        /// 2、启动侦听
        /// 3、等待客户端连接
        /// 4、连接成功后可进行数据发送
        /// 5、完成后，关闭socketClient
        /// </summary>
        static void TcpServerDemo()
        {
            //服务端的IP地址
            //请先在终端上查询服务器IP
            string serverIP = "192.168.1.229";
            //服务端的端口
            int serverPort = 6000;

            Console.WriteLine("本机作为服务端");
            Console.WriteLine("本机IP：" + serverIP + " 端口：" + serverPort);

            //使用指定的地址族，套接字类型和协议实例化新的套接字
            Socket socketServer = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            //将字符串地址转化为网际协议地址
            IPAddress ipAddress = IPAddress.Parse(serverIP);//这里为服务器IP
            //使用指定地址和端口号实例化新的网络端点
            IPEndPoint endPoint = new IPEndPoint(ipAddress, serverPort);//ipAddress为服务器IP ， 6000为服务器端口
            try {
                //socket与本地终结点相关联，绑定IP与端口
                socketServer.Bind(endPoint);
                //设置最大侦听长度，并启动侦听
                socketServer.Listen(200);
            } catch {
                Console.WriteLine("请检查IP是否正确，或端口是否被占用？");
                Console.ReadKey();
                socketServer.Close();
                return;
            }
            
            
            Console.WriteLine("启动侦听，等待客户端请求连接。。。");
            //等待客户端连接，如果没有客户端连接，即会一址等待
            //1、将硬件设备为TCP_Client
            //2、将硬件的目的IP设置与 serverIP 一致的值
            //3、将硬件的目的端口设备与 serverPort 一致的值
            //如果硬件不能连接上，请检查以上参数
            Socket socketClient = socketServer.Accept();
            Console.WriteLine("连接成功");
            string[] strs = socketClient.RemoteEndPoint.ToString().Split(':');
            Console.WriteLine("客户端IP：" + strs[0] + " 端口：" + strs[1]);

            //========================
            // 以下为数据发送与接收
            //========================

            Console.WriteLine("TCP-KP系列网络继电器功能测试：");
            //发送继电器控制指令 AT+STACH
            Console.Write("控制继电器通道1常开吸合发送：AT+STACH1=1\\r\\n");
            //将字符串指令转换为byte数组
            byte[] buf = System.Text.Encoding.Default.GetBytes("AT+STACH1=1\r\n");
            //发送AT指令
            socketClient.Send(buf);

            byte[] recbuf = new byte[10];//真正使用时，接收缓冲区需要适当的调整
            //等待硬件响应命令，接收到的数据为byte数组
            //会等待到有数据返回为止
            int recLen = socketClient.Receive(recbuf);
            //将byte数组转为字符串
            string str = System.Text.Encoding.Default.GetString(recbuf);
            str = str.Replace("\r\n", "\\r\\n");
            Console.WriteLine($"网络继电器应答：{str}");
            Console.WriteLine("指令执行成功");

            //等待300毫秒再执行
            Thread.Sleep(300);

            //发送继电器控制指令 AT+STACH
            Console.WriteLine("控制继电器通道1常开断开发送：AT+STACH1=0\\r\\n");
            //将字符串指令转换为byte数组
            buf = System.Text.Encoding.Default.GetBytes("AT+STACH1=0\r\n");
            //发送AT指令
            socketClient.Send(buf);

            recbuf = new byte[10];//真正使用时，接收缓冲区需要适当的调整
            //等待硬件响应命令，接收到的数据为byte数组
            //会等待到有数据返回为止
            recLen = socketClient.Receive(recbuf);
            //将byte数组转为字符串
            str = System.Text.Encoding.Default.GetString(recbuf);
            str = str.Replace("\r\n", "\\r\\n");
            Console.WriteLine($"网络继电器应答：{str}");
            Console.WriteLine("指令执行成功");

            //等待300毫秒再执行
            Thread.Sleep(300);

            //发送继电器控制指令 AT+STACH
            Console.WriteLine("控制继电器通道1常开接口吸合15秒后断开发送：AT+STACH1=1,15\\r\\n");
            //将字符串指令转换为byte数组
            buf = System.Text.Encoding.Default.GetBytes("AT+STACH1=1,15\r\n");
            //发送AT指令
            socketClient.Send(buf);

            recbuf = new byte[10];//真正使用时，接收缓冲区需要适当的调整
            //等待硬件响应命令，接收到的数据为byte数组
            //会等待到有数据返回为止
            recLen = socketClient.Receive(recbuf);
            //将byte数组转为字符串
            str = System.Text.Encoding.Default.GetString(recbuf);
            str = str.Replace("\r\n", "\\r\\n");
            Console.WriteLine($"网络继电器应答：{str}");
            Console.WriteLine("指令执行成功");


            //发送继电器控制指令 AT+OCCH
            Console.WriteLine("获取开关量输入接口1的状态发送：AT+OCCH1=?\\r\\n");
            //将字符串指令转换为byte数组
            buf = System.Text.Encoding.Default.GetBytes("AT+OCCH1=?\r\n");
            //发送AT指令
            socketClient.Send(buf);

            recbuf = new byte[10];//真正使用时，接收缓冲区需要适当的调整
            //等待硬件响应命令，接收到的数据为byte数组
            //会等待到有数据返回为止
            recLen = socketClient.Receive(recbuf);
            //将byte数组转为字符串
            str = System.Text.Encoding.Default.GetString(recbuf);
            str = str.Replace("\r\n", "\\r\\n");
            Console.WriteLine($"网络继电器应答：{str}");
            Console.WriteLine("指令执行成功");


            Console.WriteLine("关闭客户端连接");
            //关闭连接
            socketClient.Close();

            Console.WriteLine("测试完成");
        }

    }
}
