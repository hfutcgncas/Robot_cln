using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace PmacWPFTest
{
    class cReceiver
    {
        Socket newsock;
        EndPoint Remote;
        int recv;

        public double[] hitPar;

        byte[] data = new byte[5000];

        public void initRcv()
        {    
            
            IPEndPoint ipep = new IPEndPoint(IPAddress.Any, 7000);
            newsock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            newsock.Bind(ipep);
            //----------------------------------------------------------------
            IPEndPoint sender = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 7000);
            Remote = (EndPoint)sender;

            hitPar = new double[13];
        }

        public void ReciveData()
        {
            recv = newsock.ReceiveFrom(data, ref Remote);
            
            for (int i = 0; i < 13; i++)
            {
                hitPar[i] = BitConverter.ToDouble(data, i*8);    
            }
            return;

           // 
        }
    }
}
