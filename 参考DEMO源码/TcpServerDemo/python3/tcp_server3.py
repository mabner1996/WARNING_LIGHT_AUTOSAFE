#coding:utf-8

from socket import *
from time import ctime
import time

#HOST = '192.168.1.122'
HOST = '192.168.0.100'
#HOST = '169.254.242.115'
PORT = 6000 #端口号 port
BUFSIZ = 1024 #接收数据缓冲大小 buffer size
ADDR = (HOST, PORT)

print('This will be the host server') #本机作为服务端
print('IP of this Host：') #本机IP：'
print(HOST)
print('Port：') #端口：
print(PORT)
tcpSerSock = socket(AF_INET, SOCK_STREAM) #创建TCP服务器套接字 create TCP server rocket
tcpSerSock.bind(ADDR) #套接字与地址绑定 bound socket to address
tcpSerSock.listen(5) #监听连接，同时连接请求的最大数目the maximum number of simultaneous connectoon request (how many can it listen at the same time)


print('waiting for client to connect') #等待客户机的连接
tcpCliSock, addr = tcpSerSock.accept()  #接收继电器端连接请求 receiver terminal connection request from relay
print('Connected successfully') #连接成功
print('IP and port of the client:')#客户端IP与端口如下:
print(addr)

print('TCP-KP Series Network Relay Functional Test：') #TCP-KP系列网络继电器功能测试：

print('Control relay channel 1 normally open pull-in send：AT+STACH1=1\\r\\n') #控制继电器通道1常开吸合发送：AT+STACH1=1\\r\\n
meg = "AT+STACH1=1\r\n"
tcpCliSock.send(meg.encode())
recv_data = tcpCliSock.recv(BUFSIZ)
print('The network relay networks') #网络继电器应答：
print(recv_data.decode('gbk'))
print('指令执行成功!')
time.sleep(0.5)#休眠0.5秒

#================================================
print('Control relay channel 1 to always switch on：AT+STACH1=0\\r\\n') #控制继电器通道1常开断开发送
meg = "AT+STACH1=0\r\n"
tcpCliSock.send(meg.encode())
recv_data = tcpCliSock.recv(BUFSIZ)
print('Relay responding: ') #网络继电器应答：
print(recv_data.decode('gbk'))
print('Command executed succesfully!') #指令执行成功!
time.sleep(0.5)#休眠0.5秒

#================================================
print('Control relay channel 1 to switch on 15 seconds, and then break and send：AT+STACH1=1,15\\r\\n') #控制继电器通道1常开接口吸合15秒后断开发送
meg = "AT+STACH1=1,15\r\n"
tcpCliSock.send(meg.encode())
recv_data = tcpCliSock.recv(BUFSIZ)
print('Relay responding: ') #网络继电器应答：
print(recv_data.decode('gbk'))
print('Command executed successfully!') #指令执行成功!
time.sleep(0.5)#休眠0.5秒

#================================================
print('Get the status of input port 1 of the switch：AT+OCCH1=?\\r\\n') #获取开关量输入接口1的状态发送
meg = "AT+OCCH1=?\r\n"
tcpCliSock.send(meg.encode())
recv_data = tcpCliSock.recv(BUFSIZ)
print('Relay responding:') #网络继电器应答：
#print(recv_data.replace("\r\n", "\\r\\n").decode('gbk'))
print(recv_data.decode('gbk'))
print('Command executed successfully!') #指令执行成功!
time.sleep(0.5)#休眠0.5秒
  
print('Closing the client connection!') #关闭客户端连接！
tcpCliSock.close() #Close the connection with relay (关闭与继电器的连接)
tcpSerSock.close() #Close the server socket (关闭服务器socket)
print('Test finished') #测试完成！

print('更多控制指令的详细解释请查阅《AT指令表.PDF》')
print('本Demo仅用于开发参考，不能直接用于商业项目实施！')
