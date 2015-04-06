#include <Winsock2.h>
#include <stdio.h>
#include "conio.h"
#include "iostream"
#include <string>
#include "math.h"
#include "cUDPInterface.h"
//#include "motion.cpp"
//--------------------------------------------------------------
//�˶��᷶Χ����
//S,P���Ƕ�
//��Ƭ������Ƶ��Ϊ5.145KHz,����194.4uS
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

double	m_dCurrentPosS = 0;//S�ᵱǰλ��,�Ƕ�
double	m_dCurrentPosP = 0;//P�ᵱǰλ��,�Ƕ�
double	m_dTargetPosS = 0;//S��Ŀ��λ��,�Ƕ�
double	m_dTargetPosP = 0;//P��Ŀ��λ��,�Ƕ�


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
	////��ʼ��SP��
	//UCHAR cmd[5];
	//cmd[0] = 'A';
	//cmd[1] = 'P';
	//cmd[2] = 'C';
	//cmd[3] = (UCHAR)m_dCurrentPosP;
	//cmd[4] = 'E';
	//SerialCommSendData(cmd);//���õ�ǰλ��Ϊ�����λ��
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
	//SerialCommSendData(cmd);//���õ�ǰλ��Ϊ127
	//printf("\n��������ϵ磬�ֶ���S��P���ƶ����м�λ�ã�������S��P Home����\n");
	//printf("\n��Ҫ����S��P Home������(Y/N)\n");
	//cin >> cmd[0];

	//if (cmd[0] == 'Y' || cmd[0] == 'y')
	//{
	//	HomeAxisS();
	//}
	//else
	//{
	//	printf("\nȡ����Ҫ����S��P Home����.\n");
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
	//ֱ���趨S��P����ĵ�ǰλ��
	UCHAR cmd[5];
	cmd[0] = 'A';
	cmd[1] = 'P';
	cmd[2] = 'C';
	cmd[3] = 127;
	cmd[4] = 'E';
	SerialCommSendData(cmd);//���õ�ǰλ��Ϊ127
	cmd[2] = 'T';
	cmd[3] = 127;
	SerialCommSendData(cmd);//����Ŀ��λ��Ϊ127
	cmd[0] = 'A';
	cmd[1] = 'S';
	cmd[2] = 'C';
	cmd[3] = 127;
	cmd[4] = 'E';
	SerialCommSendData(cmd);//���õ�ǰλ��Ϊ127
	cmd[2] = 'T';
	cmd[3] = 127;
	SerialCommSendData(cmd);//����Ŀ��λ��Ϊ127	 	m_dCurrentPosS = 127;
	m_dCurrentPosP = 127;
	m_dCurrentPosS = 127;
	printf("\n��λS��P�����\n");
	return;
}

//================================================================================


bool MoveAxisS()
{
	printf("\nS�ᵱǰλ��:%.1f.\n������ת���Ƕ�(��),(80~200)��", m_dCurrentPosS);

	int InPos;
	UCHAR absPos;
//	scanf("%d", &absPos);
//	scanf_s("%d", &absPos);
	cin >> InPos;
	absPos = (UCHAR)InPos;
	if (absPos < ANGLE_MIN_S || absPos > ANGLE_MAX_S)
	{
		printf("\n�����������ݳ�����Χ������ȡ��");
		printf("\nȡ��������������һ��.\n");
		return false;
	}
	char cmdInput;
	printf("\nMoveAxisS��ȷ��S����ƶ��ڰ�ȫ����,������(Y) or (N)");
	cmdInput = _getche();
	if (cmdInput != 'Y')
	{
		printf("\nȡ��������������һ��.\n");
		return false;
	}
	UCHAR cmd[5];
	cmd[0] = 'A';
	cmd[1] = 'S';
	cmd[2] = 'T';
	cmd[3] = absPos;
	cmd[4] = 'E';
	SerialCommSendData(cmd);//���õ�ǰλ��Ϊ127
	m_dCurrentPosS = absPos;
	printf("\nת��S�ᵽָ��λ�����\n");
	return true;
}
bool MoveAxisP()
{
	printf("\nP�ᵱǰλ��:%.1f.\n������ת���Ƕ�(��),(60~200)��", m_dCurrentPosP);

	int InPos;
	UCHAR absPos;
	//scanf("%d", &absPos);
	//scanf_s("%d", &absPos);
	cin >> InPos;
	absPos = (UCHAR)InPos;
	if (absPos < ANGLE_MIN_P || absPos > ANGLE_MAX_P)
	{
		printf("\n�����������ݳ�����Χ������ȡ��");
		printf("\nȡ��������������һ��.\n");
		return false;
	}

	char cmdInput;
	printf("\nMoveAxisP��ȷ��P����ƶ��ڰ�ȫ����,������(Y) or (N)");
	cmdInput = _getche();
	if (cmdInput != 'Y')
	{
		printf("\nȡ��������������һ��.\n");
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
	printf("\nת��P�ᵽָ��λ�����\n");
	return true;
}

