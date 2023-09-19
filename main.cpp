#include <iostream>
#include "ssp_pair.h"
#include "entity.h"
#include <iomanip>
#include <ctime>
//
using namespace std;

int main() {
    int parameter=0, menu;
    Groups groups = *new Groups();
    groups.groupNum = 2;
    AllGlobeSetup();
    for(int i=0;i<groups.groupNum;i++) {
        Group group = *new Group(i);
        group.globeSetup(parameter);
        groups.groups.push_back(group);
    }
    //明文
//    for (i = 0; i < 16; i++) block[i] = i;


    while (1) {
        cout << "菜单：1.选择通信对象  2.加密  3.解密  4.删除成员  5.新增成员  6.群分裂 7.退出" << endl;
        cout << "请输入操作选项：";
        cin >> menu;
        switch (menu) {
            case 1: {
                MultiGroupSessionKey(groups);
                break;
            }
            case 2: {
//                encryptMessage();
                break;
            }
            case 3: {
//                decryptMessage(user);
                break;
            }
            case 4: {
                cout<<"请选择GroupID:\n";
                for(int i=0;i<groups.groupNum;i++) cout<<i<<" ";
                cout<<endl;
                int num;cin>>num;
                leave(&groups.groups[num]);
                break;
            }
            case 5: {
                cout<<"请选择GroupID:\n";
                for(int i=0;i<groups.groupNum;i++) cout<<i<<" ";
                cout<<endl;
                int num;cin>>num;
                join(&groups.groups[num]);
                break;
            }
            case 6: {
                cout<<"请选择GroupID:\n";
                for(int i=0;i<groups.groupNum;i++) cout<<i<<" ";
                cout<<endl;
                int num;cin>>num;
                splitting(&groups.groups[num], &groups);
                break;
            }
            case 7:
                return 0;
        }
    }

    return 0;
}
