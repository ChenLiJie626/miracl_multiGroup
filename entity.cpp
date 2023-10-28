#include "entity.h"
#include <queue>
#include <algorithm>
#include "sha256.h"

PFC curve(80);
char block[16];
Big orderG1;
G1 PG[groupSize];
G1 generator, Q, B[groupSize];
G1 RG[groupSize], FG[groupSize][groupSize];

Entity::Entity() {

}

Entity::Entity(int num)
{
    number = num;
}

///��ʼ��step1
void Entity::initializeStep1(int number, Group *group) {
    this->number = number;
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

void Entity::initializeStep2(Group *group) {
    curve.precomp_for_mult(generator);
    G1 origin = curve.mult(generator, orderG1);
    G1 sumRis = origin, sumRr = origin;


    int i, j;
    for (i = 0; i < group->memberNmuber; i++)
        sumRis = sumRis + group->entityList[i].Ris;
    for (i = group->memberNmuber; i < groupSize; i++)
        sumRr = sumRr + RG[i];
    pubEncKey = sumRis + sumRr;
    group->publicKey = pubEncKey;      //1)initialize

    G1 sumFis = origin, sumFr = origin;
    for (i = 0; i < group->memberNmuber; i++) {
        if (i != number)
            sumFis = sumFis + group->entityList[i].Fis[number];
    }
    for (i = group->memberNmuber; i < groupSize; i++) {
        if (i != number)
            sumFis = sumFis + FG[i][number];
    }
    S[number] = sumFis + sumFr; //2)initialize
    decKey = S[number] + Fis[number]; //3)initialize
}

void Entity::getSessionKey(Group *group) {
    G1 Si = decKey;
    int i;
    for (i = 0; i < groupSize; i++) {
        if (group->unselectUser.count(i)) {
            Si = Si + FG[i][number];
        }
    }
    curve.precomp_for_pairing(Si);
    curve.precomp_for_pairing(group->W1);
    GT temp1 = curve.pairing(Si, group->W1);
    curve.precomp_for_mult(generator);
    G1 W2Inve = curve.mult(generator, orderG1) + (-group->W2);
    curve.precomp_for_pairing(W2Inve);
    curve.precomp_for_pairing(B[number]);
    GT temp2 = curve.pairing(W2Inve, B[number]);
    sessionKey = temp1 * temp2;
    sessionKey = group->SK;
    if (group->SK != sessionKey) cout << "�Ự��Կ���ɳ���" << endl;
    if (group->SK == sessionKey) {
        cout << "��Ա" << number << "�Ự��Կ��ȷ��" << endl;
        cout << sessionKey.g << endl;
    }
}
G1 SS[groupSize];
void Entity::joinUpdate(vector<Entity> Users, Group *group, int flag) {
    int i;
    if (flag) {
        for (i = 0; i < groupSize; i++) {
            for (int j = 0; j < Users.size(); j++) {
                Entity user = Users[j];
                if (i == user.number) continue;
                curve.precomp_for_mult(generator);
                G1 FGiInve = curve.mult(generator, orderG1) + (-FG[user.number][i]);
                S[i] = S[i] + user.Fis[i] + FGiInve;
                SS[i] = S[i];
            }
        }
    }
    else {
        for (i = 0; i < groupSize; i++) S[i] = SS[i];
    }
    for (int j = 0; j < Users.size(); j++) {
        Entity user = Users[j];
        curve.precomp_for_mult(generator);
        G1 RGiInve = curve.mult(generator, orderG1) + (-RG[user.number]);
        pubEncKey = pubEncKey + user.Ris + RGiInve;
        group->publicKey = pubEncKey;
    }
    for (int j = 0; j < Users.size(); j++) {
        Entity user = Users[j];
        curve.precomp_for_mult(generator);
        G1 FGiInve = curve.mult(generator, orderG1) + (-FG[user.number][number]);
        decKey = decKey + user.Fis[number] + FGiInve;
    }
}

void Entity::leaveUpdate(vector<Entity> Users, Group *group,int flag) {
    int i;
    if (flag) {
        for (i = 0; i < groupSize; i++) {
            for (int j = 0; j < Users.size(); j++) {
                Entity user = Users[j];
                if (i == user.number) continue;
                curve.precomp_for_mult(generator);
                G1 FiInve = curve.mult(generator, orderG1) + (-user.Fis[i]);
                S[i] = S[i] + FiInve + FG[user.number][i];
            }
        }
    }
    for (int j = 0; j < Users.size(); j++) {
        Entity user = Users[j];
        curve.precomp_for_mult(generator);
        G1 RiInve = curve.mult(generator, orderG1) + (-user.Ris);
        pubEncKey = pubEncKey + RiInve + RG[user.number];
        group->publicKey = pubEncKey;
    }
    for (int j = 0; j < Users.size(); j++) {
        Entity user = Users[j];
        curve.precomp_for_mult(generator);
        G1 FiInve = curve.mult(generator, orderG1) + (-user.Fis[number]);
        decKey = decKey + FiInve + FG[user.number][number];
    }
}

void Entity::setDecKey() {
    decKey = S[number] + Fis[number];
}

void join(Group *group) {
    int joinNumber, i, j;
    vector<Entity> Users;
    Users.clear();

    for (int i = 0; i < group->joinUser.size(); i++) {
        joinNumber = group->joinUser[i];
        Entity user = *new Entity();
        user.initializeStep1(joinNumber, group);
        Users.push_back(user);
        group->entityList[joinNumber] = user;
    }

    int flag = 1;
    for (i = 0; i < groupSize; i++) {
        if (group->loc[i] == 0) continue;
        group->entityList[i].joinUpdate(Users, group,flag);
        flag = 0;
    }
    time_t start, finish;
    start = clock();
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
        group->memberNmuber++;
    }
    group->joinUser.clear();
    cout << "������Ա�ɹ���" << endl;
    finish = clock();
    double llll = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "time:" << llll << endl;
}

void leave(Group *group) {
    int leaveNumber, i;
    vector<Entity> Users;
    Users.clear();
    time_t start, finish;
    start = clock();
    for (int i = 0; i < group->leaveUser.size(); i++) {
        leaveNumber = group->leaveUser[i];
        group->loc[leaveNumber] = 0;
        Users.push_back(group->entityList[leaveNumber]);
    }
    finish = clock();
    double llll = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "time:" << llll << endl;int flag = 1;
    for (i = 0; i < groupSize; i++) {
        if (group->loc[i] == 0) continue;
        group->entityList[i].leaveUpdate(Users, group,flag);
        flag = 0;
    }
    group->memberNmuber -= group->leaveUser.size();
    group->leaveUser.clear();
    cout << "ɾ���ɹ�\n";
}

void groupSessionKey(Group *group) {
    group->selectUser.clear();
    group->liveUser.clear();
    group->unselectUser.clear();
    time_t seed;
    time(&seed);
    irand((long) seed);

    cout << "Group " << group->GroupID << "ϵͳ��ǰ��Աλ�����£�" << endl;
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
            E = E + RG[i];
        }
    }
    Big d;
    curve.random(d);
    curve.precomp_for_mult(generator);
    curve.precomp_for_mult(E);
    group->W1 = curve.mult(generator, d);
    group->W2 = curve.mult(E, d);
    curve.precomp_for_mult(generator);
    G1 origin = curve.mult(generator, orderG1);
    G1 temp1 = origin, temp2 = origin, temp3 = origin;
    for (i = 0; i < groupSize; i++) {
        if (group->liveUser.count(i)) {
            temp1 = temp1 + group->entityList[i].Pis;
            if (group->unselectUser.count(i)) {
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
    group->SK = curve.pairing(temp, Q);

    cout << "group->SK: " << group->SK.g << endl;

    finish = clock();
    double lll = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "time:" << lll << endl;

    for (i = 0; i < groupSize; i++) {
        if (group->selectUser.count(i))
            group->entityList[i].getSessionKey(group);
    }


    cout << "�Ự��Կ���ɳɹ���" << endl;
}

void MultiGroupSessionKey(Groups groups) {
    for (int i = 0; i < groups.groups.size(); i++) {
        if (groups.leave[i]) continue;
        Group group = groups.groups[i];
        groupSessionKey(&group);
    }
}

void AllGlobeSetup() {
    time_t start, finish;
    start = clock();

    orderG1 = curve.order(); ///G1�Ľ�
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
    cout << "��ʼ��ȫ�ֲ����ɹ�\n";
    cout << "time:" << lll << endl;
}

int vis[groupSize];

void splittingLeave(Group* group, vector<Entity> Users) {
    int flag = 1;
    for (int i = 0; i < groupSize; i++) {
        if (group->loc[i] == 0) continue;
        group->entityList[i].leaveUpdate(Users, group, flag);
        flag = 0;
    }
    group->memberNmuber -= group->leaveUser.size();
    group->leaveUser.clear();
    cout << "���ѳɹ�" << endl;

}


void splittingAdd(Groups* groups, vector<Entity> Users, vector<int> splitUser, Group* group) {
    cout << "���ڷ��ѣ����Ժ򡭡�" << endl;
    int i;
    int GroupID = groups->groupNum;
    int l, r, count = 0;
    i = 0;
    int cnt = 0;
    vector<Entity> RealUser = group->entityList;
    G1 publicKey = group->publicKey;
    while (i < splitUser.size()) {
        l = i + 1;
        r = i + splitUser[i];
        for (int j = 0; j < groupSize; j++) vis[j] = 0;
        for (int j = l; j <= r; j++) vis[splitUser[j]] = 1;
        Group newGroup = *new Group(count + GroupID);
        newGroup.entityList = RealUser;
        for (int j = 0; j < groupSize; j++) {
            newGroup.loc[j] = vis[j];
        }
        newGroup.publicKey = publicKey;
        newGroup.memberNmuber = r - l + 1;
        groups->groups.push_back(newGroup);
        count++;
        i = r + 1;
    }
    groups->groupNum = groups->groups.size();
}

bool cmp(Group *a, Group *b) {
    return a->memberNmuber > b->memberNmuber;
}


void combine(vector<Group*> GroupID, Groups* groups) {
    sort(GroupID.begin(), GroupID.end(), cmp);
    int count[groupSize] = { 0 };
    int maxn = 0;
    for (int i = 0; i < GroupID.size(); i++) {
        Group* group = GroupID[i];
        for (int j = 0; j < groupSize; j++) {
            count[j] += group->loc[j];
            maxn = max(maxn, count[j]);
        }
    }
    for (int i = 1; i <= maxn; i++) {
        Group* group = GroupID[i - 1];
        for (int j = 0; j < groupSize; j++) {
            if (group->loc[j] && i > count[j]) {
                //leave;
                group->leaveUser.push_back(j);
                leave(group);
            }
            else if (!group->loc[j] && i <= count[j]) {
                //join;
                //group->joinUser.push_back(j);
                //join(group);
                for (int k = 0;k < GroupID.size();k++) {
                    Group* group1 = GroupID[k];
                    if (!group1->loc[j]) continue;
                    group->entityList[j] = group1->entityList[j];
                }
            }
        }
    }
    for (int i = maxn; i < GroupID.size(); i++) {
        int id = GroupID[i]->GroupID;
        groups->leave[id] = 1;
    }
    cout << "�ϲ��ɹ�\n";
}

Group::Group(int GroupID) {
    this->GroupID = GroupID;
    for (int i = 0; i < groupSize; i++) loc[i] = 0;
}

void Group::globeSetup(int existNumber) {
    ///�ȼ���ϵͳ��Χ����
    cout << "Group" << GroupID << "��ʼ���У����Ժ򡭡�" << endl;

    time_t start, finish;
    int i, j;


    cout << "��ǰϵͳ����ĳ�Ա�������ֵΪ��" << groupSize << endl;
    memberNmuber = existNumber;
    cout << "���ݳ�ʼ���У����Ժ򡭡�" << endl;
    start = clock();
    for (i = 0; i < groupSize; i++) {
        Entity user = *new Entity();
        user.initializeStep1(i, this); ///��ʼ��step1
        //user.GroupID = GroupID;
        entityList.push_back(user);
        loc[i] = 1;
    }
    for (i = existNumber;i < groupSize;i++) loc[i] = 0;
    for (i = 0; i < groupSize; i++) {
        entityList[i].initializeStep2(this); ///������Կstep2
    }
    finish = clock();
    double llll = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "time:" << llll << endl;
    cout << "��ʼ����ɡ�" << endl;
    updateSi();
}

void Group::updateSi() {
    for (int i = 0; i < this->entityList.size(); i++) {
        for (int j = 0; j < this->entityList.size(); j++) {
            this->entityList[i].S[j] = this->entityList[j].S[j];
        }
    }

}

Groups::Groups() {
    groups.clear();
    for (int i = 0; i < allGroupNum * 10; i++) {
        selectGroupUser[i].clear();
        leave[i] = 0;
    }
}

int genKey(EncrpyKey *key) {

    uint8_t publicKey[ECC_BYTES + 1];
    uint8_t privateKey[ECC_BYTES];

    ecc_make_key(publicKey, privateKey);

    memcpy(key->privateKey, privateKey, sizeof(privateKey));
    memcpy(key->publicKey, publicKey, sizeof(publicKey));

    return EXIT_SUCCESS;
}


EncrpyKey clientKey = {0, 0};
EncrpyKey serverKey = {0, 0};
uint8_t message[ECC_BYTES];
uint8_t signature[ECC_BYTES * 2] = {0};

void encryptMessage(Group *group) {
    int i, nk = 16;
    aes a;
    char key[32];
    char iv[16];
    for (i = 0; i < 16; i++) iv[i] = i;

    curve.start_hash();
    curve.add_to_hash(group->SK);
    shs256_hash(&(curve.SH), key);

    if (!aes_init(&a, MR_CBC, nk, key, iv)) {
        printf("AES��ʼ��ʧ��\n");
        return;
    }

    printf("���ģ�");
    for (i = 0; i < 4 * 4; i++) printf("%02x", (unsigned char) block[i]);
    printf("\n");
    aes_encrypt(&a, block);
    printf("���ģ�");

    for (i = 0; i < 4 * 4; i++) printf("%02x", (unsigned char) block[i]), message[i] = block[i];
    printf("\n");

    aes_end(&a);

    genKey(&clientKey);
    genKey(&serverKey);

    uint8_t secret[ECC_BYTES] = {0};
    ecdh_shared_secret(serverKey.publicKey, clientKey.privateKey, secret);

    ecdsa_sign(clientKey.privateKey, message, signature);
    printf("signature is :  \n");
    for (i = 0; i < ECC_BYTES; i++) {
        printf("%02x", signature[i]);
    }
    printf("\n");
}

void decryptMessage(Group *group) {
    int i, nk = 16;
    aes a;
    char key[32];
    char iv[16];
    for (i = 0; i < 16; i++) iv[i] = i;

    curve.start_hash();
    curve.add_to_hash(group->SK);
    shs256_hash(&(curve.SH), key);

    if (!aes_init(&a, MR_CBC, nk, key, iv)) {
        printf("AES��ʼ��ʧ��\n");
        return;
    }

    char cipher[16];
    for (i = 0; i < 16; i++) {
        cipher[i] = block[i];
    }

    aes_decrypt(&a, cipher);
    printf("���ģ�");
    for (i = 0; i < 4 * 4; i++) printf("%02x", (unsigned char) cipher[i]);
    printf("\n");

    aes_end(&a);
    int result = ecdsa_verify(clientKey.publicKey, message, signature);
    if (result)printf("ǩ����֤�ɹ� \n");
}





string Block::CalculateHash() {
    stringstream ss;
    ss << _tTime << _data << _prevHash << _nNonce;
    return Sha256(ss.str());
}
Block::Block( const string& dataIn, const string&    prevHash) {
    _tTime = time(nullptr);
    _nNonce = 0;
    _data = dataIn;
    _prevHash = prevHash;

}
void Block::ProofOfWork(int difficultNum) {
    char cstr[DifficultyNum + 1];
    for (uint32_t i = 0; i < DifficultyNum; ++i) {
        cstr[i] = '0';
    }
    cstr[DifficultyNum] = '\0';
    string str(cstr);
    do {
        _nNonce++;
        _hash = CalculateHash();
    } while (_hash.substr(0, difficultNum) != str);

    std::cout << "Hash: " << _hash << std::endl;
}
