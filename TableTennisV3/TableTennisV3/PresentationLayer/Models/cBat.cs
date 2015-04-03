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
        }
        #region Hit Variables
        public int Hit_X { get; set; }
        public int Hit_Y { get; set; }
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

    //    private cUDPServer ZSP_Interface;
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
    }
}
