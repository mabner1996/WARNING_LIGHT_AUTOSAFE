using System;
using System.Collections.Generic;
using System.Linq;
//using System.Threading.Tasks;
using System.Windows.Forms;

namespace KPManagementSoft
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {

            try
            {
                #if false
                // 启动 
                SplashScreen.ShowSplashScreen();
                // 进行自己的操作：加载组件，加载文件等等 
                // 示例代码为休眠一会 
                System.Threading.Thread.Sleep(3000);
                // 关闭 
                if (SplashScreen.Instance != null)
                {
                    SplashScreen.Instance.BeginInvoke(new MethodInvoker(SplashScreen.Instance.Dispose));
                    SplashScreen.Instance = null;
                }
                #endif
 
                bool ret;
                System.Threading.Mutex mutex = new System.Threading.Mutex(true, Application.ProductName, out ret);

                if (ret)
                {
                    Application.EnableVisualStyles();
                    Application.SetCompatibleTextRenderingDefault(false);
                    Application.DoEvents();
                    Application.Run(new FrmMain());
                    mutex.ReleaseMutex();
                }
                else
                {
                    MessageBox.Show(null, "请不要同时运行多个程序", Application.ProductName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    Application.Exit();
                }
            }
            catch
            {
                Application.Exit();
            }
        }
    }
}
