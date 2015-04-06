#include <Winsock2.h>
#include <stdio.h>
#include "conio.h"
#include "iostream"
#include <string>
#include "math.h"
#include "cUDPInterface.h"
//#include "motion.cpp"
//--------------------------------------------------------------
//运动轴范围限制
//S,P，角度
//单片机发送频率为5.145KHz,周期194.4uS
#define ANGLE_MIN_S 80
#define ANGLE_MAX_S 200
#define ANGLE_MIN_P 60
#define ANGLE_MAX_P 200
//--------------------------------------------------------------

using namespace std;

extern bool bMoveShotFlag1;
extern bool bMoveShotFlag2;
extern bool bRunMode;
extern bool SerialCommInit();



extern void SerialCommSendData(UCHAR *);

//--------------------------------------------------------------

double	m_dCurrentPosS = 0;//S轴当前位置,角度
double	m_dCurrentPosP = 0;//P轴当前位置,角度
double	m_dTargetPosS = 0;//S轴目标位置,角度
double	m_dTargetPosP = 0;//P轴目标位置,角度


//--------------------------------------------------------------
void HomeAxisS();

bool MoveAxisS();
bool MoveAxisP();
bool MoveAxisSP();

//--------------------------------------------------------------
int main(void)
{
	cUDPInterface UDPInterface;


	if (false == SerialCommInit())
	{
		cout << "SerialCommInit false, quit the programm!" << endl;
		system("pause");
		return 0;
	}
	////===============================
	////初始化SP轴
	//UCHAR cmd[5];
	//cmd[0] = 'A';
	//cmd[1] = 'P';
	//cmd[2] = 'C';
	//cmd[3] = (UCHAR)m_dCurrentPosP;
	//cmd[4] = 'E';
	//SerialCommSendData(cmd);//设置当前位置为保存的位置
	//cmd[2] = 'T';
	////cmd[3]=127;
	//SerialCommSendData(cmd);
	//cmd[0] = 'A';
	//cmd[1] = 'S';
	//cmd[2] = 'C';
	//cmd[3] = (UCHAR)m_dCurrentPosS;;
	//cmd[4] = 'E';
	//SerialCommSendData(cmd);
	//cmd[2] = 'T';
	////cmd[3]=127;
	//SerialCommSendData(cmd);//设置当前位置为127
	//printf("\n如果重新上电，手动将S、P轴移动到中间位置，并运行S、P Home程序\n");
	//printf("\n需要运行S、P Home程序吗？(Y/N)\n");
	//cin >> cmd[0];

	//if (cmd[0] == 'Y' || cmd[0] == 'y')
	//{
	//	HomeAxisS();
	//}
	//else
	//{
	//	printf("\n取消需要运行S、P Home程序.\n");
	//}

	UDPInterface.ComInit();

	UDPInterface.recvBuf[1] = 's';
	while (1)
	{
		UDPInterface.RecvData();

	////	MoveAxisS();
	//	MoveAxisP();
	//	Sleep(100);
	}
	

}

//=============================================================



//=============================================================


void HomeAxisS()
{
	//直接设定S、P两轴的当前位置
	UCHAR cmd[5];
	cmd[0] = 'A';
	cmd[1] = 'P';
	cmd[2] = 'C';
	cmd[3] = 127;
	cmd[4] = 'E';
	SerialCommSendData(cmd);//设置当前位置为127
	cmd[2] = 'T';
	cmd[3] = 127;
	SerialCommSendData(cmd);//设置目标位置为127
	cmd[0] = 'A';
	cmd[1] = 'S';
	cmd[2] = 'C';
	cmd[3] = 127;
	cmd[4] = 'E';
	SerialCommSendData(cmd);//设置当前位置为127
	cmd[2] = 'T';
	cmd[3] = 127;
	SerialCommSendData(cmd);//设置目标位置为127	 	m_dCurrentPosS = 127;
	m_dCurrentPosP = 127;
	m_dCurrentPosS = 127;
	printf("\n复位S、P轴完成\n");
	return;
}

//================================================================================


bool MoveAxisS()
{
	printf("\nS轴当前位置:%.1f.\n请输入转动角度(°),(80~200)：", m_dCurrentPosS);

	int InPos;
	UCHAR absPos;
//	scanf("%d", &absPos);
//	scanf_s("%d", &absPos);
	cin >> InPos;
	absPos = (UCHAR)InPos;
	if (absPos < ANGLE_MIN_S || absPos > ANGLE_MAX_S)
	{
		printf("\n输入坐标数据超出范围，操作取消");
		printf("\n取消操作！返回上一级.\n");
		return false;
	}
	char cmdInput;
	printf("\nMoveAxisS，确定S轴的移动在安全区域,继续？(Y) or (N)");
	cmdInput = _getche();
	if (cmdInput != 'Y')
	{
		printf("\n取消操作！返回上一级.\n");
		return false;
	}
	UCHAR cmd[5];
	cmd[0] = 'A';
	cmd[1] = 'S';
	cmd[2] = 'T';
	cmd[3] = absPos;
	cmd[4] = 'E';
	SerialCommSendData(cmd);//设置当前位置为127
	m_dCurrentPosS = absPos;
	printf("\n转动S轴到指定位置完成\n");
	return true;
}
bool MoveAxisP()
{
	printf("\nP轴当前位置:%.1f.\n请输入转动角度(°),(60~200)：", m_dCurrentPosP);

	int InPos;
	UCHAR absPos;
	//scanf("%d", &absPos);
	//scanf_s("%d", &absPos);
	cin >> InPos;
	absPos = (UCHAR)InPos;
	if (absPos < ANGLE_MIN_P || absPos > ANGLE_MAX_P)
	{
		printf("\n输入坐标数据超出范围，操作取消");
		printf("\n取消操作！返回上一级.\n");
		return false;
	}

	char cmdInput;
	printf("\nMoveAxisP，确定P轴的移动在安全区域,继续？(Y) or (N)");
	cmdInput = _getche();
	if (cmdInput != 'Y')
	{
		printf("\n取消操作！返回上一级.\n");
		return false;
	}

	UCHAR cmd[5];
	cmd[0] = 'A';
	cmd[1] = 'P';
	cmd[2] = 'T';
	cmd[3] = absPos;
	cmd[4] = 'E';
	SerialCommSendData(cmd);//
	m_dCurrentPosP = absPos;
	printf("\n转动P轴到指定位置完成\n");
	return true;
}

