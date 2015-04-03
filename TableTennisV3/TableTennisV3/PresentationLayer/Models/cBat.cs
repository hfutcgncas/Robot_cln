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

        public cBat()
        {
            Hit_X = 0;
            Hit_Y = 0;
            Hit_Z = 0;

//            ZSP_Interface = new cUDPServer();
//            ZSP_Interface.initRcv();
            pmac_card = new cPMAC();
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
                hit_x = value;
                if (pmac_card != null && pmac_card.m_bDeviceOpen)
                {
                    Set_P("p1", value);
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
                hit_y = value;
                if (pmac_card != null && pmac_card.m_bDeviceOpen)
                {
                    Set_P("p2", value);
                }
            }
        }
        //--------------
        public int Hit_Z { get; set; }
        public int Hit_S { get; set; }
        public int Hit_P { get; set; }
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

        #endregion
        //    private cUDPServer ZSP_Interface;
        //----------------------------------------
        //----------------------------------------
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

        public bool Set_P(string px, int value)
        {
            pmac_card.m_PMAC_cmd = px + "=" + value.ToString();
            pmac_card.SendCMD();
            return false;
        }
    }
}
