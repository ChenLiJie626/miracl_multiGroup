#include <iostream>
#include "ssp_pair.h"
#include "entity.h"
#include <iomanip>
#include <ctime>
//
using namespace std;

int main()
{
    int i;
    entity user[groupSize];
    int parameter, menu;
    cout<<"������һ������ֵ����ʼȫ�֣�";
    cin>>parameter;
    globeSetup(parameter, user);
    //����
    for (i=0;i<16;i++) block[i]=i;


    while(1)
    {
        cout<<"�˵���1.ѡ��ͨ�Ŷ���  2.����  3.����  4.ɾ����Ա  5.������Ա  6.�˳�"<<endl;
        cout<<"���������ѡ�";
        cin>>menu;
        switch(menu)
        {
        case 1:
        {
            genSessionKey(publicKey, user);
            break;
        }
        case 2:
        {
            encryptMessage();
            break;
        }
        case 3:
        {
            decryptMessage(user);
            break;
        }
        case 4:
        {
            leave(user);
            break;
        }
        case 5:
        {
            join(user);
            break;
        }
        case 6:
            return 0;
        }
    }

    return 0;
}
