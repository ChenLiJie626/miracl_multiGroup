#include "entity.h"

int memberNmuber;
PFC curve(80);
Big orderG1;
G1 Ps[groupSize];
G1 generator, Q, D[groupSize];
G1 RG[groupSize], FG[groupSize][groupSize];
int st[groupSize]; ///st�ַ������������ʾ
set<int> Uset, Sset, Rset;
G1 publicKey;
G1 W1, W2;
GT SK;
char block[16];

Entity::Entity()
{

}


///��ʼ��step1
void Entity::initializeStep1(int number,Group *group) {
    this->number = number;
    time_t seed;
    time(&seed);
    irand((long) seed);

    int i;
    Big r, s;

    ///Step1
    curve.random(r);
    curve.random(s);
    curve.precomp_for_mult(group->generator);
    Ris = curve.mult(group->generator, r);
    curve.precomp_for_mult(group->generator);
    Pis = curve.mult(group->generator, s);  //1)initialize
    for (i = 0; i < groupSize; i++) {
        curve.precomp_for_mult(group->B[i]);
        G1 temp1 = curve.mult(group->B[i], r);
        curve.precomp_for_mult(group->Q);
        G1 temp2 = curve.mult(group->Q, s);
        Fis[i] = temp1 + temp2;  //2)initialize
    }

}
void Entity::initializeStep2(Group *group) {
    curve.precomp_for_mult(group->generator);
    G1 origin = curve.mult(group->generator, group->orderG1);
    G1 sumRis = origin, sumRr = origin;


    int i, j;
    for (i = 0; i < memberNmuber; i++)
        sumRis = sumRis + group->entityList[i].Ris;
    for (i = memberNmuber; i < groupSize; i++)
        sumRr = sumRr + group->RG[i];
    pubEncKey = sumRis + sumRr;
    group->publicKey = pubEncKey;      //1)initialize

    G1 sumFis = origin, sumFr = origin;
    for (i = 0; i < memberNmuber; i++) {
        if (i != number)
            sumFis = sumFis + group->entityList[i].Fis[number];
    }
    for (i = memberNmuber; i < groupSize; i++) {
        if (i != number)
            sumFis = sumFis + group->FG[i][number];
    }
    S[number] = sumFis + sumFr; //2)initialize
    decKey = S[number] + Fis[number]; //3)initialize
}

void Entity::getSessionKey(Group *group) {
    G1 Si = decKey;
    int i;
    for (i = 0; i < groupSize; i++) {
        if (group->unselectUser.count(i)) {
            Si = Si + group->FG[i][number];
        }
    }
    curve.precomp_for_pairing(Si);
    curve.precomp_for_pairing(group->W1);
    GT temp1 = curve.pairing(Si, group->W1);
    curve.precomp_for_mult(group->generator);
    G1 W2Inve = curve.mult(group->generator, group->orderG1) + (-group->W2);
    curve.precomp_for_pairing(W2Inve);
    curve.precomp_for_pairing(group->B[number]);
    GT temp2 = curve.pairing(W2Inve, group->B[number]);
    sessionKey = temp1 * temp2;

    if (group->SK != sessionKey) cout << "�Ự��Կ���ɳ���" << endl;
    if (group->SK == sessionKey) {
        cout << "��Ա"<<number<<"�Ự��Կ��ȷ��" << endl;
        cout << sessionKey.g << endl;
    }
}





void Entity::joinUpdate(vector<Entity> Users, Group *group) {
    int i;
    for (i = 0; i < groupSize; i++) {
        for (int j = 0; j < Users.size(); j++) {
            Entity user = Users[j];
            if (i == user.number) continue;
            curve.precomp_for_mult(group->generator);
            G1 FGiInve = curve.mult(group->generator, group->orderG1) + (-group->FG[user.number][i]);
            S[i] = S[i] + user.Fis[i] + FGiInve;
        }
    }
    for (int j = 0; j < Users.size(); j++) {
        Entity user = Users[j];
        curve.precomp_for_mult(group->generator);
        G1 RGiInve = curve.mult(group->generator, group->orderG1) + (-group->RG[user.number]);
        pubEncKey = pubEncKey + user.Ris + RGiInve;
        group->publicKey = pubEncKey;
    }
    for (int j = 0; j < Users.size(); j++) {
        Entity user = Users[j];
        curve.precomp_for_mult(group->generator);
        G1 FGiInve = curve.mult(group->generator, group->orderG1) + (-group->FG[user.number][number]);
        decKey = decKey + user.Fis[number] + FGiInve;
    }
}

void Entity::leaveUpdate(vector<Entity> Users, Group *group) {
    int i;

    for (i = 0; i < groupSize; i++) {
        for (int j = 0; j < Users.size(); j++) {
            Entity user = Users[j];
            if (i == user.number) continue;
            curve.precomp_for_mult(group->generator);
            G1 FiInve = curve.mult(group->generator, group->orderG1) + (-user.Fis[i]);
            S[i] = S[i] + FiInve + group->FG[user.number][i];
        }
    }

    for (int j = 0; j < Users.size(); j++) {
        Entity user = Users[j];
        curve.precomp_for_mult(group->generator);
        G1 RiInve = curve.mult(group->generator, group->orderG1) + (-user.Ris);
        pubEncKey = pubEncKey + RiInve + group->RG[user.number];
        group->publicKey = pubEncKey;
    }
    for (int j = 0; j < Users.size(); j++) {
        Entity user = Users[j];
        curve.precomp_for_mult(group->generator);
        G1 FiInve = curve.mult(group->generator, group->orderG1) + (-user.Fis[number]);
        decKey = decKey + FiInve + group->FG[user.number][number];
    }
}

void join(Group *group) {
    if (memberNmuber >= groupSize) {
        cout << "ϵͳ��Ա�����Ѵ����ޣ��޷���������Ա��" << endl;
    } else {
        int joinNumber, i, j;
        vector<Entity> Users;
        Users.clear();
        group->joinUser.clear();
        cout << "ϵͳ��ȱλ�����£�" << endl;
        for (i = 0; i < groupSize; i++) {
            if (group->loc[i] == 0) cout << i << " ";
        }
        cout << endl;
        cout << "���������������λ�ã����롰-1��������";
        while (1) {
            cin >> joinNumber;
            if (joinNumber == -1) {
                cout << "�������������Ժ򡭡�" << endl;
                break;
            }
            Entity user = *new Entity();
            user.initializeStep1(joinNumber, group);
            group->joinUser.push_back(joinNumber);
            Users.push_back(user);
            group->entityList[joinNumber] = user;
        }


        for (i = 0; i < groupSize; i++) {
            if (group->loc[i] == 0) continue;
            group->entityList[i].joinUpdate(Users, group);
        }
        for (int k = 0; k < group->joinUser.size(); k++) {
            joinNumber = group->joinUser[k];
            for (i = 0; i < groupSize; i++) {
                if (group->loc[i] == 1) {
                    for (j = 0; j < groupSize; j++) {
                        group->entityList[joinNumber].S[j] = group->entityList[i].S[j];
                    }
                    group->entityList[joinNumber].pubEncKey = group->entityList[i].pubEncKey;
                    group->entityList[joinNumber].setDecKey();
                    break;
                }
            }
        }
        for (int k = 0; k < group->joinUser.size(); k++) {
            joinNumber = group->joinUser[k];
            group->loc[joinNumber] = 1;
            memberNmuber++;
        }
        cout << "������Ա�ɹ���" << endl;
    }
}
void leave(Group *group) {
    if (memberNmuber == 0) {
        cout << "ϵͳû�г�Ա������ɾ����" << endl;
    } else {
        int leaveNumber, i;
        vector<Entity> Users;
        Users.clear();
        cout << "ϵͳ��ǰ��Աλ�����£�" << endl;
        for (i = 0; i < groupSize; i++) {
            if (group->loc[i] == 1) cout << i << " ";
        }
        cout << endl;
        cout << "������ͨ�Ŷ����λ�ã����롰-1��������";
        while (1) {
            cin >> leaveNumber;
            if (leaveNumber == -1) {
                cout << "�������ɻỰ��Կ�����Ժ򡭡�" << endl;
                break;
            }
            group->leaveUser.push_back(leaveNumber);
            group->loc[leaveNumber] = 0;
            Users.push_back(group->entityList[leaveNumber]);
        }


        for (i = 0; i < groupSize; i++) {
            if (group->loc[i] == 0) continue;
            group->entityList[i].leaveUpdate(Users, group);
        }
        memberNmuber-=group->leaveUser.size();
        cout << "ɾ����Ա�ɹ���" << endl;
    }
}



void Entity::setDecKey()
{
    decKey = S[number] + Fis[number];
}

void groupSessionKey(Group* group) {
    group->selectUser.clear();
    group->liveUser.clear();
    group->unselectUser.clear();
    time_t seed;
    time(&seed);
    irand((long) seed);

    cout << "Group "<<group->GroupID<<"ϵͳ��ǰ��Աλ�����£�" << endl;
    int userID, i;
    for (i = 0; i < groupSize; i++) {
        if (group->loc[i]) cout << i << " ";
    }
    cout << endl;
    cout << "������ͨ�Ŷ����λ�ã����롰-1��������";
    while (1) {
        cin >> userID;
        if (userID == -1) {
            cout << "�������ɻỰ��Կ�����Ժ򡭡�" << endl;
            break;
        }
        group->selectUser.insert(userID);
    }

    time_t start, finish;
    start = clock();

    for (i = 0; i < groupSize; i++) {
        if (group->loc[i]) {
            group->liveUser.insert(i);
            if (!group->selectUser.count(i)) {
                group->unselectUser.insert(i);
            }
        }
    }

    G1 E = group->publicKey;
    for (i = 0; i < groupSize; i++) {
        if (group->unselectUser.count(i)) {
            E = E + group->RG[i];
        }
    }
    Big d;
    curve.random(d);
    curve.precomp_for_mult(group->generator);
    curve.precomp_for_mult(E);
    group->W1 = curve.mult(group->generator, d);
    group->W2 = curve.mult(E, d);
    curve.precomp_for_mult(group->generator);
    G1 origin = curve.mult(group->generator, group->orderG1);
    G1 temp1 = origin, temp2 = origin, temp3 = origin;
    for (i = 0; i < groupSize; i++) {
        if (group->liveUser.count(i)) {
            temp1 = temp1 + group->entityList[i].Pis;
            if (group->unselectUser.count(i)) {
                temp3 = temp3 + group->PG[i];
            }
        } else {
            temp2 = temp2 + group->PG[i];
        }
    }
    G1 temp4 = temp1 + temp2 + temp3;
    curve.precomp_for_mult(temp4);
    G1 temp = curve.mult(temp4, d);
    curve.precomp_for_pairing(temp);
    curve.precomp_for_pairing(group->Q);
    group->SK = curve.pairing(temp, group->Q);

    cout<<"group->SK: "<<group->SK.g<<endl;

    finish = clock();
    double lll = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "time:" << lll << endl;

    for (i = 0; i < groupSize; i++) {
        if (group->selectUser.count(i))
            group->entityList[i].getSessionKey(group);
    }


    cout << "�Ự��Կ���ɳɹ���" << endl;
}
Group::Group(int GroupID) {
    this->GroupID = GroupID;
}

void Group::globeSetup(int securityParameter) {
    ///�ȼ���ϵͳ��Χ����
    cout << "���Ժ򡭡�" << endl;

    time_t start, finish;
    start = clock();

    orderG1 = curve.order(); ///G1�Ľ�

    time_t seed;
    time(&seed);
    irand((long) seed);
    int i, j;

    curve.random(generator); ///��ȡ����Ԫ
    curve.random(Q);

    Big r, s;

    for (i = 0; i < groupSize; i++)
        curve.random(B[i]);
    for (i = 0; i < groupSize; i++) {
        curve.random(r);
        curve.random(s);
        curve.precomp_for_mult(generator);
        RG[i] = curve.mult(generator, r);
        curve.precomp_for_mult(generator);
        PG[i] = curve.mult(generator, s);   // 1��GlobeSetup
        for (j = 0; j < groupSize; j++) {
            if (i == j) continue;
            curve.precomp_for_mult(B[j]);
            G1 temp1 = curve.mult(B[j], r);
            curve.precomp_for_mult(Q);
            G1 temp2 = curve.mult(Q, s);
            FG[i][j] = temp1 + temp2;// 2��GlobeSetup
        }
    }

    finish = clock();
    double lll = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "time:" << lll << endl;


    cout << "��ǰϵͳ����ĳ�Ա�������ֵΪ��" << groupSize << endl;

    /*do{
        cout<<"������һ��С�����ֵ��������Ϊ�˴�ϵͳ�Ĳ���������";
        cin>>memberNmuber;
    }while(memberNmuber>groupSize);*/
    memberNmuber = 5;
    cout << "���ݳ�ʼ���У����Ժ򡭡�" << endl;
    start = clock();
    for (i = 0; i < memberNmuber; i++) {
        Entity user = *new Entity();
        user.initializeStep1(i,this); ///��ʼ��step1
        //user.GroupID = GroupID;
        entityList.push_back(user);
        loc[i] = 1;
    }

    for (i = 0; i < memberNmuber; i++) {
        entityList[i].initializeStep2(this); ///������Կstep2
    }
    finish = clock();
    double llll = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "time:" << llll << endl;
    updateSi();
    cout << "��ʼ����ɡ�" << endl;


}
void Group::updateSi() {
    for (int i = 0; i < this->entityList.size(); i++) {
        if (loc[i] == 0) continue;
        for (int j = 0; j < this->entityList.size(); j++) {
            if (loc[j] == 0) continue;
            this->entityList[i].S[j] = this->entityList[j].S[j];
        }
    }

}