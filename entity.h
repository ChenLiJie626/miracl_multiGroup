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

#define groupSize 10

extern int memberNmuber;
extern Big orderG1;
extern PFC curve;
extern G1 PG[groupSize];
extern G1 generator, Q, B[groupSize];
extern G1 RG[groupSize], FG[groupSize][groupSize];
extern int st[groupSize]; ///st�ַ������������ʾ
extern set<int> Uset, Sset, Rset;
extern G1 W1, W2;
extern G1 publicKey;
extern GT SK;
extern char block[16];


class entity {
public:
    int number;
    G1 Ris, Fis[groupSize], S[groupSize];
    G1 Pis;
    G1 pubEncKey;
    int GroupID;
public:
    entity();

    void initializeStep1();

    void initializeStep2(entity user[]);

    void joinUpdate(entity joinUser);

    void leaveUpdate(entity leaveUser);

    void getSessionKey();

    int decryptMessage();

    void setDecKey();

private:
    G1 decKey;
    GT sessionKey;
};

class Group {
public:
    vector<entity *> entityList;
    int GroupID;
    int loc[groupSize];
public:
    Group(int GroupID);
};

extern void globeSetup(int securityParameter, entity user[], Group *group);

extern void join(entity user[], Group *group);

extern void leave(entity user[], Group *group);

extern void genSessionKey(G1 pubEncKey, entity user[],Group *group);

extern int encryptMessage();

extern void decryptMessage(entity user[]);

#endif // ENTITY_H_INCLUDED
