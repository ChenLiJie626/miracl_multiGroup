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
#define allGroupNum 5

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

    void initializeStep1(int number, Group *group);

    void initializeStep2(Group *group);

    void joinUpdate(vector<Entity> Users, Group *group);

    void leaveUpdate(vector<Entity> Users, Group *group);

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
    void globeSetup(int securityParameter);

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

typedef struct Key {
    uint8_t publicKey[ECC_BYTES + 1];
    uint8_t privateKey[ECC_BYTES];
} EncrpyKey;


extern void AllGlobeSetup();

extern void join(Group *group);

extern void leave(Group *group);

extern void groupSessionKey(Group *group);

extern void MultiGroupSessionKey(Groups groups);

extern void splitting(Group *group, Groups *groups, vector<Entity> Users, vector<int> splitUser);

extern void combine(vector<Group *> GroupID, Groups *groups);

extern void encryptMessage(Group *group);

extern void decryptMessage(Group *group);


#endif // ENTITY_H_INCLUDED
