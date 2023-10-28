#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED
extern "C" { ;
#include "miracl.h"
}

#include "EccTest.h"

#include "ssp_pair.h"
#include "big.h"
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <time.h>
#include <ctime>
#include <iomanip>
#include <set>
#include <memory>
#include "vector"

using namespace std;

#define groupSize 5
#define allGroupNum 20

extern PFC curve;
extern Big orderG1;
extern G1 PG[groupSize];
extern G1 generator, Q, B[groupSize];
extern G1 RG[groupSize], FG[groupSize][groupSize];
extern char block[16];

class Group;

class Entity {
public:
    int number;
    G1 Ris, Fis[groupSize], S[groupSize];
    G1 Pis;
    G1 pubEncKey;
    int GroupID;
public:
    Entity();
    Entity(int num);
    void initializeStep1(int number, Group *group);

    void initializeStep2(Group *group);

    void joinUpdate(vector<Entity> Users, Group *group, int flag);

    void leaveUpdate(vector<Entity> Users, Group *group,int flag);

    void getSessionKey(Group *group);

    int decryptMessage();

    void setDecKey();

private:
    G1 decKey;
    GT sessionKey;
};

class Group {
public:
    vector<Entity> entityList;
    int GroupID;
    int loc[groupSize];
    int memberNmuber;


    G1 publicKey;
    G1 W1, W2;//发给别人的
    GT SK;


    set<int> selectUser;
    set<int> liveUser;
    set<int> unselectUser;

    vector<int> leaveUser;
    vector<int> joinUser;
public:
    void globeSetup(int existNumber);

    void updateSi();

    Group(int GroupID);

    bool operator<(const Group &p) const {
        if (this->memberNmuber > p.memberNmuber)return true;
        return this->GroupID < p.GroupID;
    }


};

class Groups {
public:
    vector<Group> groups;
    int groupNum;
    int leave[allGroupNum * 10];
    G1 W1[allGroupNum * 10], W2[allGroupNum * 10];
    set<int> selectGroupUser[allGroupNum * 10];
public:
    Groups();
};

#define DifficultyNum    6

class Block{
public:
    string    _hash;                //当前区块的哈希
    string    _data;                //区块描述字符
    string    _prevHash;            //记录上个块的哈希值
    Block(const string&    prevHash, const string& dataIn);    //构造函数
    string CalculateHash();            //计算本区块的可能哈希 返回值在MineBlock函数中验证
    void ProofOfWork(int difficultNum);
private:
    int64_t _nNonce;            //区块随机数 用于哈希值的产生
    time_t    _tTime;                //创建时间
};

typedef struct Key {
    uint8_t publicKey[ECC_BYTES + 1];
    uint8_t privateKey[ECC_BYTES];
} EncrpyKey;


extern void AllGlobeSetup();

extern void join(Group *group);

extern void leave(Group *group);

extern void groupSessionKey(Group *group);

extern void MultiGroupSessionKey(Groups groups);

void splittingLeave(Group* group, vector<Entity> Users);

void splittingAdd(Groups* groups, vector<Entity> Users, vector<int> splitUser, Group* group);

extern void combine(vector<Group *> GroupID, Groups *groups);

extern void encryptMessage(Group *group);

extern void decryptMessage(Group *group);


#endif // ENTITY_H_INCLUDED
