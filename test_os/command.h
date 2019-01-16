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

//设置文件权限和属性============================================================================= 
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
//更改文件的所有者，更换文件所在文件夹============================================================= 
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

	//删除状态栏
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

	//添加至新用户的文件栏和状态栏
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
	//是否存在同名文件，是否替换等
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

//文件重命名=============================================================================== 
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
//复制文件==================================================================================== 
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

	//Write	filename buffer nbytes 写文件   物理空间68
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

//创建文件======================================================================================= 
void do_Create();

//复制文件 ====================================================================================== 
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
	//新的文件名存在....
	if (flag)
	{
		make_new_copy();
	}
	else   //新的文件名不存在的话要新建一个
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

//显示所有文件目录 ======================================================================================== 
void do_dir()
{
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		cout << FileInfo[curID][i].filename << "  ";
	}
	cout << endl;

}

//显示文本文件内容======================================================================================== 
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

//修改密码======================================================================================= 
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

//将文件内容写入磁盘中============================================================================ 
void do_sysc()
{
	out_to_file();
	cout << "Written to disk!" << endl;
}

//用户登录======================================================================================== 
void do_Login()
{
	//Login	userName pwd
	int flag = 0;
	for (int i = 0; i < UsrInfo.size(); i++)
	{
		if (strcmp(cmd_in.cmd_num[1].c_str(), UsrInfo[i].usrname) == 0 && strcmp(cmd_in.cmd_num[2].c_str(), UsrInfo[i].usrpass) == 0)
		{
			curID = i;   //全局记录登陆者的ID
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

//关闭程序======================================================================================== 
void do_exit()
{
	do_sysc();
	cout << "Quit" << endl;
	exit(0);
}

//注销============================================================================================ 
void do_Logout()
{
	/*cout << "您是否已经使用sysc命令保存信息至磁盘？Y/N" << endl;
	char c;
	c = getchar();
	if (c == 'Y')*/
	cout << "Please log on to the user again!" << endl;
}

//用户注册======================================================================================== 
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

			//为新的用户开辟空间
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

//创建文件======================================================================================== 
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
		//要处理接下来的内容存储...
		for (int i = 0; i < FileCluster.size(); i++)
		{
			if (FileCluster[i].is_data == 0)//盘中没有数据，           
			{
				FileInput.addr = i;
				FileCluster[i].is_data = 1;
				break;
			}
		}
		FileInfo[curID].push_back(FileInput);
		cout << "Create success!" << endl;

		//状态栏
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

//删除文件================================================================================= 
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
		//注意应该清除占用的内存

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

		//清除文件信息
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
		//清除状态栏
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

//打开文件======================================================================================== 
void do_Open()
{
	//Open  filename mode
	stringstream ss;
	ss << cmd_in.cmd_num[2];
	int temp;
	ss >> temp;
	//判断是否有权限,还没做呢......
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

//关闭文件===================================================================================== 
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
//编辑文件内容================================================================================== 
void do_Write()
{
	//Write	filename buffer nbytes 写文件   物理空间68

	char buf[1024];
	stringstream ss;
	ss << cmd_in.cmd_num[3];
	int temp;
	ss >> temp;


	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			if (FileInfo[curID][i].mode == 1 || FileInfo[curID][i].mode == 2)//判断权限
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
	//起始物理块
	int address;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			address = FileInfo[curID][i].addr;
			break;
		}
	}
	//注意：此处发生了更改！
	cout << "Please enter buffer content!" << endl;
	gets_s(buf);
	fflush(stdin);

	//strcpy(buf, cmd_in.cmd_num[2].c_str());
	 
	int wbegin;
	wbegin = FileState[curID][index].write_poit;

	//找到写指针所在的最后一个磁盘
	while (FileCluster[address].next_num != address)
		address = FileCluster[address].next_num;

	vector <int> newspace_num;//计算将要占用的物理块的数量
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

	//更新写指针
	FileState[curID][index].write_poit = wbegin;
	cout << "Written to disk" << endl;
	return;

}

//write_poit记录最终磁盘的写指针，read_poit记录全局总数的读指针的位置。
//阅读文件===================================================================================== 
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
			if (FileInfo[curID][i].mode == 0 || FileInfo[curID][i].mode == 2)//判断权限
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

//帮助文档=================================================================================== 
void do_Help()
{
	cout << "—————————————————————————————————————————-" << endl;
	cout << "|| Login    登陆  格式：Login a1 ww,将登陆用户名为a1,密码为ww的用户              ||" << endl;
	cout << "|| Logout   登出  格式：Logout,将登出当前用户                                    ||" << endl;
	cout << "|| Register 注册  格式：Register a1 ww,将注册用户名为a1,密码为ww的用户           ||" << endl;
	cout << "|| Passwd	 改密  格式：Passwd	ww  yy,将当前用户密码ww更改为新密码yy             ||" << endl;
	cout << "|| Open   打开文件    格式：Open a1 rw,将打开文件名为a1,权限为可读写的文件       ||" << endl;
	cout << "|| Close  关闭文件    格式：Close a1,将打开文件名为a1的文件                      ||" << endl;
	cout << "|| Create 建立文件    格式：Create a1 rw,将建立文件名为a1,权限为可读写的文件     ||" << endl;
	cout << "|| Delete 删除文件    格式：Delete a1,将删除文件名为a1的文件                     ||" << endl;
	cout << "|| Write  写文件      格式：Write a1 buffer 20bytes                              ||" << endl;
	cout << "|| Read   读文件      格式：Read a1 buffer  20bytes读文件                        ||" << endl;
	cout << "|| dir    展示文件目录    格式：dir,将列出该用户下所有文件                       ||" << endl;
	cout << "|| Chmod  改变文件权限    格式：Chmod a1 r/w,将文件a1的权限修改为只读/只写       ||" << endl;
	cout << "|| Chown  改变文件拥有者  格式：Chown a1 admin,将文件名为a1的文件拥有者改为admin ||" << endl;
	cout << "|| Mv	   修变文件名      格式：Mv a1 b1,将文件a1重命名为b1                      ||" << endl;
	cout << "|| Copy   文件拷贝        格式：Copy a1 b1,将文件b1拷贝到文件a1中                || " << endl;
	cout << "|| Type   显示文件内容    格式：Type a1,将显示文件名为a1的内容                   ||" << endl;
	cout << "|| Exit   退出系统        格式：Exit,将退出并关闭文件系统                        ||" << endl;
	cout << "|| sysc   同步到磁盘      格式：sysc,将文件同步到物理磁盘Analogdisk内            ||" << endl;
	cout << "—————————————————————————————————————————- " << endl;
}

//文件写入磁盘===============================================================================
void doTempWrite()
{
	//Write	filename buffer nbytes 写文件   物理空间68

	char buf[1024];
	stringstream ss;
	ss << cmd_in.cmd_num[3];
	int temp;
	ss >> temp;


	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			if (FileInfo[curID][i].mode == 1 || FileInfo[curID][i].mode == 2)//判断权限
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
	//起始物理块
	int address;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			address = FileInfo[curID][i].addr;
			break;
		}
	}
	//注意：此处发生了更改！
	/*cout << "Please enter the buff：" << endl;
	gets(buf);
	fflush(stdin);*/


	strcpy(buf, cmd_in.cmd_num[2].c_str());

	int wbegin;
	wbegin = FileState[curID][index].write_poit;

	//找到写指针所在的最后一个磁盘
	while (FileCluster[address].next_num != address)
		address = FileCluster[address].next_num;

	vector <int> newspace_num;//计算将要占用的物理块的数量
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

	//更新写指针
	FileState[curID][index].write_poit = wbegin;
	cout << "Written to disk!" << endl;
	return;

}

//清屏========================================================================================= 
void do_Clear()
{
	system("cls");
}

#endif