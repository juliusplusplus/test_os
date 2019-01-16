#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "common.h"
#include "MFD.h"
#include "Fileopera.h"



extern vector < vector<UFD> > FileInfo;
extern UFD FileInput;
extern vector<Cluster> FileCluster;
extern vector<MFD> UsrInfo;
extern vector< vector<UOF> > FileState;
extern UOF StateInput;
extern vector< Cluster> FileCluster;
extern Cluster ClusterInput;

//�����ļ�Ȩ�޺�����============================================================================= 
void do_Chmod()
{
	int temp;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			stringstream ss;
			ss << cmd_in.cmd_num[2];
			ss >> temp;
			FileInfo[curID][i].mode = temp;
			break;
		}
	}
	for (int i = 0; i < FileState[curID].size(); i++)
	{
		if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			FileState[curID][i].mode = temp;
			break;
		}
	}
	cout << "Modified permission successfully!" << endl;
}
//�����ļ��������ߣ������ļ������ļ���============================================================= 
void do_Chown()
{
	// Chown filename new_owner 
	vector<UFD>::iterator it;
	for (it = FileInfo[curID].begin(); it != FileInfo[curID].end(); it++)
	{
		if (strcmp((*it).filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			strcpy(FileInput.filename, (*it).filename);
			FileInput.length = (*it).length;
			FileInput.mode = (*it).mode;
			FileInput.addr = (*it).addr;

			FileInfo[curID].erase(it);
			break;
		}
	}

	//ɾ��״̬��
	vector<UOF>::iterator it_num1;
	for (it_num1 = FileState[curID].begin(); it_num1 != FileState[curID].end(); it_num1++)
	{
		if (strcmp((*it_num1).filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			strcpy(StateInput.filename, (*it_num1).filename);
			StateInput.mode = (*it_num1).mode;
			StateInput.state = (*it_num1).state;
			StateInput.read_poit = (*it_num1).read_poit;
			StateInput.write_poit = (*it_num1).write_poit;

			FileState[curID].erase(it_num1);
			break;
		}
	}

	//��������û����ļ�����״̬��
	int tempID;
	for (int i = 0; i < UsrInfo.size(); i++)
	{
		if (strcmp(UsrInfo[i].usrname, cmd_in.cmd_num[2].c_str()) == 0)
		{
			tempID = i;
			break;
		}
	}
	int flag = 0;
	//�Ƿ����ͬ���ļ����Ƿ��滻��
	for (int i = 0; i < FileInfo[tempID].size(); i++)
	{
		if (strcmp(FileInfo[tempID][i].filename, cmd_in.cmd_num[2].c_str()) == 0)
		{
			flag = 1;
			break;
		}
	}
	if (!flag)
	{
		FileInfo[tempID].push_back(FileInput);
		FileState[tempID].push_back(StateInput);
		cout << "File owner changes successfully!" << endl;
	}
	else
	{
		cout << "File name repeat!" << endl;
	}
}

//�ļ�������=============================================================================== 
void do_Mv()
{
	//Mv srcFile desFile
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			strcpy(FileInfo[curID][i].filename, cmd_in.cmd_num[2].c_str());
			break;
		}
	}

	for (int i = 0; i < FileState[curID].size(); i++)
	{
		if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			strcpy(FileState[curID][i].filename, cmd_in.cmd_num[2].c_str());
			break;
		}
	}
	cout << "Renaming successful!" << endl;
}
//void do_Write();
void doTempWrite();
//�����ļ�==================================================================================== 
void make_new_copy()
{
	//Type filename
	string Tempbuff;
	int addre;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			addre = FileInfo[curID][i].addr;
			break;
		}
	}
	int index;
	for (int i = 0; i < FileState[curID].size(); i++)
	{
		if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			index = i;
			break;
		}
	}

	while (1)
	{
		if (FileCluster[addre].next_num == addre)
		{
			for (int i = 0; i < FileState[curID][index].write_poit; i++)
			{
				//cout << FileCluster[addre].data[i];
				Tempbuff += FileCluster[addre].data[i];
			}
			break;
		}
		else
		{
			for (int i = 0; i < 256; i++)
			{
				//cout << FileCluster[addre].data[i];
				Tempbuff += FileCluster[addre].data[i];
			}
			if (FileCluster[addre].next_num != addre)
			{
				addre = FileCluster[addre].next_num;
			}
			else
				break;
		}
	}

	//Write	filename buffer nbytes д�ļ�   ����ռ�68
	cmd_in.cmd_num[0] = "Write";
	cmd_in.cmd_num[1] = cmd_in.cmd_num[2];
	cmd_in.cmd_num[2] = Tempbuff;
	stringstream ss;
	ss << Tempbuff.length();
	string curtp;
	ss >> curtp;
	cmd_in.cmd_num[3] = curtp;
	//cout << cmd_in.cmd_num[0] << " " << cmd_in.cmd_num[1] << "  " << cmd_in.cmd_num[2] << " " << cmd_in.cmd_num[3] << endl;
	//do_Write();
	doTempWrite();

}

//�����ļ�======================================================================================= 
void do_Create();

//�����ļ� ====================================================================================== 
void do_Copy()
{
	//Copy  srcFile desFile 
	int address;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			address = FileInfo[curID][i].addr;
			break;
		}
	}

	int flag = 0;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[2].c_str()) == 0)
		{
			flag = 1;
			break;
		}
	}
	string s1, s2, s3;
	s1 = cmd_in.cmd_num[0];
	s2 = cmd_in.cmd_num[1];
	s3 = cmd_in.cmd_num[2];
	//�µ��ļ�������....
	if (flag)
	{
		make_new_copy();
	}
	else   //�µ��ļ��������ڵĻ�Ҫ�½�һ��
	{
		//Create name mode
		cmd_in.cmd_num[0] = "Create";
		cmd_in.cmd_num[1] = cmd_in.cmd_num[2];
		cmd_in.cmd_num[2] = "2";
		do_Create();
		cmd_in.cmd_num[0] = s1;
		cmd_in.cmd_num[1] = s2;
		cmd_in.cmd_num[2] = s3;
		make_new_copy();

	}

}

//��ʾ�����ļ�Ŀ¼ ======================================================================================== 
void do_dir()
{
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		cout << FileInfo[curID][i].filename << "  ";
	}
	cout << endl;

}

//��ʾ�ı��ļ�����======================================================================================== 
void do_Type()
{
	//Type filename
	int address;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			address = FileInfo[curID][i].addr;
			break;
		}
	}
	int index;
	for (int i = 0; i < FileState[curID].size(); i++)
	{
		if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			index = i;
			break;
		}
	}

	while (1)
	{
		if (FileCluster[address].next_num == address)
		{
			for (int i = 0; i < FileState[curID][index].write_poit; i++)
				cout << FileCluster[address].data[i];
			break;
		}
		else
		{
			for (int i = 0; i < 256; i++)
			{
				cout << FileCluster[address].data[i];
			}
			if (FileCluster[address].next_num != address)
			{
				address = FileCluster[address].next_num;
			}
			else
				break;
		}
	}
	cout << endl;
}

//�޸�����======================================================================================= 
void do_Passwd()
{
	//Passwd oldPwd  newPwd 
	if (strcmp(UsrInfo[curID].usrpass, cmd_in.cmd_num[1].c_str()) == 0)
	{
		strcpy(UsrInfo[curID].usrpass, cmd_in.cmd_num[2].c_str());
		cout << "Password changed!" << endl;
	}
	else
		cout << "The password is wrong!" << endl;
}

//���ļ�����д�������============================================================================ 
void do_sysc()
{
	out_to_file();
	cout << "Written to disk!" << endl;
}

//�û���¼======================================================================================== 
void do_Login()
{
	//Login	userName pwd
	int flag = 0;
	for (int i = 0; i < UsrInfo.size(); i++)
	{
		if (strcmp(cmd_in.cmd_num[1].c_str(), UsrInfo[i].usrname) == 0 && strcmp(cmd_in.cmd_num[2].c_str(), UsrInfo[i].usrpass) == 0)
		{
			curID = i;   //ȫ�ּ�¼��½�ߵ�ID
			flag = 1;
			break;
		}
	}
	if (flag)
	{
		cout << "Landed" << endl;
		return;
	}
	else
	{
		cout << "User name does not exist, please login again!" << endl;
	}
}

//�رճ���======================================================================================== 
void do_exit()
{
	do_sysc();
	cout << "Quit" << endl;
	exit(0);
}

//ע��============================================================================================ 
void do_Logout()
{
	/*cout << "���Ƿ��Ѿ�ʹ��sysc�������Ϣ�����̣�Y/N" << endl;
	char c;
	c = getchar();
	if (c == 'Y')*/
	cout << "Please log on to the user again!" << endl;
}

//�û�ע��======================================================================================== 
void do_register()
{
	//Register usrName pwd
	if (UsrInfo.size() < 16)
	{
		int flag = 1;
		for (int i = 0; i < UsrInfo.size(); i++)
		{
			if (strcmp(cmd_in.cmd_num[1].c_str(), UsrInfo[i].usrname) == 0)
			{
				flag = 0;
				break;
			}
		}
		if (flag)
		{
			strcpy(UsrInput.usrname, cmd_in.cmd_num[1].c_str());
			strcpy(UsrInput.usrpass, cmd_in.cmd_num[2].c_str());
			UsrInput.link = UsrInfo.size();
			UsrInfo.push_back(UsrInput);

			//Ϊ�µ��û����ٿռ�
			vector<UFD> t;
			FileInfo.push_back(t);

			vector<UOF> temp;
			FileState.push_back(temp);

			cout << "Registration is successful!" << endl;

		}
		else
		{
			cout << "User name repeat, please re-register!" << endl;
		}
	}
	else
		cout << "There is not enough space!" << endl;
}

//�����ļ�======================================================================================== 
void do_Create()
{
	//create name mode
	if (FileInfo[curID].size() < 16)
	{
		int flag = 1;
		for (int i = 0; i < FileInfo[curID].size(); i++)
		{
			if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
			{
				flag = 0;
				break;
			}
		}
		if (!flag)
		{
			cout << "You can't have the same name!" << endl;
			return;
		}

		strcpy(FileInput.filename, cmd_in.cmd_num[1].c_str());
		stringstream ss;
		ss << cmd_in.cmd_num[2];
		int temp;
		ss >> temp;
		FileInput.mode = temp;
		FileInput.length = 0;
		//Ҫ��������������ݴ洢...
		for (int i = 0; i < FileCluster.size(); i++)
		{
			if (FileCluster[i].is_data == 0)//����û�����ݣ�           
			{
				FileInput.addr = i;
				FileCluster[i].is_data = 1;
				break;
			}
		}
		FileInfo[curID].push_back(FileInput);
		cout << "Create success!" << endl;

		//״̬��
		strcpy(StateInput.filename, cmd_in.cmd_num[1].c_str());
		StateInput.state = 0;
		StateInput.mode = FileInput.mode;
		StateInput.read_poit = 0;
		StateInput.write_poit = 0;

		FileState[curID].push_back(StateInput);

	}
	else
		cout << "You have exceed the limit!" << endl;


}

//ɾ���ļ�================================================================================= 
void do_Delete()
{
	//delete filename
	int flag = 0;
	vector<UFD>::iterator it;
	for (it = FileInfo[curID].begin(); it != FileInfo[curID].end(); it++)
	{
		if (strcmp((*it).filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			//FileInfo[curID].erase(it);
			flag = 1;
			break;
		}
	}
	if (flag)
	{
		//ע��Ӧ�����ռ�õ��ڴ�

		//Type filename
		int address;
		for (int i = 0; i < FileInfo[curID].size(); i++)
		{
			if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
			{
				address = FileInfo[curID][i].addr;
				break;
			}
		}
		int index;
		for (int i = 0; i < FileState[curID].size(); i++)
		{
			if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
			{
				index = i;
				break;
			}
		}

		vector<int> reset_addr;
		reset_addr.clear();

		while (1)
		{
			if (FileCluster[address].next_num == address)
			{
				for (int i = 0; i < FileState[curID][index].write_poit; i++)
					FileCluster[address].data[i] = 0;
				reset_addr.push_back(address);
				break;
			}
			else
			{
				reset_addr.push_back(address);
				for (int i = 0; i < 256; i++)
				{
					FileCluster[address].data[i] = 0;
				}
				if (FileCluster[address].next_num != address)
				{
					address = FileCluster[address].next_num;
				}
				else
					break;
			}
		}

		for (int w = 0; w < reset_addr.size(); w++)
		{
			FileCluster[reset_addr[w]].is_data = 0;
			FileCluster[reset_addr[w]].next_num = reset_addr[w];
		}

		//����ļ���Ϣ
		vector<UFD>::iterator it;
		for (it = FileInfo[curID].begin(); it != FileInfo[curID].end(); it++)
		{
			if (strcmp((*it).filename, cmd_in.cmd_num[1].c_str()) == 0)
			{
				FileInfo[curID].erase(it);
				flag = 1;
				break;
			}
		}
		//���״̬��
		vector<UOF>::iterator it1;
		for (it1 = FileState[curID].begin(); it1 != FileState[curID].end(); it1++)
		{
			if (strcmp((*it1).filename, cmd_in.cmd_num[1].c_str()) == 0)
			{
				FileState[curID].erase(it1);
				break;
			}
		}

		cout << "Deleted" << endl;
	}
	else
		cout << "Cancelled!" << endl;


}

//���ļ�======================================================================================== 
void do_Open()
{
	//Open  filename mode
	stringstream ss;
	ss << cmd_in.cmd_num[2];
	int temp;
	ss >> temp;
	//�ж��Ƿ���Ȩ��,��û����......
	int flag = 0;
	for (int i = 0; i < FileState[curID].size(); i++)
	{
		if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			FileState[curID][i].state = 1;
			flag = 1;
			break;
		}
	}
	if (flag)
		cout << "Opened" << endl;
	else
	{
		strcpy(StateInput.filename, cmd_in.cmd_num[1].c_str());
		StateInput.mode = temp;
		StateInput.state = 1;
		StateInput.read_poit = StateInput.write_poit = 0;
		FileState[curID].push_back(StateInput);
		cout << "Opened" << endl;
	}

}

//�ر��ļ�===================================================================================== 
void do_Close()
{
	//close filename
	vector<UOF>::iterator it;
	for (it = FileState[curID].begin(); it != FileState[curID].end(); it++)
	{
		if (strcmp((*it).filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			FileState[curID].erase(it);
			cout << "Closed" << endl;
			break;
		}
	}
}

/*int num;
int next_num;
int is_data;
char data[256];*/
//�༭�ļ�����================================================================================== 
void do_Write()
{
	//Write	filename buffer nbytes д�ļ�   ����ռ�68

	char buf[1024];
	stringstream ss;
	ss << cmd_in.cmd_num[3];
	int temp;
	ss >> temp;


	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			if (FileInfo[curID][i].mode == 1 || FileInfo[curID][i].mode == 2)//�ж�Ȩ��
			{
				break;
			}
			else
			{
				cout << "No permission: to write" << endl;
				return;
			}
		}
	}

	int index;
	for (int i = 0; i < FileState[curID].size(); i++)
	{
		if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			index = i;
			break;
		}
	}
	//��ʼ�����
	int address;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			address = FileInfo[curID][i].addr;
			break;
		}
	}
	//ע�⣺�˴������˸��ģ�
	cout << "Please enter buffer content!" << endl;
	gets_s(buf);
	fflush(stdin);

	//strcpy(buf, cmd_in.cmd_num[2].c_str());
	 
	int wbegin;
	wbegin = FileState[curID][index].write_poit;

	//�ҵ�дָ�����ڵ����һ������
	while (FileCluster[address].next_num != address)
		address = FileCluster[address].next_num;

	vector <int> newspace_num;//���㽫Ҫռ�õ�����������
	newspace_num.clear();

	//int num = (256-wbegin+temp) / 256-1;
	if (temp <= 256 - wbegin)
		num = 0;
	else
	{
		num = ceil((temp - (256 - wbegin))*1.0 / 256);
	}

	newspace_num.push_back(address);

	//cout << newspace_num.size() << endl;//

	for (int i = 0; i < FileCluster.size(); i++)
	{
		if (newspace_num.size() == num + 1)
			break;
		if (FileCluster[i].is_data == 0)
		{
			newspace_num.push_back(i);
			FileCluster[i].is_data = 1;
		}
	}

	for (int k = 0; k < newspace_num.size() - 1; k++)
	{
		FileCluster[newspace_num[k]].next_num = newspace_num[k + 1];
	}
	for (int i = 0; i < temp; i++)
	{
		if (wbegin == 256)
		{
			wbegin = 0;
			address = FileCluster[address].next_num;
		}
		FileCluster[address].data[wbegin] = buf[i];
		wbegin++;
	}

	//����дָ��
	FileState[curID][index].write_poit = wbegin;
	cout << "Written to disk" << endl;
	return;

}

//write_poit��¼���մ��̵�дָ�룬read_poit��¼ȫ�������Ķ�ָ���λ�á�
//�Ķ��ļ�===================================================================================== 
void do_Read()
{
	//Read 	filename buffer nbytes 
	char buf[1024];
	stringstream ss;
	ss << cmd_in.cmd_num[3];
	int temp;
	ss >> temp;

	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			if (FileInfo[curID][i].mode == 0 || FileInfo[curID][i].mode == 2)//�ж�Ȩ��
			{
				break;
			}
			else
			{
				cout << "No permission: to read" << endl;
				return;
			}
		}
	}

	int index;
	for (int i = 0; i < FileState[curID].size(); i++)
	{
		if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			index = i;
			break;
		}
	}

	int address;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			address = FileInfo[curID][i].addr;
			break;
		}
	}

	int rbegin;
	rbegin = FileState[curID][index].read_poit;
	int addcur = rbegin / 256;

	for (int i = 0; i < addcur; i++)
	{
		address = FileCluster[address].next_num;
	}

	for (int i = 0; i < temp; i++)
	{
		if (rbegin % 256 == 255)
		{
			address = FileCluster[address].next_num;
		}
		buf[i] = FileCluster[address].data[rbegin % 256];
		rbegin++;
	}

	FileState[curID][index].read_poit = rbegin;

	cout << "Read out:" << endl;
	for (int i = 0; i < temp; i++)
		cout << buf[i];
	cout << endl;

}

//�����ĵ�=================================================================================== 
void do_Help()
{
	cout << "����������������������������������������������������������������������������������-" << endl;
	cout << "|| Login    ��½  ��ʽ��Login a1 ww,����½�û���Ϊa1,����Ϊww���û�              ||" << endl;
	cout << "|| Logout   �ǳ�  ��ʽ��Logout,���ǳ���ǰ�û�                                    ||" << endl;
	cout << "|| Register ע��  ��ʽ��Register a1 ww,��ע���û���Ϊa1,����Ϊww���û�           ||" << endl;
	cout << "|| Passwd	 ����  ��ʽ��Passwd	ww  yy,����ǰ�û�����ww����Ϊ������yy             ||" << endl;
	cout << "|| Open   ���ļ�    ��ʽ��Open a1 rw,�����ļ���Ϊa1,Ȩ��Ϊ�ɶ�д���ļ�       ||" << endl;
	cout << "|| Close  �ر��ļ�    ��ʽ��Close a1,�����ļ���Ϊa1���ļ�                      ||" << endl;
	cout << "|| Create �����ļ�    ��ʽ��Create a1 rw,�������ļ���Ϊa1,Ȩ��Ϊ�ɶ�д���ļ�     ||" << endl;
	cout << "|| Delete ɾ���ļ�    ��ʽ��Delete a1,��ɾ���ļ���Ϊa1���ļ�                     ||" << endl;
	cout << "|| Write  д�ļ�      ��ʽ��Write a1 buffer 20bytes                              ||" << endl;
	cout << "|| Read   ���ļ�      ��ʽ��Read a1 buffer  20bytes���ļ�                        ||" << endl;
	cout << "|| dir    չʾ�ļ�Ŀ¼    ��ʽ��dir,���г����û��������ļ�                       ||" << endl;
	cout << "|| Chmod  �ı��ļ�Ȩ��    ��ʽ��Chmod a1 r/w,���ļ�a1��Ȩ���޸�Ϊֻ��/ֻд       ||" << endl;
	cout << "|| Chown  �ı��ļ�ӵ����  ��ʽ��Chown a1 admin,���ļ���Ϊa1���ļ�ӵ���߸�Ϊadmin ||" << endl;
	cout << "|| Mv	   �ޱ��ļ���      ��ʽ��Mv a1 b1,���ļ�a1������Ϊb1                      ||" << endl;
	cout << "|| Copy   �ļ�����        ��ʽ��Copy a1 b1,���ļ�b1�������ļ�a1��                || " << endl;
	cout << "|| Type   ��ʾ�ļ�����    ��ʽ��Type a1,����ʾ�ļ���Ϊa1������                   ||" << endl;
	cout << "|| Exit   �˳�ϵͳ        ��ʽ��Exit,���˳����ر��ļ�ϵͳ                        ||" << endl;
	cout << "|| sysc   ͬ��������      ��ʽ��sysc,���ļ�ͬ�����������Analogdisk��            ||" << endl;
	cout << "����������������������������������������������������������������������������������- " << endl;
}

//�ļ�д�����===============================================================================
void doTempWrite()
{
	//Write	filename buffer nbytes д�ļ�   ����ռ�68

	char buf[1024];
	stringstream ss;
	ss << cmd_in.cmd_num[3];
	int temp;
	ss >> temp;


	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			if (FileInfo[curID][i].mode == 1 || FileInfo[curID][i].mode == 2)//�ж�Ȩ��
			{
				break;
			}
			else
			{
				cout << "No permission: to write" << endl;
				return;
			}
		}
	}

	int index;
	for (int i = 0; i < FileState[curID].size(); i++)
	{
		if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			index = i;
			break;
		}
	}
	//��ʼ�����
	int address;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			address = FileInfo[curID][i].addr;
			break;
		}
	}
	//ע�⣺�˴������˸��ģ�
	/*cout << "Please enter the buff��" << endl;
	gets(buf);
	fflush(stdin);*/


	strcpy(buf, cmd_in.cmd_num[2].c_str());

	int wbegin;
	wbegin = FileState[curID][index].write_poit;

	//�ҵ�дָ�����ڵ����һ������
	while (FileCluster[address].next_num != address)
		address = FileCluster[address].next_num;

	vector <int> newspace_num;//���㽫Ҫռ�õ�����������
	newspace_num.clear();

	//int num = (256-wbegin+temp) / 256-1;
	if (temp <= 256 - wbegin)
		num = 0;
	else
	{
		num = ceil((temp - (256 - wbegin))*1.0 / 256);
	}

	newspace_num.push_back(address);

	//cout << newspace_num.size() << endl;//

	for (int i = 0; i < FileCluster.size(); i++)
	{
		if (newspace_num.size() == num + 1)
			break;
		if (FileCluster[i].is_data == 0)
		{
			newspace_num.push_back(i);
			FileCluster[i].is_data = 1;
		}
	}

	for (int k = 0; k < newspace_num.size() - 1; k++)
	{
		FileCluster[newspace_num[k]].next_num = newspace_num[k + 1];
	}
	for (int i = 0; i < temp; i++)
	{
		if (wbegin == 256)
		{
			wbegin = 0;
			address = FileCluster[address].next_num;
		}
		FileCluster[address].data[wbegin] = buf[i];
		wbegin++;
	}

	//����дָ��
	FileState[curID][index].write_poit = wbegin;
	cout << "Written to disk!" << endl;
	return;

}

//����========================================================================================= 
void do_Clear()
{
	system("cls");
}

#endif