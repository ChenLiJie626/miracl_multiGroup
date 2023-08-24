#include "zzn2.h"

using namespace std;

BOOL qr(const ZZn2& x)
{
	ZZn2 y=x;
    return (zzn2_qr(&(y.fn)));
/*
    ZZn s,xa,xb;
    int qnr=get_mip()->qnr;
cout << "in qr(Zn2)" << endl;
    if (x.iszero()) return TRUE;
    x.get(xa,xb);
    if (xb.iszero()) return TRUE;
    if (qnr==-1)
    {
        if (xa.iszero()) return TRUE;
    }

    s=xb;
    s*=s;
    if (qnr== -2) s+=s;
    if (!qr(xa*xa+s))
        return FALSE;
    return TRUE;
*/
}

ZZn2 operator*(const ZZn2& x,const ZZn2& y)
{
 ZZn2 w=x;
 if (&x==&y) w*=w;
 else        w*=y;
 return w;
}

ZZn2 operator*(const ZZn2& x,const ZZn& y)
{ZZn2 w=x; w*=y; return w;}

ZZn2 operator*(const ZZn& y,const ZZn2& x)
{ZZn2 w=x; w*=y; return w;}

ZZn2 operator*(const ZZn2& x,int y)
{ZZn2 w=x; w*=y; return w;}

ZZn2 operator*(int y,const ZZn2& x)
{ZZn2 w=x; w*=y;return w;}

ZZn2 operator+(const ZZn2& x,const ZZn2& y)
{ZZn2 w=x; w+=y; return w; }

ZZn2 operator+(const ZZn2& x,const ZZn& y)
{ZZn2 w=x; w+=y; return w; }

// ZZn2 powering of unitary elements

ZZn2 powu(const ZZn2& x,const Big& e)
{
    int i,j,nb,n,nbw,nzs;
    ZZn2 u,u2,t[11];
    Big k,k3;

    if (e.iszero()) return (ZZn2)one();
	k=e;
	if (e<0) k=-k;

    u=x;
    if (k.isone())
	{
		if (e<0) u=conj(u);
		return u;
	}
//
// Prepare table for windowing
//
    k3=3*k;
    u2=(u*u);
    t[0]=u;

    for (i=1;i<=10;i++)
        t[i]=u2*t[i-1];

    nb=bits(k3);
    for (i=nb-2;i>=1;)
    {
        n=naf_window(k,k3,i,&nbw,&nzs,11);

        for (j=0;j<nbw;j++) u*=u;
        if (n>0) u*=t[n/2];
        if (n<0) u*=conj(t[(-n)/2]);
        i-=nbw;
        if (nzs)
        {
            for (j=0;j<nzs;j++) u*=u;
            i-=nzs;
        }
    }
	if (e<0) u=conj(u);
    return u;
}

BOOL is_on_curve(const ZZn2& x)
{
    ZZn2 w;
    int twist=get_mip()->TWIST;

    if (twist==MR_QUADRATIC)
    {
		w=x*x*x+txx(txx((ZZn2)getA()))*x+txx(txx(txx((ZZn2)getB())));
    }
    else
    {
        w=x*x*x+getA()*x+getB();
    }

    if (qr(w)) return TRUE;
    return FALSE;
}

ZZn2 conj(const ZZn2& x)
{
    ZZn2 u=x;
    u.conj();
    return u;
}

// regular ZZn2 powering - but see powl function in zzn.h

ZZn2 pow(const ZZn2& x,const Big& k)
{
    int i,j,nb,n,nbw,nzs;
    ZZn2 u,u2,t[16];

    if (x.iszero()) return (ZZn2)0;
    if (k==0) return (ZZn2)1;
    u=x;
    if (k==1) return u;
//
// Prepare table for windowing
//
    u2=(u*u);
    t[0]=u;

    for (i=1;i<16;i++)
        t[i]=u2*t[i-1];

// Left to right method - with windows

    nb=bits(k);
    if (nb>1) for (i=nb-2;i>=0;)
    {
        n=window(k,i,&nbw,&nzs,5);
        for (j=0;j<nbw;j++) u*=u;
        if (n>0) u*=t[n/2];
        i-=nbw;
        if (nzs)
        {
            for (j=0;j<nzs;j++) u*=u;
            i-=nzs;
        }
    }
    return u;
}

ZZn2& ZZn2::operator/=(const ZZn2& x)
{
    ZZn2 inv=x;
    zzn2_inv(&inv.fn);
    zzn2_mul(&fn,&inv.fn,&fn);
    return *this;
}

ZZn2& ZZn2::operator/=(const ZZn& x)
{
    ZZn t=one()/x;
    zzn2_smul(&fn,t.getzzn(),&fn);
    return *this;
}

void ZZn2::get(Big& x,Big& y)  const
{{redc(fn.a,x.getbig()); redc(fn.b,y.getbig()); }}

void ZZn2::get(Big& x) const
{{redc(fn.a,x.getbig());} }

void ZZn2::get(ZZn& x,ZZn& y)  const
{{copy(fn.a,x.getzzn()); copy(fn.b,y.getzzn()); }}

void ZZn2::get(ZZn& x) const
{{copy(fn.a,x.getzzn());} }

ZZn2& ZZn2::operator/=(int i)
{
    if (i==2) {zzn2_div2(&fn); return *this;}

    ZZn t=one()/i;
    zzn2_smul(&fn,t.getzzn(),&fn);
    return *this;
}

ZZn2 operator/(const ZZn2& x,const ZZn2& y)
{ZZn2 w=x; w/=y; return w;}

ZZn2 operator/(const ZZn2& x,const ZZn& y)
{ZZn2 w=x; w/=y; return w;}

ZZn2 operator/(const ZZn2& x,int i)
{ZZn2 w=x; w/=i; return w;}

ZZn2 txx(const ZZn2& w)
{ // multiply w by t^2 where x^4-t is irreducible polynomial for ZZn4
  //
  // for p=5 mod 8 t=sqrt(sqrt(-2)), qnr=-2
  // for p=3 mod 8 t=sqrt(1+sqrt(-1)), qnr=-1
  // for p=7 mod 8 and p=2,3 mod 5 t=sqrt(2+sqrt(-1)), qnr=-1
  ZZn2 u=w;
  zzn2_txx(&(u.fn));
  return u;
}

ostream& operator<<(ostream& s,const ZZn2& xx)
{
    ZZn2 b=xx;
    Big x,y;
    b.get(x,y);
    s << "[" << x << "," << y << "]";
    return s;
}


