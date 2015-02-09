 
#include "stdio.h"
#include "iostream"
#include "windows.h"
#include "Rs232.h"

using namespace std;

HANDLE 	hCom;
char strRcv[100];

bool SerialCommInit()
{
	 	hCom=CreateFile("COM1",//COM1��
		GENERIC_READ|GENERIC_WRITE, //�������д
		0, //��ռ��ʽ
		NULL,
		OPEN_EXISTING, //�򿪶����Ǵ���
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, //�ص���ʽ
		NULL);
	if(hCom==(HANDLE)-1)
	{
		//AfxMessageBox("��COMʧ��!");
		cout<<"��COMʧ��!"<<endl;
		return false;
	}

	SetupComm(hCom,100,100); //���뻺����������������Ĵ�С����100

	COMMTIMEOUTS TimeOuts;
	//�趨����ʱ
	TimeOuts.ReadIntervalTimeout=MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier=0;
	TimeOuts.ReadTotalTimeoutConstant=0;
	//�ڶ�һ�����뻺���������ݺ���������������أ�
	//�������Ƿ������Ҫ����ַ���

	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier=100;
	TimeOuts.WriteTotalTimeoutConstant=500;
	SetCommTimeouts(hCom,&TimeOuts); //���ó�ʱ

	DCB dcb;
	GetCommState(hCom,&dcb);
	dcb.BaudRate=9600; //������Ϊ9600
	dcb.ByteSize=8; //ÿ���ֽ���8λ
	dcb.Parity=NOPARITY; //����żУ��λ
//	dcb.StopBits=TWOSTOPBITS; //����ֹͣλ
	dcb.StopBits=ONESTOPBIT; //һ��ֹͣλ

	SetCommState(hCom,&dcb);
	PurgeComm(hCom,PURGE_TXCLEAR|PURGE_RXCLEAR);

	printf("Serial Port Initializes sucess.\n");
	return true;

}

char * SerialCommReadData()
{
		OVERLAPPED m_osRead;
	memset(&m_osRead,0,sizeof(OVERLAPPED));
	m_osRead.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	COMSTAT ComStat;
	DWORD dwErrorFlags;
	
	memset(strRcv,'\0',100);
	DWORD dwBytesRead=100;//��ȡ���ֽ���
	BOOL bReadStat;

	ClearCommError(hCom,&dwErrorFlags,&ComStat);
	dwBytesRead=min(dwBytesRead, (DWORD)ComStat.cbInQue);
	bReadStat=ReadFile(hCom,strRcv,
		dwBytesRead,&dwBytesRead,&m_osRead);
	if(!bReadStat)
	{
		if(GetLastError()==ERROR_IO_PENDING)
	    //GetLastError()��������ERROR_IO_PENDING,�����������ڽ��ж�����
		{
			WaitForSingleObject(m_osRead.hEvent,2000);
		    //ʹ��WaitForSingleObject�����ȴ���ֱ����������ɻ���ʱ�Ѵﵽ2����
		    //�����ڶ�����������Ϻ�m_osRead��hEvent�¼����Ϊ���ź�
		}
	}

	PurgeComm(hCom, PURGE_TXABORT|
		PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	return strRcv;
}

void SerialCommSendData(UCHAR *pSendData)
{
	//pSendData="AZM0E"

//ͨ��Э��
//������ʽA X1 X2 X3 E 
//A��E��ʼ�����ַ�
//X1��Z��S��P��
//X2��������C������ָ���ᵱǰλ�ã�T������ָ����Ŀ��λ�ã�M����ѯ��ǰ���״̬��
//AZCXE //����Z�ᵱǰλ�ã�����xΪ00~FF
//AZTXE //����Z��Ŀ��λ�ã�����xΪ00~FF
//AZM0E //��ѯZ��״̬


//	char lpOutBuffer[5];
//	memset(lpOutBuffer,'\0',7);
//	lpOutBuffer[0]='A';
//	lpOutBuffer[1]='z';
//	lpOutBuffer[2]='M';
//	lpOutBuffer[3]='0';

//	lpOutBuffer[6]='\x03';


	OVERLAPPED m_osWrite;
	memset(&m_osWrite,0,sizeof(OVERLAPPED));
	m_osWrite.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	
	DWORD dwBytesWrite = 5;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	ClearCommError(hCom,&dwErrorFlags,&ComStat);
	bWriteStat=WriteFile(hCom,pSendData,
		dwBytesWrite,& dwBytesWrite,&m_osWrite);

	if(!bWriteStat)
	{
		if(GetLastError()==ERROR_IO_PENDING)
		{
			WaitForSingleObject(m_osWrite.hEvent,1000);
		}
	}
}

/*

void main()
{
	if (false ==SerialCommInit()) 
	{
		cout<<"SerialCommInit false, quit the programm!"<<endl;
		system("pause");
		return;
	}

//send
	char lpOutBuffer[7];
	memset(lpOutBuffer,'\0',7);
	lpOutBuffer[0]='a';
	lpOutBuffer[1]='0';
	lpOutBuffer[2]='0';
	lpOutBuffer[3]='1';
	lpOutBuffer[4]='0';
	lpOutBuffer[5]='1';
	lpOutBuffer[6]='\n';

	Sleep(3000);

	SerialCommSendData(lpOutBuffer,7);
	SerialCommSendData(lpOutBuffer,7);
	SerialCommSendData(lpOutBuffer,7);

	Sleep(1000);

//read
	char rcvData[1024];
	for (int i=0; i < 1; i++)
	{
		strcpy(rcvData,SerialCommReadData());
		cout<<rcvData<<endl;
		Sleep(100);
	}
	
	system("pause");
}*/
