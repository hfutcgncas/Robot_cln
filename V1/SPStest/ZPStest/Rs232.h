

#include<windows.h>
	#pragma once
	#define RS232_H
	#define RXQUEUE         4096　　// 接收缓冲区大小
	#define TXQUEUE         1024    // 发送缓冲区大小
	#define MAXBLOCK        1024

	
	class CRs232
	{
	public:  
		 CRs232();  
		 ~CRs232();  
		 BOOL IsConnected( ); // 判断串口是否已经打开 
		 int ReadCommBlock(LPVOID Buffer, int nMaxLength, DWORD dwWaitTime);// 从串口读数据
		 HANDLE m_hComDev;// 串口设备句柄
		 OVERLAPPED m_osRead,m_osWrite;
		 HANDLE m_hThreadTerm;  // 事件句柄：控制通讯线程是否终止的同步对象
		 BOOL m_bConnected;//    串口是否已经打开
		 int OpenConnection(LPCTSTR szComPort ="COM1",DWORD dwBaudRate =9600,BYTE cBycByteSize=8,BYTE cStopBits=ONESTOPBIT,BYTE cParity=NOPARITY,BOOL bDTR_DSR=0,BOOL bRTS_CTS=0,BOOL bXON_XOFF=0);// 打开并配置串口  
		 BOOL CloseConnection( );  // 关闭、释放串口  
		 int WriteCommBlock( LPCVOID lpByte, UINT nBytesToWrite); //往串口写数据  

		 char * ReadString(char *szBuffer, DWORD dwBufferLength, DWORD dwWaitTime = 20);//读字符串

	};