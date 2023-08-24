#ifndef SSP_PAIR_H_INCLUDED
#define SSP_PAIR_H_INCLUDED

// k=2 Super-Singular curve over GF(P)
#include "zzn2.h"	// GT
#include "ecn.h"	// G1
#include <ctime>
#define G1_TYPE ECn
#define G1_SUBTYPE ZZn
#define GT_TYPE ZZn2
#define WINDOW_SIZE 8 // window size for precomputation
extern void read_only_error(void);

class G1
{
public:
	G1_TYPE g;
	G1_SUBTYPE *ptable;
	G1_TYPE *mtable;
	int mtbits;

	G1()   {mtable=NULL; mtbits=0; ptable=NULL;}
	G1(const G1& w) {mtable=NULL; mtbits=0; ptable=NULL; g=w.g;}

	G1& operator=(const G1& w)
	{
	    mtable=NULL; mtbits=0; ptable=NULL;
		if (mtable==NULL && ptable==NULL) g=w.g;
		else read_only_error();
		return *this;
	}
	int spill(char *&);   // spill precomputation to byte array, and return length
	void restore(char *); // restore precomputation from byte array

	friend G1 operator-(const G1&);
	friend G1 operator+(const G1&,const G1&);
	friend BOOL operator==(const G1& x,const G1& y)
      {if (x.g==y.g) return TRUE; else return FALSE;}
	friend BOOL operator!=(const G1& x,const G1& y)
      {if (x.g!=y.g) return TRUE; else return FALSE;}

	~G1()  {if (ptable!=NULL) {delete [] ptable; ptable=NULL;}
			if (mtable!=NULL) {delete [] mtable; mtable=NULL;}}
};

class GT
{
public:
	GT_TYPE g;

	GT_TYPE *etable;
	int etbits;
	GT() {etable=NULL; etbits=0;}
	GT(const GT& w) {etable=NULL; etbits=0; g=w.g;}
	GT(int d) {etable=NULL; g=d;}

	GT& operator=(const GT& w)
	{
		if (etable==NULL) g=w.g;
		else read_only_error();
		return *this;
	}
	int spill(char *&);   // spill precomputation to byte array, and return length
	void restore(char *); // restore precomputation from byte array
	friend GT operator*(const GT&,const GT&);
	friend GT operator/(const GT&,const GT&);
	friend BOOL operator==(const GT& x,const GT& y)
      {if (x.g==y.g) return TRUE; else return FALSE;}
	friend BOOL operator!=(const GT& x,const GT& y)
      {if (x.g!=y.g) return TRUE; else return FALSE;}
	~GT() {if (etable!=NULL) {delete [] etable; etable=NULL;}}
};

// pairing friendly curve class

class PFC
{
public:
	Big *mod;
	Big *cof;
	int S;
	Big *ord;
	sha256 SH;
	csprng *RNG;

	PFC(int, csprng *rng=NULL);
	Big order(void) {return *ord;}

	GT power(const GT&,const Big&);
	G1 mult(const G1&,const Big&);
	void hash_and_map(G1&,char *);
	void random(Big&);
	void rankey(Big&);
	void random(G1&);
	BOOL member(const GT&);		   // test if element is member of pairing friendly group

	int precomp_for_pairing(G1&);  // precompute multiples of G1 that occur in Miller loop
	int precomp_for_mult(G1&,BOOL small=FALSE);     // (precomputation may not be implemented!)
	int precomp_for_power(GT&,BOOL small=FALSE);    // returns number of precomputed values
// small=TRUE if exponent is always less than full group size and equal to 2*Security level
// creates a smaller table

	int spill(G1&,char *&);
	void restore(char *,G1&);

	Big hash_to_aes_key(const GT&);
	Big hash_to_group(char *);///
	GT miller_loop(const G1&,const G1&);///
	GT final_exp(const GT&);///
	GT pairing(const G1&,const G1&);///

// parameters: number of pairings n, pointers to pair of G1 elements
	GT multi_pairing(int n,G1 **,G1 **); //product of pairings
	GT multi_miller(int n,G1 **,G1 **);
	void start_hash(void);
	void add_to_hash(const G1&);
	void add_to_hash(const GT&);
	void add_to_hash(const Big&);
	Big finish_hash_to_group(void);///
	~PFC() {
		delete mod; delete cof;
		 }
};


extern void force(ZZn&,ZZn&,ZZn&,ECn&);
extern void extract(ECn&,ZZn&,ZZn&,ZZn&);
extern void force(ZZn&,ZZn&,ECn&);
extern void extract(ECn&,ZZn&,ZZn&);///


#endif // SSP_PAIR_H_INCLUDED
