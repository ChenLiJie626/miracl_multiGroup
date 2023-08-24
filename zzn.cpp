#include "zzn.h"

ZZn operator-(const ZZn& b)
{ZZn x=b; nres_negate(x.fn,x.fn); return x;}

ZZn operator+(const ZZn& b,int i)
{ZZn abi=b; abi+=i; return abi;}
ZZn operator+(int i, const ZZn& b)
{ZZn aib=b; aib+=i; return aib;}
ZZn operator+(const ZZn& b1, const ZZn& b2)
{ZZn abb=b1; abb+=b2; return abb;}

ZZn operator-(const ZZn& b, int i)
{ZZn mbi=b; mbi-=i; return mbi;}
ZZn operator-(int i, const ZZn& b)
{ZZn mib=i; mib-=b; return mib;}
ZZn operator-(const ZZn& b1, const ZZn& b2)
{ZZn mbb=b1; mbb-=b2;  return mbb;}

ZZn operator*(const ZZn& b,int i)
{ZZn xbb=b; xbb*=i; return xbb;}
ZZn operator*(int i, const ZZn& b)
{ZZn xbb=b; xbb*=i; return xbb;}
ZZn operator*(const ZZn& b1, const ZZn& b2)
{ZZn xbb=b1; xbb*=b2; return xbb;}

ZZn operator/(const ZZn& b1, int i)
{ZZn z=b1; z/=i; return z; }
ZZn operator/(int i, const ZZn& b2)
{ZZn z=i; nres_moddiv(z.fn,b2.fn,z.fn); return z;}
ZZn operator/(const ZZn& b1, const ZZn& b2)
{ZZn z=b1; z/=b2; return z;}

ZZn& ZZn::operator/=(int i)
{
 if (i==1) return *this;
 if (i==2)
 { // make a special effort... modulus is odd
    nres_div2(fn,fn);
    return *this;
 }
 ZZn x=i;
 nres_moddiv(fn,x.fn,fn);
 return *this;
}

ZZn getA(void)
{
 ZZn w;
 if (get_mip()->Asize<MR_TOOBIG) w=get_mip()->Asize;
 else w=get_mip()->A;
 return w;
}

big ZZn::getzzn(void) const
         { return fn;}

ZZn getB(void)
{
 ZZn w;
 if (get_mip()->Bsize<MR_TOOBIG) w=get_mip()->Bsize;
 else w=get_mip()->B;
 return w;
}

ZZn one(void)
{
    ZZn w;
    w=get_mip()->one;
    return w;
}

