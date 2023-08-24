#ifndef ZZN_H_INCLUDED
#define ZZN_H_INCLUDED

#include "big.h"

#define MR_INIT_ZZN fn=mirvar(0);
#define MR_CLONE_ZZN(x) copy(x,fn);
#define MR_ZERO_ZZN zero(fn);


class ZZn
{
    big fn;

public:
    ZZn()               {MR_INIT_ZZN MR_ZERO_ZZN}
    ZZn(int i)          {MR_INIT_ZZN if (i==0) MR_ZERO_ZZN else {convert(i,fn); nres(fn,fn);} }

    ZZn& operator=(const ZZn& c)    {MR_CLONE_ZZN(c.fn) return *this;}///
    ZZn& operator=(big c)           {MR_CLONE_ZZN(c) return *this; }

    ZZn& operator=(int i)  {if (i==0) MR_ZERO_ZZN else {convert(i,fn); nres(fn,fn);} return *this;}
    ZZn& operator=(char* s){cinstr(fn,s); nres(fn,fn); return *this;}



    friend ZZn operator-(const ZZn&);
    friend ZZn operator+(const ZZn&,int);
    friend ZZn operator+(int, const ZZn&);
    friend ZZn operator+(const ZZn&, const ZZn&);///

    friend ZZn operator-(const ZZn&, int);
    friend ZZn operator-(int, const ZZn&);
    friend ZZn operator-(const ZZn&, const ZZn&);///

    friend ZZn operator*(const ZZn&,int);
    friend ZZn operator*(int, const ZZn&);
    friend ZZn operator*(const ZZn&, const ZZn&);///

    friend ZZn operator/(const ZZn&, int);
    friend ZZn operator/(int, const ZZn&);
    friend ZZn operator/(const ZZn&, const ZZn&);

    ZZn& operator/=(const ZZn& b) {nres_moddiv(fn,b.fn,fn); return *this;}
    ZZn& operator/=(int);

    ZZn& operator+=(int i)
        {ZZn inc=i; nres_modadd(fn,inc.fn,fn);return *this;}
    ZZn& operator-=(int i)
        {ZZn dec=i; nres_modsub(fn,dec.fn,fn); return *this;}
    ZZn& operator+=(const ZZn& b)
        {nres_modadd(fn,b.fn,fn); return *this;}
    ZZn& operator-=(const ZZn& b)
        {nres_modsub(fn,b.fn,fn); return *this;}
    ZZn& operator*=(const ZZn& b)
        {nres_modmult(fn,b.fn,fn); return *this;}
    ZZn& operator*=(int i)
        {nres_premult(fn,i,fn); return *this;}

    friend ZZn getA(void);        // get A parameter of elliptic curve
    friend ZZn getB(void);        // get B parameter of elliptic curve
    friend ZZn  one(void);
    friend big getbig(ZZn& z) {return z.fn;}


    big getzzn(void) const;

    ~ZZn()
    {
        mr_free(fn);
    }
};

extern ZZn randn(void);
extern ZZn getA(void);
extern ZZn getB(void);
extern ZZn one(void);
#endif // ZZN_H_INCLUDED
