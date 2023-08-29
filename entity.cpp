#include "entity.h"

int t = 10;
int memberNmuber;
PFC curve(80);
char block[16];

Entity::Entity() {

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

///��ʼ��step2
void Entity::initializeStep2(Group *group) {
    curve.precomp_for_mult(group->generator);
    G1 origin = curve.mult(group->generator, group->orderG1);
    G1 sumRis = origin, sumRr = origin;


    int i, j;
    for (i = 0; i < memberNmuber; i++)
        sumRis = sumRis + group->entityList[i]->Ris;
    for (i = memberNmuber; i < groupSize; i++)
        sumRr = sumRr + group->RG[i];
    pubEncKey = sumRis + sumRr;
    group->publicKey = pubEncKey;      //1)initialize

    G1 sumFis = origin, sumFr = origin;
    for (i = 0; i < memberNmuber; i++) {
        if (i != number)
            sumFis = sumFis + group->entityList[i]->Fis[number];
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
        cout << "�Ự��Կ��ȷ��" << endl;
        cout << sessionKey.g << endl;
    }
}



void Entity::setDecKey() {
    decKey = S[number] + Fis[number];
}






Group::Group(int GroupID) {
    this->GroupID = GroupID;
}

void Group::groupSessionKey() {
    time_t seed;
    time(&seed);
    irand((long) seed);

    cout << "Group "<<GroupID<<"ϵͳ��ǰ��Աλ�����£�" << endl;
    int userID, i;
    for (i = 0; i < groupSize; i++) {
        if (loc[i]) cout << i << " ";
    }
    cout << endl;
    cout << "������ͨ�Ŷ����λ�ã����롰-1��������";
    while (1) {
        cin >> userID;
        if (userID == -1) {
            cout << "�������ɻỰ��Կ�����Ժ򡭡�" << endl;
            break;
        }
        selectUser.insert(userID);
    }

    time_t start, finish;
    start = clock();

    for (i = 0; i < groupSize; i++) {
        if (loc[i]) {
            liveUser.insert(i);
            if (!selectUser.count(i)) {
                unselectUser.insert(i);
            }
        }
    }

    G1 E = publicKey;
    for (i = 0; i < groupSize; i++) {
        if (unselectUser.count(i)) {
            E = E + RG[i];
        }
    }
    Big d;
    curve.random(d);
    curve.precomp_for_mult(generator);
    curve.precomp_for_mult(E);
    W1 = curve.mult(generator, d);
    W2 = curve.mult(E, d);
    curve.precomp_for_mult(generator);
    G1 origin = curve.mult(generator, orderG1);
    G1 temp1 = origin, temp2 = origin, temp3 = origin;
    for (i = 0; i < groupSize; i++) {
        if (liveUser.count(i)) {
            temp1 = temp1 + entityList[i]->Pis;
            if (unselectUser.count(i)) {
                temp3 = temp3 + PG[i];
            }
        } else {
            temp2 = temp2 + PG[i];
        }
    }
    G1 temp4 = temp1 + temp2 + temp3;
    curve.precomp_for_mult(temp4);
    G1 temp = curve.mult(temp4, d);
    curve.precomp_for_pairing(temp);
    curve.precomp_for_pairing(Q);
    SK = curve.pairing(temp, Q);

    finish = clock();
    double lll = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "time:" << lll << endl;

    for (i = 0; i < groupSize; i++) {
        if (selectUser.count(i))
            entityList[i]->getSessionKey(this);
    }


    cout << "�Ự��Կ���ɳɹ���" << endl;
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
        Entity entity = Entity();
        entity.initializeStep1(i, this); ///��ʼ��step1
        entity.GroupID = GroupID;
        entityList.push_back(&entity);
        loc[i] = 1;
    }

    for (i = 0; i < memberNmuber; i++) {
        entityList[i]->initializeStep2(this); ///������Կstep2
    }

    finish = clock();
    double llll = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "time:" << llll << endl;

    cout << "��ʼ����ɡ�" << endl;
}
