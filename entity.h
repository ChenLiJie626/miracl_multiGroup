#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED
extern "C" { ;
#include "miracl.h"
}

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
#include "vector"

using namespace std;

#define groupSize 5
#define groupNum 5

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
    G1 W1, W2;
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

};

class Groups {
public:
    vector<Group> groups;
    G1 W1[groupNum], W2[groupNum];
    set<int> selectGroupUser[groupNum];
public:
    Groups();
};
extern void AllGlobeSetup();

extern void join(Group *group);

extern void leave(Group *group);

extern void groupSessionKey(Group *group);

extern void MultiGroupSessionKey(Groups groups);

#endif // ENTITY_H_INCLUDED
