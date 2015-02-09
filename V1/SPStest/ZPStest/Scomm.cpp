 
#include "stdio.h"
#include "iostream"
#include "windows.h"
#include "Rs232.h"

using namespace std;

HANDLE 	hCom;
char strRcv[100];

bool SerialCommInit()
{
	 	hCom=CreateFile("COM1",//COM1口
		GENERIC_READ|GENERIC_WRITE, //允许读和写
		0, //独占方式
		NULL,
		OPEN_EXISTING, //打开而不是创建
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, //重叠方式
		NULL);
	if(hCom==(HANDLE)-1)
	{
		//AfxMessageBox("打开COM失败!");
		cout<<"打开COM失败!"<<endl;
		return false;
	}

	SetupComm(hCom,100,100); //输入缓冲区和输出缓冲区的大小都是100

	COMMTIMEOUTS TimeOuts;
	//设定读超时
	TimeOuts.ReadIntervalTimeout=MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier=0;
	TimeOuts.ReadTotalTimeoutConstant=0;
	//在读一次输入缓冲区的内容后读操作就立即返回，
	//而不管是否读入了要求的字符。

	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier=100;
	TimeOuts.WriteTotalTimeoutConstant=500;
	SetCommTimeouts(hCom,&TimeOuts); //设置超时

	DCB dcb;
	GetCommState(hCom,&dcb);
	dcb.BaudRate=9600; //波特率为9600
	dcb.ByteSize=8; //每个字节有8位
	dcb.Parity=NOPARITY; //无奇偶校验位
//	dcb.StopBits=TWOSTOPBITS; //两个停止位
	dcb.StopBits=ONESTOPBIT; //一个停止位

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
	DWORD dwBytesRead=100;//读取的字节数
	BOOL bReadStat;

	ClearCommError(hCom,&dwErrorFlags,&ComStat);
	dwBytesRead=min(dwBytesRead, (DWORD)ComStat.cbInQue);
	bReadStat=ReadFile(hCom,strRcv,
		dwBytesRead,&dwBytesRead,&m_osRead);
	if(!bReadStat)
	{
		if(GetLastError()==ERROR_IO_PENDING)
	    //GetLastError()函数返回ERROR_IO_PENDING,表明串口正在进行读操作
		{
			WaitForSingleObject(m_osRead.hEvent,2000);
		    //使用WaitForSingleObject函数等待，直到读操作完成或延时已达到2秒钟
		    //当串口读操作进行完毕后，m_osRead的hEvent事件会变为有信号
		}
	}

	PurgeComm(hCom, PURGE_TXABORT|
		PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	return strRcv;
}

void SerialCommSendData(UCHAR *pSendData)
{
	//pSendData="AZM0E"

//通信协议
//基本格式A X1 X2 X3 E 
//A，E开始结束字符
//X1：Z，S，P轴
//X2：操作：C：设置指定轴当前位置，T：设置指定轴目标位置，M：查询当前轴的状态。
//AZCXE //设置Z轴当前位置，其中x为00~FF
//AZTXE //设置Z轴目标位置，其中x为00~FF
//AZM0E //查询Z轴状态


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
