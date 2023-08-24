#ifndef ZZN2_H_INCLUDED
#define ZZN2_H_INCLUDED

#include "zzn.h"

#ifdef ZZNS
#define MR_INIT_ZZN2 {fn.a=&at; at.w=a; at.len=UZZNS; fn.b=&bt; bt.w=b; bt.len=UZZNS;}
#define MR_CLONE_ZZN2(x) {at.len=x.at.len; bt.len=x.bt.len; for (int i=0;i<UZZNS;i++) {a[i]=x.a[i]; b[i]=x.b[i];}}
#define MR_ZERO_ZZN2 {at.len=bt.len=0; for (int i=0;i<UZZNS;i++) {a[i]=b[i]=0;}}
#else
#define MR_INIT_ZZN2 {fn.a=mirvar(0); fn.b=mirvar(0);}
#define MR_CLONE_ZZN2(x) {zzn2_copy((zzn2 *)&x.fn,&fn);}
#define MR_ZERO_ZZN2 {zzn2_zero(&fn);}
#endif

class ZZn2
{
    zzn2 fn;
#ifdef ZZNS
    mr_small a[UZZNS];
    mr_small b[UZZNS];
    bigtype at,bt;
#endif

public:
    ZZn2()   {MR_INIT_ZZN2 MR_ZERO_ZZN2 }
    ZZn2(int w) {MR_INIT_ZZN2 if (w==0) MR_ZERO_ZZN2 else zzn2_from_int(w,&fn);}
    ZZn2(int w,int z){MR_INIT_ZZN2 zzn2_from_ints(w,z,&fn);}
    ZZn2(const ZZn2& w) {MR_INIT_ZZN2 MR_CLONE_ZZN2(w) }
    ZZn2(const Big &x,const Big& y) {MR_INIT_ZZN2 zzn2_from_bigs(x.getbig(),y.getbig(),&fn); }
    ZZn2(const ZZn &x,const ZZn& y) {MR_INIT_ZZN2 zzn2_from_zzns(x.getzzn(),y.getzzn(),&fn); }
    ZZn2(const ZZn &x) {MR_INIT_ZZN2 zzn2_from_zzn(x.getzzn(),&fn);}
    ZZn2(const Big &x)              {MR_INIT_ZZN2 zzn2_from_big(x.getbig(),&fn); }

    BOOL iszero()  const { return zzn2_iszero((zzn2 *)&fn); }

    void set(const Big &x,const Big &y) {zzn2_from_bigs(x.getbig(),y.getbig(),&fn); }
    void set(const ZZn &x,const ZZn &y) {zzn2_from_zzns(x.getzzn(),y.getzzn(),&fn);}///
    ZZn2& operator=(int i) {if (i==0) MR_ZERO_ZZN2 else zzn2_from_int(i,&fn); return *this;}///
    ZZn2& operator=(const ZZn& x) {zzn2_from_zzn(x.getzzn(),&fn); return *this; }///
    ZZn2& operator=(const ZZn2& x) {MR_CLONE_ZZN2(x) return *this; }///
    ZZn2& operator*=(const ZZn2& x) {zzn2_mul(&fn,(zzn2 *)&x.fn,&fn); return *this; }///

    friend ZZn2 operator*(const ZZn2&,const ZZn2&);
    friend ZZn2 operator*(const ZZn2&,const ZZn&);
    friend ZZn2 operator*(const ZZn&,const ZZn2&);
    friend ZZn2 operator*(int,const ZZn2&);
    friend ZZn2 operator*(const ZZn2&,int);


    friend ZZn2 operator/(const ZZn2&,const ZZn2&);
    friend ZZn2 operator/(const ZZn2&,const ZZn&);
    friend ZZn2 operator/(const ZZn2&,int);

    friend ZZn2 operator+(const ZZn2&,const ZZn2&);
    friend ZZn2 operator+(const ZZn2&,const ZZn&);

    ZZn2& operator+=(const ZZn& x) {zzn2_sadd(&fn,x.getzzn(),&fn); return *this; }
    ZZn2& operator+=(const ZZn2& x){zzn2_add(&fn,(zzn2 *)&x.fn,&fn); return *this;}

    ZZn2& operator/=(const ZZn2&);
    ZZn2& operator/=(const ZZn&);
    ZZn2& operator/=(int);

    ZZn2& conj() {zzn2_conj(&fn,&fn); return *this;}

    friend ZZn2 pow(const ZZn2&,const Big&);
    friend ZZn2 powu(const ZZn2&,const Big&);
    friend ZZn2 conj(const ZZn2&);
    friend ZZn2 txx(const ZZn2&);

    void get(Big &,Big &) const;
    void get(Big &) const;

    void get(ZZn &,ZZn &) const;
    void get(ZZn &) const;

    friend BOOL operator==(const ZZn2& x,const ZZn2& y)
    {return (zzn2_compare((zzn2 *)&x.fn,(zzn2 *)&y.fn)); }

    friend BOOL operator!=(const ZZn2& x,const ZZn2& y)
    {return !(zzn2_compare((zzn2 *)&x.fn,(zzn2 *)&y.fn)); }

    friend BOOL qr(const ZZn2&);
    friend BOOL is_on_curve(const ZZn2&);
#ifndef MR_NO_STANDARD_IO
    friend ostream& operator<<(ostream&,const ZZn2&);
#endif

    ~ZZn2()
    {
    //    MR_ZERO_ZZN2  // slower but safer
#ifndef ZZNS
        mr_free(fn.a);
        mr_free(fn.b);
#endif
    }
};

#endif // ZZN2_H_INCLUDED
