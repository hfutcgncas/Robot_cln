

#include<windows.h>
	#pragma once
	#define RS232_H
	#define RXQUEUE         4096����// ���ջ�������С
	#define TXQUEUE         1024    // ���ͻ�������С
	#define MAXBLOCK        1024

	
	class CRs232
	{
	public:  
		 CRs232();  
		 ~CRs232();  
		 BOOL IsConnected( ); // �жϴ����Ƿ��Ѿ��� 
		 int ReadCommBlock(LPVOID Buffer, int nMaxLength, DWORD dwWaitTime);// �Ӵ��ڶ�����
		 HANDLE m_hComDev;// �����豸���
		 OVERLAPPED m_osRead,m_osWrite;
		 HANDLE m_hThreadTerm;  // �¼����������ͨѶ�߳��Ƿ���ֹ��ͬ������
		 BOOL m_bConnected;//    �����Ƿ��Ѿ���
		 int OpenConnection(LPCTSTR szComPort ="COM1",DWORD dwBaudRate =9600,BYTE cBycByteSize=8,BYTE cStopBits=ONESTOPBIT,BYTE cParity=NOPARITY,BOOL bDTR_DSR=0,BOOL bRTS_CTS=0,BOOL bXON_XOFF=0);// �򿪲����ô���  
		 BOOL CloseConnection( );  // �رա��ͷŴ���  
		 int WriteCommBlock( LPCVOID lpByte, UINT nBytesToWrite); //������д����  

		 char * ReadString(char *szBuffer, DWORD dwBufferLength, DWORD dwWaitTime = 20);//���ַ���

	};