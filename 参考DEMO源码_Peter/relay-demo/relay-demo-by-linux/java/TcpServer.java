/**
 * jdk:1.8
 * 执行:javac TcpServer.java  ---->  将产生TcpServer.class文件
 * 再执行:java TcpServer
 */

//package tcptest;

import java.io.*;
import java.net.*;

public class TcpServer {

	private BufferedReader reader;
	private BufferedWriter writer;
	private ServerSocket server;
	private Socket client;
	//服务端使用的端口
	private int serverPort = 6000;

	void TcpServerDemo() {

		try {
			System.out.println("本机作为服务端");
			System.out.println("本机端口：" + serverPort);
			server = new ServerSocket(serverPort);

			System.out.println("等待客户机的连接");
			client = server.accept();
			System.out.println("连接成功");
			String clientAddr = client.getRemoteSocketAddress().toString().substring(1);//需要跟下一句合并
			String[] clientAddrs = clientAddr.split(":");
			System.out.println("客户端IP：" + clientAddrs[0] + " 端口：" + clientAddrs[1]);

			reader = new BufferedReader(new InputStreamReader(client.getInputStream()));
			writer = new BufferedWriter(new OutputStreamWriter(client.getOutputStream()));

			controllerMessage();

			System.out.println("更多控制指令的详细解释请查阅《TCP-KP型网络继电器AT指令表.PDF》");
            System.out.println("本Demo仅用于开发参考，不能直接用于商业项目实施！");
		}catch(Exception e) {
			e.printStackTrace();
		}
	}

	private void controllerMessage() {

		try {
			System.out.println("TCP-KP系列网络继电器功能测试：");

			System.out.println("控制继电器通道1常开吸合发送：AT+STACH1=1\\r\\n");
			String msg = "AT+STACH1=1\r\n";
			//发送数据
			writer.write(msg);
			writer.flush();
			//读数据
			msg = reader.readLine();
			msg = msg.replace("\r\n", "\\r\\n");
			System.out.println("网络继电器应答：" + msg);
			System.out.println("指令执行成功!");
			Thread.sleep(500);
			
			//===========================================================
			System.out.println("控制继电器通道1常开断开发送：AT+STACH1=0\\r\\n");
			msg = "AT+STACH1=0\r\n";
			//发送数据
			writer.write(msg);
			writer.flush();
			//读数据
			msg = reader.readLine();
			msg = msg.replace("\r\n", "\\r\\n");
			System.out.println("网络继电器应答：" + msg);
			System.out.println("指令执行成功!");
			Thread.sleep(500);

			//===========================================================
			System.out.println("控制继电器通道1常开接口吸合15秒后断开发送：AT+STACH1=1,15\\r\\n");
			msg = "AT+STACH1=1,15\r\n";
			//发送数据
			writer.write(msg);
			writer.flush();
			//读数据
			msg = reader.readLine();
			msg = msg.replace("\r\n", "\\r\\n");
			System.out.println("网络继电器应答：" + msg);
			System.out.println("指令执行成功!");
			Thread.sleep(500);

			//===========================================================
			System.out.println("获取开关量输入接口1的状态发送：AT+OCCH1=?\\r\\n");
			msg = "AT+OCCH1=?\r\n";
			//发送数据
			writer.write(msg);
			writer.flush();
			//读数据
			msg = reader.readLine();
			msg = msg.replace("\r\n", "\\r\\n");
			System.out.println("网络继电器应答：" + msg);
			System.out.println("指令执行成功!");
			Thread.sleep(500);

		}catch(Exception e) {
			e.printStackTrace();
		}

		try {
			System.out.println("关闭客户端连接！");
			if(reader != null) reader.close();
			if(client != null) client.close();
			if(writer != null) writer.close();
		}catch(IOException e) {
			e.printStackTrace();
		}
		System.out.println("测试完成！");
	}

	public static void main(String[] args) {
		TcpServer tcp = new TcpServer();
		tcp.TcpServerDemo();
	}

}