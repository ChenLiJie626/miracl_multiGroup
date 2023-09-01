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

extern int memberNmuber;
extern PFC curve;
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

    Big orderG1;
    G1 PG[groupSize], RG[groupSize];
    G1 generator, Q, B[groupSize];
    G1 FG[groupSize][groupSize];
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

extern void globeSetup(int securityParameter);

extern void join(Group *group);

extern void leave(Group *group);

void groupSessionKey(Group *group);

extern void genSessionKey(G1 pubEncKey, Entity user[], vector<Group> groups);

extern int encryptMessage();

extern void decryptMessage(Entity user[]);

#endif // ENTITY_H_INCLUDED
