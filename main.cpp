#include <iostream>
#include "ssp_pair.h"
#include "entity.h"
#include "sm3_reference.h"
#include <iomanip>
#include <ctime>
#include <fstream>
#include "EccTest.h"
//
using namespace std;


#define CONTENT "This is a content"


int kdf(unsigned char *zl, unsigned char *zr, int klen, unsigned char *kbuf) {
/*
return 0: kbuf is 0, unusable
       1: kbuf is OK
*/
    unsigned char buf[70];
    unsigned char digest[32];
    unsigned int ct = 0x00000001;
    int i, m, n;
    unsigned char *p;

    memcpy(buf, zl, 32);
    memcpy(buf + 32, zr, 32);

    m = klen / 32;
    n = klen % 32;
    p = kbuf;

    for (i = 0; i < m; i++) {
        buf[64] = (ct >> 24) & 0xFF;
        buf[65] = (ct >> 16) & 0xFF;
        buf[66] = (ct >> 8) & 0xFF;
        buf[67] = ct & 0xFF;
        SM3Calc(buf, 68, p);
        p += 32;
        ct++;
    }

    if (n != 0) {
        buf[64] = (ct >> 24) & 0xFF;
        buf[65] = (ct >> 16) & 0xFF;
        buf[66] = (ct >> 8) & 0xFF;
        buf[67] = ct & 0xFF;
        SM3Calc(buf, 68, digest);
    }

    memcpy(p, digest, n);

    for (i = 0; i < klen; i++) {
        if (kbuf[i] != 0)
            break;
    }

    if (i < klen)
        return 1;
    else
        return 0;
}

int main() {
//    Big x1, y1; // c1 
//    Big x2, y2;
//    unsigned char zl[32], zr[32], msg[32];
//    int klen =128;
//    big_to_bytes(32, x2.getbig(), (char *) zl, TRUE);
//    big_to_bytes(32, y2.getbig(), (char *) zr, TRUE);
//    kdf(zl, zr, klen, msg);



    int parameter = 0, menu;
    Groups groups = *new Groups();
    groups.groupNum = 2;
    AllGlobeSetup();
    for (int i = 0; i < groups.groupNum; i++) {
        Group group = *new Group(i);
        group.globeSetup(parameter);
        groups.groups.push_back(group);
    }
    //����
//    for (i = 0; i < 16; i++) block[i] = i;


    while (1) {
        cout << "�˵���1.ѡ��ͨ�Ŷ���  2.���ͼ�����Ϣ��ǩ��  3.������Ϣ��֤ǩ��  4.ɾ����Ա  5.������Ա  6.Ⱥ���� 7. Ⱥ�ϲ� 8.�˳�"
             << endl;
        cout << "���������ѡ�";
        cin >> menu;
        switch (menu) {
            case 1: {
                MultiGroupSessionKey(groups);
                break;
            }
            case 2: {
                encryptMessage(&groups.groups[0]);
                break;
            }
            case 3: {
                decryptMessage(&groups.groups[0]);
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
                Group *group = &groups.groups[num];
                if (group->memberNmuber == 0) {
                    cout << "ϵͳû�г�Ա������ɾ����" << endl;
                } else {
                    cout << "ϵͳ��ǰ��Աλ�����£�" << endl;
                    for (int i = 0; i < groupSize; i++) {
                        if (group->loc[i] == 1) cout << i << " ";
                    }
                    cout << endl;
                    cout << "������ͨ�Ŷ����λ�ã����롰-1��������";
                    while (true) {
                        cin >> leaveNumber;
                        if (leaveNumber == -1) {
                            cout << "�������ɻỰ��Կ�����Ժ򡭡�" << endl;
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
                Group *group = &groups.groups[num];
                if (group->memberNmuber >= groupSize) {
                    cout << "ϵͳ��Ա�����Ѵ����ޣ��޷���������Ա��" << endl;
                } else {
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
                splitting(&groups.groups[num], &groups, Users,splitUser);
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
                vector<Group *> GroupID;
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
                combine(GroupID, &groups);
                break;
            }
            case 8:
                return 0;
        }
    }

    return 0;
}
