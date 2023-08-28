#include "entity.h"

int t = 10;
int memberNmuber;
PFC curve(80);
Big orderG1;
G1 PG[groupSize], RG[groupSize];
G1 generator, Q, B[groupSize];
G1 FG[groupSize][groupSize];
int st[groupSize]; ///st�ַ������������ʾ
set<int> Uset, Sset, Rset;
G1 publicKey;
G1 W1, W2;
GT SK;
char block[16];

entity::entity() {

}


///��ʼ��step1
void entity::initializeStep1() {

    time_t seed;
    time(&seed);
    irand((long) seed);

    int i;
    Big r, s;

    ///Step1
    curve.random(r);
    curve.random(s);
    curve.precomp_for_mult(generator);
    Ris = curve.mult(generator, r);
    curve.precomp_for_mult(generator);
    Pis = curve.mult(generator, s);  //1)initialize
    for (i = 0; i < groupSize; i++) {
        curve.precomp_for_mult(B[i]);
        G1 temp1 = curve.mult(B[i], r);
        curve.precomp_for_mult(Q);
        G1 temp2 = curve.mult(Q, s);
        Fis[i] = temp1 + temp2;  //2)initialize
    }

}

///��ʼ��step2
void entity::initializeStep2(entity user[]) {
    curve.precomp_for_mult(generator);
    G1 origin = curve.mult(generator, orderG1);
    G1 sumRis = origin, sumRr = origin;

    int i, j;
    for (i = 0; i < memberNmuber; i++)
        sumRis = sumRis + user[i].Ris;
    for (i = memberNmuber; i < groupSize; i++)
        sumRr = sumRr + RG[i];
    pubEncKey = sumRis + sumRr;
    publicKey = pubEncKey;      //1)initialize

    for (i = 0; i < groupSize; i++) {
        G1 sumF = origin, sumFG = origin;

        if (i == 0) {
            sumF = user[1].Fis[i];
            j = 2;
        } else {
            sumF = user[0].Fis[i];
            j = 1;
        }
        for (; j < memberNmuber; j++) {
            if (i == j) continue;
            sumF = sumF + user[j].Fis[i];
        }

        if (memberNmuber < groupSize) {
            if (i == memberNmuber) {
                if ((memberNmuber + 1) != groupSize) {
                    j++;
                    sumFG = FG[j][i];
                    j++;
                }

            } else {
                sumFG = FG[j][i];
                j++;
            }
            for (; j < groupSize; j++) {
                if (i == j) continue;
                sumFG = sumFG + FG[j][i];
            }
        }

        S[i] = sumF + sumFG;
    }

    decKey = S[number] + Fis[number]; //3)initialize
}

void entity::joinUpdate(entity joinUser) {
    int joinNumber = joinUser.number;
    int i;

    for (i = 0; i < groupSize; i++) {
        if (i == joinNumber) continue;
        curve.precomp_for_mult(generator);
        G1 FGiInve = curve.mult(generator, orderG1) + (-FG[joinNumber][i]);
        S[i] = S[i] + joinUser.Fis[i] + FGiInve;
    }

    curve.precomp_for_mult(generator);
    G1 RGiInve = curve.mult(generator, orderG1) + (-RG[joinNumber]);
    pubEncKey = pubEncKey + joinUser.Ris + RGiInve;
    publicKey = pubEncKey;

    curve.precomp_for_mult(generator);
    G1 FGiInve = curve.mult(generator, orderG1) + (-FG[joinNumber][number]);
    decKey = decKey + joinUser.Fis[number] + FGiInve;
}

void entity::leaveUpdate(entity leaveUser) {
    int leaveNumber = leaveUser.number;
    int i;

    for (i = 0; i < groupSize; i++) {
        if (i == leaveNumber) continue;
        curve.precomp_for_mult(generator);
        G1 FiInve = curve.mult(generator, orderG1) + (-leaveUser.Fis[i]);
        S[i] = S[i] + FiInve + FG[leaveNumber][i];
    }

    curve.precomp_for_mult(generator);
    G1 RiInve = curve.mult(generator, orderG1) + (-leaveUser.Ris);
    pubEncKey = pubEncKey + RiInve + RG[leaveNumber];
    publicKey = pubEncKey;

    curve.precomp_for_mult(generator);
    G1 FiInve = curve.mult(generator, orderG1) + (-leaveUser.Fis[number]);
    decKey = decKey + FiInve + FG[leaveNumber][number];
}

void entity::getSessionKey() {
    G1 Si = decKey;
    int i;
    for (i = 0; i < groupSize; i++) {
        if (Rset.count(i)) {
            Si = Si + FG[i][number];
        }
    }
    curve.precomp_for_pairing(Si);
    curve.precomp_for_pairing(W1);
    GT temp1 = curve.pairing(Si, W1);
    curve.precomp_for_mult(generator);
    G1 W2Inve = curve.mult(generator, orderG1) + (-W2);
    curve.precomp_for_pairing(W2Inve);
    curve.precomp_for_pairing(B[number]);
    GT temp2 = curve.pairing(W2Inve, B[number]);
    sessionKey = temp1 * temp2;

    if (SK != sessionKey) cout << "�Ự��Կ���ɳ���" << endl;
    if (SK == sessionKey) {
        cout << "�Ự��Կ��ȷ��" << endl;
        cout << sessionKey.g << endl;
    }
}

int entity::decryptMessage() {
    int i, nk = 16;
    aes a;
    char key[32];
    char iv[16];
    for (i = 0; i < 16; i++) iv[i] = i;

    curve.start_hash();
    curve.add_to_hash(SK);
    shs256_hash(&(curve.SH), key);

    if (!aes_init(&a, MR_CBC, nk, key, iv)) {
        printf("AES��ʼ��ʧ�ܡ�\n");
        return 0;
    }

    char cipher[16];
    for (i = 0; i < 16; i++) {
        cipher[i] = block[i];
    }

    aes_decrypt(&a, cipher);
    cout << "��Ա" << number;
    printf("���ܣ� ");
    for (i = 0; i < 4 * NB; i++) printf("%02x", cipher[i]);
    printf("\n");

    aes_end(&a);
}

void entity::setDecKey() {
    decKey = S[number] + Fis[number];
}

void globeSetup(int securityParameter, entity user[], Group *group) {
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

    Big r[groupSize], s[groupSize];

    for (i = 0; i < groupSize; i++)
        curve.random(B[i]);
    for (i = 0; i < groupSize; i++) {
        curve.random(r[i]);
        curve.random(s[i]);
        curve.precomp_for_mult(generator);
        RG[i] = curve.mult(generator, r[i]);
        curve.precomp_for_mult(generator);
        PG[i] = curve.mult(generator, s[i]);   // 1��GlobeSetup
        for (j = 0; j < groupSize; j++) {
            if (i == j) continue;
            curve.precomp_for_mult(B[j]);
            G1 temp1 = curve.mult(B[j], r[i]);
            curve.precomp_for_mult(Q);
            G1 temp2 = curve.mult(Q, s[i]);
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
    cout << "������һ��С�����ֵ��������Ϊ�˴�ϵͳ�Ĳ���������\n";
    memberNmuber = 10;
    cout << "���ݳ�ʼ���У����Ժ򡭡�" << endl;
    start = clock();

    for (i = 0; i < memberNmuber; i++) {
        user[i].number = i;
        user[i].initializeStep1(); ///��ʼ��step1
        st[i] = 1;
        group->entityList.push_back(&user[i]);
        group->loc[i] = 1;

    }
    for (i = 0; i < memberNmuber; i++) {
        user[i].initializeStep2(user); ///������Կstep2
    }
    finish = clock();
    double llll = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "time:" << llll << endl;

    cout << "��ʼ����ɡ�" << endl;
}

void join(entity user[], Group *group) {
    if (memberNmuber >= groupSize) {
        cout << "ϵͳ��Ա�����Ѵ����ޣ��޷���������Ա��" << endl;
    } else {

        int joinNumber, i, j;
        cout << "ϵͳ��ȱλ�����£�" << endl;
        for (i = 0; i < groupSize; i++) {
            if (group->loc[i] == 0) cout << i << " ";
        }
        cout << endl;
        cout << "��ѡ��������Ա��λ�ã�";
        cin >> joinNumber;
        cout << "�������������Ժ򡭡�" << endl;
        user[joinNumber].number = joinNumber;
        user[joinNumber].initializeStep1();

        for (i = 0; i < groupSize; i++) {
            if (group->loc[i] == 0) continue;
            user[i].joinUpdate(user[joinNumber]);
        }

        for (i = 0; i < groupSize; i++) {
            if (group->loc[i] == 1) {
                for (j = 0; j < groupSize; j++) {
                    user[joinNumber].S[j] = user[i].S[j];
                }
                user[joinNumber].pubEncKey = user[i].pubEncKey;
                user[joinNumber].setDecKey();
                break;
            }
        }

        group->loc[joinNumber] = 1;
        memberNmuber++;
        cout << "������Ա�ɹ���" << endl;
    }
}

void leave(entity user[], Group *group) {
    if (memberNmuber == 0) {
        cout << "ϵͳû�г�Ա������ɾ����" << endl;
    } else {
        int leaveNumber, i;
        cout << "ϵͳ��ǰ��Աλ�����£�" << endl;
        for (i = 0; i < groupSize; i++) {
            if (group->loc[i]) cout << i << " ";
        }
        cout << endl;
        cout << "��ѡ��ɾ����Ա��λ�ã�";
        cin >> leaveNumber;
        cout << "����ɾ�������Ժ򡭡�" << endl;
        group->loc[leaveNumber] = 0;

        for (i = 0; i < groupSize; i++) {
            if (group->loc[i] == 0) continue;
            user[i].leaveUpdate(user[leaveNumber]);
        }

        memberNmuber--;
        cout << "ɾ����Ա�ɹ���" << endl;
    }
}

void genSessionKey(G1 pubEncKey, entity user[], Group *group) {
    Uset.clear();
    Sset.clear();
    Rset.clear();

    time_t seed;
    time(&seed);
    irand((long) seed);

    cout << "ϵͳ��ǰ��Աλ�����£�" << endl;
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
        Uset.insert(userID);
    }

    time_t start, finish;
    start = clock();

    for (i = 0; i < groupSize; i++) {
        if (group->loc[i]) {
            Sset.insert(i);
            if (!Uset.count(i)) {
                Rset.insert(i);
            }
        }
    }

    G1 E = pubEncKey;
    for (i = 0; i < groupSize; i++) {
        if (Rset.count(i)) {
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
        if (Sset.count(i)) {
            temp1 = temp1 + user[i].Pis;
            if (Rset.count(i)) {
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
        if (Uset.count(i))
            user[i].getSessionKey();
    }


    cout << "�Ự��Կ���ɳɹ���" << endl;
}

int encryptMessage() {
    int i, nk = 16;
    aes a;
    char key[32];
    char iv[16];
    for (i = 0; i < 16; i++) iv[i] = i;

    curve.start_hash();
    curve.add_to_hash(SK);
    shs256_hash(&(curve.SH), key);

    if (!aes_init(&a, MR_CBC, nk, key, iv)) {
        printf("AES��ʼ��ʧ�ܡ�\n");
        return 0;
    }

    printf("���ģ�   ");
    for (i = 0; i < 4 * NB; i++) printf("%02x", block[i]);
    printf("\n");
    aes_encrypt(&a, block);
    printf("���ģ� ");
    for (i = 0; i < 4 * NB; i++) printf("%02x", (unsigned char) block[i]);
    printf("\n");

    aes_end(&a);
}

void decryptMessage(entity user[]) {
    int i;
    for (i = 0; i < groupSize; i++) {
        if (Uset.count(i)) {
            user[i].decryptMessage();
        }
    }
}

Group::Group(int GroupID) {
    this->GroupID = GroupID;
}
