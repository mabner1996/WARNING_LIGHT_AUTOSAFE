using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Net.NetworkInformation;
using System.Text;

namespace KPManagementSoft
{
    public class CPublic
    {
      
        /// <summary>
        /// 判断字符串是否为数值型
        /// </summary>
        /// <param name="oText"></param>
        /// <returns></returns>
        static public bool IsNumberic(string oText)
        {
            try
            {
                int var1 = Convert.ToInt32(oText);
                return true;
            }
            catch
            {
                return false;
            }
        }
       
        /// <summary>
        /// 查看TCP与UDP端口是否被占用
        /// </summary>
        /// <param name="tempPort">端口号</param>
        /// <returns>如果端口被占用，即返回true，没有被占用即返回false</returns>
        public static bool CheckPort(string tempPort)
        {
            Process p = new Process();
            p.StartInfo = new ProcessStartInfo("netstat", "-an");
            p.StartInfo.CreateNoWindow = true;
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            p.StartInfo.RedirectStandardOutput = true;
            p.Start();
            string result = p.StandardOutput.ReadToEnd().ToLower();//最后都转换成小写字母
            System.Net.IPAddress[] addressList = Dns.GetHostByName(Dns.GetHostName()).AddressList;//
            List<string> ipList = new List<string>();
            ipList.Add("127.0.0.1");//====
            ipList.Add("0.0.0.0");
            for (int i = 0; i < addressList.Length; i++)
            {
                ipList.Add(addressList[i].ToString());
            }
            bool use = false;
            for (int i = 0; i < ipList.Count; i++)
            {
                if (result.IndexOf("tcp    " + ipList[i] + ":" + tempPort) >= 0 || result.IndexOf("udp    " + ipList[i] + ":" + tempPort) >= 0)
                {
                    use = true;
                    break;
                }
            }
            p.Close();
            return use;
        }
    }
}
