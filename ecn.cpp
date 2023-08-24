#include "ecn.h"

/// ecurve_add leaves some "goodies" in w6,w7 and w8
/// Note if A=0 for the elliptic curve, ex1 doesn't return anything useful
/// ex2=Z*Z, ex1=2Y^2, lam = line slope

int ECn::add(const ECn& b,big *lam,big *ex1,big *ex2) const
{
    int r=ecurve_add(b.p,p); *lam=get_mip()->w8;
    if (ex1!=NULL) *ex1=get_mip()->w7;
    if (ex2!=NULL) *ex2=get_mip()->w6;
    return r;
}

epoint * ECn::get_point() const
{ return p; }

BOOL ECn::iszero() const
        {if (p->marker==MR_EPOINT_INFINITY) return TRUE; return FALSE;}

ECn operator-(const ECn& e)
{ ECn t=e; epoint_negate(t.p); return t;}


int ECn::get(Big& x,Big& y) const
        {return epoint_get(p,x.getbig(),y.getbig());}
int ECn::get(Big& x) const
        {return epoint_get(p,x.getbig(),x.getbig());}


ECn mul(int n,const Big *y,ECn *x)
{
    ECn w;
    int i;
    big *a=(big *)mr_alloc(n,sizeof(big));
    epoint **b=(epoint **)mr_alloc(n,sizeof(epoint *));
    for (i=0;i<n;i++)
    {
        a[i]=y[i].getbig();
        b[i]=x[i].p;
    }
    ecurve_multn(n,a,b,w.p);

    mr_free(b);
    mr_free(a);

    return w;
}

ECn mul(const Big& e1,const ECn& p1,const Big& e2,const ECn& p2)
{
    ECn t;
    ecurve_mult2(e1.getbig(),p1.get_point(),e2.getbig(),p2.get_point(),t.get_point());
    return t;
}

BOOL is_on_curve(const Big& a)
{ return epoint_x(a.fn);}

ostream& operator<<(ostream& s,const ECn& b)
{
    Big x,y;
    if (b.iszero())
        s << "(Infinity)";
    else
    {
        b.get(x,y);
        s << "(" << x << "," << y << ")";
    }
    return s;
}
