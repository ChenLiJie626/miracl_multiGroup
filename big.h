#ifndef BIG_H_INCLUDED
#define BIG_H_INCLUDED

#include <cstdlib>
#include <cstdio>
#include <iostream>
using namespace std;
extern "C" {
#include "miracl.h"
}
#ifndef MR_NO_STANDARD_IO
#include <iostream>
using std::istream;
using std::ostream;
#endif

#ifndef MIRACL_CLASS
#define MIRACL_CLASS

#ifdef __cplusplus
#ifdef MR_GENERIC_MT
#error "The generic method isn't supported for C++, its C only"
#endif
#endif

class Miracl
{ /* dummy class to initialise MIRACL - MUST be called before any Bigs    *
   * are created. This could be a problem for static/global data declared *
   * in modules other than the main module */
    miracl *mr;
public:
    Miracl(int nd,mr_small nb=0)
                                 {mr=mirsys(nd,nb);
}
    miracl *operator&() {return mr;}
    ~Miracl()                    {mirexit();}
};

#endif

#define MR_INIT_BIG fn=mirvar(0);

class Big
{
    big fn;

public:

    Big()        {MR_INIT_BIG }
    Big(int j)   {MR_INIT_BIG convert(j,fn); }
    Big(unsigned int j) {MR_INIT_BIG uconvert(j,fn); }
    Big(long lg) {MR_INIT_BIG lgconv(lg,fn);}
    Big(unsigned long lg) {MR_INIT_BIG ulgconv(lg,fn);}
#define mr_dltype __int64

    Big(mr_dltype dl) {MR_INIT_BIG dlconv(dl,fn);}

    Big(char* s) {MR_INIT_BIG cinstr(fn,s);}
    Big(big& c)  {MR_INIT_BIG copy(c,fn);}
    Big(const Big& c)  {MR_INIT_BIG copy(c.fn,fn);}
    Big(big* c)  { fn=*c; }

    Big& operator=(int i)  {convert(i,fn); return *this;}///
    Big& operator=(long lg){lgconv(lg,fn); return *this;}

    Big& operator=(mr_dltype dl){dlconv(dl,fn); return *this;}

    Big& operator=(mr_small s) {fn->len=1; fn->w[0]=s; return *this;}
    Big& operator=(const Big& b) {copy(b.fn,fn); return *this;}
    Big& operator=(big& b) {copy(b,fn); return *this;}
    Big& operator=(big* b) {fn=*b; return *this;}

    Big& operator=(char* s){ cinstr(fn,s);return *this;}

    Big& operator+=(int i) {incr(fn,i,fn); return *this;}///
    Big& operator+=(const Big& b){add(fn,b.fn,fn); return *this;}///

    Big& operator-=(int i)  {decr(fn,i,fn); return *this;}///
    Big& operator-=(const Big& b) {subtract(fn,b.fn,fn); return *this;}///

    Big& operator*=(int i)  {premult(fn,i,fn); return *this;}///
    Big& operator*=(const Big& b) {multiply(fn,b.fn,fn); return *this;}///

    Big& operator/=(int i)  {subdiv(fn,i,fn);    return *this;}///
    Big& operator/=(const Big& b) {divide(fn,b.fn,fn); return *this;}///

    Big& operator%=(int i)  {convert(subdiv(fn,i,fn),fn); return *this;}///
    Big& operator%=(const Big& b) {divide(fn,b.fn,b.fn); return *this;}///

    mr_small& operator[](int i) {return fn->w[i];}///

    void negate() const;
    BOOL iszero() const;
    BOOL isone() const;
    int get(int index)          { int m; m=getdig(fn,index); return m; }
    void set(int index,int n)   { putdig(n,fn,index);}
    int len() const;

    big getbig() const;

    friend class Flash;

    friend Big operator-(const Big&);

    friend Big operator+(const Big&,int);
    friend Big operator+(int,const Big&);
    friend Big operator+(const Big&,const Big&);

    friend Big operator-(const Big&, int);
    friend Big operator-(int,const Big&);
    friend Big operator-(const Big&,const Big&);

    friend Big operator*(const Big&, int);
    friend Big operator*(int,const Big&);
    friend Big operator*(const Big&,const Big&);

    friend BOOL fmth(int n,const Big&,const Big&,Big&);   // fast mult - top half

    friend Big operator/(const Big&,int);
    friend Big operator/(const Big&,const Big&);

    friend int operator%(const Big&, int);
    friend Big operator%(const Big&, const Big&);

    friend Big from_binary(int,char *);///

	friend int to_binary(const Big& b,int max,char *ptr,BOOL justify=FALSE)
	{
		return big_to_bytes(max,b.fn,ptr,justify);
	}
    //friend int to_binary(const Big&,int,char *,BOOL justify=FALSE);
    friend Big modmult(const Big&,const Big&,const Big&);
    friend Big mad(const Big&,const Big&,const Big&,const Big&,Big&);
    friend Big norm(const Big&);
    friend Big sqrt(const Big&);
    friend Big root(const Big&,int);
    friend Big gcd(const Big&,const Big&);
	friend int recode(const Big& e,int t,int w,int i) {return recode(e.fn,t,w,i);}

#ifndef MR_FP
    friend Big land(const Big&,const Big&);  // logical AND
    friend Big lxor(const Big&,const Big&);   // logical XOR
#endif
    friend Big pow(const Big&,int);               // x^m
    friend Big pow(const Big&, int, const Big&);  // x^m mod n
    friend Big pow(int, const Big&, const Big&);  // x^m mod n
    friend Big pow(const Big&, const Big&, const Big&);  // x^m mod n
    friend Big pow(const Big&, const Big&, const Big&, const Big&, const Big&);
                                                         // x^m.y^k mod n
    friend Big pow(int,Big *,Big *,Big);  // x[0]^m[0].x[1].m[1]... mod n
    //friend Big luc(const Big& ,const Big&, const Big&, Big *b4=NULL);
	friend Big moddiv(const Big&,const Big&,const Big&);
    friend Big inverse(const Big&, const Big&);
    friend void multi_inverse(int,Big*,const Big&,Big *);

    friend Big rand(const Big&);     /// 0 < rand < parameter
    friend Big rand(int,int);        // (digits,base) e.g. (32,16)
    friend Big randbits(int);        // n random bits
    friend Big strong_rand(csprng *,const Big&);
    friend Big strong_rand(csprng *,int,int);

    friend Big abs(const Big&);
// This next only works if MIRACL is using a binary base...
    friend int bit(const Big& b,int i)  {return mr_testbit(b.fn,i);}///
    friend int bits(const Big& b) {return logb2(b.fn);}///
    friend int ham(const Big& b)  {return hamming(b.fn);}///

    friend BOOL operator>=(const Big& b1,const Big& b2)
             {if (mr_compare(b1.fn,b2.fn)>=0) return TRUE; else return FALSE;}
    friend BOOL operator==(const Big& b1,const Big& b2)
             {if (mr_compare(b1.fn,b2.fn)==0) return TRUE; else return FALSE;}
    friend BOOL operator<(const Big& b1,const Big& b2)
              {if (mr_compare(b1.fn,b2.fn)<0) return TRUE; else return FALSE;}
    friend BOOL operator>(const Big& b1,const Big& b2)
              {if (mr_compare(b1.fn,b2.fn)>0) return TRUE; else return FALSE;}

    friend BOOL prime(const Big& b) {return isprime(b.fn);}
    friend void ecurve(const Big&,const Big&,const Big&,int);///
    friend BOOL is_on_curve(const Big&);
    friend void modulo(const Big&);
    friend BOOL modulo(int,int,int,int,BOOL);
    friend Big get_modulus(void);///
	friend int window(const Big& x,int i,int* nbs,int *nzs,int window_size=5)///
    {
        return mr_window(x.fn,i,nbs,nzs,window_size);
    }
    friend int naf_window(const Big& x,const Big& x3,int i,int* nbs,int* nzs,int store=11)
	{
		return mr_naf_window(x.fn,x3.fn,i,nbs,nzs,store);
	}


/* Note that when inputting text as a number the CR is NOT   *
 * included in the text, unlike C I/O which does include CR. */

#ifndef MR_NO_STANDARD_IO

    friend istream& operator>>(istream&, Big&);
    friend ostream& operator<<(ostream&, const Big&);
    friend ostream& otfloat(ostream&,const Big&,int);

#endif

// output Big to a String
    friend char * operator<<(char * s,const Big&);

    ~Big() {
        // zero(fn);
#ifndef BIGS
        mr_free(fn);
#endif
    }
};

extern Big get_modulus(void);
extern Big from_binary(int,char *);///
extern Big strong_rand(csprng *,int,int);

using namespace std;



#endif // BIG_H_INCLUDED
