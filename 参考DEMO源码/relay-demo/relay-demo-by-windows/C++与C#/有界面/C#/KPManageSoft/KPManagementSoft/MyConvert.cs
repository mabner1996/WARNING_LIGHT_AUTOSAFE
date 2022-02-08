using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace KPManagementSoft
{
    public class MyConvert
    {
        /// <summary>
        /// 字符串转化byte数组
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        static public byte[] StringToByteArray(string str)
        {
            return System.Text.Encoding.Default.GetBytes(str);
        }

        /// <summary>
        /// byte数据，转化为字符串
        /// </summary>
        /// <param name="byteArray"></param>
        /// <returns></returns>
        static public string ByteArrayToString(byte[] byteArray)
        {
            return System.Text.Encoding.Default.GetString(byteArray);
        }
        /// <summary>
        /// byte数据，转化为字符串
        /// </summary>
        /// <param name="byteArray"></param>
        /// <param name="len">需要转换的字节数</param>
        /// <returns></returns>
        static public string ByteArrayToString(byte[] byteArray, int len)
        {
            byte[] buf = new byte[len];
            for (int i = 0; i < len; i++)
            {
                buf[i] = byteArray[i];
            }
            return System.Text.Encoding.Default.GetString(buf);
        }

        /// <summary>
        /// 将字符串转为Ascii 的十六进制
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        static public byte[] StringToAsciiByte(string str)
        {
            return System.Text.Encoding.ASCII.GetBytes(str);
        }

        /// <summary>
        /// 将Ascii的十六进制数组转化为字符串
        /// </summary>
        /// <param name="byteArray"></param>
        /// <returns></returns>
        static public string AsciiByteToString(byte[] byteArray)
        {
            return System.Text.Encoding.ASCII.GetString(byteArray);
        }
        /// <summary>
        /// 将Ascii的十六进制数组转化为字符串
        /// </summary>
        /// <param name="byteArray"></param>
        /// <param name="len">需要转换的字节数</param>
        /// <returns></returns>
        static public string AsciiByteToString(byte[] byteArray,int len)
        {
            byte[] buf = new byte[len];
            for(int i=0;i<len;i++)
            {
                buf[i] = byteArray[i];
            }
            return System.Text.Encoding.ASCII.GetString(buf);
        }

        /// <summary>
        /// 将字符串转化为定长字节数组
        /// </summary>
        /// <param name="str">待转化的字符串</param>
        /// <param name="len"></param>
        /// <returns></returns>
        static public byte[] StringToFixedLengthByteArray(string str,int len)
        {
            byte[] temp = StringToByteArray(str);
            if (temp.Length >= len)
            {
                byte[] buf = new byte[len];
                Array.Copy(temp, buf, len-1);
                buf[len - 1] = 0;
                return buf;
            }
            else
            {
                byte[] buf = new byte[temp.Length+1];
                Array.Copy(temp, buf, temp.Length);
                buf[temp.Length] = 0;
                return buf;
            }
        }

    }
}
