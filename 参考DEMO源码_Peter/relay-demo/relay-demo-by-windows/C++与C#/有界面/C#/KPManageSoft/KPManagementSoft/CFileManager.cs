using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace KPManagementSoft
{
    /// <summary>
    /// 每创建一个升级连接，即需要
    /// </summary>
    public class CFileManager
    {
        static object lockerCF = new object();
        /// <summary>
        /// 用于管理文件
        /// int为线程ID
        /// CFileManager为CFileManager类的实例
        /// </summary>
        static public Dictionary<int, CFileManager> FileManager = new Dictionary<int, CFileManager>();
        /// <summary>
        /// 软件路径
        /// </summary>
        static public string AppPath = null;

        /// <summary>
        /// 这是操作文件的字段
        /// </summary>
        public long fileLength = 0;//文件总长度
        public int totalFreamNum = 0;//总帧数
        public int remainFreamNum = 0;//剩余帧数，对应着协议的 NNNN
        public const int freamDataLength = 512;//516;
        public byte[] freamData = new byte[freamDataLength];
        

        /// <summary>
        /// 日志记录
        /// </summary>
        /// <param name="logStr"></param>
        static public void WriteLog(string logStr)
        {
            lock (lockerCF)
            {
                DirectoryInfo dir = new DirectoryInfo("log");
                dir.Create();//自行判断一下是否存在。

                string FileName = "log\\" + DateTime.Now.ToString("yyyy-MM-dd") + ".log";
                if (AppPath != null)
                {
                    FileName = AppPath + "\\" + FileName;
                }
                FileStream fs = new FileStream(FileName, FileMode.Append);//如果没有该文件即创建，如果存在，即打开并在末尾添加内容
                string str1 = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss:fff") + "   ";
                //获得字节数组
                Debug.Print(str1 + logStr + "\r\n");
                byte[] data = Encoding.Default.GetBytes(str1 + logStr + "\r\n");
                //开始写入
                fs.Write(data, 0, data.Length);
                //清空缓冲区、关闭流
                fs.Flush();
                fs.Close();
            }
        }
    }
}