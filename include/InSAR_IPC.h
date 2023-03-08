#ifndef INSAR_IPC_H_
#define INSAR_IPC_H_

#include <string>
#include <Windows.h>
#include"para_struct.h"

// �¼�����

// ʹ��ע��:
// CLYXCIPC server, client
// server.InitIPCMemory(true, "server", 0);
// server.InitSelfEvent("serverEvent", FALSE);
// client.InitIPCMemory(false, "client");
// client.InitSelfEvent("clientEvent", TRUE);
// server.InitOtherEvent("clientEvent");
// client.InitOtherEvent("serverEvent");

class InSAR_IPC {

public:
	InSAR_IPC(void);
	~InSAR_IPC();

private:
	HANDLE hMapFile;
	bool bServer;
	HANDLE selfEvent, otherEvent;
	SYSTEM_INFO info;

public:
	// ��ʼ�������ڴ�
	bool InitIPCMemory(bool bSever, LPCWSTR fileName, DWORD dwServerMapSize = 0);
	// �����ڴ��ȡ
	bool ReadData(DWORD& dwOffset, DWORD dwSize, char* buf);
	// �����ڴ�д��
	bool WriteData(DWORD& dwOffset, char* buf, DWORD dwSize);

public:
	// ͨ���¼���ʼ������ʼ���Լ��¼�
	bool InitSelfEvent(LPCWSTR eventName, BOOL bInitState);
	// ͨ�ų�ʼ������ʼ���Է��¼� 
	bool InitOtherEvent(LPCWSTR eventName, BOOL bInitState);
	// ͨ�ų�ʼ������ʼ���Է��¼� 
	bool OpenSelfEvent(LPCWSTR eventName);
	// ͨ�ų�ʼ������ʼ���Է��¼� 
	bool OpenOtherEvent(LPCWSTR eventName);
	// �������ź�״̬
	void SetEventIntf(bool bSelf);
	// �������ź�״̬
	void ResetEventIntf(bool bSelf);
	// ��ȡ�¼�
	HANDLE GetEvent(bool bSelf);
};



enum eCallbackType
{
	eCallbackType_Unknown = 0,			// δ֪����
	eCallbackType_UpdateCmplt,			// ���µ�ǰ��ɽ���
	eCallbackType_MsgFinished,			// ֪ͨ�������
	eCallbackType_MsgError,				// ֪ͨ����������
	eCallbackType_RequestData,			// ��������
	eCallbackType_SaveData				// �洢����
};

// IPC��Ϣͷ
struct stcIPCMsgHeader
{
	/*��Ϣ����*/
	int msgType;
	/*������Ϣ*/
	int progress;
	/*��Ϣ����*/
	char message[2048];
	/*�ش�����*/
	template_DEM_para_back callback_para;
	stcIPCMsgHeader()
	{
		msgType = eCallbackType::eCallbackType_Unknown;
		message[0] = 0;
		progress = 1;
	}
};

#endif
