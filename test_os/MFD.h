#ifndef _MFD_H_
#define _MFD_H_

#include"common.h"
#include"command.h"
using namespace std;

//32���ֽ�
typedef struct mfd
{
	char usrname[14]; //�û���
	char usrpass[14]; //�û�����
	int link;  //���û���UFD���ڵ��̺�

}MFD;

typedef struct ufd
{
	char filename[14];
	int mode; // �ļ�Ȩ��0 - readonly;  1 - writeonly;  2 - read / write
	int length;
	int addr;
}UFD;

typedef struct uof
{
	char filename[14];
	int mode;
	int state; //0����,1��
	int read_poit; //��дָ��
	int write_poit;

}UOF;

typedef struct cluster
{
	int next_num;
	int is_data;
	char data[256];

}Cluster;

typedef void(*func)(void);
typedef struct hand
{
	char *pname;
	func handler;
}HAND_TO;

typedef struct cmd
{
	string cmd_num[4];
}COMMAND;


COMMAND cmd_in;
extern HAND_TO handlerlist[];

void cmd_in_parse()
{
	string s;
	getline(cin, s);
	
		stringstream stream(s);
		int i = 0;
		//���������cmd_in ��
		while (stream >> cmd_in.cmd_num[i])
			i++;
		//���������б�
		int flag = 0;
		for (int i = 0; handlerlist[i].pname!=NULL; i++)
		{
			if (strcmp(handlerlist[i].pname, cmd_in.cmd_num[0].c_str()) == 0)
			{
				flag = 1;
				handlerlist[i].handler();
				break;
			}
		}
		if (!flag)
			cout << "Command not found!" << endl;
	
}


#endif _MFD_H_