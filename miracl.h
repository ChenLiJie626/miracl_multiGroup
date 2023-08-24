#ifndef MIRACL_H_INCLUDED
#define MIRACL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIRACL 32
#define MR_LITTLE_ENDIAN    /* This may need to be changed        */
#define mr_utype int
                            /* the underlying type is usually int *
                             * but see mrmuldv.any                */
#define mr_unsign32 unsigned int
                            /* 32 bit unsigned type               */
#define MR_IBITS      32    /* bits in int  */
#define MR_LBITS      32    /* bits in long */
#define MR_FLASH      52
                            /* delete this definition if integer  *
                             * only version of MIRACL required    */
                            /* Number of bits per double mantissa */

#define mr_dltype __int64   /* ... or long long for Unix/Linux */
#define mr_unsign64 unsigned __int64

#define MAXBASE ((mr_small)1<<(MIRACL-1))
#define MR_EPOINT_GENERAL         0
#define MR_OVER                   0
#define MR_ECB                    0
#define MR_NOTSET                 0
#define MR_PROJECTIVE             0
#define MR_AFFINE                 1
#define MR_ERR_BASE_TOO_BIG       1
#define MR_EPOINT_NORMALIZED      1
#define MR_ADD                    1
#define MR_EPOINT_INFINITY        2
#define MR_DOUBLE                 2
#define MR_BEST                   2
#define MR_ERR_DIV_BY_ZERO        2
#define MR_ERR_OVERFLOW           3
#define MR_ERR_NEG_RESULT         4
#define SQR_FASTER_THRESHOLD      5
#define MR_ERR_BAD_FORMAT         5
#define MR_ERR_BAD_BASE           6
#define MR_ERR_BAD_PARAMETERS     7
#define MR_ECC_STORE_N            8
#define MR_ERR_OUT_OF_MEMORY      8
#define MR_ERR_NEG_ROOT           9
#define MR_ERR_NEG_POWER         10
#define MR_ERR_BAD_ROOT          11
#define MR_ERR_INT_OP            12
#define MR_ERR_TOO_BIG           14
#define NV                       14
#define MR_ERR_IO_OVERFLOW       17
#define MR_ERR_NO_MIRSYS         18
#define MR_ERR_BAD_MODULUS       19
#define MR_ERR_NO_MODULUS        20
#define MR_ERR_NOT_SUPPORTED     22
#define MR_MAXDEPTH              24
#define NJ                       24
#define MR_ERR_COMPOSITE_MODULUS 28
#define MR_MAX_M_T_S             64
#define MR_MAXPRIME 1000
#define MR_SL sizeof(long)

#define NB 4
#define MR_CBC   1
#define MR_CFB1  2
#define MR_CFB2  3
#define MR_CFB4  5
#define MR_PCFB1 10
#define MR_PCFB2 11
#define MR_PCFB4 13
#define MR_OFB1  14
#define MR_OFB2  15
#define MR_OFB4  17
#define MR_OFB8  21
#define MR_OFB16 29

#define SHA3_ROUNDS 24
#define rotl(x,n) (((x)<<n) | ((x)>>(64-n)))

#define MR_BYTE unsigned char
#define MR_TOBYTE(x) ((MR_BYTE)(x))
#define mr_abs(x)  ((x)<0? (-(x)) : (x))
#define MR_ROUNDUP(a,b) ((a)-1)/(b)+1
#define mr_size(n) (((sizeof(struct bigtype)+((n)+2)*sizeof(mr_utype))-1)/MR_SL+1)*MR_SL
#define mr_big_reserve(n,m) ((n)*mr_size(m)+MR_SL)
#define mr_esize_a(n) (((sizeof(epoint)+mr_big_reserve(2,(n)))-1)/MR_SL+1)*MR_SL
#define mr_ecp_reserve_a(n,m) ((n)*mr_esize_a(m)+MR_SL)

#define mr_esize(n) (((sizeof(epoint)+mr_big_reserve(3,(n)))-1)/MR_SL+1)*MR_SL
#define mr_ecp_reserve(n,m) ((n)*mr_esize(m)+MR_SL)


#define M4 (MIRACL-4)

#define MR_DEFAULT_BUFFER_SIZE 1024
#define forever for(;;)

#define MR_OUT  mr_mip->depth--;
#define MR_IN(N) mr_mip->depth++; if (mr_mip->depth<MR_MAXDEPTH) {mr_mip->trace[mr_mip->depth]=(N); if (mr_mip->TRACER) mr_track(_MIPPO_); }

typedef int BOOL;

#define TRUE 1
#define FALSE 0
#define OFF 0
#define ON 1
#define PLUS 1
#define MINUS (-1)

typedef unsigned mr_utype mr_small;
typedef unsigned mr_dltype mr_large;

#define MR_DIV(a,b)    ((a)/(b))
#define MR_REMAIN(a,b) ((a)%(b))
#define MR_LROUND(a)   ((a))

#define MR_QUADRATIC 2

#define MR_BOT 0
#define MR_TOP 1

typedef unsigned int mr_lentype;

#define MR_SPACES 28

union doubleword
{
    mr_large d;
    mr_small h[2];
};

struct bigtype
{
    mr_lentype len;
    mr_small *w;
};

typedef struct bigtype *big;
typedef big zzn;
typedef big flash;

#define MR_MSBIT ((mr_lentype)1<<(MR_IBITS-1))
#define MR_OBITS (MR_MSBIT-1)

#define MR_TOOBIG (1<<(MR_IBITS-2))
/* Default Hash function output size in bytes */
#define MR_HASH_BYTES     32
#define NK   37           /* 21 */

typedef struct {
mr_utype *C;
mr_utype *V;
mr_utype *M;
int NP;
} small_chinese;


typedef struct {
int marker;
big X;
big Y;
big Z;
} epoint;

typedef struct {
mr_unsign32 ira[NK];  /* random number...   */
int         rndptr;   /* ...array & pointer */
mr_unsign32 borrow;
int pool_ptr;
char pool[MR_HASH_BYTES];    /* random pool */
} csprng;

/* secure hash Algorithm structure */

typedef struct {
mr_unsign32 length[2];
mr_unsign32 h[8];
mr_unsign32 w[80];
} sha256;

typedef struct {
mr_unsign64 length;
mr_unsign64 S[5][5];
int rate,len;
} sha3;

typedef struct {
int Nk,Nr;
int mode;
mr_unsign32 fkey[60];
mr_unsign32 rkey[60];
char f[16];
} aes;

typedef sha256 sha;

typedef struct
{
    big a;
    big b;
} zzn2;

typedef struct {

mr_small base;       /* number base     */
mr_small apbase;     /* apparent base   */
int   pack;          /* packing density */
int   lg2b;          /* bits in base    */
mr_small base2;      /* 2^mr_lg2b          */
BOOL (*user)(void);  /* pointer to user supplied function */

BOOL  check;         /* overflow check  */
BOOL  fout;          /* Output to file   */
BOOL  fin;           /* Input from file  */
BOOL  active;

FILE  *infile;       /* Input file       */
FILE  *otfile;       /* Output file      */

mr_unsign32 ira[NK];  /* random number...   */
int         rndptr;   /* ...array & pointer */
mr_unsign32 borrow;

BOOL SS;               /* True for Super-Singular  */

mr_small ndash;
big modulus;
big pR;
BOOL ACTIVE;
BOOL MONTY;
int coord;
int Asize,Bsize;

int logN;           /* constants for fast fourier fft multiplication */
int nprimes,degree;
small_chinese chin;

BOOL same;
BOOL first_one;
BOOL debug;

big w0;            /* workspace bigs  */
big w1,w2,w3,w4;
big w5,w6,w7;
big w8,w9,w10,w11;
big w12,w13,w14,w15;
big sru;
big one;

big A,B;

/* User modifiables */
int  IOBSIZ;       /* size of i/o buffer */

BOOL ERCON;        /* error control   */
int  ERNUM;        /* last error code *////
int  NTRY;         /* no. of tries for probablistic primality testing   */

int  INPLEN;       /* input length               */
int  IOBASE;       /* base for input and output */

int   nib;           /* length of bigs  */
BOOL TRACER;       /* turns trace tracker on/off */
int   depth;                 /* error tracing ..*/
int   trace[MR_MAXDEPTH];    /* .. mechanism    */

int *PRIMES;        /* small primes array         */
char *IOBUFF;       /* i/o buffer    */

char *workspace;
int TWIST; /* set to twisted curve */
int qnr;    /* a QNR -1 for p=3 mod 4, -2 for p=5 mod 8, 0 otherwise */
int cnr;    /* a cubic non-residue */
int pmod8;
int pmod9;
BOOL NO_CARRY;

} miracl;

#define _MIPT_
#define _MIPTO_  void
#define _MIPD_
#define _MIPDO_  void
#define _MIPP_
#define _MIPPO_

extern miracl *mr_mip;  /* pointer to MIRACL's only global variable */
extern miracl *get_mip(void );
extern mr_small prepare_monty(_MIPT_ big);
extern mr_small brand(_MIPTO_ );
extern void  irand(_MIPT_ mr_unsign32);

extern flash  mirvar(_MIPT_ int);
extern flash  mirvar_mem(_MIPT_ char *,int);
extern miracl *mirsys(int,mr_small);
extern miracl *mirsys_basic(miracl *,int,mr_small);
extern flash  mirvar_mem_variable(char *,int,int);
extern void   mirexit(_MIPTO_ );

extern void   *memalloc(_MIPT_ int);
extern void*  ecp_memalloc(_MIPT_ int);
void ecp_memkill(_MIPT_ char *,int);
extern void   memkill(_MIPT_ char *,int);

extern void  gprime(_MIPT_ int);
extern BOOL  isprime(_MIPT_ big);
extern int   trial_division(_MIPT_ big,big);

extern void  set_io_buffer_size(_MIPT_ int);
extern void  add(_MIPT_ big,big,big);
extern void  subtract(_MIPT_ big,big,big);
extern void  incr(_MIPT_ big,int,big);
extern void  decr(_MIPT_ big,int,big);
extern void  premult(_MIPT_ big,int,big);
extern int   subdiv(_MIPT_ big,int,big);
extern int   big_to_bytes(_MIPT_ int,big,char *,BOOL);
extern void  bigrand(_MIPT_ big,big);
extern void  bigdig(_MIPT_ int,int,big);
extern void  bigbits(_MIPT_ int,big);
extern void  divide(_MIPT_ big,big,big);
extern void  multiply(_MIPT_ big,big,big);
extern void  copy(flash,flash);
extern int   cinstr(_MIPT_ flash,char *);
extern void  convert(_MIPT_ int,big);
extern void  uconvert(_MIPT_ unsigned int,big);
extern void  lgconv(_MIPT_ long,big);
extern void  ulgconv(_MIPT_ unsigned long,big);
extern void  tconvert(_MIPT_ mr_utype,big);
extern void  dlconv(_MIPT_ mr_dltype,big);
extern void  negify(flash,flash);
extern int   getdig(_MIPT_ big,int);
extern void  putdig(_MIPT_ int,big,int);
extern int   recode(_MIPT_ big ,int ,int ,int );
extern int   logb2(_MIPT_ big);
extern int   hamming(_MIPT_ big);
extern void  bytes_to_big(_MIPT_ int,const char *,big);
extern void  zero(flash);
extern void  redc(_MIPT_ big,big);
extern int   invmodp(_MIPT_ big,big,big);
extern int   xgcd(_MIPT_ big,big,big,big,big);
extern void  mad(_MIPT_ big,big,big,big,big,big);
extern int   jack(_MIPT_ big,big);
extern void  absol(flash,flash);
extern int   numdig(_MIPT_ big);
extern void  kill_monty(_MIPTO_ );
extern int   otstr(_MIPT_ flash,char *);
extern int   instr(_MIPT_ flash,char *);
extern int   cotstr(_MIPT_ flash,char *);
extern void  insign(int,flash);
extern int   exsign(flash);
extern void  expb2(_MIPT_ int,big);
extern int   size(big);
extern int   remain(_MIPT_ big,int);
extern void  expint(_MIPT_ int,int,big);

extern void  power(_MIPT_ big,long,big,big);
extern void  powmod(_MIPT_ big,big,big,big);
extern void  powmod2(_MIPT_ big,big,big,big,big,big);
extern int   powltr(_MIPT_ int,big,big,big);

extern void  strong_bigrand(_MIPT_ csprng *,big,big);
extern void  strong_bigdig(_MIPT_ csprng *,int,int,big);
extern int   strong_rng(csprng *);

extern void  shs256_init(sha256 *);
extern void  shs256_process(sha256 *,int);
extern void  shs256_hash(sha256 *,char *);
extern void  sha3_init(sha3 *,int);
extern void  sha3_process(sha3 *,int);
extern void  sha3_hash(sha3 *,char *);

extern void ecurve_init(_MIPT_ big,big,big,int);
extern int  ecurve_add(_MIPT_ epoint *,epoint *);
extern void ecurve_double(_MIPT_ epoint*);
extern int  ecurve_mult(_MIPT_ big,epoint *,epoint *);
extern void ecurve_mult2(_MIPT_ big,epoint *,big,epoint *,epoint *);
extern int  ecurve_sub(_MIPT_ epoint *,epoint *);
extern void ecurve_multn(_MIPT_ int,big *,epoint**,epoint *);

extern BOOL    epoint_x(_MIPT_ big);
extern void    epoint_copy(epoint *,epoint *);
extern BOOL    epoint_set(_MIPT_ big,big,int,epoint*);
extern int     epoint_get(_MIPT_ epoint*,big,big);
extern BOOL    epoint_norm(_MIPT_ epoint *);
extern BOOL    epoint_comp(_MIPT_ epoint *,epoint *);
extern epoint* epoint_init(_MIPTO_ );
extern epoint* epoint_init_mem(_MIPT_ char *,int);
extern epoint* epoint_init_mem_variable(_MIPT_ char *,int,int);
extern void    epoint_negate(_MIPT_ epoint *);
extern BOOL    epoint_multi_norm(_MIPT_ int,big *,epoint **);
extern void    epoint_free(epoint *);

extern void  nres(_MIPT_ big,big);
extern void  nres_negate(_MIPT_ big,big);
extern void  nres_modadd(_MIPT_ big,big,big);
extern void  nres_modsub(_MIPT_ big,big,big);
extern void  nres_premult(_MIPT_ big,int,big);
extern void  nres_modmult(_MIPT_ big,big,big);
extern BOOL  nres_sqroot(_MIPT_ big,big);
extern int   nres_moddiv(_MIPT_ big,big,big);
extern void  nres_div2(_MIPT_ big,big);
extern void  nres_complex(_MIPT_ big,big,big,big);
extern void  nres_lazy(_MIPT_ big,big,big,big,big,big);
extern void  nres_double_modadd(_MIPT_ big,big,big);
extern void  nres_double_modsub(_MIPT_ big,big,big);
extern void  nres_powmod(_MIPT_ big,big,big);
extern void  nres_powltr(_MIPT_ int,big,big);
extern void  nres_powmod2(_MIPT_ big,big,big,big,big);
extern BOOL  nres_multi_inverse(_MIPT_ int,big *,big *);
extern void  nres_lucas(_MIPT_ big,big,big,big);

extern BOOL  aes_init(aes *,int,int,char *,char *);
extern void  aes_getreg(aes *,char *);
extern void  aes_ecb_encrypt(aes *,MR_BYTE *);
extern void  aes_ecb_decrypt(aes *,MR_BYTE *);
extern mr_unsign32 aes_encrypt(aes *,char *);
extern mr_unsign32 aes_decrypt(aes *,char *);
extern void  aes_reset(aes *,int,char *);
extern void  aes_end(aes *);

extern void zzn2_zero(zzn2 *);
extern void zzn2_from_int(_MIPT_ int,zzn2 *);
extern void zzn2_from_ints(_MIPT_ int,int,zzn2 *);
extern void zzn2_from_zzns(big,big,zzn2 *);
extern void zzn2_from_bigs(_MIPT_ big,big,zzn2 *);
extern void zzn2_from_zzn(big,zzn2 *);
extern void zzn2_from_big(_MIPT_ big, zzn2 *);
extern void zzn2_copy(zzn2 *,zzn2 *);
extern BOOL zzn2_iszero(zzn2 *);
extern void zzn2_mul(_MIPT_ zzn2 *,zzn2 *,zzn2 *);
extern BOOL zzn2_compare(zzn2 *,zzn2 *);
extern void zzn2_add(_MIPT_ zzn2 *,zzn2 *,zzn2 *);
extern void zzn2_sadd(_MIPT_ zzn2 *,big,zzn2 *);
extern void zzn2_conj(_MIPT_ zzn2 *,zzn2 *);
extern void zzn2_inv(_MIPT_ zzn2 *);
extern void zzn2_smul(_MIPT_ zzn2 *,big,zzn2 *);
extern void zzn2_div2(_MIPT_ zzn2 *);
extern void zzn2_txx(_MIPT_ zzn2 *);
extern void zzn2_sqr(_MIPT_ zzn2 *,zzn2 *);
extern void zzn2_timesi(_MIPT_ zzn2 *);
extern BOOL zzn2_qr(_MIPT_ zzn2 *);

extern mr_small sgcd(mr_small,mr_small);
extern BOOL     subdivisible(_MIPT_ big,int);
extern mr_small normalise(_MIPT_ big,big);
extern mr_small mr_shiftbits(mr_small,int);
extern mr_small mr_sdiv(_MIPT_ big,mr_small,big);
extern void     mr_psub(_MIPT_ big,big,big);
extern void     mr_padd(_MIPT_ big,big,big);
extern void     mr_shift(_MIPT_ big,int,big);
extern mr_small mr_setbase(_MIPT_ mr_small);
extern void     mr_lzero(big);
extern void     mr_track(_MIPTO_ );
extern int      mr_compare(big,big);
extern BOOL     mr_notint(flash);
extern int      mr_window2(_MIPT_ big,big,int,int *,int *);
extern void     mr_berror(_MIPT_ int);
extern void     mr_pmul(_MIPT_ big,mr_small,big);
extern miracl   *mr_first_alloc(void);
extern void     *mr_alloc(_MIPT_ int,int);
extern void     mr_free(void *);
extern int      mr_testbit(_MIPT_ big,int);
extern int      mr_window(_MIPT_ big,int,int *,int *,int);
extern int      mr_naf_window(_MIPT_ big,big,int,int *,int *,int);
extern void     mr_jsf(_MIPT_ big,big,big,big,big,big);
extern void     mr_addbit(_MIPT_ big,int);


extern mr_small muldvm(mr_small,mr_small,mr_small,mr_small *);
extern mr_small muldvd(mr_small,mr_small,mr_small,mr_small *);
extern void     muldvd2(mr_small,mr_small,mr_small *,mr_small *);
extern mr_small muldiv(mr_small,mr_small,mr_small,mr_small,mr_small *);

extern BOOL  nroot(_MIPT_ big,int,big);

#endif // MIRACL_H_INCLUDED
