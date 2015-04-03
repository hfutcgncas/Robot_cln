using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

//using System.ComponentModel;
//using System.Data;
//using System.Drawing;

using System.IO;
using System.Net.Sockets;
using System.Threading;
using System.Net;



namespace TableTennisV3.ToolResource
{
    class cTCPIPServer
    {
        //    private static Socket ConnectSocket( int port)
        //    {
        //        Socket s = null;

        //        IPAddress address = new IPAddress(new byte[] { 127, 0, 0, 1 });

        //        IPEndPoint ipe = new IPEndPoint(address, port);
        //        Socket tempSocket =  new Socket(ipe.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

        //       tempSocket.Connect(ipe);

        //        if (tempSocket.Connected)
        //        {
        //           s = tempSocket;
        //        }

        //        return s;
        //    }
        //    //
        //    public void sendmymsg()
        //    {
        //        Socket s = ConnectSocket(5567);

        //        byte[] msg = Encoding.UTF8.GetBytes("This is a test");
        //        byte[] bytes = new byte[256];
        //        try
        //        {
        //            // Blocks until send returns. 
        //            int byteCount = s.Send(msg, SocketFlags.None);
        //            Console.WriteLine("Sent {0} bytes.", byteCount);

        //            // Get reply from the server.
        //            byteCount = s.Receive(bytes, SocketFlags.None);
        //            if (byteCount > 0)
        //                Console.WriteLine(Encoding.UTF8.GetString(bytes));
        //        }
        //        catch (SocketException e)
        //        {
        //            Console.WriteLine("{0} Error code: {1}.", e.Message, e.ErrorCode);
        //        }
        //    }

        public static void StartClient()
        {
            // Data buffer for incoming data.
            byte[] bytes = new byte[1024];

            // Connect to a remote device.
            try
            {
                // Establish the remote endpoint for the socket.
                // This example uses port 11000 on the local computer.
                IPAddress ipAddress = new IPAddress(new byte[] { 127, 0, 0, 1 }); ;
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, 5567);

                // Create a TCP/IP  socket.
                Socket sender = new Socket(AddressFamily.InterNetwork,
                    SocketType.Stream, ProtocolType.Tcp);

                // Connect the socket to the remote endpoint. Catch any errors.
                try
                {
                    sender.Connect(remoteEP);

                    Console.WriteLine("Socket connected to {0}",
                        sender.RemoteEndPoint.ToString());

                    // Encode the data string into a byte array.
                    byte[] msg = Encoding.ASCII.GetBytes("This is a test<EOF>");

                    // Send the data through the socket.
                    int bytesSent = sender.Send(msg);

                    //// Receive the response from the remote device.
                    //int bytesRec = sender.Receive(bytes);
                    //Console.WriteLine("Echoed test = {0}",
                    //    Encoding.ASCII.GetString(bytes, 0, bytesRec));

                    // Release the socket.
                    sender.Shutdown(SocketShutdown.Both);
                    sender.Close();

                }
                catch (ArgumentNullException ane)
                {
                    Console.WriteLine("ArgumentNullException : {0}", ane.ToString());
                }
                catch (SocketException se)
                {
                    Console.WriteLine("SocketException : {0}", se.ToString());
                }
                catch (Exception e)
                {
                    Console.WriteLine("Unexpected exception : {0}", e.ToString());
                }

            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }
    }


   
}
