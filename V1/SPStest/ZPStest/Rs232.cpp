#include  "StdAfx.h"
#include  "rs232.h"
#include <Windows.h> 


CRs232::CRs232(void)
{

 memset(  &m_osWrite,  0,  sizeof(OVERLAPPED)  )  ;
 memset(  &m_osRead,  0,  sizeof(OVERLAPPED)  )  ;
 m_bConnected  =  FALSE;

 m_hThreadTerm  =  CreateEvent(  NULL,  FALSE,  FALSE,  NULL  );  
 m_osRead.hEvent  =  CreateEvent(  NULL,  TRUE,  FALSE,  NULL  );   
 m_osWrite.hEvent  =  CreateEvent(  NULL,  TRUE,  FALSE,  NULL  );  

 }

CRs232::~CRs232(void)
{
 if(  m_bConnected  )          //    如果已打开串口
     CloseConnection();      //    关闭、释放串口
	CloseHandle(  m_hThreadTerm  );  
	CloseHandle(  m_osRead.hEvent)  ;
	CloseHandle(  m_osWrite.hEvent)  ;
}
int  CRs232::OpenConnection(  LPCTSTR  szComPort,DWORD  dwBaudRate,BYTE  cByteSize,    
            BYTE  cStopBits,BYTE  cParity,BOOL  bDTR_DSR,BOOL  bRTS_CTS,BOOL  bXON_XOFF  )
{
 BOOL e_InitializeComm =TRUE;
 BOOL e_OpenComm =FALSE;
 if(  m_bConnected  )        //  已经打开  
 return  0;    
 SetEvent(m_osWrite.hEvent);   
    m_hComDev  =  CreateFile(  szComPort,  GENERIC_READ  |  GENERIC_WRITE,  0,  NULL,  OPEN_EXISTING,  FILE_FLAG_OVERLAPPED,  NULL  );
 if(  m_hComDev  ==  INVALID_HANDLE_VALUE  )  
  return  e_OpenComm;    //  返回不能打开串口错误代码
    DCB  dcb;
    dcb.DCBlength  =  sizeof(DCB);    
 GetCommState(m_hComDev,&dcb  );        
 
 dcb.BaudRate  =  dwBaudRate;   
 dcb.ByteSize  =  cByteSize;  
 dcb.Parity  =  cParity;     
 dcb.StopBits  =  ONESTOPBIT;      
 //  硬件流控制.  bDTR_DSR、bRTS_CTS一般取默认值0，有Modem时应取1  
 dcb.fOutxDsrFlow  =  (  bDTR_DSR!=0);    //    
 if(bDTR_DSR)    
  dcb.fDtrControl  =  DTR_CONTROL_HANDSHAKE  ;    //  
 else    
  dcb.fDtrControl  =  DTR_CONTROL_ENABLE  ;
      dcb.fOutxCtsFlow   =  (bRTS_CTS!=0)  ;  
        if  (bRTS_CTS)    
  dcb.fRtsControl  =  RTS_CONTROL_HANDSHAKE  ;  
 else    
  dcb.fRtsControl  =  RTS_CONTROL_ENABLE  ;
      dcb.fInX  =  dcb.fOutX  =  (bXON_XOFF!=0);    //  软件流控制  
        dcb.fBinary  =  TRUE  ;    //   
      dcb.fParity  =  TRUE  ;    
      if(  SetCommState(m_hComDev,&dcb)  )  
        {    
  //  配置串口参数成功    
  SetCommMask(m_hComDev,EV_RXCHAR);
//  设置接收、发送缓冲区大小    
  SetupComm(m_hComDev,4096,4096);
  COMMTIMEOUTS    CommTimeOuts  ; 
  CommTimeOuts.ReadIntervalTimeout  =  MAXDWORD;        
  CommTimeOuts.ReadTotalTimeoutMultiplier  =  0  ;   
  CommTimeOuts.ReadTotalTimeoutConstant  =  10;        
  CommTimeOuts.WriteTotalTimeoutMultiplier  =  0;        
  CommTimeOuts.WriteTotalTimeoutConstant  =  0;       
 SetCommTimeouts(  m_hComDev,  &CommTimeOuts  );   
        m_bConnected  =  TRUE  ;    
   return  TRUE; 
 
 }  
 m_bConnected  =  FALSE  ;
 CloseHandle(  m_hComDev  )  ;  
 return FALSE;
}

int  CRs232::ReadCommBlock(  LPVOID Buffer,  int  nMaxLength , DWORD dwWaitTime = 10)
{  
 /*
 COMSTAT  ComStat;    //  串口状态结构  
  DWORD  dwLength,  dwErrorFlags;  
  ClearCommError(m_hComDev,&dwErrorFlags,&ComStat); 
  dwLength = min((DWORD)  nMaxLength,  ComStat.cbInQue);  //    将要读取的数据字节数
     //dwLength = nMaxLength;
  if(dwLength> 0  )  
  {  
     if(  !ReadFile(m_hComDev, lpszBlock,dwLength,&dwLength,&m_osRead))    //  从串口读入数据      
         if(  GetLastError()  ==  ERROR_IO_PENDING)       
         if(  !GetOverlappedResult(m_hComDev,  &m_osRead,  &dwLength,  TRUE)  )  //  挂起，直到操作完成        
         ClearCommError(  m_hComDev,  &dwErrorFlags,  &ComStat  );    
  }
  return  dwLength;    //  返回已读入的数据字节数  */
/*
if( m_hComDev == NULL ) return 0;

BOOL bReadStatus;
DWORD dwErrorFlags;
COMSTAT ComStat;
DWORD  dwBytesRead;
ClearCommError( m_hComDev, &dwErrorFlags, &ComStat );
if( !ComStat.cbInQue ) return 0;

	dwBytesRead = min(nMaxLength,(DWORD) ComStat.cbInQue);

	bReadStatus = ReadFile( m_hComDev, lpszBlock, dwBytesRead, &dwBytesRead, &m_osRead );
	if( !bReadStatus )
	{
		if( GetLastError() == ERROR_IO_PENDING ){
		WaitForSingleObject( m_osRead.hEvent, 2000 );
		return dwBytesRead;
	}
	return 0;
}
return dwBytesRead;
*/
if( m_hComDev == NULL ) return 0;

		COMSTAT  Stat;
		DWORD dwError;
		
		if(::ClearCommError(m_hComDev, &dwError, &Stat) && dwError > 0)
		{
			::PurgeComm(m_hComDev, PURGE_RXABORT | PURGE_RXCLEAR);
			return 0;
		}
		
		if(!Stat.cbInQue) // 缓冲区无数据
			return 0;
		
		unsigned long uReadLength = 0;
		DWORD dwBufferLength;
		dwBufferLength =(DWORD)nMaxLength > Stat.cbInQue ? Stat.cbInQue : (DWORD)nMaxLength;
		
		if(!::ReadFile(m_hComDev, Buffer, dwBufferLength, &uReadLength, &m_osRead)) 
		{
			if(::GetLastError() == ERROR_IO_PENDING) 
			{
				WaitForSingleObject(m_osRead.hEvent, dwWaitTime); // 结束异步I/O
				if(!::GetOverlappedResult(m_hComDev, &m_osRead, &uReadLength, false))
				{
					if(::GetLastError() != ERROR_IO_INCOMPLETE)
						uReadLength = 0;
				}
			}
			else
				uReadLength = 0;
		}
		
		return uReadLength;

} 
int CRs232::WriteCommBlock(  LPCVOID  lpByte,  UINT  nBytesToWrite)
{  
 int e_NotConnected =0;
 int  e_TransmitError =0;
 int e_TransmitNotFinished =0;
 DWORD   dwBytesWritten;
 //DWORD	 dwErrorFlags; 
// COMSTAT  ComStat;
     if(  !m_bConnected  )  //  串口没有打开      
       return  e_NotConnected;  
        if(WaitForSingleObject(m_osWrite.hEvent,0)  !=  WAIT_OBJECT_0)      
       return  e_TransmitNotFinished;    
        ResetEvent(m_osWrite.hEvent);  
 if(!WriteFile(m_hComDev,lpByte,nBytesToWrite,&dwBytesWritten,&m_osWrite))
 {
/*
      //  往串口发送数据
      if(  GetLastError()  ==  ERROR_IO_PENDING  )     
       return  0;    //  正在后台发送    
    	ClearCommError(  m_hComDev,  &dwErrorFlags,  &ComStat  )  ;  //  出现错误，清除错误标志  
        return    e_TransmitError;    //  发送出错  */
    
  if(  GetLastError()  ==  ERROR_IO_PENDING  ) 
  {
	WaitForSingleObject( m_osWrite.hEvent, 1000 );
	return dwBytesWritten;
  }
	return 0;
 }  
 //return  0;  //  WriteFile返回１,发送成功}
 return dwBytesWritten;
}
BOOL CRs232::CloseConnection( )
{ 
 if( !m_bConnected ) 
  return FALSE;  
 SetEvent(m_hThreadTerm); 
 m_bConnected = FALSE;  
 SetCommMask( m_hComDev, 0 );  
 
 EscapeCommFunction(m_hComDev,CLRDTR );
 CloseHandle(m_hComDev);//  关闭、释放串口 
 return ( TRUE ) ;
}

BOOL CRs232::IsConnected( )
{
 if( !m_bConnected ) 
  return FALSE;
 else 
 return TRUE;
}



char * CRs232::ReadString(char *szBuffer, DWORD dwBufferLength, DWORD dwWaitTime)
{
		unsigned long uReadLength = ReadCommBlock(szBuffer, dwBufferLength - 1, dwWaitTime);
		szBuffer[uReadLength] = '\0';
		return szBuffer;
}