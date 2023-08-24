#include "big.h"
extern "C" {
#include "miracl.h"
}
int operator%(const Big& b, int i)
{Big mdbi; return(subdiv(b.fn,i, mdbi.fn));}
Big operator%(const Big& b1, const Big& b2)
{Big mdbb;copy(b1.fn,mdbb.fn);divide(mdbb.fn,b2.fn,b2.fn);return mdbb;}

Big from_binary(int len,char *ptr)
{Big z;  bytes_to_big(len,ptr,z.fn); return z;}

void ecurve(const Big& a,const Big& b,const Big& p,int t)
{
    ecurve_init(a.fn,b.fn,p.fn,t);
}

Big operator-(const Big& b)
{Big nb; negify(b.fn,nb.fn); return nb;}

Big operator-(const Big& b, int i)
{Big mbi; decr(b.fn, i, mbi.fn); return mbi;}
Big operator-(int i, const Big& b)
{Big mib;decr(b.fn, i, mib.fn);negify(mib.fn,mib.fn);return mib;}
Big operator-(const Big& b1, const Big& b2)
{Big mbb; subtract(b1.fn,b2.fn,mbb.fn); return mbb;}

Big operator+(const Big& b,int i)
{Big abi; incr(b.fn, i, abi.fn); return abi;}
Big operator+(int i,const Big& b)
{Big aib; incr(b.fn, i, aib.fn); return aib;}
Big operator+(const Big& b1, const Big& b2)
{Big abb;add(b1.fn,b2.fn,abb.fn);return abb;}

Big operator*(const Big& b, int i)
{Big xbi; premult(b.fn, i, xbi.fn); return xbi;}
Big operator*(int i, const Big& b)
{Big xib; premult(b.fn, i, xib.fn); return xib;}
Big operator*(const Big& b1, const Big& b2)
{Big xbb; multiply(b1.fn,b2.fn,xbb.fn); return xbb;}

Big operator/(const Big& b, int i)
{Big dbi; subdiv(b.fn, i, dbi.fn); return dbi;}
Big operator/(const Big& b1, const Big& b2)
{Big dbb; copy(b1.fn,dbb.fn); divide(dbb.fn,b2.fn,dbb.fn); return dbb;}

void modulo(const Big& n) {prepare_monty(n.fn);}

Big get_modulus()
{
    Big m;
    miracl *mip=get_mip();
    copy(mip->modulus,m.fn);
    return m;
}

big Big::getbig() const
         { return fn;}

BOOL Big::iszero() const
         { if (size(fn)==0) return TRUE; return FALSE;}
BOOL Big::isone() const
         { if (size(fn)==1) return TRUE; return FALSE;}

ostream& operator<<(ostream& s, const Big& x)
{
    miracl *mip=get_mip();
	cotstr(x.fn,mip->IOBUFF);
    s << mip->IOBUFF;
    return s;
}

Big pow(const Big& b,int n)
{Big z;int x;
if (mr_abs(x=size(b.fn))<MR_TOOBIG) expint(x,n,z.fn);
              else power(b.fn,n,z.fn,z.fn);return z;}
Big pow(const Big& b1,int n, const Big& b3)
{Big z; power(b1.fn,n,b3.fn,z.fn); return z;}
Big pow(int x, const Big& b2, const Big& b3)
{Big z; powltr(x,b2.fn,b3.fn,z.fn); return z;}
Big pow(const Big& b1, const Big& b2, const Big& b3)
{Big z; powmod(b1.fn,b2.fn,b3.fn,z.fn); return z;}
Big pow(const Big& b1,const Big& b2,const Big& b3,const Big& b4,const Big& b5)
{Big z; powmod2(b1.fn,b2.fn,b3.fn,b4.fn,b5.fn,z.fn); return z;}

Big inverse(const Big& b1, const Big& b2)
{Big z; xgcd(b1.fn,b2.fn,z.fn,z.fn,z.fn);return z;}

Big modmult(const Big& b1,const Big& b2,const Big& m)
{Big z; mad(b1.fn,b2.fn,b2.fn,m.fn,m.fn,z.fn); return z;}

Big rand(const Big& b) {Big z; bigrand(b.fn,z.fn); return z;}
Big rand(int n,int b) {Big z; bigdig(n,b,z.fn);  return z;}
Big randbits(int n) {Big z; bigbits(n,z.fn); return z;}
Big strong_rand(csprng *rng,const Big& b) {Big z; strong_bigrand(rng,b.fn,z.fn); return z;}
Big strong_rand(csprng *rng,int n,int b) {Big z; strong_bigdig(rng,n,b,z.fn);  return z;}

Big root(const Big &b,int n) {Big z; nroot(b.fn, n, z.fn); return z;}

