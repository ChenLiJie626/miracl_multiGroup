#include <iostream>
#include "ssp_pair.h"
#include "entity.h"
#include <iomanip>
#include <ctime>
#include <fstream>
#include "EccTest.h"
//
using namespace std;


#define CONTENT "This is a content"



int main() {
    //    Big x1, y1; // c1
    //    Big x2, y2;
    //    unsigned char zl[32], zr[32], msg[32];
    //    int klen =128;
    //    big_to_bytes(32, x2.getbig(), (char *) zl, TRUE);
    //    big_to_bytes(32, y2.getbig(), (char *) zr, TRUE);
    //    kdf(zl, zr, klen, msg);


    AllGlobeSetup();
    int parameter = 0, menu;
    Groups groups = *new Groups();
    cout << "请输入创建的群组数量:\n";
    cin >> groups.groupNum;
    for (int i = 0; i < groups.groupNum; i++) {
        cout << "请输入Group " << i << " 创建的人数:\n";
        int existNumber;
        cin >> existNumber;
        Group group = *new Group(i);
        group.globeSetup(existNumber);
        groups.groups.push_back(group);
    }
    //明文
//    for (i = 0; i < 16; i++) block[i] = i;


    while (1) {
        cout << "菜单：1.选择通信对象  2.发送加密消息且签名  3.接收消息验证签名  4.删除成员  5.新增成员  6.群分裂 7. 群合并 8.工作量证明 9.退出"
             << endl;
        cout << "请输入操作选项：";
        cin >> menu;
        switch (menu) {
            case 1: {
                MultiGroupSessionKey(groups);
                break;
            }
            case 2: {
                time_t start, finish;
                start = clock();
                encryptMessage(&groups.groups[0]);
                finish = clock();
                double llll = (double)(finish - start) / CLOCKS_PER_SEC;
                cout << "time:" << llll << endl;
                break;
            }
            case 3: {
                time_t start, finish;
                start = clock();
                decryptMessage(&groups.groups[0]);
                finish = clock();
                double llll = (double)(finish - start) / CLOCKS_PER_SEC;
                cout << "time:" << llll << endl;
                break;
            }
            case 4: {
                cout << "请选择GroupID:\n";
                for (int i = 0; i < groups.groupNum; i++) {
                    if (groups.leave[i]) continue;
                    cout << i << " ";
                }
                cout << endl;
                int num, leaveNumber;
                cin >> num;
                Group* group = &groups.groups[num];
                if (group->memberNmuber == 0) {
                    cout << "系统没有成员，不能删除。" << endl;
                }
                else {
                    cout << "系统当前成员位置如下：" << endl;
                    for (int i = 0; i < groupSize; i++) {
                        if (group->loc[i] == 1) cout << i << " ";
                    }
                    cout << endl;
                    cout << "请输入通信对象的位置，输入“-1”结束：";
                    while (true) {
                        cin >> leaveNumber;
                        if (leaveNumber == -1) {
                            cout << "正在删除，请稍候……" << endl;
                            break;
                        }
                        group->leaveUser.push_back(leaveNumber);
                    }
                    leave(group);
                }
                break;
            }
            case 5: {
                cout << "请选择GroupID:\n";
                for (int i = 0; i < groups.groupNum; i++) {
                    if (groups.leave[i]) continue;
                    cout << i << " ";
                }
                cout << endl;
                int num, joinNumber;
                cin >> num;
                Group* group = &groups.groups[num];
                if (group->memberNmuber >= groupSize) {
                    cout << "系统成员人数已达上限，无法再新增成员。" << endl;
                }
                else {
                    cout << "系统空缺位置如下：" << endl;
                    for (int i = 0; i < groupSize; i++) {
                        if (group->loc[i] == 0) cout << i << " ";
                    }
                    cout << endl;
                    cout << "请输入新增对象的位置，输入“-1”结束：";
                    while (true) {
                        cin >> joinNumber;
                        if (joinNumber == -1) {
                            cout << "正在新增，请稍候……" << endl;
                            break;
                        }
                        group->joinUser.push_back(joinNumber);
                    }
                    join(group);
                }
                break;
            }
            case 6: {
                cout << "请选择GroupID:\n";
                for (int i = 0; i < groups.groupNum; i++) {
                    if (groups.leave[i]) continue;
                    cout << i << " ";
                }
                cout << endl;
                int num;
                cin >> num;
                cout << "Group " << groups.groups[num].GroupID << "系统当前成员位置如下：" << endl;
                int userID, count;
                for (int i = 0; i < groupSize; i++) {
                    if (groups.groups[num].loc[i]) cout << i << " ";
                }
                cout << endl;
                cout << "请输入分裂数量后群数量" << endl;
                cin >> count;
                vector<Entity> Users;
                vector<int> splitUser;
                Users.clear();
                splitUser.clear();
                for (int i = 1; i <= count; i++) {
                    cout << "第" << i << "组的用户ID，输入-1截止：\n ";
                    vector<int> tmp;
                    tmp.clear();
                    while (true) {
                        int x;
                        cin >> x;
                        if (x == -1) break;
                        tmp.push_back(x);
                    }
                    if (i == 1) continue;
                    splitUser.push_back(tmp.size());
                    for (int k = 0; k < tmp.size(); k++) {
                        groups.groups[num].leaveUser.push_back(tmp[k]);
                        groups.groups[num].loc[tmp[k]] = 0;
                        Users.push_back(groups.groups[num].entityList[tmp[k]]);
                        splitUser.push_back(tmp[k]);
                    }
                }
                time_t start, finish;
                start = clock();
                splittingAdd(&groups, Users, splitUser, &groups.groups[num]);
                splittingLeave(&groups.groups[num], Users);
                finish = clock();
                double lll = (double)(finish - start) / CLOCKS_PER_SEC;
                cout << "time:" << lll << endl;
                break;
            }
            case 7: {
                for (int i = 0; i < groups.groupNum; i++) {
                    if (groups.leave[i]) continue;
                    cout << i << " ";
                }
                cout << endl;
                cout << "请输入GroupID，输入“-1”结束：";
                int num;
                vector<Group*> GroupID;
                set<int> tmp;
                while (true) {
                    cin >> num;
                    tmp.insert(num);
                    if (num == -1) {
                        cout << "正在进行合并，请稍候……" << endl;
                        break;
                    }
                    GroupID.push_back(&groups.groups[num]);
                }
                time_t start, finish;
                start = clock();
                combine(GroupID, &groups);
                finish = clock();
                double lll = (double)(finish - start) / CLOCKS_PER_SEC;
                cout << "time:" << lll << endl;
                break;
            }
            case 8: {
                time_t start, finish;
                start = clock();
                Block* p = new Block("Genesis Block", "");
                p->ProofOfWork(DifficultyNum);
                finish = clock();
                double llll = (double)(finish - start) / CLOCKS_PER_SEC;
                cout << "time:" << llll << endl;
                break;
            }
            case 9:
                return 0;
        }
    }

    return 0;
}
