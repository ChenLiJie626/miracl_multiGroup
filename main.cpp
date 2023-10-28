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
    cout << "�����봴����Ⱥ������:\n";
    cin >> groups.groupNum;
    for (int i = 0; i < groups.groupNum; i++) {
        cout << "������Group " << i << " ����������:\n";
        int existNumber;
        cin >> existNumber;
        Group group = *new Group(i);
        group.globeSetup(existNumber);
        groups.groups.push_back(group);
    }
    //����
//    for (i = 0; i < 16; i++) block[i] = i;


    while (1) {
        cout << "�˵���1.ѡ��ͨ�Ŷ���  2.���ͼ�����Ϣ��ǩ��  3.������Ϣ��֤ǩ��  4.ɾ����Ա  5.������Ա  6.Ⱥ���� 7. Ⱥ�ϲ� 8.������֤�� 9.�˳�"
             << endl;
        cout << "���������ѡ�";
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
                cout << "��ѡ��GroupID:\n";
                for (int i = 0; i < groups.groupNum; i++) {
                    if (groups.leave[i]) continue;
                    cout << i << " ";
                }
                cout << endl;
                int num, leaveNumber;
                cin >> num;
                Group* group = &groups.groups[num];
                if (group->memberNmuber == 0) {
                    cout << "ϵͳû�г�Ա������ɾ����" << endl;
                }
                else {
                    cout << "ϵͳ��ǰ��Աλ�����£�" << endl;
                    for (int i = 0; i < groupSize; i++) {
                        if (group->loc[i] == 1) cout << i << " ";
                    }
                    cout << endl;
                    cout << "������ͨ�Ŷ����λ�ã����롰-1��������";
                    while (true) {
                        cin >> leaveNumber;
                        if (leaveNumber == -1) {
                            cout << "����ɾ�������Ժ򡭡�" << endl;
                            break;
                        }
                        group->leaveUser.push_back(leaveNumber);
                    }
                    leave(group);
                }
                break;
            }
            case 5: {
                cout << "��ѡ��GroupID:\n";
                for (int i = 0; i < groups.groupNum; i++) {
                    if (groups.leave[i]) continue;
                    cout << i << " ";
                }
                cout << endl;
                int num, joinNumber;
                cin >> num;
                Group* group = &groups.groups[num];
                if (group->memberNmuber >= groupSize) {
                    cout << "ϵͳ��Ա�����Ѵ����ޣ��޷���������Ա��" << endl;
                }
                else {
                    cout << "ϵͳ��ȱλ�����£�" << endl;
                    for (int i = 0; i < groupSize; i++) {
                        if (group->loc[i] == 0) cout << i << " ";
                    }
                    cout << endl;
                    cout << "���������������λ�ã����롰-1��������";
                    while (true) {
                        cin >> joinNumber;
                        if (joinNumber == -1) {
                            cout << "�������������Ժ򡭡�" << endl;
                            break;
                        }
                        group->joinUser.push_back(joinNumber);
                    }
                    join(group);
                }
                break;
            }
            case 6: {
                cout << "��ѡ��GroupID:\n";
                for (int i = 0; i < groups.groupNum; i++) {
                    if (groups.leave[i]) continue;
                    cout << i << " ";
                }
                cout << endl;
                int num;
                cin >> num;
                cout << "Group " << groups.groups[num].GroupID << "ϵͳ��ǰ��Աλ�����£�" << endl;
                int userID, count;
                for (int i = 0; i < groupSize; i++) {
                    if (groups.groups[num].loc[i]) cout << i << " ";
                }
                cout << endl;
                cout << "���������������Ⱥ����" << endl;
                cin >> count;
                vector<Entity> Users;
                vector<int> splitUser;
                Users.clear();
                splitUser.clear();
                for (int i = 1; i <= count; i++) {
                    cout << "��" << i << "����û�ID������-1��ֹ��\n ";
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
                cout << "������GroupID�����롰-1��������";
                int num;
                vector<Group*> GroupID;
                set<int> tmp;
                while (true) {
                    cin >> num;
                    tmp.insert(num);
                    if (num == -1) {
                        cout << "���ڽ��кϲ������Ժ򡭡�" << endl;
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
