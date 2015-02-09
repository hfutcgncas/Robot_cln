using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;




using PCOMMSERVERLib;
using System.Threading;

namespace PmacWPFTest
{

    delegate void Delegate_ShowMsg(string msg);
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    /// 
    public partial class MainWindow : Window
    {
        //pmac 相关变量       
        public static PCOMMSERVERLib.PmacDeviceClass Pmac;
        public int m_nDevice, m_nMotor, m_nServo, m_nDPRBase, m_nDPRSize;
        public bool m_bDeviceOpen, m_bDPRAvailable, m_bTurbo;

        cReceiver visionReceiver;

        public MainWindow()
        {
            InitializeComponent();

            Pmac = new PmacDeviceClass();

            SetAllEvents();

            m_bDeviceOpen = false;

            

        }

        void MainWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            
            if (m_bDeviceOpen)
            {
                // Call ClosePmacDevice()
               Pmac.Close(m_nDevice);
               m_bDeviceOpen = false;
            }

            MessageBox.Show("Closing called");

        }
    

        private void button_Click(object sender, RoutedEventArgs e)
        {
            int dev = m_nDevice;
            bool bSuccess = false;
            string str;

            string type;

            Pmac.SelectDevice(0, out dev, out bSuccess);
            if (bSuccess)
            {
                //改变面板状态
                button_Sev.IsEnabled = true;
                m_bDeviceOpen = true;
                textBlock_ConSta.Text = "CONNECTED";

                if (m_bDeviceOpen)
                    Pmac.Close(m_nDevice);
                m_nDevice = dev;
                m_bDPRAvailable = false;
                m_nDPRBase = 0;
                m_nDPRSize = 0;
                Pmac.Open(m_nDevice, out m_bDeviceOpen);
                if (m_bDeviceOpen)
                {
                    m_bDPRAvailable = Pmac.get_DPRAvailable(m_nDevice);
                    m_nDPRSize = Pmac.get_DPRSize(m_nDevice);
                    Pmac.DPRAddressRange(m_nDevice, false, false, out m_nDPRBase);
                    m_bTurbo = Pmac.get_IsTurbo(m_nDevice);
                   // m_bFGEnabled = Pmac.get_DPRMotorRptEnabled(m_nDevice);
                    // Setup for this device
                    if (m_bTurbo)
                    {
                        type = "Turbo";
                    }
                    else
                    {
                        type = "NO Turbo";
                    }
                    str = string.Format("Device number {0:D} opened successfully."+type,m_nDevice);
                 }
                else
                {
                    button_Sev.IsEnabled = false;
                    // Disable all required
                    str = string.Format("Device number {0:D} failed to open.", m_nDevice);
                }
                textBlock_ConSta.Text = str;
            }
        }

        private void BT_SendCMD_Click(object sender, RoutedEventArgs e)
        {
            string cmd = TB_cmdIn.GetLineText(0);
            string Rt;
            int Statue;

            Pmac.GetResponseEx(m_nDevice, cmd, true, out Rt, out Statue);

            TB_rtCMD.Text = Rt;

        }

        private void button_Sev_Click(object sender, RoutedEventArgs e)
        {
            string cmd = "J\\";
            string Rt;
            int Statue;

            Pmac.GetResponseEx(m_nDevice, cmd, true, out Rt, out Statue);
            TB_rtCMD.Text = Rt;

            button_HM.IsEnabled = true;
        }

        private void slider_Pos_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            string cmd = "P1 = ";
            string Rt;
            int Statue;

            cmd += ((int)slider_Pos.Value).ToString();

            Pmac.GetResponseEx(m_nDevice, cmd, true, out Rt, out Statue);

            Pmac.GetResponseEx(m_nDevice, "P1", true, out Rt, out Statue);
            TB_rtCMD.Text = Rt;
        }

        private void BT_comm2Vision_Click(object sender, RoutedEventArgs e)//开启打球的程序并与之通信
        {
            visionReceiver = new cReceiver();
            visionReceiver.initRcv();

            Thread RcvThread = new Thread(new ThreadStart(delegate
            {
                double x;
                double z;
                double v;
                double t;

                while (true)
                {
                    visionReceiver.ReciveData();
                    if(visionReceiver.hitPar[0] == 0)//收到错误的包
                    {
                        continue;
                    }

                    if(button_HM.IsEnabled)
                    {
                        x = visionReceiver.hitPar[1];
                        z = visionReceiver.hitPar[2];
                        v = visionReceiver.hitPar[3];
                        t = visionReceiver.hitPar[4];
                        setPadTargit(x, z, v, t); //给PMAC命令，让其打球
                    }
                   
                    recMsg.Dispatcher.Invoke(new Action(delegate
                    {
                        recMsg.Text = " X  = " + visionReceiver.hitPar[1].ToString() + 
                                    "\n Z  = " + visionReceiver.hitPar[2].ToString() +
                                    "\n Vx = " + visionReceiver.hitPar[3].ToString() +
                                    "\n Vy = " + visionReceiver.hitPar[4].ToString() +
                                    "\n Vz = " + visionReceiver.hitPar[5].ToString() +
                                    "\n tg = " + visionReceiver.hitPar[12].ToString() ;
                        
                    }), null);            
                    //Thread.Sleep(5);
                }         
            }
            ));
            RcvThread.Start();
        }

        //----------------------------------------------
        //注册PMAC的各种事件
        private void SetAllEvents()
        {
            if (Pmac != null)
            {
                _IPmacDeviceEvents_UnsolicitedEventHandler DUnsolicitedEventE
                 = new _IPmacDeviceEvents_UnsolicitedEventHandler(OnUnsolicited);
                Pmac.Unsolicited += DUnsolicitedEventE;
                _IPmacDeviceEvents_ProgressEventHandler DProgressEventE
                = new _IPmacDeviceEvents_ProgressEventHandler(OnProgress);
                Pmac.Progress += DProgressEventE;
                _IPmacDeviceEvents_MessageEventHandler DMessageEventE
                = new _IPmacDeviceEvents_MessageEventHandler(OnMessage);
                Pmac.Message += DMessageEventE;
                _IPmacDeviceEvents_ErrorEventHandler DErrorEventE
              = new _IPmacDeviceEvents_ErrorEventHandler(OnError);
                Pmac.Error += DErrorEventE;
                _IPmacDeviceEvents_InterruptEventHandler DInterruptEventE
                = new _IPmacDeviceEvents_InterruptEventHandler(OnInterrupt);
                Pmac.Interrupt += DInterruptEventE;
            }

        }

        // An unsolicited event from the server
        void OnUnsolicited(int device, String Text)
        {
            textBlock_Log.Text = Text;
        }
        // A progress update from the server
        void OnProgress(int device, int percent)
        {
            PB_OnRunning.Value = percent;
        }
        // A message from the server
        void OnMessage(int device, String Text, bool linefeed)
        {
            textBlock_Log.Text = Text;
        }
        // A error message from the server
        void OnError(int device, String filename, int errorNumber, int lineNumber, String msg)
        {
            string str;
            str = string.Format("{0} {1:D}, Line:{ 2:D},{ 4} { 3} { 4} ",filename,errorNumber,lineNumber,msg,'"') ;

            textBlock_Log.Text = str;
        }
        // A interrupt message from the server
        void OnInterrupt(int device, int interruptLevel)
        {
            String msg = "";
            switch (interruptLevel)
            {
                case 1:
                    msg = "** Interrupt-> In Position **"; break;
                case 2:
                    msg = "** Interrupt-> Buffer Request **"; break;
                case 4:
                    msg = "** Interrupt-> Warning Following Error **"; break;
                case 8:
                    msg = "** Interrupt-> Fatal Following Error **"; break;
                case 16:
                    msg = "** Interrupt-> Host Request **"; break;
                case 32:
                    msg = "** Interrupt-> IR5 **"; break;
                case 64:
                    msg = "** Interrupt-> IR6 **"; break;
                case 128:
                    msg = "** Interrupt-> IR7 **"; break;
                default: msg = "** Interrupt-> Spurious **"; break;
            }
            textBlock_Log.Text = msg;
        }

        bool setPadTargit(double x,double z,double v,double t)
        {
            string cmd;
            string Rt;
            int Statue;

            cmd = "P1 = " + x.ToString();
            Pmac.GetResponseEx(m_nDevice, cmd, true, out Rt, out Statue);

            cmd = "P2 = " + z.ToString();
            Pmac.GetResponseEx(m_nDevice, cmd, true, out Rt, out Statue);

            cmd = "P3 = " + v.ToString();
            Pmac.GetResponseEx(m_nDevice, cmd, true, out Rt, out Statue);

            cmd = "P4 = " + t.ToString();
            Pmac.GetResponseEx(m_nDevice, cmd, true, out Rt, out Statue);
         
            return false;
        }
    }
}
