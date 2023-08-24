#ifndef ECN_H_INCLUDED
#define ECN_H_INCLUDED

#include <cstring>
#include "big.h"

#ifdef ZZNS
#define MR_INIT_ECN memset(mem,0,mr_ecp_reserve(1,ZZNS)); p=(epoint *)epoint_init_mem_variable(mem,0,ZZNS);
#else
#define MR_INIT_ECN mem=(char *)ecp_memalloc(1); p=(epoint *)epoint_init_mem(mem,0);
#endif

class ECn
{
    epoint *p;
#ifdef ZZNS
    char mem[mr_ecp_reserve(1,ZZNS)];
#else
    char *mem;
#endif
public:
    ECn()                           {MR_INIT_ECN }
    ECn(const Big &x,const Big& y)  {MR_INIT_ECN
                                   epoint_set(x.getbig(),y.getbig(),0,p); }
    ECn(const Big& x,int cb)             {MR_INIT_ECN
                                   epoint_set(x.getbig(),x.getbig(),cb,p); }
    ECn(const ECn &b)                   {MR_INIT_ECN epoint_copy(b.p,p);}

    ECn& operator=(const ECn& b)  {epoint_copy(b.p,p);return *this;}///
    ECn& operator+=(const ECn& b) {ecurve_add(b.p,p); return *this;}
    ECn& operator*=(const Big& k) {ecurve_mult(k.getbig(),p,p); return *this;}

    int add(const ECn&,big *,big *ex1=NULL,big *ex2=NULL) const;///
                                  // returns line slope as a big
    friend ECn mul(const Big&, const ECn&, const Big&, const ECn&);
    friend ECn mul(int, const Big *, ECn *);

    friend void normalise(ECn &e) {epoint_norm(e.p);}///
    friend ECn operator-(const ECn&);

    friend BOOL operator==(const ECn& a,const ECn& b)
                                  {return epoint_comp(a.p,b.p);}
    friend BOOL operator!=(const ECn& a,const ECn& b)
                                  {return (!epoint_comp(a.p,b.p));}

    epoint *get_point() const;
    int get_status() {return p->marker;}
    BOOL iszero() const;
    BOOL set(const Big& x,const Big& y)    { return epoint_set(x.getbig(),y.getbig(),0,p);}
    BOOL set(const Big& x,int cb=0)  {return epoint_set(x.getbig(),x.getbig(),cb,p);}
    int get(Big& x,Big& y) const;
    int get(Big& x) const;

#ifndef MR_NO_STANDARD_IO

    friend ostream& operator<<(ostream&,const ECn&);

#endif

    ~ECn() {
#ifndef ZZNS
        mr_free(mem);
#endif
 }

};


#endif // ECN_H_INCLUDED
