using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;



using TableTennisV3.ToolResource;

namespace TableTennisV3.PresentationLayer.Models
{
    class cBat
    {
        public int XMax = 50000;
        public int XMin = -50000;

        public int YMax = 50000;
        public int YMin = -10000;

        public int ZMax = 165;
        public int ZMin = 80;

        public int SMax = 165;
        public int SMin = 80;

        public int PMax = 160;
        public int PMin = 110;


        public cBat()
        {
            Hit_X = 0;
            Hit_Y = 0;
            Hit_Z = 0;

            Hit_S = 127;
            Hit_P = 127;

            pmac_card  = new cPMAC();
            ZSP_Sender = new cTCPListenor();

            
        }
        #region Variable
        #region Hit Variables
        //--------------
        private int hit_x;
        public int Hit_X
        {
            get { return hit_x; }
            set
            {
                if (value > XMax) value = XMax;
                if (value < XMin) value = XMin;
                hit_x = value;
                if (pmac_card != null && pmac_card.m_bDeviceOpen)
                {
                    Set_PMAC_P("p1", hit_x);
                }
            }
        }
        //--------------
        private int hit_y;
        public int Hit_Y
        {
            get { return hit_y; }
            set
            {
                if (value > YMax) value = YMax;
                if (value < YMin) value = YMin;
                hit_y = value;
                if (pmac_card != null && pmac_card.m_bDeviceOpen)
                {
                    Set_PMAC_P("p2", hit_y);
                }
            }
        }
        //--------------
        private int hit_z;
        public int Hit_Z
        {
            get { return hit_z; }
            set
            {
                hit_z = value;
                if (ZSP_Sender != null && ZSP_Sender.isConected)
                {
                    ZSP_Sender.SendMsg("z" + hit_z.ToString() + "s" + hit_s.ToString() + "p" + hit_p.ToString() + "E");
                }
            }
        }

        private int hit_s;
        public int Hit_S
        {
            get { return hit_s; }
            set
            {
                if (value > SMax) value = SMax;
                if (value < SMin) value = SMin;
                hit_s = value;
                if (ZSP_Sender != null && ZSP_Sender.isConected)
                {
                    ZSP_Sender.SendMsg("z" + hit_z.ToString() + "s" + hit_s.ToString() + "p" + hit_p.ToString() + "E");
                }
            }
        }

        private int hit_p;
        public int Hit_P
        {
            get { return hit_p; }
            set
            {
                if (value > PMax) value = PMax;
                if (value < PMin) value = PMin;
                hit_p = value;
                if (ZSP_Sender != null && ZSP_Sender.isConected)
                {
                    ZSP_Sender.SendMsg("z" + hit_z.ToString() + "s" + hit_s.ToString() + "p" + hit_p.ToString() + "E");
                   // ZSP_Sender.WaitForReceive();
                }
            }
        }
        #endregion

        #region Realtime Variables
        public double Realtime_X { get; set; }
        public double Realtime_Y { get; set; }
        public double Realtime_Z { get; set; }
        public double Realtime_S { get; set; }
        public double Realtime_P { get; set; }
        #endregion

        #region PMAC Variables
        public cPMAC pmac_card;
        public string pmac_msg { get { return pmac_card.m_PMAC_msg; } }
        #endregion

        public cTCPListenor ZSP_Sender;

        #endregion
        //    private cUDPServer ZSP_Interface;
        //----------------------------------------
        //----------------------------------------
        #region Method
        public bool Home()
        {
            return false;
        }

        public bool MoveTo()
        {
      //      ZSP_Interface.ReciveData();
            byte[] ZSP_Target = new byte[4];
            ZSP_Target[0] = (byte)Hit_Z;
            ZSP_Target[1] = (byte)Hit_S;
            ZSP_Target[2] = (byte)Hit_P;

       //     ZSP_Interface.SendData(ZSP_Target);
            return false;
        }

        public bool HitBall()
        {
            return false;
        }

        public bool Set_PMAC_P(string px, int value)
        {
            pmac_card.m_PMAC_cmd = px + "=" + value.ToString();
            pmac_card.SendCMD();
            return false;
        }

        #endregion
    }
}
