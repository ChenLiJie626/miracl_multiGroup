#include <iostream>
#include "ssp_pair.h"
#include "entity.h"
#include <iomanip>
#include <ctime>
//
using namespace std;

int main() {
    int i;
    Entity user[groupSize];
    Group group(1);
    int parameter, menu;
    cout << "请输入一个任意值，开始全局：";
    cin >> parameter;
    group.globeSetup(parameter);
    //明文
    for (i = 0; i < 16; i++) block[i] = i;


    while (1) {
        cout << "菜单：1.选择通信对象  2.加密  3.解密  4.删除成员  5.新增成员  6.退出" << endl;
        cout << "请输入操作选项：";
        cin >> menu;
        switch (menu) {
            case 1: {
                group.groupSessionKey();
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
//                leave(user,&group);
                break;
            }
            case 5: {
//                join(user,&group);
                break;
            }
            case 6:
                return 0;
        }
    }

    return 0;
}
