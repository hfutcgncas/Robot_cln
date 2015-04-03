using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Sockets;

namespace TableTennisV3.ToolResource
{
    class cUDPServer
    {
        Socket newsock;
        EndPoint Remote;
        int recv;

        byte[] data = new byte[5000];

        public void initRcv()
        {

            IPEndPoint ipep = new IPEndPoint(IPAddress.Any, 6000);
            newsock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            newsock.Bind(ipep);
            //----------------------------------------------------------------
            IPEndPoint sender = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 6000);
            Remote = (EndPoint)sender;

        }

        public void ReciveData()
        {
            recv = newsock.ReceiveFrom(data,  ref Remote);
            return;
        }

        public void SendData(byte[] buffer)
        {
            newsock.SendTo(buffer, Remote);
            
        }

        ~cUDPServer()
        {
            newsock.Close();
        }

    }

}
