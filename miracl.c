#include "miracl.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define H0 0x6A09E667L
#define H1 0xBB67AE85L
#define H2 0x3C6EF372L
#define H3 0xA54FF53AL
#define H4 0x510E527FL
#define H5 0x9B05688CL
#define H6 0x1F83D9ABL
#define H7 0x5BE0CD19L

static const mr_unsign64 RC[24]={
0x0000000000000001LL,0x0000000000008082LL,0x800000000000808ALL,0x8000000080008000LL,
0x000000000000808BLL,0x0000000080000001LL,0x8000000080008081LL,0x8000000000008009LL,
0x000000000000008ALL,0x0000000000000088LL,0x0000000080008009LL,0x000000008000000ALL,
0x000000008000808BLL,0x800000000000008BLL,0x8000000000008089LL,0x8000000000008003LL,
0x8000000000008002LL,0x8000000000000080LL,0x000000000000800ALL,0x800000008000000ALL,
0x8000000080008081LL,0x8000000000008080LL,0x0000000080000001LL,0x8000000080008008LL};

static const mr_unsign32 K[64]={
0x428a2f98L,0x71374491L,0xb5c0fbcfL,0xe9b5dba5L,0x3956c25bL,0x59f111f1L,0x923f82a4L,0xab1c5ed5L,
0xd807aa98L,0x12835b01L,0x243185beL,0x550c7dc3L,0x72be5d74L,0x80deb1feL,0x9bdc06a7L,0xc19bf174L,
0xe49b69c1L,0xefbe4786L,0x0fc19dc6L,0x240ca1ccL,0x2de92c6fL,0x4a7484aaL,0x5cb0a9dcL,0x76f988daL,
0x983e5152L,0xa831c66dL,0xb00327c8L,0xbf597fc7L,0xc6e00bf3L,0xd5a79147L,0x06ca6351L,0x14292967L,
0x27b70a85L,0x2e1b2138L,0x4d2c6dfcL,0x53380d13L,0x650a7354L,0x766a0abbL,0x81c2c92eL,0x92722c85L,
0xa2bfe8a1L,0xa81a664bL,0xc24b8b70L,0xc76c51a3L,0xd192e819L,0xd6990624L,0xf40e3585L,0x106aa070L,
0x19a4c116L,0x1e376c08L,0x2748774cL,0x34b0bcb5L,0x391c0cb3L,0x4ed8aa4aL,0x5b9cca4fL,0x682e6ff3L,
0x748f82eeL,0x78a5636fL,0x84c87814L,0x8cc70208L,0x90befffaL,0xa4506cebL,0xbef9a3f7L,0xc67178f2L};

static char *names[] =
{(char *)"your program",(char *)"innum",(char *)"otnum",(char *)"jack",(char *)"normalise",
(char *)"multiply",(char *)"divide",(char *)"incr",(char *)"decr",(char *)"premult",
(char *)"subdiv",(char *)"fdsize",(char *)"egcd",(char *)"cbase",
(char *)"cinnum",(char *)"cotnum",(char *)"nroot",(char *)"power",
(char *)"powmod",(char *)"bigdig",(char *)"bigrand",(char *)"nxprime",(char *)"isprime",
(char *)"mirvar",(char *)"mad",(char *)"multi_inverse",(char *)"putdig",
(char *)"add",(char *)"subtract",(char *)"mirsys",(char *)"xgcd",
(char *)"fpack",(char *)"dconv",(char *)"mr_shift",(char *)"mround",(char *)"fmul",
(char *)"fdiv",(char *)"fadd",(char *)"fsub",(char *)"fcomp",(char *)"fconv",
(char *)"frecip",(char *)"fpmul",(char *)"fincr",(char *)"",(char *)"ftrunc",
(char *)"frand",(char *)"sftbit",(char *)"build",(char *)"logb2",(char *)"expint",
(char *)"fpower",(char *)"froot",(char *)"fpi",(char *)"fexp",(char *)"flog",(char *)"fpowf",
(char *)"ftan",(char *)"fatan",(char *)"fsin",(char *)"fasin",(char *)"fcos",(char *)"facos",
(char *)"ftanh",(char *)"fatanh",(char *)"fsinh",(char *)"fasinh",(char *)"fcosh",
(char *)"facosh",(char *)"flop",(char *)"gprime",(char *)"powltr",(char *)"fft_mult",
(char *)"crt_init",(char *)"crt",(char *)"otstr",(char *)"instr",(char *)"cotstr",(char *)"cinstr",(char *)"powmod2",
(char *)"prepare_monty",(char *)"nres",(char *)"redc",(char *)"nres_modmult",(char *)"nres_powmod",
(char *)"nres_moddiv",(char *)"nres_powltr",(char *)"divisible",(char *)"remain",
(char *)"fmodulo",(char *)"nres_modadd",(char *)"nres_modsub",(char *)"nres_negate",
(char *)"ecurve_init",(char *)"ecurve_add",(char *)"ecurve_mult",
(char *)"epoint_init",(char *)"epoint_set",(char *)"epoint_get",(char *)"nres_powmod2",
(char *)"nres_sqroot",(char *)"sqroot",(char *)"nres_premult",(char *)"ecurve_mult2",
(char *)"ecurve_sub",(char *)"trial_division",(char *)"nxsafeprime",(char *)"nres_lucas",(char *)"lucas",
(char *)"brick_init",(char *)"pow_brick",(char *)"set_user_function",
(char *)"nres_powmodn",(char *)"powmodn",(char *)"ecurve_multn",
(char *)"ebrick_init",(char *)"mul_brick",(char *)"epoint_norm",(char *)"nres_multi_inverse",(char *)"",
(char *)"nres_dotprod",(char *)"epoint_negate",(char *)"ecurve_multi_add",
(char *)"ecurve2_init",(char *)"",(char *)"epoint2_set",(char *)"epoint2_norm",(char *)"epoint2_get",
(char *)"epoint2_comp",(char *)"ecurve2_add",(char *)"epoint2_negate",(char *)"ecurve2_sub",
(char *)"ecurve2_multi_add",(char *)"ecurve2_mult",(char *)"ecurve2_multn",(char *)"ecurve2_mult2",
(char *)"ebrick2_init",(char *)"mul2_brick",(char *)"prepare_basis",(char *)"strong_bigrand",
(char *)"bytes_to_big",(char *)"big_to_bytes",(char *)"set_io_buffer_size",
(char *)"epoint_getxyz",(char *)"epoint_double_add",(char *)"nres_double_inverse",
(char *)"double_inverse",(char *)"epoint_x",(char *)"hamming",(char *)"expb2",(char *)"bigbits",
(char *)"nres_lazy",(char *)"zzn2_imul",(char *)"nres_double_modadd",(char *)"nres_double_modsub",
/*155*/(char *)"",(char *)"zzn2_from_int",(char *)"zzn2_negate",(char *)"zzn2_conj",(char *)"zzn2_add",
(char *)"zzn2_sub",(char *)"zzn2_smul",(char *)"zzn2_mul",(char *)"zzn2_inv",(char *)"zzn2_timesi",(char *)"zzn2_powl",
(char *)"zzn2_from_bigs",(char *)"zzn2_from_big",(char *)"zzn2_from_ints",
(char *)"zzn2_sadd",(char *)"zzn2_ssub",(char *)"zzn2_times_irp",(char *)"zzn2_div2",
(char *)"zzn3_from_int",(char *)"zzn3_from_ints",(char *)"zzn3_from_bigs",
(char *)"zzn3_from_big",(char *)"zzn3_negate",(char *)"zzn3_powq",(char *)"zzn3_init",
(char *)"zzn3_add",(char *)"zzn3_sadd",(char *)"zzn3_sub",(char *)"zzn3_ssub",(char *)"zzn3_smul",
(char *)"zzn3_imul",(char *)"zzn3_mul",(char *)"zzn3_inv",(char *)"zzn3_div2",(char *)"zzn3_timesi",
(char *)"epoint_multi_norm",(char *)"mr_jsf",(char *)"epoint2_multi_norm",
(char *)"ecn2_compare",(char *)"ecn2_norm",(char *)"ecn2_set",(char *)"zzn2_txx",
(char *)"zzn2_txd",(char *)"nres_div2",(char *)"nres_div3",(char *)"zzn2_div3",
(char *)"ecn2_setx",(char *)"ecn2_rhs",(char *)"zzn2_qr",(char *)"zzn2_sqrt",(char *)"ecn2_add",(char *)"ecn2_mul2_jsf",(char *)"ecn2_mul",
(char *)"nres_div5",(char *)"zzn2_div5",(char *)"zzn2_sqr",(char *)"ecn2_add_sub",(char *)"ecn2_psi",(char *)"invmodp",
(char *)"zzn2_multi_inverse",(char *)"ecn2_multi_norm",(char *)"ecn2_precomp",(char *)"ecn2_mul4_gls_v",
(char *)"ecn2_mul2",(char *)"ecn2_precomp_gls",(char *)"ecn2_mul2_gls",
(char *)"ecn2_brick_init",(char *)"ecn2_mul_brick_gls",(char *)"ecn2_multn",(char *)"zzn3_timesi2",
(char *)"nres_complex",(char *)"zzn4_from_int",(char *)"zzn4_negate",(char *)"zzn4_conj",(char *)"zzn4_add",(char *)"zzn4_sadd",(char *)"zzn4_sub",(char *)"zzn4_ssub",(char *)"zzn4_smul",(char *)"zzn4_sqr",
(char *)"zzn4_mul",(char *)"zzn4_inv",(char *)"zzn4_div2",(char *)"zzn4_powq",(char *)"zzn4_tx",(char *)"zzn4_imul",(char *)"zzn4_lmul",(char *)"zzn4_from_big",
(char *)"ecn2_mult4"};

#define PAD  0x80
#define ZERO 0

/* functions */

#define S(n,x) (((x)>>n) | ((x)<<(32-n)))
#define R(n,x) ((x)>>n)

#define Ch(x,y,z)  ((x&y)^(~(x)&z))
#define Maj(x,y,z) ((x&y)^(x&z)^(y&z))
#define Sig0(x)    (S(2,x)^S(13,x)^S(22,x))
#define Sig1(x)    (S(6,x)^S(11,x)^S(25,x))
#define theta0(x)  (S(7,x)^S(18,x)^R(3,x))
#define theta1(x)  (S(17,x)^S(19,x)^R(10,x))

//AES
#define MR_WORD mr_unsign32
#define ROTL(x) (((x)>>7)|((x)<<1))
#define ROTL8(x) (((x)<<8)|((x)>>24))
#define ROTL16(x) (((x)<<16)|((x)>>16))
#define ROTL24(x) (((x)<<24)|((x)>>8))

static const MR_BYTE InCo[4]={0xB,0xD,0x9,0xE};

static const MR_BYTE ptab[]=
{1,3,5,15,17,51,85,255,26,46,114,150,161,248,19,53,
95,225,56,72,216,115,149,164,247,2,6,10,30,34,102,170,
229,52,92,228,55,89,235,38,106,190,217,112,144,171,230,49,
83,245,4,12,20,60,68,204,79,209,104,184,211,110,178,205,
76,212,103,169,224,59,77,215,98,166,241,8,24,40,120,136,
131,158,185,208,107,189,220,127,129,152,179,206,73,219,118,154,
181,196,87,249,16,48,80,240,11,29,39,105,187,214,97,163,
254,25,43,125,135,146,173,236,47,113,147,174,233,32,96,160,
251,22,58,78,210,109,183,194,93,231,50,86,250,21,63,65,
195,94,226,61,71,201,64,192,91,237,44,116,156,191,218,117,
159,186,213,100,172,239,42,126,130,157,188,223,122,142,137,128,
155,182,193,88,232,35,101,175,234,37,111,177,200,67,197,84,
252,31,33,99,165,244,7,9,27,45,119,153,176,203,70,202,
69,207,74,222,121,139,134,145,168,227,62,66,198,81,243,14,
18,54,90,238,41,123,141,140,143,138,133,148,167,242,13,23,
57,75,221,124,132,151,162,253,28,36,108,180,199,82,246,1};

static const MR_BYTE ltab[]=
{0,255,25,1,50,2,26,198,75,199,27,104,51,238,223,3,
100,4,224,14,52,141,129,239,76,113,8,200,248,105,28,193,
125,194,29,181,249,185,39,106,77,228,166,114,154,201,9,120,
101,47,138,5,33,15,225,36,18,240,130,69,53,147,218,142,
150,143,219,189,54,208,206,148,19,92,210,241,64,70,131,56,
102,221,253,48,191,6,139,98,179,37,226,152,34,136,145,16,
126,110,72,195,163,182,30,66,58,107,40,84,250,133,61,186,
43,121,10,21,155,159,94,202,78,212,172,229,243,115,167,87,
175,88,168,80,244,234,214,116,79,174,233,213,231,230,173,232,
44,215,117,122,235,22,11,245,89,203,95,176,156,169,81,160,
127,12,246,111,23,196,73,236,216,67,31,45,164,118,123,183,
204,187,62,90,251,96,177,134,59,82,161,108,170,85,41,157,
151,178,135,144,97,190,220,252,188,149,207,205,55,63,91,209,
83,57,132,60,65,162,109,71,20,42,158,93,86,242,211,171,
68,17,146,217,35,32,46,137,180,124,184,38,119,153,227,165,
103,74,237,222,197,49,254,24,13,99,140,128,192,247,112,7};

static const MR_BYTE fbsub[]=
{99,124,119,123,242,107,111,197,48,1,103,43,254,215,171,118,
202,130,201,125,250,89,71,240,173,212,162,175,156,164,114,192,
183,253,147,38,54,63,247,204,52,165,229,241,113,216,49,21,
4,199,35,195,24,150,5,154,7,18,128,226,235,39,178,117,
9,131,44,26,27,110,90,160,82,59,214,179,41,227,47,132,
83,209,0,237,32,252,177,91,106,203,190,57,74,76,88,207,
208,239,170,251,67,77,51,133,69,249,2,127,80,60,159,168,
81,163,64,143,146,157,56,245,188,182,218,33,16,255,243,210,
205,12,19,236,95,151,68,23,196,167,126,61,100,93,25,115,
96,129,79,220,34,42,144,136,70,238,184,20,222,94,11,219,
224,50,58,10,73,6,36,92,194,211,172,98,145,149,228,121,
231,200,55,109,141,213,78,169,108,86,244,234,101,122,174,8,
186,120,37,46,28,166,180,198,232,221,116,31,75,189,139,138,
112,62,181,102,72,3,246,14,97,53,87,185,134,193,29,158,
225,248,152,17,105,217,142,148,155,30,135,233,206,85,40,223,
140,161,137,13,191,230,66,104,65,153,45,15,176,84,187,22};

static const MR_BYTE rbsub[]=
{82,9,106,213,48,54,165,56,191,64,163,158,129,243,215,251,
124,227,57,130,155,47,255,135,52,142,67,68,196,222,233,203,
84,123,148,50,166,194,35,61,238,76,149,11,66,250,195,78,
8,46,161,102,40,217,36,178,118,91,162,73,109,139,209,37,
114,248,246,100,134,104,152,22,212,164,92,204,93,101,182,146,
108,112,72,80,253,237,185,218,94,21,70,87,167,141,157,132,
144,216,171,0,140,188,211,10,247,228,88,5,184,179,69,6,
208,44,30,143,202,63,15,2,193,175,189,3,1,19,138,107,
58,145,17,65,79,103,220,234,151,242,207,206,240,180,230,115,
150,172,116,34,231,173,53,133,226,249,55,232,28,117,223,110,
71,241,26,113,29,41,197,137,111,183,98,14,170,24,190,27,
252,86,62,75,198,210,121,32,154,219,192,254,120,205,90,244,
31,221,168,51,136,7,199,49,177,18,16,89,39,128,236,95,
96,81,127,169,25,181,74,13,45,229,122,159,147,201,156,239,
160,224,59,77,174,42,245,176,200,235,187,60,131,83,153,97,
23,43,4,126,186,119,214,38,225,105,20,99,85,33,12,125};

static const MR_WORD rco[]=
{1,2,4,8,16,32,64,128,27,54,108,216,171,77,154,47};

static const MR_WORD ftable[]=
{0xa56363c6,0x847c7cf8,0x997777ee,0x8d7b7bf6,0xdf2f2ff,0xbd6b6bd6,
0xb16f6fde,0x54c5c591,0x50303060,0x3010102,0xa96767ce,0x7d2b2b56,
0x19fefee7,0x62d7d7b5,0xe6abab4d,0x9a7676ec,0x45caca8f,0x9d82821f,
0x40c9c989,0x877d7dfa,0x15fafaef,0xeb5959b2,0xc947478e,0xbf0f0fb,
0xecadad41,0x67d4d4b3,0xfda2a25f,0xeaafaf45,0xbf9c9c23,0xf7a4a453,
0x967272e4,0x5bc0c09b,0xc2b7b775,0x1cfdfde1,0xae93933d,0x6a26264c,
0x5a36366c,0x413f3f7e,0x2f7f7f5,0x4fcccc83,0x5c343468,0xf4a5a551,
0x34e5e5d1,0x8f1f1f9,0x937171e2,0x73d8d8ab,0x53313162,0x3f15152a,
0xc040408,0x52c7c795,0x65232346,0x5ec3c39d,0x28181830,0xa1969637,
0xf05050a,0xb59a9a2f,0x907070e,0x36121224,0x9b80801b,0x3de2e2df,
0x26ebebcd,0x6927274e,0xcdb2b27f,0x9f7575ea,0x1b090912,0x9e83831d,
0x742c2c58,0x2e1a1a34,0x2d1b1b36,0xb26e6edc,0xee5a5ab4,0xfba0a05b,
0xf65252a4,0x4d3b3b76,0x61d6d6b7,0xceb3b37d,0x7b292952,0x3ee3e3dd,
0x712f2f5e,0x97848413,0xf55353a6,0x68d1d1b9,0x0,0x2cededc1,
0x60202040,0x1ffcfce3,0xc8b1b179,0xed5b5bb6,0xbe6a6ad4,0x46cbcb8d,
0xd9bebe67,0x4b393972,0xde4a4a94,0xd44c4c98,0xe85858b0,0x4acfcf85,
0x6bd0d0bb,0x2aefefc5,0xe5aaaa4f,0x16fbfbed,0xc5434386,0xd74d4d9a,
0x55333366,0x94858511,0xcf45458a,0x10f9f9e9,0x6020204,0x817f7ffe,
0xf05050a0,0x443c3c78,0xba9f9f25,0xe3a8a84b,0xf35151a2,0xfea3a35d,
0xc0404080,0x8a8f8f05,0xad92923f,0xbc9d9d21,0x48383870,0x4f5f5f1,
0xdfbcbc63,0xc1b6b677,0x75dadaaf,0x63212142,0x30101020,0x1affffe5,
0xef3f3fd,0x6dd2d2bf,0x4ccdcd81,0x140c0c18,0x35131326,0x2fececc3,
0xe15f5fbe,0xa2979735,0xcc444488,0x3917172e,0x57c4c493,0xf2a7a755,
0x827e7efc,0x473d3d7a,0xac6464c8,0xe75d5dba,0x2b191932,0x957373e6,
0xa06060c0,0x98818119,0xd14f4f9e,0x7fdcdca3,0x66222244,0x7e2a2a54,
0xab90903b,0x8388880b,0xca46468c,0x29eeeec7,0xd3b8b86b,0x3c141428,
0x79dedea7,0xe25e5ebc,0x1d0b0b16,0x76dbdbad,0x3be0e0db,0x56323264,
0x4e3a3a74,0x1e0a0a14,0xdb494992,0xa06060c,0x6c242448,0xe45c5cb8,
0x5dc2c29f,0x6ed3d3bd,0xefacac43,0xa66262c4,0xa8919139,0xa4959531,
0x37e4e4d3,0x8b7979f2,0x32e7e7d5,0x43c8c88b,0x5937376e,0xb76d6dda,
0x8c8d8d01,0x64d5d5b1,0xd24e4e9c,0xe0a9a949,0xb46c6cd8,0xfa5656ac,
0x7f4f4f3,0x25eaeacf,0xaf6565ca,0x8e7a7af4,0xe9aeae47,0x18080810,
0xd5baba6f,0x887878f0,0x6f25254a,0x722e2e5c,0x241c1c38,0xf1a6a657,
0xc7b4b473,0x51c6c697,0x23e8e8cb,0x7cdddda1,0x9c7474e8,0x211f1f3e,
0xdd4b4b96,0xdcbdbd61,0x868b8b0d,0x858a8a0f,0x907070e0,0x423e3e7c,
0xc4b5b571,0xaa6666cc,0xd8484890,0x5030306,0x1f6f6f7,0x120e0e1c,
0xa36161c2,0x5f35356a,0xf95757ae,0xd0b9b969,0x91868617,0x58c1c199,
0x271d1d3a,0xb99e9e27,0x38e1e1d9,0x13f8f8eb,0xb398982b,0x33111122,
0xbb6969d2,0x70d9d9a9,0x898e8e07,0xa7949433,0xb69b9b2d,0x221e1e3c,
0x92878715,0x20e9e9c9,0x49cece87,0xff5555aa,0x78282850,0x7adfdfa5,
0x8f8c8c03,0xf8a1a159,0x80898909,0x170d0d1a,0xdabfbf65,0x31e6e6d7,
0xc6424284,0xb86868d0,0xc3414182,0xb0999929,0x772d2d5a,0x110f0f1e,
0xcbb0b07b,0xfc5454a8,0xd6bbbb6d,0x3a16162c};

static const MR_WORD rtable[]=
{0x50a7f451,0x5365417e,0xc3a4171a,0x965e273a,0xcb6bab3b,0xf1459d1f,
0xab58faac,0x9303e34b,0x55fa3020,0xf66d76ad,0x9176cc88,0x254c02f5,
0xfcd7e54f,0xd7cb2ac5,0x80443526,0x8fa362b5,0x495ab1de,0x671bba25,
0x980eea45,0xe1c0fe5d,0x2752fc3,0x12f04c81,0xa397468d,0xc6f9d36b,
0xe75f8f03,0x959c9215,0xeb7a6dbf,0xda595295,0x2d83bed4,0xd3217458,
0x2969e049,0x44c8c98e,0x6a89c275,0x78798ef4,0x6b3e5899,0xdd71b927,
0xb64fe1be,0x17ad88f0,0x66ac20c9,0xb43ace7d,0x184adf63,0x82311ae5,
0x60335197,0x457f5362,0xe07764b1,0x84ae6bbb,0x1ca081fe,0x942b08f9,
0x58684870,0x19fd458f,0x876cde94,0xb7f87b52,0x23d373ab,0xe2024b72,
0x578f1fe3,0x2aab5566,0x728ebb2,0x3c2b52f,0x9a7bc586,0xa50837d3,
0xf2872830,0xb2a5bf23,0xba6a0302,0x5c8216ed,0x2b1ccf8a,0x92b479a7,
0xf0f207f3,0xa1e2694e,0xcdf4da65,0xd5be0506,0x1f6234d1,0x8afea6c4,
0x9d532e34,0xa055f3a2,0x32e18a05,0x75ebf6a4,0x39ec830b,0xaaef6040,
0x69f715e,0x51106ebd,0xf98a213e,0x3d06dd96,0xae053edd,0x46bde64d,
0xb58d5491,0x55dc471,0x6fd40604,0xff155060,0x24fb9819,0x97e9bdd6,
0xcc434089,0x779ed967,0xbd42e8b0,0x888b8907,0x385b19e7,0xdbeec879,
0x470a7ca1,0xe90f427c,0xc91e84f8,0x0,0x83868009,0x48ed2b32,
0xac70111e,0x4e725a6c,0xfbff0efd,0x5638850f,0x1ed5ae3d,0x27392d36,
0x64d90f0a,0x21a65c68,0xd1545b9b,0x3a2e3624,0xb1670a0c,0xfe75793,
0xd296eeb4,0x9e919b1b,0x4fc5c080,0xa220dc61,0x694b775a,0x161a121c,
0xaba93e2,0xe52aa0c0,0x43e0223c,0x1d171b12,0xb0d090e,0xadc78bf2,
0xb9a8b62d,0xc8a91e14,0x8519f157,0x4c0775af,0xbbdd99ee,0xfd607fa3,
0x9f2601f7,0xbcf5725c,0xc53b6644,0x347efb5b,0x7629438b,0xdcc623cb,
0x68fcedb6,0x63f1e4b8,0xcadc31d7,0x10856342,0x40229713,0x2011c684,
0x7d244a85,0xf83dbbd2,0x1132f9ae,0x6da129c7,0x4b2f9e1d,0xf330b2dc,
0xec52860d,0xd0e3c177,0x6c16b32b,0x99b970a9,0xfa489411,0x2264e947,
0xc48cfca8,0x1a3ff0a0,0xd82c7d56,0xef903322,0xc74e4987,0xc1d138d9,
0xfea2ca8c,0x360bd498,0xcf81f5a6,0x28de7aa5,0x268eb7da,0xa4bfad3f,
0xe49d3a2c,0xd927850,0x9bcc5f6a,0x62467e54,0xc2138df6,0xe8b8d890,
0x5ef7392e,0xf5afc382,0xbe805d9f,0x7c93d069,0xa92dd56f,0xb31225cf,
0x3b99acc8,0xa77d1810,0x6e639ce8,0x7bbb3bdb,0x97826cd,0xf418596e,
0x1b79aec,0xa89a4f83,0x656e95e6,0x7ee6ffaa,0x8cfbc21,0xe6e815ef,
0xd99be7ba,0xce366f4a,0xd4099fea,0xd67cb029,0xafb2a431,0x31233f2a,
0x3094a5c6,0xc066a235,0x37bc4e74,0xa6ca82fc,0xb0d090e0,0x15d8a733,
0x4a9804f1,0xf7daec41,0xe50cd7f,0x2ff69117,0x8dd64d76,0x4db0ef43,
0x544daacc,0xdf0496e4,0xe3b5d19e,0x1b886a4c,0xb81f2cc1,0x7f516546,
0x4ea5e9d,0x5d358c01,0x737487fa,0x2e410bfb,0x5a1d67b3,0x52d2db92,
0x335610e9,0x1347d66d,0x8c61d79a,0x7a0ca137,0x8e14f859,0x893c13eb,
0xee27a9ce,0x35c961b7,0xede51ce1,0x3cb1477a,0x59dfd29c,0x3f73f255,
0x79ce1418,0xbf37c773,0xeacdf753,0x5baafd5f,0x146f3ddf,0x86db4478,
0x81f3afca,0x3ec468b9,0x2c342438,0x5f40a3c2,0x72c31d16,0xc25e2bc,
0x8b493c28,0x41950dff,0x7101a839,0xdeb30c08,0x9ce4b4d8,0x90c15664,
0x6184cb7b,0x70b632d5,0x745c6c48,0x4257b8d0};

static const MR_WORD ftable1[]=
{0x6363c6a5,0x7c7cf884,0x7777ee99,0x7b7bf68d,0xf2f2ff0d,0x6b6bd6bd,
0x6f6fdeb1,0xc5c59154,0x30306050,0x1010203,0x6767cea9,0x2b2b567d,
0xfefee719,0xd7d7b562,0xabab4de6,0x7676ec9a,0xcaca8f45,0x82821f9d,
0xc9c98940,0x7d7dfa87,0xfafaef15,0x5959b2eb,0x47478ec9,0xf0f0fb0b,
0xadad41ec,0xd4d4b367,0xa2a25ffd,0xafaf45ea,0x9c9c23bf,0xa4a453f7,
0x7272e496,0xc0c09b5b,0xb7b775c2,0xfdfde11c,0x93933dae,0x26264c6a,
0x36366c5a,0x3f3f7e41,0xf7f7f502,0xcccc834f,0x3434685c,0xa5a551f4,
0xe5e5d134,0xf1f1f908,0x7171e293,0xd8d8ab73,0x31316253,0x15152a3f,
0x404080c,0xc7c79552,0x23234665,0xc3c39d5e,0x18183028,0x969637a1,
0x5050a0f,0x9a9a2fb5,0x7070e09,0x12122436,0x80801b9b,0xe2e2df3d,
0xebebcd26,0x27274e69,0xb2b27fcd,0x7575ea9f,0x909121b,0x83831d9e,
0x2c2c5874,0x1a1a342e,0x1b1b362d,0x6e6edcb2,0x5a5ab4ee,0xa0a05bfb,
0x5252a4f6,0x3b3b764d,0xd6d6b761,0xb3b37dce,0x2929527b,0xe3e3dd3e,
0x2f2f5e71,0x84841397,0x5353a6f5,0xd1d1b968,0x0,0xededc12c,
0x20204060,0xfcfce31f,0xb1b179c8,0x5b5bb6ed,0x6a6ad4be,0xcbcb8d46,
0xbebe67d9,0x3939724b,0x4a4a94de,0x4c4c98d4,0x5858b0e8,0xcfcf854a,
0xd0d0bb6b,0xefefc52a,0xaaaa4fe5,0xfbfbed16,0x434386c5,0x4d4d9ad7,
0x33336655,0x85851194,0x45458acf,0xf9f9e910,0x2020406,0x7f7ffe81,
0x5050a0f0,0x3c3c7844,0x9f9f25ba,0xa8a84be3,0x5151a2f3,0xa3a35dfe,
0x404080c0,0x8f8f058a,0x92923fad,0x9d9d21bc,0x38387048,0xf5f5f104,
0xbcbc63df,0xb6b677c1,0xdadaaf75,0x21214263,0x10102030,0xffffe51a,
0xf3f3fd0e,0xd2d2bf6d,0xcdcd814c,0xc0c1814,0x13132635,0xececc32f,
0x5f5fbee1,0x979735a2,0x444488cc,0x17172e39,0xc4c49357,0xa7a755f2,
0x7e7efc82,0x3d3d7a47,0x6464c8ac,0x5d5dbae7,0x1919322b,0x7373e695,
0x6060c0a0,0x81811998,0x4f4f9ed1,0xdcdca37f,0x22224466,0x2a2a547e,
0x90903bab,0x88880b83,0x46468cca,0xeeeec729,0xb8b86bd3,0x1414283c,
0xdedea779,0x5e5ebce2,0xb0b161d,0xdbdbad76,0xe0e0db3b,0x32326456,
0x3a3a744e,0xa0a141e,0x494992db,0x6060c0a,0x2424486c,0x5c5cb8e4,
0xc2c29f5d,0xd3d3bd6e,0xacac43ef,0x6262c4a6,0x919139a8,0x959531a4,
0xe4e4d337,0x7979f28b,0xe7e7d532,0xc8c88b43,0x37376e59,0x6d6ddab7,
0x8d8d018c,0xd5d5b164,0x4e4e9cd2,0xa9a949e0,0x6c6cd8b4,0x5656acfa,
0xf4f4f307,0xeaeacf25,0x6565caaf,0x7a7af48e,0xaeae47e9,0x8081018,
0xbaba6fd5,0x7878f088,0x25254a6f,0x2e2e5c72,0x1c1c3824,0xa6a657f1,
0xb4b473c7,0xc6c69751,0xe8e8cb23,0xdddda17c,0x7474e89c,0x1f1f3e21,
0x4b4b96dd,0xbdbd61dc,0x8b8b0d86,0x8a8a0f85,0x7070e090,0x3e3e7c42,
0xb5b571c4,0x6666ccaa,0x484890d8,0x3030605,0xf6f6f701,0xe0e1c12,
0x6161c2a3,0x35356a5f,0x5757aef9,0xb9b969d0,0x86861791,0xc1c19958,
0x1d1d3a27,0x9e9e27b9,0xe1e1d938,0xf8f8eb13,0x98982bb3,0x11112233,
0x6969d2bb,0xd9d9a970,0x8e8e0789,0x949433a7,0x9b9b2db6,0x1e1e3c22,
0x87871592,0xe9e9c920,0xcece8749,0x5555aaff,0x28285078,0xdfdfa57a,
0x8c8c038f,0xa1a159f8,0x89890980,0xd0d1a17,0xbfbf65da,0xe6e6d731,
0x424284c6,0x6868d0b8,0x414182c3,0x999929b0,0x2d2d5a77,0xf0f1e11,
0xb0b07bcb,0x5454a8fc,0xbbbb6dd6,0x16162c3a};

static const MR_WORD rtable1[]=
{0xa7f45150,0x65417e53,0xa4171ac3,0x5e273a96,0x6bab3bcb,0x459d1ff1,
0x58faacab,0x3e34b93,0xfa302055,0x6d76adf6,0x76cc8891,0x4c02f525,
0xd7e54ffc,0xcb2ac5d7,0x44352680,0xa362b58f,0x5ab1de49,0x1bba2567,
0xeea4598,0xc0fe5de1,0x752fc302,0xf04c8112,0x97468da3,0xf9d36bc6,
0x5f8f03e7,0x9c921595,0x7a6dbfeb,0x595295da,0x83bed42d,0x217458d3,
0x69e04929,0xc8c98e44,0x89c2756a,0x798ef478,0x3e58996b,0x71b927dd,
0x4fe1beb6,0xad88f017,0xac20c966,0x3ace7db4,0x4adf6318,0x311ae582,
0x33519760,0x7f536245,0x7764b1e0,0xae6bbb84,0xa081fe1c,0x2b08f994,
0x68487058,0xfd458f19,0x6cde9487,0xf87b52b7,0xd373ab23,0x24b72e2,
0x8f1fe357,0xab55662a,0x28ebb207,0xc2b52f03,0x7bc5869a,0x837d3a5,
0x872830f2,0xa5bf23b2,0x6a0302ba,0x8216ed5c,0x1ccf8a2b,0xb479a792,
0xf207f3f0,0xe2694ea1,0xf4da65cd,0xbe0506d5,0x6234d11f,0xfea6c48a,
0x532e349d,0x55f3a2a0,0xe18a0532,0xebf6a475,0xec830b39,0xef6040aa,
0x9f715e06,0x106ebd51,0x8a213ef9,0x6dd963d,0x53eddae,0xbde64d46,
0x8d5491b5,0x5dc47105,0xd406046f,0x155060ff,0xfb981924,0xe9bdd697,
0x434089cc,0x9ed96777,0x42e8b0bd,0x8b890788,0x5b19e738,0xeec879db,
0xa7ca147,0xf427ce9,0x1e84f8c9,0x0,0x86800983,0xed2b3248,
0x70111eac,0x725a6c4e,0xff0efdfb,0x38850f56,0xd5ae3d1e,0x392d3627,
0xd90f0a64,0xa65c6821,0x545b9bd1,0x2e36243a,0x670a0cb1,0xe757930f,
0x96eeb4d2,0x919b1b9e,0xc5c0804f,0x20dc61a2,0x4b775a69,0x1a121c16,
0xba93e20a,0x2aa0c0e5,0xe0223c43,0x171b121d,0xd090e0b,0xc78bf2ad,
0xa8b62db9,0xa91e14c8,0x19f15785,0x775af4c,0xdd99eebb,0x607fa3fd,
0x2601f79f,0xf5725cbc,0x3b6644c5,0x7efb5b34,0x29438b76,0xc623cbdc,
0xfcedb668,0xf1e4b863,0xdc31d7ca,0x85634210,0x22971340,0x11c68420,
0x244a857d,0x3dbbd2f8,0x32f9ae11,0xa129c76d,0x2f9e1d4b,0x30b2dcf3,
0x52860dec,0xe3c177d0,0x16b32b6c,0xb970a999,0x489411fa,0x64e94722,
0x8cfca8c4,0x3ff0a01a,0x2c7d56d8,0x903322ef,0x4e4987c7,0xd138d9c1,
0xa2ca8cfe,0xbd49836,0x81f5a6cf,0xde7aa528,0x8eb7da26,0xbfad3fa4,
0x9d3a2ce4,0x9278500d,0xcc5f6a9b,0x467e5462,0x138df6c2,0xb8d890e8,
0xf7392e5e,0xafc382f5,0x805d9fbe,0x93d0697c,0x2dd56fa9,0x1225cfb3,
0x99acc83b,0x7d1810a7,0x639ce86e,0xbb3bdb7b,0x7826cd09,0x18596ef4,
0xb79aec01,0x9a4f83a8,0x6e95e665,0xe6ffaa7e,0xcfbc2108,0xe815efe6,
0x9be7bad9,0x366f4ace,0x99fead4,0x7cb029d6,0xb2a431af,0x233f2a31,
0x94a5c630,0x66a235c0,0xbc4e7437,0xca82fca6,0xd090e0b0,0xd8a73315,
0x9804f14a,0xdaec41f7,0x50cd7f0e,0xf691172f,0xd64d768d,0xb0ef434d,
0x4daacc54,0x496e4df,0xb5d19ee3,0x886a4c1b,0x1f2cc1b8,0x5165467f,
0xea5e9d04,0x358c015d,0x7487fa73,0x410bfb2e,0x1d67b35a,0xd2db9252,
0x5610e933,0x47d66d13,0x61d79a8c,0xca1377a,0x14f8598e,0x3c13eb89,
0x27a9ceee,0xc961b735,0xe51ce1ed,0xb1477a3c,0xdfd29c59,0x73f2553f,
0xce141879,0x37c773bf,0xcdf753ea,0xaafd5f5b,0x6f3ddf14,0xdb447886,
0xf3afca81,0xc468b93e,0x3424382c,0x40a3c25f,0xc31d1672,0x25e2bc0c,
0x493c288b,0x950dff41,0x1a83971,0xb30c08de,0xe4b4d89c,0xc1566490,
0x84cb7b61,0xb632d570,0x5c6c4874,0x57b8d042};

static const MR_WORD ftable2[]=
{0x63c6a563,0x7cf8847c,0x77ee9977,0x7bf68d7b,0xf2ff0df2,0x6bd6bd6b,
0x6fdeb16f,0xc59154c5,0x30605030,0x1020301,0x67cea967,0x2b567d2b,
0xfee719fe,0xd7b562d7,0xab4de6ab,0x76ec9a76,0xca8f45ca,0x821f9d82,
0xc98940c9,0x7dfa877d,0xfaef15fa,0x59b2eb59,0x478ec947,0xf0fb0bf0,
0xad41ecad,0xd4b367d4,0xa25ffda2,0xaf45eaaf,0x9c23bf9c,0xa453f7a4,
0x72e49672,0xc09b5bc0,0xb775c2b7,0xfde11cfd,0x933dae93,0x264c6a26,
0x366c5a36,0x3f7e413f,0xf7f502f7,0xcc834fcc,0x34685c34,0xa551f4a5,
0xe5d134e5,0xf1f908f1,0x71e29371,0xd8ab73d8,0x31625331,0x152a3f15,
0x4080c04,0xc79552c7,0x23466523,0xc39d5ec3,0x18302818,0x9637a196,
0x50a0f05,0x9a2fb59a,0x70e0907,0x12243612,0x801b9b80,0xe2df3de2,
0xebcd26eb,0x274e6927,0xb27fcdb2,0x75ea9f75,0x9121b09,0x831d9e83,
0x2c58742c,0x1a342e1a,0x1b362d1b,0x6edcb26e,0x5ab4ee5a,0xa05bfba0,
0x52a4f652,0x3b764d3b,0xd6b761d6,0xb37dceb3,0x29527b29,0xe3dd3ee3,
0x2f5e712f,0x84139784,0x53a6f553,0xd1b968d1,0x0,0xedc12ced,
0x20406020,0xfce31ffc,0xb179c8b1,0x5bb6ed5b,0x6ad4be6a,0xcb8d46cb,
0xbe67d9be,0x39724b39,0x4a94de4a,0x4c98d44c,0x58b0e858,0xcf854acf,
0xd0bb6bd0,0xefc52aef,0xaa4fe5aa,0xfbed16fb,0x4386c543,0x4d9ad74d,
0x33665533,0x85119485,0x458acf45,0xf9e910f9,0x2040602,0x7ffe817f,
0x50a0f050,0x3c78443c,0x9f25ba9f,0xa84be3a8,0x51a2f351,0xa35dfea3,
0x4080c040,0x8f058a8f,0x923fad92,0x9d21bc9d,0x38704838,0xf5f104f5,
0xbc63dfbc,0xb677c1b6,0xdaaf75da,0x21426321,0x10203010,0xffe51aff,
0xf3fd0ef3,0xd2bf6dd2,0xcd814ccd,0xc18140c,0x13263513,0xecc32fec,
0x5fbee15f,0x9735a297,0x4488cc44,0x172e3917,0xc49357c4,0xa755f2a7,
0x7efc827e,0x3d7a473d,0x64c8ac64,0x5dbae75d,0x19322b19,0x73e69573,
0x60c0a060,0x81199881,0x4f9ed14f,0xdca37fdc,0x22446622,0x2a547e2a,
0x903bab90,0x880b8388,0x468cca46,0xeec729ee,0xb86bd3b8,0x14283c14,
0xdea779de,0x5ebce25e,0xb161d0b,0xdbad76db,0xe0db3be0,0x32645632,
0x3a744e3a,0xa141e0a,0x4992db49,0x60c0a06,0x24486c24,0x5cb8e45c,
0xc29f5dc2,0xd3bd6ed3,0xac43efac,0x62c4a662,0x9139a891,0x9531a495,
0xe4d337e4,0x79f28b79,0xe7d532e7,0xc88b43c8,0x376e5937,0x6ddab76d,
0x8d018c8d,0xd5b164d5,0x4e9cd24e,0xa949e0a9,0x6cd8b46c,0x56acfa56,
0xf4f307f4,0xeacf25ea,0x65caaf65,0x7af48e7a,0xae47e9ae,0x8101808,
0xba6fd5ba,0x78f08878,0x254a6f25,0x2e5c722e,0x1c38241c,0xa657f1a6,
0xb473c7b4,0xc69751c6,0xe8cb23e8,0xdda17cdd,0x74e89c74,0x1f3e211f,
0x4b96dd4b,0xbd61dcbd,0x8b0d868b,0x8a0f858a,0x70e09070,0x3e7c423e,
0xb571c4b5,0x66ccaa66,0x4890d848,0x3060503,0xf6f701f6,0xe1c120e,
0x61c2a361,0x356a5f35,0x57aef957,0xb969d0b9,0x86179186,0xc19958c1,
0x1d3a271d,0x9e27b99e,0xe1d938e1,0xf8eb13f8,0x982bb398,0x11223311,
0x69d2bb69,0xd9a970d9,0x8e07898e,0x9433a794,0x9b2db69b,0x1e3c221e,
0x87159287,0xe9c920e9,0xce8749ce,0x55aaff55,0x28507828,0xdfa57adf,
0x8c038f8c,0xa159f8a1,0x89098089,0xd1a170d,0xbf65dabf,0xe6d731e6,
0x4284c642,0x68d0b868,0x4182c341,0x9929b099,0x2d5a772d,0xf1e110f,
0xb07bcbb0,0x54a8fc54,0xbb6dd6bb,0x162c3a16};

static const MR_WORD rtable2[]=
{0xf45150a7,0x417e5365,0x171ac3a4,0x273a965e,0xab3bcb6b,0x9d1ff145,
0xfaacab58,0xe34b9303,0x302055fa,0x76adf66d,0xcc889176,0x2f5254c,
0xe54ffcd7,0x2ac5d7cb,0x35268044,0x62b58fa3,0xb1de495a,0xba25671b,
0xea45980e,0xfe5de1c0,0x2fc30275,0x4c8112f0,0x468da397,0xd36bc6f9,
0x8f03e75f,0x9215959c,0x6dbfeb7a,0x5295da59,0xbed42d83,0x7458d321,
0xe0492969,0xc98e44c8,0xc2756a89,0x8ef47879,0x58996b3e,0xb927dd71,
0xe1beb64f,0x88f017ad,0x20c966ac,0xce7db43a,0xdf63184a,0x1ae58231,
0x51976033,0x5362457f,0x64b1e077,0x6bbb84ae,0x81fe1ca0,0x8f9942b,
0x48705868,0x458f19fd,0xde94876c,0x7b52b7f8,0x73ab23d3,0x4b72e202,
0x1fe3578f,0x55662aab,0xebb20728,0xb52f03c2,0xc5869a7b,0x37d3a508,
0x2830f287,0xbf23b2a5,0x302ba6a,0x16ed5c82,0xcf8a2b1c,0x79a792b4,
0x7f3f0f2,0x694ea1e2,0xda65cdf4,0x506d5be,0x34d11f62,0xa6c48afe,
0x2e349d53,0xf3a2a055,0x8a0532e1,0xf6a475eb,0x830b39ec,0x6040aaef,
0x715e069f,0x6ebd5110,0x213ef98a,0xdd963d06,0x3eddae05,0xe64d46bd,
0x5491b58d,0xc471055d,0x6046fd4,0x5060ff15,0x981924fb,0xbdd697e9,
0x4089cc43,0xd967779e,0xe8b0bd42,0x8907888b,0x19e7385b,0xc879dbee,
0x7ca1470a,0x427ce90f,0x84f8c91e,0x0,0x80098386,0x2b3248ed,
0x111eac70,0x5a6c4e72,0xefdfbff,0x850f5638,0xae3d1ed5,0x2d362739,
0xf0a64d9,0x5c6821a6,0x5b9bd154,0x36243a2e,0xa0cb167,0x57930fe7,
0xeeb4d296,0x9b1b9e91,0xc0804fc5,0xdc61a220,0x775a694b,0x121c161a,
0x93e20aba,0xa0c0e52a,0x223c43e0,0x1b121d17,0x90e0b0d,0x8bf2adc7,
0xb62db9a8,0x1e14c8a9,0xf1578519,0x75af4c07,0x99eebbdd,0x7fa3fd60,
0x1f79f26,0x725cbcf5,0x6644c53b,0xfb5b347e,0x438b7629,0x23cbdcc6,
0xedb668fc,0xe4b863f1,0x31d7cadc,0x63421085,0x97134022,0xc6842011,
0x4a857d24,0xbbd2f83d,0xf9ae1132,0x29c76da1,0x9e1d4b2f,0xb2dcf330,
0x860dec52,0xc177d0e3,0xb32b6c16,0x70a999b9,0x9411fa48,0xe9472264,
0xfca8c48c,0xf0a01a3f,0x7d56d82c,0x3322ef90,0x4987c74e,0x38d9c1d1,
0xca8cfea2,0xd498360b,0xf5a6cf81,0x7aa528de,0xb7da268e,0xad3fa4bf,
0x3a2ce49d,0x78500d92,0x5f6a9bcc,0x7e546246,0x8df6c213,0xd890e8b8,
0x392e5ef7,0xc382f5af,0x5d9fbe80,0xd0697c93,0xd56fa92d,0x25cfb312,
0xacc83b99,0x1810a77d,0x9ce86e63,0x3bdb7bbb,0x26cd0978,0x596ef418,
0x9aec01b7,0x4f83a89a,0x95e6656e,0xffaa7ee6,0xbc2108cf,0x15efe6e8,
0xe7bad99b,0x6f4ace36,0x9fead409,0xb029d67c,0xa431afb2,0x3f2a3123,
0xa5c63094,0xa235c066,0x4e7437bc,0x82fca6ca,0x90e0b0d0,0xa73315d8,
0x4f14a98,0xec41f7da,0xcd7f0e50,0x91172ff6,0x4d768dd6,0xef434db0,
0xaacc544d,0x96e4df04,0xd19ee3b5,0x6a4c1b88,0x2cc1b81f,0x65467f51,
0x5e9d04ea,0x8c015d35,0x87fa7374,0xbfb2e41,0x67b35a1d,0xdb9252d2,
0x10e93356,0xd66d1347,0xd79a8c61,0xa1377a0c,0xf8598e14,0x13eb893c,
0xa9ceee27,0x61b735c9,0x1ce1ede5,0x477a3cb1,0xd29c59df,0xf2553f73,
0x141879ce,0xc773bf37,0xf753eacd,0xfd5f5baa,0x3ddf146f,0x447886db,
0xafca81f3,0x68b93ec4,0x24382c34,0xa3c25f40,0x1d1672c3,0xe2bc0c25,
0x3c288b49,0xdff4195,0xa8397101,0xc08deb3,0xb4d89ce4,0x566490c1,
0xcb7b6184,0x32d570b6,0x6c48745c,0xb8d04257};

static const MR_WORD ftable3[]=
{0xc6a56363,0xf8847c7c,0xee997777,0xf68d7b7b,0xff0df2f2,0xd6bd6b6b,
0xdeb16f6f,0x9154c5c5,0x60503030,0x2030101,0xcea96767,0x567d2b2b,
0xe719fefe,0xb562d7d7,0x4de6abab,0xec9a7676,0x8f45caca,0x1f9d8282,
0x8940c9c9,0xfa877d7d,0xef15fafa,0xb2eb5959,0x8ec94747,0xfb0bf0f0,
0x41ecadad,0xb367d4d4,0x5ffda2a2,0x45eaafaf,0x23bf9c9c,0x53f7a4a4,
0xe4967272,0x9b5bc0c0,0x75c2b7b7,0xe11cfdfd,0x3dae9393,0x4c6a2626,
0x6c5a3636,0x7e413f3f,0xf502f7f7,0x834fcccc,0x685c3434,0x51f4a5a5,
0xd134e5e5,0xf908f1f1,0xe2937171,0xab73d8d8,0x62533131,0x2a3f1515,
0x80c0404,0x9552c7c7,0x46652323,0x9d5ec3c3,0x30281818,0x37a19696,
0xa0f0505,0x2fb59a9a,0xe090707,0x24361212,0x1b9b8080,0xdf3de2e2,
0xcd26ebeb,0x4e692727,0x7fcdb2b2,0xea9f7575,0x121b0909,0x1d9e8383,
0x58742c2c,0x342e1a1a,0x362d1b1b,0xdcb26e6e,0xb4ee5a5a,0x5bfba0a0,
0xa4f65252,0x764d3b3b,0xb761d6d6,0x7dceb3b3,0x527b2929,0xdd3ee3e3,
0x5e712f2f,0x13978484,0xa6f55353,0xb968d1d1,0x0,0xc12ceded,
0x40602020,0xe31ffcfc,0x79c8b1b1,0xb6ed5b5b,0xd4be6a6a,0x8d46cbcb,
0x67d9bebe,0x724b3939,0x94de4a4a,0x98d44c4c,0xb0e85858,0x854acfcf,
0xbb6bd0d0,0xc52aefef,0x4fe5aaaa,0xed16fbfb,0x86c54343,0x9ad74d4d,
0x66553333,0x11948585,0x8acf4545,0xe910f9f9,0x4060202,0xfe817f7f,
0xa0f05050,0x78443c3c,0x25ba9f9f,0x4be3a8a8,0xa2f35151,0x5dfea3a3,
0x80c04040,0x58a8f8f,0x3fad9292,0x21bc9d9d,0x70483838,0xf104f5f5,
0x63dfbcbc,0x77c1b6b6,0xaf75dada,0x42632121,0x20301010,0xe51affff,
0xfd0ef3f3,0xbf6dd2d2,0x814ccdcd,0x18140c0c,0x26351313,0xc32fecec,
0xbee15f5f,0x35a29797,0x88cc4444,0x2e391717,0x9357c4c4,0x55f2a7a7,
0xfc827e7e,0x7a473d3d,0xc8ac6464,0xbae75d5d,0x322b1919,0xe6957373,
0xc0a06060,0x19988181,0x9ed14f4f,0xa37fdcdc,0x44662222,0x547e2a2a,
0x3bab9090,0xb838888,0x8cca4646,0xc729eeee,0x6bd3b8b8,0x283c1414,
0xa779dede,0xbce25e5e,0x161d0b0b,0xad76dbdb,0xdb3be0e0,0x64563232,
0x744e3a3a,0x141e0a0a,0x92db4949,0xc0a0606,0x486c2424,0xb8e45c5c,
0x9f5dc2c2,0xbd6ed3d3,0x43efacac,0xc4a66262,0x39a89191,0x31a49595,
0xd337e4e4,0xf28b7979,0xd532e7e7,0x8b43c8c8,0x6e593737,0xdab76d6d,
0x18c8d8d,0xb164d5d5,0x9cd24e4e,0x49e0a9a9,0xd8b46c6c,0xacfa5656,
0xf307f4f4,0xcf25eaea,0xcaaf6565,0xf48e7a7a,0x47e9aeae,0x10180808,
0x6fd5baba,0xf0887878,0x4a6f2525,0x5c722e2e,0x38241c1c,0x57f1a6a6,
0x73c7b4b4,0x9751c6c6,0xcb23e8e8,0xa17cdddd,0xe89c7474,0x3e211f1f,
0x96dd4b4b,0x61dcbdbd,0xd868b8b,0xf858a8a,0xe0907070,0x7c423e3e,
0x71c4b5b5,0xccaa6666,0x90d84848,0x6050303,0xf701f6f6,0x1c120e0e,
0xc2a36161,0x6a5f3535,0xaef95757,0x69d0b9b9,0x17918686,0x9958c1c1,
0x3a271d1d,0x27b99e9e,0xd938e1e1,0xeb13f8f8,0x2bb39898,0x22331111,
0xd2bb6969,0xa970d9d9,0x7898e8e,0x33a79494,0x2db69b9b,0x3c221e1e,
0x15928787,0xc920e9e9,0x8749cece,0xaaff5555,0x50782828,0xa57adfdf,
0x38f8c8c,0x59f8a1a1,0x9808989,0x1a170d0d,0x65dabfbf,0xd731e6e6,
0x84c64242,0xd0b86868,0x82c34141,0x29b09999,0x5a772d2d,0x1e110f0f,
0x7bcbb0b0,0xa8fc5454,0x6dd6bbbb,0x2c3a1616};

static const MR_WORD rtable3[]=
{0x5150a7f4,0x7e536541,0x1ac3a417,0x3a965e27,0x3bcb6bab,0x1ff1459d,
0xacab58fa,0x4b9303e3,0x2055fa30,0xadf66d76,0x889176cc,0xf5254c02,
0x4ffcd7e5,0xc5d7cb2a,0x26804435,0xb58fa362,0xde495ab1,0x25671bba,
0x45980eea,0x5de1c0fe,0xc302752f,0x8112f04c,0x8da39746,0x6bc6f9d3,
0x3e75f8f,0x15959c92,0xbfeb7a6d,0x95da5952,0xd42d83be,0x58d32174,
0x492969e0,0x8e44c8c9,0x756a89c2,0xf478798e,0x996b3e58,0x27dd71b9,
0xbeb64fe1,0xf017ad88,0xc966ac20,0x7db43ace,0x63184adf,0xe582311a,
0x97603351,0x62457f53,0xb1e07764,0xbb84ae6b,0xfe1ca081,0xf9942b08,
0x70586848,0x8f19fd45,0x94876cde,0x52b7f87b,0xab23d373,0x72e2024b,
0xe3578f1f,0x662aab55,0xb20728eb,0x2f03c2b5,0x869a7bc5,0xd3a50837,
0x30f28728,0x23b2a5bf,0x2ba6a03,0xed5c8216,0x8a2b1ccf,0xa792b479,
0xf3f0f207,0x4ea1e269,0x65cdf4da,0x6d5be05,0xd11f6234,0xc48afea6,
0x349d532e,0xa2a055f3,0x532e18a,0xa475ebf6,0xb39ec83,0x40aaef60,
0x5e069f71,0xbd51106e,0x3ef98a21,0x963d06dd,0xddae053e,0x4d46bde6,
0x91b58d54,0x71055dc4,0x46fd406,0x60ff1550,0x1924fb98,0xd697e9bd,
0x89cc4340,0x67779ed9,0xb0bd42e8,0x7888b89,0xe7385b19,0x79dbeec8,
0xa1470a7c,0x7ce90f42,0xf8c91e84,0x0,0x9838680,0x3248ed2b,
0x1eac7011,0x6c4e725a,0xfdfbff0e,0xf563885,0x3d1ed5ae,0x3627392d,
0xa64d90f,0x6821a65c,0x9bd1545b,0x243a2e36,0xcb1670a,0x930fe757,
0xb4d296ee,0x1b9e919b,0x804fc5c0,0x61a220dc,0x5a694b77,0x1c161a12,
0xe20aba93,0xc0e52aa0,0x3c43e022,0x121d171b,0xe0b0d09,0xf2adc78b,
0x2db9a8b6,0x14c8a91e,0x578519f1,0xaf4c0775,0xeebbdd99,0xa3fd607f,
0xf79f2601,0x5cbcf572,0x44c53b66,0x5b347efb,0x8b762943,0xcbdcc623,
0xb668fced,0xb863f1e4,0xd7cadc31,0x42108563,0x13402297,0x842011c6,
0x857d244a,0xd2f83dbb,0xae1132f9,0xc76da129,0x1d4b2f9e,0xdcf330b2,
0xdec5286,0x77d0e3c1,0x2b6c16b3,0xa999b970,0x11fa4894,0x472264e9,
0xa8c48cfc,0xa01a3ff0,0x56d82c7d,0x22ef9033,0x87c74e49,0xd9c1d138,
0x8cfea2ca,0x98360bd4,0xa6cf81f5,0xa528de7a,0xda268eb7,0x3fa4bfad,
0x2ce49d3a,0x500d9278,0x6a9bcc5f,0x5462467e,0xf6c2138d,0x90e8b8d8,
0x2e5ef739,0x82f5afc3,0x9fbe805d,0x697c93d0,0x6fa92dd5,0xcfb31225,
0xc83b99ac,0x10a77d18,0xe86e639c,0xdb7bbb3b,0xcd097826,0x6ef41859,
0xec01b79a,0x83a89a4f,0xe6656e95,0xaa7ee6ff,0x2108cfbc,0xefe6e815,
0xbad99be7,0x4ace366f,0xead4099f,0x29d67cb0,0x31afb2a4,0x2a31233f,
0xc63094a5,0x35c066a2,0x7437bc4e,0xfca6ca82,0xe0b0d090,0x3315d8a7,
0xf14a9804,0x41f7daec,0x7f0e50cd,0x172ff691,0x768dd64d,0x434db0ef,
0xcc544daa,0xe4df0496,0x9ee3b5d1,0x4c1b886a,0xc1b81f2c,0x467f5165,
0x9d04ea5e,0x15d358c,0xfa737487,0xfb2e410b,0xb35a1d67,0x9252d2db,
0xe9335610,0x6d1347d6,0x9a8c61d7,0x377a0ca1,0x598e14f8,0xeb893c13,
0xceee27a9,0xb735c961,0xe1ede51c,0x7a3cb147,0x9c59dfd2,0x553f73f2,
0x1879ce14,0x73bf37c7,0x53eacdf7,0x5f5baafd,0xdf146f3d,0x7886db44,
0xca81f3af,0xb93ec468,0x382c3424,0xc25f40a3,0x1672c31d,0xbc0c25e2,
0x288b493c,0xff41950d,0x397101a8,0x8deb30c,0xd89ce4b4,0x6490c156,
0x7b6184cb,0xd570b632,0x48745c6c,0xd04257b8};


/*** Multi-Threaded Support ***/

miracl *mr_mip=NULL;  /* MIRACL's one and only global variable */
miracl *get_mip()
{
    return (miracl *)mr_mip;
}
miracl *mirsys(int nd,mr_small nb)
{
    mr_mip=mr_first_alloc();
    mr_mip=get_mip();

    return mirsys_basic(mr_mip,nd,nb);
}

int getdig(_MIPD_ big x,int i)
{  /* extract a packed digit */
    int k;
    mr_small n;
    i--;
    n=x->w[i/mr_mip->pack];

    if (mr_mip->pack==1) return (int)n;
    k=i%mr_mip->pack;
    for (i=1;i<=k;i++)
        n=MR_DIV(n,mr_mip->apbase);
    return (int)MR_REMAIN(n,mr_mip->apbase);
}

void putdig(_MIPD_ int n,big x,int i)
{  /* insert a digit into a packed word */
    int j,k,lx;
    mr_small m,p;
    mr_lentype s;
    if (mr_mip->ERNUM) return;

    MR_IN(26)

    s=(x->len&(MR_MSBIT));
    lx=(int)(x->len&(MR_OBITS));
    m=getdig(_MIPP_ x,i);
    p=n;
    i--;
    j=i/mr_mip->pack;
    k=i%mr_mip->pack;
    for (i=1;i<=k;i++)
    {
        m*=mr_mip->apbase;
        p*=mr_mip->apbase;
    }
    if (j>=mr_mip->nib && (mr_mip->check || j>=2*mr_mip->nib))
    {
        mr_berror(_MIPP_ MR_ERR_OVERFLOW);
        MR_OUT
        return;
    }

    x->w[j]=(x->w[j]-m)+p;
    if (j>=lx) x->len=((j+1)|s);
    mr_lzero(x);
    MR_OUT
}


miracl *mirsys_basic(miracl *mr_mip,int nd,mr_small nb)
{
    int i;

    mr_small b,nw;

    if (mr_mip==NULL) return NULL;
    mr_mip->depth=0;
    mr_mip->trace[0]=0;
    mr_mip->depth++;
    mr_mip->trace[mr_mip->depth]=29;
                    /* digest hardware configuration */
    mr_mip->ERCON=FALSE;
    mr_mip->logN=0;
    mr_mip->degree=0;
    mr_mip->chin.NP=0;

    mr_mip->user=NULL;
    mr_mip->same=FALSE;
    mr_mip->first_one=FALSE;
    mr_mip->debug=FALSE;
    mr_mip->coord=MR_NOTSET;

    if (nb==1 || nb>MAXBASE)
    {
        mr_berror(_MIPP_ MR_ERR_BAD_BASE);
        MR_OUT
        return mr_mip;
    }
    mr_setbase(_MIPP_ nb);

    b=mr_mip->base;

    mr_mip->lg2b=0;
    mr_mip->base2=1;
    if (b==0)
    {
        mr_mip->lg2b=MIRACL;
        mr_mip->base2=0;
    }
    else while (b>1)
    {
        b=MR_DIV(b,2);
        mr_mip->lg2b++;
        mr_mip->base2*=2;
    }
/* calculate total space for bigs */
/*

 big -> |int len|small *ptr| alignment space | size in words +1| alignment up to multiple of 4 |


*/
    if (nd>0) nw=MR_ROUNDUP(nd,mr_mip->pack);
    else      nw=MR_ROUNDUP(8*(-nd),mr_mip->lg2b);

    if (nw<1) nw=1;
    mr_mip->nib=(int)(nw+1);   /* add one extra word for small overflows */

    mr_mip->check=ON;

    mr_mip->IOBASE=10;   /* defaults */
    mr_mip->ERNUM=0;

    mr_mip->NTRY=6;
    mr_mip->MONTY=ON;
    mr_mip->TRACER=OFF;
    mr_mip->INPLEN=0;
    mr_mip->IOBSIZ=MR_DEFAULT_BUFFER_SIZE;
    mr_mip->PRIMES=NULL;
    mr_mip->IOBUFF=(char *)mr_alloc(_MIPP_ MR_DEFAULT_BUFFER_SIZE+1,1);
    mr_mip->IOBUFF[0]='\0';
    mr_mip->qnr=0;
    mr_mip->cnr=0;
    mr_mip->TWIST=0;
    mr_mip->pmod8=0;
	mr_mip->pmod9=0;

    mr_mip->ira[0]=0x55555555;
    mr_mip->ira[1]=0x12345678;

    for (i=2;i<NK;i++)
        mr_mip->ira[i]=mr_mip->ira[i-1]+mr_mip->ira[i-2]+0x1379BDF1;
    mr_mip->rndptr=NK;
    mr_mip->borrow=0;

    mr_mip->nib=2*mr_mip->nib+1;
    if (mr_mip->nib!=(int)(mr_mip->nib&(MR_OBITS)))
    {
        mr_berror(_MIPP_ MR_ERR_TOO_BIG);
        mr_mip->nib=(mr_mip->nib-1)/2;
        MR_OUT
        return mr_mip;
    }
    mr_mip->workspace=(char *)memalloc(_MIPP_ MR_SPACES);  /* grab workspace */

    mr_mip->fin=FALSE;
    mr_mip->fout=FALSE;
    mr_mip->active=ON;

    mr_mip->nib=(mr_mip->nib-1)/2;

/* allocate memory for workspace variables */

    mr_mip->w0=mirvar_mem(_MIPP_ mr_mip->workspace,0);  /* double length */
    mr_mip->w1=mirvar_mem(_MIPP_ mr_mip->workspace,2);
    mr_mip->w2=mirvar_mem(_MIPP_ mr_mip->workspace,3);
    mr_mip->w3=mirvar_mem(_MIPP_ mr_mip->workspace,4);
    mr_mip->w4=mirvar_mem(_MIPP_ mr_mip->workspace,5);
    mr_mip->w5=mirvar_mem(_MIPP_ mr_mip->workspace,6);  /* double length */
    mr_mip->w6=mirvar_mem(_MIPP_ mr_mip->workspace,8);  /* double length */
    mr_mip->w7=mirvar_mem(_MIPP_ mr_mip->workspace,10); /* double length */
    mr_mip->w8=mirvar_mem(_MIPP_ mr_mip->workspace,12);
    mr_mip->w9=mirvar_mem(_MIPP_ mr_mip->workspace,13);
    mr_mip->w10=mirvar_mem(_MIPP_ mr_mip->workspace,14);
    mr_mip->w11=mirvar_mem(_MIPP_ mr_mip->workspace,15);
    mr_mip->w12=mirvar_mem(_MIPP_ mr_mip->workspace,16);
    mr_mip->w13=mirvar_mem(_MIPP_ mr_mip->workspace,17);
    mr_mip->w14=mirvar_mem(_MIPP_ mr_mip->workspace,18);
    mr_mip->w15=mirvar_mem(_MIPP_ mr_mip->workspace,19);
    mr_mip->sru=mirvar_mem(_MIPP_ mr_mip->workspace,20);
    mr_mip->modulus=mirvar_mem(_MIPP_ mr_mip->workspace,21);
    mr_mip->pR=mirvar_mem(_MIPP_ mr_mip->workspace,22); /* double length */
    mr_mip->A=mirvar_mem(_MIPP_ mr_mip->workspace,24);
    mr_mip->B=mirvar_mem(_MIPP_ mr_mip->workspace,25);
    mr_mip->one=mirvar_mem(_MIPP_ mr_mip->workspace,26);
    MR_OUT
    return mr_mip;
}


void mirexit(_MIPDO_ )
{ /* clean up after miracl */

    int i;
    mr_mip->ERCON=FALSE;
    mr_mip->active=OFF;
    memkill(_MIPP_ mr_mip->workspace,MR_SPACES);
    for (i=0;i<NK;i++) mr_mip->ira[i]=0L;
    set_io_buffer_size(_MIPP_ 0);
    if (mr_mip->PRIMES!=NULL) mr_free(mr_mip->PRIMES);

    mr_free(mr_mip);

    mr_mip=NULL;
}


flash mirvar(_MIPD_ int iv)
{ /* initialize big/flash number */
    flash x;
    int align;
    char *ptr;
    if(mr_mip==NULL) printf("3\n");
    if (mr_mip->ERNUM) return NULL;
    MR_IN(23);
    if (!(mr_mip->active))
    {
        mr_berror(_MIPP_ MR_ERR_NO_MIRSYS);
        MR_OUT
        return NULL;
    }

    x=(big)mr_alloc(_MIPP_ mr_size(mr_mip->nib-1),1);
    if (x==NULL)
    {
        MR_OUT
        return x;
    }

    ptr=(char *)&x->w;
    align=(unsigned long)(ptr+sizeof(mr_small *))%sizeof(mr_small);

    x->w=(mr_small *)(ptr+sizeof(mr_small *)+sizeof(mr_small)-align);

    if (iv!=0) convert(_MIPP_ iv,x);
    MR_OUT
    return x;
}

flash mirvar_mem(_MIPD_ char *mem,int index)
{ /* initialize big/flash number from pre-allocated memory */

    if (mr_mip->ERNUM) return NULL;

    return mirvar_mem_variable(mem,index,mr_mip->nib-1);

}

static void mputs(char *s)
{ /* output a string */
    int i=0;
    while (s[i]!=0) fputc((int)s[i++],stdout);
}

void mr_berror(_MIPD_ int nerr)
{  /*  Big number error routine  */
    int i;

if (mr_mip->ERCON)
{
    mr_mip->ERNUM=nerr;
    return;
}
mputs((char *)"\nMIRACL error from routine ");
if (mr_mip->depth<MR_MAXDEPTH) mputs(names[mr_mip->trace[mr_mip->depth]]);
else                           mputs((char *)"???");
fputc('\n',stdout);

for (i=mr_mip->depth-1;i>=0;i--)
{
    mputs((char *)"              called from ");
    if (i<MR_MAXDEPTH) mputs(names[mr_mip->trace[i]]);
    else               mputs((char *)"???");
    fputc('\n',stdout);
}

switch (nerr)
{
case 1 :
mputs((char *)"Number base too big for representation\n");
break;
case 2 :
mputs((char *)"Division by zero attempted\n");
break;
case 3 :
mputs((char *)"Overflow - Number too big\n");
break;
case 4 :
mputs((char *)"Internal result is negative\n");
break;
case 5 :
mputs((char *)"Input format error\n");
break;
case 6 :
mputs((char *)"Illegal number base\n");
break;
case 7 :
mputs((char *)"Illegal parameter usage\n");
break;
case 8 :
mputs((char *)"Out of space\n");
break;
case 9 :
mputs((char *)"Even root of a negative number\n");
break;
case 10:
mputs((char *)"Raising integer to negative power\n");
break;
case 11:
mputs((char *)"Attempt to take illegal root\n");
break;
case 12:
mputs((char *)"Integer operation attempted on Flash number\n");
break;
case 13:
mputs((char *)"Flash overflow\n");
break;
case 14:
mputs((char *)"Numbers too big\n");
break;
case 15:
mputs((char *)"Log of a non-positive number\n");
break;
case 16:
mputs((char *)"Flash to double conversion failure\n");
break;
case 17:
mputs((char *)"I/O buffer overflow\n");
break;
case 18:
mputs((char *)"MIRACL not initialised - no call to mirsys()\n");
break;
case 19:
mputs((char *)"Illegal modulus \n");
break;
case 20:
mputs((char *)"No modulus defined\n");
break;
case 21:
mputs((char *)"Exponent too big\n");
break;
case 22:
mputs((char *)"Unsupported Feature - check mirdef.h\n");
break;
case 23:
mputs((char *)"Specified double length type isn't double length\n");
break;
case 24:
mputs((char *)"Specified basis is NOT irreducible\n");
break;
case 25:
mputs((char *)"Unable to control Floating-point rounding\n");
break;
case 26:
mputs((char *)"Base must be binary (MR_ALWAYS_BINARY defined in mirdef.h ?)\n");
break;
case 27:
mputs((char *)"No irreducible basis defined\n");
break;
case 28:
mputs((char *)"Composite modulus\n");
break;
case 29:
mputs((char *)"Input/output error when reading from RNG device node\n");
break;
default:
mputs((char *)"Undefined error\n");
break;
}
exit(0);
}


void *mr_alloc(_MIPD_ int num,int size)
{
    char *p;

    if (mr_mip==NULL)
    {
        p=(char *)calloc(num,size);
        return (void *)p;
    }

    if (mr_mip->ERNUM) return NULL;

    p=(char *)calloc(num,size);
    if (p==NULL) mr_berror(_MIPP_ MR_ERR_OUT_OF_MEMORY);
    return (void *)p;

}

///
void mr_free(void *addr)
{
    if (addr==NULL) return;
    free(addr);
    return;
}

int exsign(flash x)
{ /* extract sign of big/flash number */
    if ((x->len&(MR_MSBIT))==0) return PLUS;
    else                        return MINUS;
}

void mr_padd(_MIPD_ big x,big y,big z)
{ /*  add two  big numbers, z=x+y where *
   *  x and y are positive              */
    int i,lx,ly,lz,la;
    mr_small carry,psum;
    mr_small *gx,*gy,*gz;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    lx = (int)x->len;
    ly = (int)y->len;

    if (ly>lx)
    {
        lz=ly;
        la=lx;
        if (x!=z) copy(y,z);
        else la=ly;
    }
    else
    {
        lz=lx;
        la=ly;
        if (y!=z) copy(x,z);
        else la=lx;
    }
    carry=0;
    z->len=lz;
    gx=x->w; gy=y->w; gz=z->w;
    if (lz<mr_mip->nib || !mr_mip->check) z->len++;
#ifndef MR_SIMPLE_BASE
    if (mr_mip->base==0)
    {
#endif
        for (i=0;i<la;i++)
        { /* add by columns to length of the smaller number */
            psum=gx[i]+gy[i]+carry;
            if (psum>gx[i]) carry=0;
            else if (psum<gx[i]) carry=1;
            gz[i]=psum;
        }
        for (;i<lz && carry>0;i++ )
        { /* add by columns to the length of larger number (if there is a carry) */
            psum=gx[i]+gy[i]+carry;
            if (psum>gx[i]) carry=0;
            else if (psum<gx[i]) carry=1;
            gz[i]=psum;
        }
        if (carry)
        { /* carry left over - possible overflow */
            if (mr_mip->check && i>=mr_mip->nib)
            {
                mr_berror(_MIPP_ MR_ERR_OVERFLOW);
                return;
            }
            gz[i]=carry;
        }
#ifndef MR_SIMPLE_BASE
    }
    else
    {
        for (i=0;i<la;i++)
        { /* add by columns */
            psum=gx[i]+gy[i]+carry;
            carry=0;
            if (psum>=mr_mip->base)
            { /* set carry */
                carry=1;
                psum-=mr_mip->base;
            }
            gz[i]=psum;
        }
        for (;i<lz && carry>0;i++)
        {
            psum=gx[i]+gy[i]+carry;
            carry=0;
            if (psum>=mr_mip->base)
            { /* set carry */
                carry=1;
                psum-=mr_mip->base;
            }
            gz[i]=psum;
        }
        if (carry)
        { /* carry left over - possible overflow */
            if (mr_mip->check && i>=mr_mip->nib)
            {
                mr_berror(_MIPP_ MR_ERR_OVERFLOW);
                return;
            }
            gz[i]=carry;
        }
    }
#endif
    if (gz[z->len-1]==0) z->len--;

}

void mr_psub(_MIPD_ big x,big y,big z)
{  /*  subtract two big numbers z=x-y      *
    *  where x and y are positive and x>y  */
    int i,lx,ly;
    mr_small borrow,pdiff;
    mr_small *gx,*gy,*gz;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    lx = (int)x->len;
    ly = (int)y->len;
    if (ly>lx)
    {
        mr_berror(_MIPP_ MR_ERR_NEG_RESULT);
        return;
    }
    if (y!=z) copy(x,z);
    else ly=lx;
    z->len=lx;
    gx=x->w; gy=y->w; gz=z->w;
    borrow=0;
#ifndef MR_SIMPLE_BASE
    if (mr_mip->base==0)
    {
#endif
        for (i=0;i<ly || borrow>0;i++)
        { /* subtract by columns */
            if (i>lx)
            {
                mr_berror(_MIPP_ MR_ERR_NEG_RESULT);
                return;
            }
            pdiff=gx[i]-gy[i]-borrow;
            if (pdiff<gx[i]) borrow=0;
            else if (pdiff>gx[i]) borrow=1;
            gz[i]=pdiff;
        }
#ifndef MR_SIMPLE_BASE
    }
    else for (i=0;i<ly || borrow>0;i++)
    { /* subtract by columns */
        if (i>lx)
        {
            mr_berror(_MIPP_ MR_ERR_NEG_RESULT);
            return;
        }
        pdiff=gy[i]+borrow;
        borrow=0;
        if (gx[i]>=pdiff) pdiff=gx[i]-pdiff;
        else
        { /* set borrow */
            pdiff=mr_mip->base+gx[i]-pdiff;
            borrow=1;
        }
        gz[i]=pdiff;
    }
#endif
    mr_lzero(z);
}

static void mr_select(_MIPD_ big x,int d,big y,big z)
{ /* perform required add or subtract operation */
    int sx,sy,sz,jf,xgty;
    sx=exsign(x);
    sy=exsign(y);
    sz=0;
    x->len&=MR_OBITS;  /* force operands to be positive */
    y->len&=MR_OBITS;
    xgty=mr_compare(x,y);
    jf=(1+sx)+(1+d*sy)/2;
    switch (jf)
    { /* branch according to signs of operands */
    case 0:
        if (xgty>=0)
            mr_padd(_MIPP_ x,y,z);
        else
            mr_padd(_MIPP_ y,x,z);
        sz=MINUS;
        break;
    case 1:
        if (xgty<=0)
        {
            mr_psub(_MIPP_ y,x,z);
            sz=PLUS;
        }
        else
        {
            mr_psub(_MIPP_ x,y,z);
            sz=MINUS;
        }
        break;
    case 2:
        if (xgty>=0)
        {
            mr_psub(_MIPP_ x,y,z);
            sz=PLUS;
        }
        else
        {
            mr_psub(_MIPP_ y,x,z);
            sz=MINUS;
        }
        break;
    case 3:
        if (xgty>=0)
            mr_padd(_MIPP_ x,y,z);
        else
            mr_padd(_MIPP_ y,x,z);
        sz=PLUS;
        break;
    }
    if (sz<0) z->len^=MR_MSBIT;         /* set sign of result         */
    if (x!=z && sx<0) x->len^=MR_MSBIT; /* restore signs to operands  */
    if (y!=z && y!=x && sy<0) y->len^=MR_MSBIT;
}

void add(_MIPD_ big x,big y,big z)
{  /* add two signed big numbers together z=x+y */
    if (mr_mip->ERNUM) return;

    MR_IN(27)

    mr_select(_MIPP_ x,PLUS,y,z);

    MR_OUT
}

void subtract(_MIPD_ big x,big y,big z)
{ /* subtract two big signed numbers z=x-y */
    if (mr_mip->ERNUM) return;

    MR_IN(28)

    mr_select(_MIPP_ x,MINUS,y,z);

    MR_OUT
}

void incr(_MIPD_ big x,int n,big z)
{  /* add int to big number: z=x+n */
    if (mr_mip->ERNUM) return;

    MR_IN(7)

    convert(_MIPP_ n,mr_mip->w0);
    mr_select(_MIPP_ x,PLUS,mr_mip->w0,z);

    MR_OUT
}

void decr(_MIPD_ big x,int n,big z)
{  /* subtract int from big number: z=x-n */
    if (mr_mip->ERNUM) return;

    MR_IN(8)

    convert(_MIPP_ n,mr_mip->w0);
    mr_select(_MIPP_ x,MINUS,mr_mip->w0,z);

    MR_OUT
}


void multiply(_MIPD_ big x,big y,big z)
{  /*  multiply two big numbers: z=x.y  */
    int i,xl,yl,j,ti;
    mr_small carry,*xg,*yg,*w0g;

    mr_lentype sz;
    big w0;

    if (mr_mip->ERNUM) return;
    if (y->len==0 || x->len==0)
    {
        zero(z);
        return;
    }
    if (x!=mr_mip->w5 && y!=mr_mip->w5 && z==mr_mip->w5) w0=mr_mip->w5;
    else w0=mr_mip->w0;    /* local pointer */

    MR_IN(5)

    sz=((x->len&MR_MSBIT)^(y->len&MR_MSBIT));
    xl=(int)(x->len&MR_OBITS);
    yl=(int)(y->len&MR_OBITS);
    zero(w0);
    if (mr_mip->check && xl+yl>mr_mip->nib)
    {
        mr_berror(_MIPP_ MR_ERR_OVERFLOW);
        MR_OUT
        return;
    }
    if (mr_mip->base==0)
    {
        xg=x->w; yg=y->w; w0g=w0->w;
        if (x==y && xl>SQR_FASTER_THRESHOLD)
                             /* extra hassle make it not    */
                             /* worth it for small numbers */
        { /* fast squaring */
            for (i=0;i<xl-1;i++)
            {  /* long multiplication */
                carry=0;
                for (j=i+1;j<xl;j++)
                { /* Only do above the diagonal */
                    muldvd2(x->w[i],x->w[j],&carry,&w0->w[i+j]);
                }
                w0->w[xl+i]=carry;
            }
            w0->len=xl+xl-1;
            mr_padd(_MIPP_ w0,w0,w0);     /* double it */
            carry=0;
            for (i=0;i<xl;i++)
            { /* add in squared elements */
                ti=i+i;
                muldvd2(x->w[i],x->w[i],&carry,&w0->w[ti]);
                w0->w[ti+1]+=carry;
                if (w0->w[ti+1]<carry) carry=1;
                else                   carry=0;
            }
        }
        else for (i=0;i<xl;i++)
        { /* long multiplication */
            carry=0;
            for (j=0;j<yl;j++)
            { /* multiply each digit of y by x[i] */
                muldvd2(x->w[i],y->w[j],&carry,&w0->w[i+j]);
            }
            w0->w[yl+i]=carry;
        }
    }
    else
    {
        if (x==y && xl>SQR_FASTER_THRESHOLD)
        { /* squaring can be done nearly twice as fast */
            for (i=0;i<xl-1;i++)
            { /* long multiplication */
                carry=0;
                for (j=i+1;j<xl;j++)
                { /* Only do above the diagonal */
              carry=muldiv(x->w[i],x->w[j],w0->w[i+j]+carry,mr_mip->base,&w0->w[i+j]);

                }
                w0->w[xl+i]=carry;
            }
            w0->len=xl+xl-1;
            mr_padd(_MIPP_ w0,w0,w0);     /* double it */
            carry=0;
            for (i=0;i<xl;i++)
            { /* add in squared elements */
                ti=i+i;
                carry=muldiv(x->w[i],x->w[i],w0->w[ti]+carry,mr_mip->base,&w0->w[ti]);
                w0->w[ti+1]+=carry;
                carry=0;
                if (w0->w[ti+1]>=mr_mip->base)
                {
                    carry=1;
                    w0->w[ti+1]-=mr_mip->base;
                }
            }
        }
        else for (i=0;i<xl;i++)
        { /* long multiplication */
            carry=0;
            for (j=0;j<yl;j++)
            { /* multiply each digit of y by x[i] */
                carry=muldiv(x->w[i],y->w[j],w0->w[i+j]+carry,mr_mip->base,&w0->w[i+j]);
            }
            w0->w[yl+i]=carry;
        }
    }
    w0->len=(sz|(xl+yl)); /* set length and sign of result */

    mr_lzero(w0);
    copy(w0,z);
    MR_OUT
}

void premult(_MIPD_ big x,int n,big z)
{ /* premultiply a big number by an int z=x.n */
    if (mr_mip->ERNUM) return;

    MR_IN(9)
    if (n==0)  /* test for some special cases  */
    {
        zero(z);
        MR_OUT
        return;
    }
    if (n==1)
    {
        copy(x,z);
        MR_OUT
        return;
    }
    if (n<0)
    {
        n=(-n);
        mr_pmul(_MIPP_ x,(mr_small)n,z);
        if (z->len!=0) z->len^=MR_MSBIT;
    }
    else mr_pmul(_MIPP_ x,(mr_small)n,z);
    MR_OUT
}

mr_small muldvd(mr_small a,mr_small b,mr_small c,mr_small *rp)
{
    union doubleword dble;
    dble.d=(mr_large)a*b+c;

    *rp=dble.h[MR_BOT];
    return dble.h[MR_TOP];
}

void mr_pmul(_MIPD_ big x,mr_small sn,big z)
{
    int m,xl;
    mr_lentype sx;
    mr_small carry,*xg,*zg;

    if (x!=z)
    {
        zero(z);
        if (sn==0) return;
    }
    else if (sn==0)
    {
        zero(z);
        return;
    }
    m=0;
    carry=0;
    sx=x->len&MR_MSBIT;
    xl=(int)(x->len&MR_OBITS);

    if (mr_mip->base==0)
    {
        xg=x->w; zg=z->w;
/* inline 8086 assembly - substitutes for loop below */
        for (m=0;m<xl;m++)
            carry=muldvd(x->w[m],sn,carry,&z->w[m]);
        if (carry>0)
        {
            m=xl;
            if (m>=mr_mip->nib && mr_mip->check)
            {
                mr_berror(_MIPP_ MR_ERR_OVERFLOW);
                return;
            }
            z->w[m]=carry;
            z->len=m+1;
        }
        else z->len=xl;
    }
    else while (m<xl || carry>0)
    { /* multiply each digit of x by n */

        if (m>mr_mip->nib && mr_mip->check)
        {
            mr_berror(_MIPP_ MR_ERR_OVERFLOW);
            return;
        }
        carry=muldiv(x->w[m],sn,carry,mr_mip->base,&z->w[m]);

        m++;
        z->len=m;
    }
    if (z->len!=0) z->len|=sx;
}


int subdiv(_MIPD_ big x,int n,big z)
{  /*  subdivide a big number by an int   z=x/n  *
    *  returns int remainder                     */
    mr_lentype sx;
    int r,i,msb;
    mr_small lsb;
    if (mr_mip->ERNUM) return 0;

    MR_IN(10)
    if (n==0) mr_berror(_MIPP_ MR_ERR_DIV_BY_ZERO);
    if (mr_mip->ERNUM)
    {
        MR_OUT
        return 0;
    }

    if (x->len==0)
    {
        zero(z);
        MR_OUT
        return 0;
    }
    if (n==1) /* special case */
    {
        copy(x,z);
        MR_OUT
        return 0;
    }
    sx=(x->len&MR_MSBIT);
    if (n==2 && mr_mip->base==0)
    { /* fast division by 2 using shifting */

        copy(x,z);
        msb=(int)(z->len&MR_OBITS)-1;
        r=(int)z->w[0]&1;
        for (i=0;;i++)
        {
            z->w[i]>>=1;
            if (i==msb)
            {
                if (z->w[i]==0) mr_lzero(z);
                break;
            }
            lsb=z->w[i+1]&1;
            z->w[i]|=(lsb<<(MIRACL-1));
        }

        MR_OUT
        if (sx==0) return r;
        else       return (-r);
    }
    if (n<0)
    {
        n=(-n);
        r=(int)mr_sdiv(_MIPP_ x,(mr_small)n,z);
        if (z->len!=0) z->len^=MR_MSBIT;
    }
    else r=(int)mr_sdiv(_MIPP_ x,(mr_small)n,z);
    MR_OUT
    if (sx==0) return r;
    else       return (-r);
}

mr_small mr_sdiv(_MIPD_ big x,mr_small sn,big z)
{
    int i,xl;
    mr_small sr,*xg,*zg;
    sr=0;
    xl=(int)(x->len&MR_OBITS);
    if (x!=z) zero(z);
    if (mr_mip->base==0)
    {
        xg=x->w; zg=z->w;
/* inline - substitutes for loop below */
        for (i=xl-1;i>=0;i--)
        {
            z->w[i]=muldvm(sr,x->w[i],sn,&sr);
        }
    }
    else for (i=xl-1;i>=0;i--)
    { /* divide each digit of x by n */
        z->w[i]=muldiv(sr,mr_mip->base,x->w[i],sn,&sr);
    }
    z->len=x->len;
    mr_lzero(z);
    return sr;
}

mr_small muldvm(mr_small a,mr_small c,mr_small m,mr_small *rp)
{
    mr_small q;
    union doubleword dble;
    dble.h[MR_BOT]=c;
    dble.h[MR_TOP]=a;

    q=(mr_small)(dble.d/m);
    *rp=(mr_small)(dble.d-(mr_large)q*m);
    return q;
}


mr_small normalise(_MIPD_ big x,big y)
{ /* normalise divisor */
    mr_small norm,r;
    int len;

    MR_IN(4)

    if (x!=y) copy(x,y);
    len=(int)(y->len&MR_OBITS);
    if (mr_mip->base==0)
    {
        if ((r=y->w[len-1]+1)==0) norm=1;
        else norm=muldvm((mr_small)1,(mr_small)0,r,&r);
        if (norm!=1) mr_pmul(_MIPP_ y,norm,y);
    }
    else
    {
        norm=MR_DIV(mr_mip->base,(mr_small)(y->w[len-1]+1));
        if (norm!=1) mr_pmul(_MIPP_ y,norm,y);
    }
    MR_OUT
    return norm;
}

void gprime(_MIPD_ int maxp)
{ /* generate all primes less than maxp into global PRIMES */
    char *sv;
    int pix,i,k,prime;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    if (mr_mip->ERNUM) return;
    if (maxp<=0)
    {
        if (mr_mip->PRIMES!=NULL) mr_free(mr_mip->PRIMES);
        mr_mip->PRIMES=NULL;
        return;
    }

    MR_IN(70)

    if (maxp>=MR_TOOBIG)
    {
         mr_berror(_MIPP_ MR_ERR_TOO_BIG);
         MR_OUT
         return;
    }
    if (maxp<MR_MAXPRIME) maxp=MR_MAXPRIME;
    maxp=(maxp+1)/2;
    sv=(char *)mr_alloc(_MIPP_ maxp,1);
    if (sv==NULL)
    {
        mr_berror(_MIPP_ MR_ERR_OUT_OF_MEMORY);
        MR_OUT
        return;
    }
    pix=1;
    for (i=0;i<maxp;i++)
        sv[i]=TRUE;
    for (i=0;i<maxp;i++)
    if (sv[i])
    { /* using sieve of Eratosthenes */
        prime=i+i+3;
        for (k=i+prime;k<maxp;k+=prime)
            sv[k]=FALSE;
        pix++;
    }
    if (mr_mip->PRIMES!=NULL) mr_free(mr_mip->PRIMES);
    mr_mip->PRIMES=(int *)mr_alloc(_MIPP_ pix+2,sizeof(int));
    if (mr_mip->PRIMES==NULL)
    {
        mr_free(sv);
        mr_berror(_MIPP_ MR_ERR_OUT_OF_MEMORY);
        MR_OUT
        return;
    }
    mr_mip->PRIMES[0]=2;
    pix=1;
    for (i=0;i<maxp;i++)
        if (sv[i]) mr_mip->PRIMES[pix++]=i+i+3;
    mr_mip->PRIMES[pix]=0;
    mr_free(sv);
    MR_OUT
    return;
}

int trial_division(_MIPD_ big x,big y)
{
  /* general purpose trial-division function, works in two modes */
  /* if (x==y) quick test for candidate prime, using trial       *
   * division by the small primes in the instance array PRIMES[] */
  /* returns 0 if definitely not a prime *
   * returns 1 if definitely is  a prime *
   * returns 2 if possibly a prime       */
  /* if x!=y it continues to extract small factors, and returns  *
   * the largest factor of x in y,                               *
   * i.e. completely factors over the small primes in PRIMES     *
   * In this case the returned value refers to the status of y   */
  /* returns 1 if y is definitely a prime  (and x was smooth)    *
   * returns 2 if y is possibly a prime                          */
    int i;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    if (mr_mip->ERNUM) return 0;
    if (size(x)<=1) return 0;

    MR_IN(105)

    copy(x,y);

#ifndef MR_STATIC
    if (mr_mip->PRIMES==NULL) gprime(_MIPP_ MR_MAXPRIME);
#endif
    for (i=0;mr_mip->PRIMES[i]!=0;i++)
    { /* test for divisible by first few primes */
        while (subdiv(_MIPP_ y,mr_mip->PRIMES[i],mr_mip->w1)==0)
        {
            if (x==y)
            {
                MR_OUT
                if (size(mr_mip->w1)==1) return 1;
                else return 0;
            }
            else
            {
                if (size(mr_mip->w1)==1)
                { /* y is largest prime factor */
                    MR_OUT
                    return 1;
                }
                copy(mr_mip->w1,y);
                continue;
            }
        }
        if (size(mr_mip->w1)<=mr_mip->PRIMES[i])
        {
            MR_OUT
            return 1;
        }
    }
    MR_OUT
    return 2;
}

BOOL isprime(_MIPD_ big x)
{  /*  test for primality (probably); TRUE if x is prime. test done NTRY *
    *  times; chance of wrong identification << (1/4)^NTRY. Note however *
    *  that this is an extreme upper bound. For example for a 100 digit  *
    *  "prime" the chances of false witness are actually < (.00000003)^NTRY *
    *  See Kim & Pomerance "The probability that a random probable prime *
    *  is Composite", Math. Comp. October 1989 pp.721-741                *
    *  The value of NTRY is now adjusted internally to account for this. */
    int j,k,n,r,times,d;

    if (mr_mip->ERNUM) return TRUE;
    if (size(x)<=1) return FALSE;

    MR_IN(22)

    k=trial_division(_MIPP_ x,x);
    if (k==0)
    {
        MR_OUT
        return FALSE;
    }
    if (k==1)
    {
        MR_OUT
        return TRUE;
    }

/* Miller-Rabin */

    decr(_MIPP_ x,1,mr_mip->w1); /* calculate k and mr_w8 ...    */
    k=0;
    while (subdiv(_MIPP_ mr_mip->w1,2,mr_mip->w1)==0)
    {
        k++;
        copy(mr_mip->w1,mr_mip->w8);
    }              /* ... such that x=1+mr_w8*2**k */
    times=mr_mip->NTRY;
    if (times>100) times=100;
    d=logb2(_MIPP_ x);    /* for larger primes, reduce NTRYs */
    if (d>220) times=2+((10*times)/(d-210));

    for (n=1;n<=times;n++)
    { /* perform test times times */
        j=0;
        do
        {
            r=(int)brand(_MIPPO_ )%MR_TOOBIG;
            if (size(x)<MR_TOOBIG) r%=size(x);
        } while (r<2);
        powltr(_MIPP_ r,mr_mip->w8,x,mr_mip->w9);
/* use small random numbers...  */
        decr(_MIPP_ x,1,mr_mip->w2);

        while ((j>0 || size(mr_mip->w9)!=1)
              && mr_compare(mr_mip->w9,mr_mip->w2)!=0)
        {
            j++;
            if ((j>1 && size(mr_mip->w9)==1) || j==k)
            { /* definitely not prime */
                MR_OUT
                return FALSE;
            }
            mad(_MIPP_ mr_mip->w9,mr_mip->w9,mr_mip->w9,x,x,mr_mip->w9);
        }

        if (mr_mip->user!=NULL) if (!(*mr_mip->user)())
        {
            MR_OUT
            return FALSE;
        }
    }

    MR_OUT
    return TRUE;  /* probably prime */
}

epoint* epoint_init_mem_variable(_MIPD_ char *mem,int index,int sz)
{
    epoint *p;
    char *ptr;
    int offset,r;

    offset=0;
    r=(unsigned long)mem%MR_SL;
    if (r>0) offset=MR_SL-r;

    if (mr_mip->coord==MR_AFFINE)
        p=(epoint *)&mem[offset+index*mr_esize_a(sz)];
    else
        p=(epoint *)&mem[offset+index*mr_esize(sz)];

    ptr=(char *)p+sizeof(epoint);
    p->X=mirvar_mem_variable(ptr,0,sz);
    p->Y=mirvar_mem_variable(ptr,1,sz);
    if (mr_mip->coord!=MR_AFFINE) p->Z=mirvar_mem_variable(ptr,2,sz);
    p->marker=MR_EPOINT_INFINITY; if(p->marker!=NULL)
    return p;
}

epoint* epoint_init_mem(_MIPD_ char *mem,int index)
{
    if (mr_mip->ERNUM) return NULL;

    return epoint_init_mem_variable(_MIPP_ mem,index,mr_mip->nib-1);
}

flash mirvar_mem_variable(char *mem,int index,int sz)
{
    flash x;
    int align;
    char *ptr;
    int offset,r;

/* alignment */
    offset=0;
    r=(unsigned long)mem%MR_SL;
    if (r>0) offset=MR_SL-r;

    x=(big)&mem[offset+mr_size(sz)*index];
    ptr=(char *)&x->w;
    align=(unsigned long)(ptr+sizeof(mr_small *))%sizeof(mr_small);
    x->w=(mr_small *)(ptr+sizeof(mr_small *)+sizeof(mr_small)-align);

    return x;
}

static void epoint_getrhs(_MIPD_ big x,big y)
{ /* x and y must be different */

  /* find x^3+Ax+B */
    nres_modmult(_MIPP_ x,x,y);

    nres_modmult(_MIPP_ y,x,y);
    if (mr_abs(mr_mip->Asize)==MR_TOOBIG)
        nres_modmult(_MIPP_ x,mr_mip->A,mr_mip->w1);
    else
        nres_premult(_MIPP_ x,mr_mip->Asize,mr_mip->w1);
    nres_modadd(_MIPP_ y,mr_mip->w1,y);
    if (mr_abs(mr_mip->Bsize)==MR_TOOBIG)
        nres_modadd(_MIPP_ y,mr_mip->B,y);
    else
    {
        convert(_MIPP_ mr_mip->Bsize,mr_mip->w1);
        nres(_MIPP_ mr_mip->w1,mr_mip->w1);
        nres_modadd(_MIPP_ y,mr_mip->w1,y);
    }
}

BOOL epoint_set(_MIPD_ big x,big y,int cb,epoint *p)
{ /* initialise a point on active ecurve            *
   * if x or y == NULL, set to point at infinity    *
   * if x==y, a y co-ordinate is calculated - if    *
   * possible - and cb suggests LSB 0/1  of y       *
   * (which "decompresses" y). Otherwise, check     *
   * validity of given (x,y) point, ignoring cb.    *
   * Returns TRUE for valid point, otherwise FALSE. */

    BOOL valid;

    if (mr_mip->ERNUM) return FALSE;

    MR_IN(97)

    if (x==NULL || y==NULL)
    {
        copy(mr_mip->one,p->X);
        copy(mr_mip->one,p->Y);
        p->marker=MR_EPOINT_INFINITY;
        MR_OUT
        return TRUE;
    }

/* find x^3+Ax+B */

    nres(_MIPP_ x,p->X);
    epoint_getrhs(_MIPP_ p->X,mr_mip->w3);

    valid=FALSE;

    if (x!=y)
    { /* compare with y^2 */
        nres(_MIPP_ y,p->Y);
        nres_modmult(_MIPP_ p->Y,p->Y,mr_mip->w1);

        if (mr_compare(mr_mip->w1,mr_mip->w3)==0) valid=TRUE;
    }
    else
    { /* no y supplied - calculate one. Find square root */
        valid=nres_sqroot(_MIPP_ mr_mip->w3,p->Y);
    /* check LSB - have we got the right root? */
        redc(_MIPP_ p->Y,mr_mip->w1);
        if (remain(_MIPP_ mr_mip->w1,2)!=cb)
            mr_psub(_MIPP_ mr_mip->modulus,p->Y,p->Y);
    }
    if (valid)
    {
        p->marker=MR_EPOINT_NORMALIZED;
        return TRUE;
    }

    MR_OUT
    return FALSE;
}

void nres(big x,big y)
{ /* convert x to n-residue format */
    if (mr_mip->ERNUM) return;

    MR_IN(81)

    if (size(mr_mip->modulus)==0)
    {
        mr_berror(_MIPP_ MR_ERR_NO_MODULUS);
        MR_OUT
        return;
    }
    copy(x,y);

    divide(y,mr_mip->modulus,mr_mip->modulus);
    if (size(y)<0) add(_MIPP_ y,mr_mip->modulus,y);
    if (!mr_mip->MONTY)
    {
        MR_OUT
        return;
    }
    mr_mip->check=OFF;

    mr_shift(_MIPP_ y,(int)mr_mip->modulus->len,mr_mip->w0);
    divide(_MIPP_ mr_mip->w0,mr_mip->modulus,mr_mip->modulus);
    mr_mip->check=ON;
    copy(mr_mip->w0,y);

    MR_OUT
}

void copy(flash x,flash y)
{  /* copy x to y: y=x  */
    int i,nx,ny;
    mr_small *gx,*gy;
    if (x==y || y==NULL) return;

    if (x==NULL)
    {
        zero(y);
        return;
    }

    ny=(y->len&(MR_OBITS));
    nx=(x->len&(MR_OBITS));
    gx=x->w;
    gy=(y->w);
    if(gy==NULL) gy=(mr_small *)malloc(sizeof(mr_small)*nx);
    for (i=nx;i<ny;i++)
        gy[i]=0;
    for (i=0;i<nx;i++)
        gy[i]=gx[i];
    y->len=x->len;
    y->w=gy;

}

void divide(big x,big y,big z)
{  /*  divide two big numbers  z=x/y : x=x mod y  *
    *  returns quotient only if  divide(x,y,x)    *
    *  returns remainder only if divide(x,y,y)    */
    mr_small carry,attemp,ldy,sdy,ra,r,d,tst,psum;
    mr_lentype sx,sy,sz;
    mr_small borrow,dig,*w0g,*yg;
    int i,k,m,x0,y0,w00;
    big w0;
    BOOL check;
    if (mr_mip->ERNUM) return;
    w0=mr_mip->w0;

    MR_IN(6)

    if (x==y) mr_berror(_MIPP_ MR_ERR_BAD_PARAMETERS);

    if (y->len==0) mr_berror(_MIPP_ MR_ERR_DIV_BY_ZERO);
    if (mr_mip->ERNUM)
    {
        MR_OUT
        return;
    }
    sx=(x->len&MR_MSBIT);   /*  extract signs ... */
    sy=(y->len&MR_MSBIT);
    sz=(sx^sy);
    x->len&=MR_OBITS;   /* ... and force operands to positive  */
    y->len&=MR_OBITS;
    x0=(int)x->len;
    y0=(int)y->len;
    copy(x,w0);
    w00=(int)w0->len;
    if (mr_mip->check && (w00-y0+1>mr_mip->nib))
    {
        mr_berror(_MIPP_ MR_ERR_OVERFLOW);
        MR_OUT
        return;
    }
    d=0;
    if (x0==y0)
    {
        if (x0==1) /* special case - x and y are both mr_smalls */
        {
            d=MR_DIV(w0->w[0],y->w[0]);
            w0->w[0]=MR_REMAIN(w0->w[0],y->w[0]);
            mr_lzero(w0);
        }
        else if (MR_DIV(w0->w[x0-1],4)<y->w[x0-1])
        while (mr_compare(w0,y)>=0)
        {  /* mr_small quotient - so do up to four subtracts instead */
            mr_psub(_MIPP_ w0,y,w0);
            d++;
        }
    }
    if (mr_compare(w0,y)<0)
    {  /*  x less than y - so x becomes remainder */
        if (x!=z)  /* testing parameters */
        {
            copy(w0,x);
            if (x->len!=0) x->len|=sx;
        }
        if (y!=z)
        {
            zero(z);
            z->w[0]=d;
            if (d>0) z->len=(sz|1);
        }
        y->len|=sy;
        MR_OUT
        return;
    }

    if (y0==1)
    {  /* y is int - so use subdiv instead */
        r=mr_sdiv(_MIPP_ w0,y->w[0],w0);
        if (y!=z)
        {
            copy(w0,z);
            z->len|=sz;
        }
        if (x!=z)
        {
            zero(x);
            x->w[0]=r;
            if (r>0) x->len=(sx|1);
        }
        y->len|=sy;
        MR_OUT
        return;
    }
    if (y!=z) zero(z);
    d=normalise(_MIPP_ y,y);
    check=mr_mip->check;
    mr_mip->check=OFF;
    if (mr_mip->base==0)
    {
        if (d!=1) mr_pmul(_MIPP_ w0,d,w0);
        ldy=y->w[y0-1];
        sdy=y->w[y0-2];
        w0g=w0->w; yg=y->w;
        for (k=w00-1;k>=y0-1;k--)
        {  /* long division */
            carry=0;
            if (w0->w[k+1]==ldy) /* guess next quotient digit */
            {
                attemp=(mr_small)(-1);
                ra=ldy+w0->w[k];
                if (ra<ldy) carry=1;
            }
            else attemp=muldvm(w0->w[k+1],w0->w[k],ldy,&ra);
            while (carry==0)
            {
                tst=muldvd(sdy,attemp,(mr_small)0,&r);
                if (tst< ra || (tst==ra && r<=w0->w[k-1])) break;
                attemp--;  /* refine guess */
                ra+=ldy;
                if (ra<ldy) carry=1;
            }
            m=k-y0+1;
            if (attemp>0)
            { /* do partial subtraction */
                borrow=0;
    /*  inline - substitutes for loop below */
                for (i=0;i<y0;i++)
                {
                  borrow=muldvd(attemp,y->w[i],borrow,&dig);
                  if (w0->w[m+i]<dig) borrow++;
                  w0->w[m+i]-=dig;
                }

                if (w0->w[k+1]<borrow)
                {  /* whoops! - over did it */
                    w0->w[k+1]=0;
                    carry=0;
                    for (i=0;i<y0;i++)
                    {  /* compensate for error ... */
                        psum=w0->w[m+i]+y->w[i]+carry;
                        if (psum>y->w[i]) carry=0;
                        if (psum<y->w[i]) carry=1;
                        w0->w[m+i]=psum;
                    }
                    attemp--;  /* ... and adjust guess */
                }
                else w0->w[k+1]-=borrow;
            }
            if (k==w00-1 && attemp==0) w00--;
            else if (y!=z) z->w[m]=attemp;
        }
    }
    else
    {   /* have to do it the hard way */
        if (d!=1) mr_pmul(_MIPP_ w0,d,w0);
        ldy=y->w[y0-1];
        sdy=y->w[y0-2];

        for (k=w00-1;k>=y0-1;k--)
        {  /* long division */


            if (w0->w[k+1]==ldy) /* guess next quotient digit */
            {
                attemp=mr_mip->base-1;
                ra=ldy+w0->w[k];
            }
            else attemp=muldiv(w0->w[k+1],mr_mip->base,w0->w[k],ldy,&ra);
            while (ra<mr_mip->base)
            {
                tst=muldiv(sdy,attemp,(mr_small)0,mr_mip->base,&r);
                if (tst< ra || (tst==ra && r<=w0->w[k-1])) break;
                attemp--;  /* refine guess */
                ra+=ldy;
            }
            m=k-y0+1;
            if (attemp>0)
            { /* do partial subtraction */
                borrow=0;
                for (i=0;i<y0;i++)
                {
                  borrow=muldiv(attemp,y->w[i],borrow,mr_mip->base,&dig);
                  if (w0->w[m+i]<dig)
                  { /* set borrow */
                      borrow++;
                      w0->w[m+i]+=(mr_mip->base-dig);
                  }
                  else w0->w[m+i]-=dig;
                }
                if (w0->w[k+1]<borrow)
                {  /* whoops! - over did it */
                    w0->w[k+1]=0;
                    carry=0;
                    for (i=0;i<y0;i++)
                    {  /* compensate for error ... */
                        psum=w0->w[m+i]+y->w[i]+carry;
                        carry=0;
                        if (psum>=mr_mip->base)
                        {
                            carry=1;
                            psum-=mr_mip->base;
                        }
                        w0->w[m+i]=psum;
                    }
                    attemp--;  /* ... and adjust guess */
                }
                else
                    w0->w[k+1]-=borrow;
            }
            if (k==w00-1 && attemp==0) w00--;
            else if (y!=z) z->w[m]=attemp;
        }
    }
    if (y!=z) z->len=((w00-y0+1)|sz); /* set sign and length of result */

    w0->len=y0;

    mr_lzero(y);
    mr_lzero(z);

    if (x!=z)
    {
        mr_lzero(w0);
        if (d!=1) mr_sdiv(_MIPP_ w0,d,x);
        else copy(w0,x);
        if (x->len!=0) x->len|=sx;
    }
    if (d!=1) mr_sdiv(_MIPP_ y,d,y);
    y->len|=sy;
    mr_mip->check=check;

    MR_OUT
}

void insign(int s,flash x)
{  /* assert sign of big/flash number */
    if (x->len==0) return;
    if (s<0) x->len|=MR_MSBIT;
    else     x->len&=MR_OBITS;
}

mr_small mr_shiftbits(mr_small x,int n)
{
    if (n==0) return x;
    if (n>0) x<<=n;
    else x>>=(-n);
    return x;
}

static void cbase(_MIPD_ big x,mr_small oldbase,big y)
{  /*  change radix of x from oldbase to base  */
    int i,s;
    mr_small n;
    BOOL done;
    if (mr_mip->ERNUM) return;
    if (mr_mip->base==oldbase)
    {
        copy(x,y);
        return;
    }

    MR_IN(13)

    s=exsign(x);
    copy(x,mr_mip->w1);
    done=TRUE;
    insign(PLUS,mr_mip->w1);

    forever
    {
        zero(mr_mip->w6);
        convert(_MIPP_ 1,mr_mip->w0);

        for (i=0;i<(int)mr_mip->w1->len;i++)
        { /* this is a bit slow - but not time critical */


            mr_pmul(_MIPP_ mr_mip->w0,mr_mip->w1->w[i],mr_mip->w5);

            add(_MIPP_ mr_mip->w6,mr_mip->w5,mr_mip->w6);
            if (oldbase==0)
            { /* bit of a frig! */
                n=mr_shiftbits(1,MIRACL/2);
                mr_pmul(_MIPP_ mr_mip->w0,n,mr_mip->w0);
                mr_pmul(_MIPP_ mr_mip->w0,n,mr_mip->w0);
            }
            else mr_pmul(_MIPP_ mr_mip->w0,oldbase,mr_mip->w0);
        }
        if (mr_mip->ERNUM || done) break;
    }
    copy(mr_mip->w6,y);

    insign(s,y);
    MR_OUT
}

int cinstr(_MIPD_ flash x,char *string)
{  /*  input big number in base IOBASE  */
    mr_small newb,oldb,b;
    mr_lentype lx;
    int ipt;
    if (mr_mip->ERNUM) return 0;

    MR_IN(78)

    newb=mr_mip->IOBASE;
    oldb=mr_mip->apbase;
    mr_setbase(_MIPP_ newb); /* temporarily change base ... */
    b=mr_mip->base;
    mr_mip->check=OFF;
    ipt=instr(_MIPP_ mr_mip->w5,string); /* ... and get number  */


    mr_mip->check=ON;
    lx=(mr_mip->w5->len&MR_OBITS);
    if ((int)lx>mr_mip->nib)
    { /* numerator or denominator too big */
        mr_berror(_MIPP_ MR_ERR_OVERFLOW);
        MR_OUT
        return 0;
    }
    mr_setbase(_MIPP_ oldb);      /* restore original base */

    cbase(_MIPP_ mr_mip->w5,b,x);

    MR_OUT
    return ipt;
}

int instr(_MIPD_ flash x,char *string)
{  /*  input a big number       *
    *  returns length in digits */
    int i,ipt,n,s,e,pads;
	BOOL first_after_pad;
    int ch,lc;
#ifdef MR_FLASH
    BOOL frac;
#endif
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    if (mr_mip->ERNUM) return 0;

    MR_IN(76)

    if (mr_mip->apbase==0 || mr_mip->apbase>256)
    {
        mr_berror(_MIPP_ MR_ERR_BASE_TOO_BIG);
        MR_OUT
        return 0;
    }

    if (!mr_mip->active)
    {
        mr_berror(_MIPP_ MR_ERR_NO_MIRSYS);
        MR_OUT
        return 0;
    }

    zero(x);
    if (mr_mip->fin) string=mr_mip->IOBUFF;
    if (mr_mip->INPLEN==0)
    { /* inputting ASCII bytes */
#ifndef MR_NO_FILE_IO

        if (mr_mip->fin)
        { /* read in characters */
            i=0;
            do
            {
                ch=fgetc(mr_mip->infile);
                if (ch==EOF) break;
                string[i++]=ch;

                if (i>=mr_mip->IOBSIZ)
                {
                    mr_berror(_MIPP_ MR_ERR_IO_OVERFLOW);
                    MR_OUT
                    return 0;
                }
            } while (ch!='\n' && ch!='\0');
            string[i]='\0';
        }

#endif
        forever
        { /*  get input length  */
            ch=(unsigned char)string[mr_mip->INPLEN];
            if (ch=='\0') break;
            if (mr_mip->apbase<=60 && ch=='\n') break;
            mr_mip->INPLEN++;
            if (string==mr_mip->IOBUFF && mr_mip->INPLEN>=mr_mip->IOBSIZ)
            {
                mr_berror(_MIPP_ MR_ERR_IO_OVERFLOW);
                MR_OUT
                return 0;
            }
        }
    }
    else
    { /* inputting BINARY bytes */
        if (string==mr_mip->IOBUFF && mr_mip->INPLEN>=mr_mip->IOBSIZ)
        {
            mr_berror(_MIPP_ MR_ERR_IO_OVERFLOW);
            MR_OUT
            return 0;
        }

#ifndef MR_NO_FILE_IO

        if (mr_mip->fin) for(i=0;i<mr_mip->INPLEN;i++)
        {
            if ((ch=fgetc(mr_mip->infile))==EOF)
            {
                mr_mip->INPLEN=i;
                break;
            }
            string[i]=MR_TOBYTE(ch);
        }

#endif

    }
    n=0;
    s=PLUS;
    e=0;
#ifdef MR_FLASH
    frac=FALSE;
#endif
    if (mr_mip->INPLEN>0 && mr_mip->apbase<=60)
    { /* skip leading blanks and check sign */
#ifdef MR_FLASH
        if (string[mr_mip->INPLEN-1]=='/') mr_mip->INPLEN--;
#endif

        while (string[e]==' ') e++;
        if (string[e]=='-')
        { /* check sign */
             s=MINUS;
             e++;
        }
        if (string[e]=='+') e++;
    }

	pads=0; first_after_pad=TRUE;
    for (i=mr_mip->INPLEN-1;i>=e;i--)
    {
        ch=(unsigned char)string[i];
        if (mr_mip->apbase<=60 || mr_mip->apbase==64)
        { /* check for slash or dot and convert character to number */
#ifdef MR_FLASH
            if (mr_mip->apbase<=60 && !frac)
            {
                if (ch=='/')
                {
                    frac=TRUE;
                    copy(x,mr_mip->w0);
                    zero(x);
                    n=0;
                    continue;
                }
                if (ch=='.')
                {
                    frac=TRUE;
                    zero(mr_mip->w0);
                    putdig(_MIPP_ 1,mr_mip->w0,n+1);
                    continue;
                }
            }
#endif
            ch+=80;
            if (mr_mip->apbase==64)
            { /* base64 */
                if (ch<=112) continue; /* ignore white space */
                if (ch>144 && ch<171) ch-=145;
                if (ch>176 && ch<203) ch-=151;
                if (ch>127 && ch<138) ch-=76;
                if (ch==123) ch=62;
                if (ch==127) ch=63;
                if (ch==141) {pads++; continue;} /* pads '=' */
            }
            else
            {
                if (ch>127 && ch<138) ch-=128;
                if (ch>144 && ch<171) ch-=135;
                if (mr_mip->apbase<=16)
                {
                    if (ch>176 && ch<203) ch-=167;
                }
                else
                {
                    if (ch>176 && ch<203) ch-=141;
                }
            }
        }

        if ((mr_small)ch>=mr_mip->apbase || pads>2)
        {
            mr_berror(_MIPP_ MR_ERR_BAD_FORMAT);
            MR_OUT
            return 0;
        }

		if (pads && first_after_pad)
		{ /* there was padding, so adjust */
			lc=ch>>(2*pads);
			first_after_pad=FALSE;
			continue;
		}

		n++;
		if (pads)
		{
			putdig(_MIPP_ 0x3f&((ch<<(6-2*pads))|lc),x,n);
			lc=(ch>>(2*pads));
			continue;
		}

        putdig(_MIPP_ ch,x,n);
    }

	if (pads && lc>0)
		putdig(_MIPP_ lc,x,++n);

    ipt=mr_mip->INPLEN;
    mr_mip->INPLEN=0;
    insign(s,x);
    mr_lzero(x);
    MR_OUT
    return ipt;
}

/* allocate space for a number of epoints from the heap */

void *ecp_memalloc(_MIPD_ int num)
{

    if (mr_mip->coord==MR_AFFINE)
        return mr_alloc(_MIPP_  mr_ecp_reserve_a(num,mr_mip->nib-1),1);
    else
        return mr_alloc(_MIPP_  mr_ecp_reserve(num,mr_mip->nib-1),1);
}


void convert(_MIPD_ int n ,big x)
{  /*  convert signed integer n to big number format  */
    mr_lentype s;

    if (n==0) {zero(x); return;}
    s=0;
    if (n<0)
    {
        s=MR_MSBIT;
        n=(-n);
    }
    uconvert(_MIPP_ (unsigned int)n,x);
    x->len|=s;
}


void uconvert(_MIPD_ unsigned int n ,big x)
{  /*  convert unsigned integer n to big number format  */
    int m;
    zero(x);
    if (n==0) return;
    m=0;
    if (mr_mip->base==0)
    {
        x->w[m++]=(mr_small)n;
    }
    else while (n>0)
    {
        x->w[m++]=MR_REMAIN((mr_small)n,mr_mip->base);
		n=(unsigned int)((mr_small)n/mr_mip->base);
    }
    x->len=m;
}

void lgconv(_MIPD_ long n,big x)
{ /* convert signed long integer to big number format - rarely needed */
    mr_lentype s;
    if (n==0) {zero(x); return;}
    s=0;
    if (n<0)
    {
        s=MR_MSBIT;
        n=(-n);
    }
    ulgconv(_MIPP_ (unsigned long)n,x);

    x->len|=s;
}

void ulgconv(_MIPD_ unsigned long n,big x)
{ /* convert unsigned long integer to big number format - rarely needed */
    int m;
    zero(x);
    if (n==0) return;

    m=0;
    if (mr_mip->base==0)
    {
        while (n>0)
        {
            x->w[m++]=(mr_small)(n%(1L<<(MIRACL)));
            n/=(1L<<(MIRACL));
        }
    }
    else while (n>0)
    {
        x->w[m++]=MR_REMAIN(n,mr_mip->base);
		n=(unsigned long)((mr_small)n/mr_mip->base);
    }
    x->len=m;
}


void tconvert(_MIPD_ mr_utype n,big x)
{
	mr_lentype s;
    if (n==0) {zero(x); return;}
    s=0;
    if (n<0)
    {
        s=MR_MSBIT;
        n=(-n);
    }
	x->w[0]=n;
	x->len=1;
    x->len|=s;
}

BOOL epoint_x(_MIPD_ big x)
{ /* test if x is associated with a point on the   *
   * currently active curve                        */
    int j;

#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    if (mr_mip->ERNUM) return FALSE;

    MR_IN(147)

    if (x==NULL) return FALSE;

    nres(_MIPP_ x,mr_mip->w2);
    epoint_getrhs(_MIPP_ mr_mip->w2,mr_mip->w3);

    if (size(mr_mip->w3)==0)
    {
        MR_OUT
        return TRUE;
    }

    redc(_MIPP_ mr_mip->w3,mr_mip->w4);
    j=jack(_MIPP_ mr_mip->w4,mr_mip->modulus);

    MR_OUT
    if (j==1) return TRUE;
    return FALSE;
}


void irand(_MIPD_ mr_unsign32 seed)
{ /* initialise random number system */
    int i,in;
    mr_unsign32 t,m=1L;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    mr_mip->borrow=0L;
    mr_mip->rndptr=0;
    mr_mip->ira[0]=seed;
    for (i=1;i<NK;i++)
    { /* fill initialisation vector */
        in=(NV*i)%NK;
        mr_mip->ira[in]=m;
        t=m;
        m=seed-m;
        seed=t;
    }
    for (i=0;i<1000;i++) brand(_MIPPO_ ); /* "warm-up" & stir the generator */
}


void dlconv(_MIPD_ mr_dltype n,big x)
{ /* convert double length integer to big number format - rarely needed */
    int m;
    mr_lentype s;
    zero(x);
    if (n==0) return;
    s=0;
    if (n<0)
    {
        s=MR_MSBIT;
        n=(-n);
    }
    m=0;
    if (mr_mip->base==0)
    {
        while (n>0)
        {
            x->w[m++]=(mr_small)(n%((mr_dltype)1<<(MIRACL)));
            n/=((mr_dltype)1<<(MIRACL));
        }
    }
    else while (n>0)
    {
        x->w[m++]=(mr_small)MR_REMAIN(n,mr_mip->base);
        n/=mr_mip->base;
    }
    x->len=(m|s);
}

void negify(flash x,flash y)
{ /* negate a big/flash variable: y=-x */
    copy(x,y);
    if (y->len!=0) y->len^=MR_MSBIT;
}


static void shs_transform(sha256 *sh)
{ /* basic transformation step */
    mr_unsign32 a,b,c,d,e,f,g,h,t1,t2;
    int j;
    for (j=16;j<64;j++)
        sh->w[j]=theta1(sh->w[j-2])+sh->w[j-7]+theta0(sh->w[j-15])+sh->w[j-16];

    a=sh->h[0]; b=sh->h[1]; c=sh->h[2]; d=sh->h[3];
    e=sh->h[4]; f=sh->h[5]; g=sh->h[6]; h=sh->h[7];

    for (j=0;j<64;j++)
    { /* 64 times - mush it up */
        t1=h+Sig1(e)+Ch(e,f,g)+K[j]+sh->w[j];
        t2=Sig0(a)+Maj(a,b,c);
        h=g; g=f; f=e;
        e=d+t1;
        d=c;
        c=b;
        b=a;
        a=t1+t2;
    }
    sh->h[0]+=a; sh->h[1]+=b; sh->h[2]+=c; sh->h[3]+=d;
    sh->h[4]+=e; sh->h[5]+=f; sh->h[6]+=g; sh->h[7]+=h;
}

static void sha_transform(sha3 *sh)
{
	int i,j,k;
	mr_unsign64 C[5],D[5],B[5][5];

	for (k=0;k<SHA3_ROUNDS;k++)
	{
		C[0]=sh->S[0][0]^sh->S[0][1]^sh->S[0][2]^sh->S[0][3]^sh->S[0][4];
		C[1]=sh->S[1][0]^sh->S[1][1]^sh->S[1][2]^sh->S[1][3]^sh->S[1][4];
		C[2]=sh->S[2][0]^sh->S[2][1]^sh->S[2][2]^sh->S[2][3]^sh->S[2][4];
		C[3]=sh->S[3][0]^sh->S[3][1]^sh->S[3][2]^sh->S[3][3]^sh->S[3][4];
		C[4]=sh->S[4][0]^sh->S[4][1]^sh->S[4][2]^sh->S[4][3]^sh->S[4][4];

		D[0]=C[4]^rotl(C[1],1);
		D[1]=C[0]^rotl(C[2],1);
		D[2]=C[1]^rotl(C[3],1);
		D[3]=C[2]^rotl(C[4],1);
		D[4]=C[3]^rotl(C[0],1);

		for (i=0;i<5;i++)
			for (j=0;j<5;j++)
				sh->S[i][j]^=D[i];  /* let the compiler unroll it! */

		B[0][0]=sh->S[0][0];
		B[1][3]=rotl(sh->S[0][1],36);
		B[2][1]=rotl(sh->S[0][2],3);
		B[3][4]=rotl(sh->S[0][3],41);
		B[4][2]=rotl(sh->S[0][4],18);

		B[0][2]=rotl(sh->S[1][0],1);
		B[1][0]=rotl(sh->S[1][1],44);
		B[2][3]=rotl(sh->S[1][2],10);
		B[3][1]=rotl(sh->S[1][3],45);
		B[4][4]=rotl(sh->S[1][4],2);

		B[0][4]=rotl(sh->S[2][0],62);
		B[1][2]=rotl(sh->S[2][1],6);
		B[2][0]=rotl(sh->S[2][2],43);
		B[3][3]=rotl(sh->S[2][3],15);
		B[4][1]=rotl(sh->S[2][4],61);

		B[0][1]=rotl(sh->S[3][0],28);
		B[1][4]=rotl(sh->S[3][1],55);
		B[2][2]=rotl(sh->S[3][2],25);
		B[3][0]=rotl(sh->S[3][3],21);
		B[4][3]=rotl(sh->S[3][4],56);

		B[0][3]=rotl(sh->S[4][0],27);
		B[1][1]=rotl(sh->S[4][1],20);
		B[2][4]=rotl(sh->S[4][2],39);
		B[3][2]=rotl(sh->S[4][3],8);
		B[4][0]=rotl(sh->S[4][4],14);

		for (i=0;i<5;i++)
			for (j=0;j<5;j++)
				sh->S[i][j]=B[i][j]^(~B[(i+1)%5][j]&B[(i+2)%5][j]);

		sh->S[0][0]^=RC[k];
	}
}

void shs256_init(sha256 *sh)
{ /* re-initialise */
    int i;
    for (i=0;i<64;i++) sh->w[i]=0L;
    sh->length[0]=sh->length[1]=0L;
    sh->h[0]=H0;
    sh->h[1]=H1;
    sh->h[2]=H2;
    sh->h[3]=H3;
    sh->h[4]=H4;
    sh->h[5]=H5;
    sh->h[6]=H6;
    sh->h[7]=H7;
}

void shs256_process(sha256 *sh,int byte)
{ /* process the next message byte */
    int cnt;

    cnt=(int)((sh->length[0]/32)%16);

    sh->w[cnt]<<=8;
    sh->w[cnt]|=(mr_unsign32)(byte&0xFF);

    sh->length[0]+=8;
    if (sh->length[0]==0L) { sh->length[1]++; sh->length[0]=0L; }
    if ((sh->length[0]%512)==0) shs_transform(sh);
}

void shs256_hash(sha256 *sh,char hash[32])
{ /* pad message and finish - supply digest */
    int i;
    mr_unsign32 len0,len1;
    len0=sh->length[0];
    len1=sh->length[1];
    shs256_process(sh,PAD);
    while ((sh->length[0]%512)!=448) shs256_process(sh,ZERO);
    sh->w[14]=len1;
    sh->w[15]=len0;
    shs_transform(sh);
    for (i=0;i<32;i++)
    { /* convert to bytes */
        hash[i]=(char)((sh->h[i/4]>>(8*(3-i%4))) & 0xffL);
    }
    shs256_init(sh);
}

int size(big x)
{  /*  get size of big number;  convert to *
    *  integer - if possible               */
    int n,m;
    mr_lentype s;
    if (x==NULL) return 0;
    s=(x->len&MR_MSBIT);
    m=(int)(x->len&MR_OBITS);
    if (m==0) return 0;
    if (m==1 && x->w[0]<(mr_small)MR_TOOBIG) n=(int)x->w[0];
    else                                     n=MR_TOOBIG;
    if (s==MR_MSBIT) return (-n);
    return n;
}

/* initialise elliptic curve */

void ecurve_init(_MIPD_ big a,big b,big p,int type)
{ /* Initialize the active ecurve    *
   * Asize indicate size of A        *
   * Bsize indicate size of B        */
    int as;
    if (mr_mip->ERNUM) return;

    MR_IN(93)
    mr_mip->SS=FALSE;       /* no special support for super-singular curves */

    prepare_monty(_MIPP_ p);

    mr_mip->Asize=size(a);
    if (mr_abs(mr_mip->Asize)==MR_TOOBIG)
    {
        if (mr_mip->Asize>=0)
        { /* big positive number - check it isn't minus something small */
           copy(a,mr_mip->w1);
           divide(_MIPP_ mr_mip->w1,p,p);
           subtract(_MIPP_ p,mr_mip->w1,mr_mip->w1);
           as=size(mr_mip->w1);
           if (as<MR_TOOBIG) mr_mip->Asize=-as;
        }
    }
    nres(_MIPP_ a,mr_mip->A);

    mr_mip->Bsize=size(b);
    if (mr_abs(mr_mip->Bsize)==MR_TOOBIG)
    {
        if (mr_mip->Bsize>=0)
        { /* big positive number - check it isn't minus something small */
           copy(b,mr_mip->w1);
           divide(_MIPP_ mr_mip->w1,p,p);
           subtract(_MIPP_ p,mr_mip->w1,mr_mip->w1);
           as=size(mr_mip->w1);
           if (as<MR_TOOBIG) mr_mip->Bsize=-as;
        }
    }

    nres(_MIPP_ b,mr_mip->B);
    if (type==MR_BEST) mr_mip->coord=MR_PROJECTIVE;
    else mr_mip->coord=type;
    MR_OUT
    return;
}

BOOL nroot(_MIPD_ big x,int n,big w)
{  /*  extract  lower approximation to nth root   *
    *  w=x^(1/n) returns TRUE for exact root      *
    *  uses Newtons method                        */
    int sx,dif,s,p,d,lg2,lgx,rem;
    BOOL full;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    if (mr_mip->ERNUM) return FALSE;
    if (size(x)==0 || n==1)
    {
        copy(x,w);
        return TRUE;
    }

    MR_IN(16)

    if (n<1) mr_berror(_MIPP_ MR_ERR_BAD_ROOT);
    sx=exsign(x);
    if (n%2==0 && sx==MINUS) mr_berror(_MIPP_ MR_ERR_NEG_ROOT);
    if (mr_mip->ERNUM)
    {
        MR_OUT
        return FALSE;
    }
    insign(PLUS,x);
    lgx=logb2(_MIPP_ x);
    if (n>=lgx)
    { /* root must be 1 */
        insign(sx,x);
        convert(_MIPP_ sx,w);
        MR_OUT
        if (lgx==1) return TRUE;
        else        return FALSE;
    }
    expb2(_MIPP_ 1+(lgx-1)/n,mr_mip->w2);           /* guess root as 2^(log2(x)/n) */
    s=(-(((int)x->len-1)/n)*n);
    mr_shift(_MIPP_ mr_mip->w2,s/n,mr_mip->w2);
    lg2=logb2(_MIPP_ mr_mip->w2)-1;
    full=FALSE;
    if (s==0) full=TRUE;
    d=0;
    p=1;
    while (!mr_mip->ERNUM)
    { /* Newtons method */
        copy(mr_mip->w2,mr_mip->w3);
        mr_shift(_MIPP_ x,s,mr_mip->w4);
        mr_mip->check=OFF;
        power(_MIPP_ mr_mip->w2,n-1,mr_mip->w6,mr_mip->w6);
        mr_mip->check=ON;
        divide(_MIPP_ mr_mip->w4,mr_mip->w6,mr_mip->w2);
        rem=size(mr_mip->w4);
        subtract(_MIPP_ mr_mip->w2,mr_mip->w3,mr_mip->w2);
        dif=size(mr_mip->w2);
        subdiv(_MIPP_ mr_mip->w2,n,mr_mip->w2);
        add(_MIPP_ mr_mip->w2,mr_mip->w3,mr_mip->w2);
        p*=2;
        if(p<lg2+d*mr_mip->lg2b) continue;
        if (full && mr_abs(dif)<n)
        { /* test for finished */
            while (dif<0)
            {
                rem=0;
                decr(_MIPP_ mr_mip->w2,1,mr_mip->w2);
                mr_mip->check=OFF;
                power(_MIPP_ mr_mip->w2,n,mr_mip->w6,mr_mip->w6);
                mr_mip->check=ON;
                dif=mr_compare(x,mr_mip->w6);
            }
            copy(mr_mip->w2,w);
            insign(sx,w);
            insign(sx,x);
            MR_OUT
            if (rem==0 && dif==0) return TRUE;
            else                  return FALSE;
        }
        else
        { /* adjust precision */
            d*=2;
            if (d==0) d=1;
            s+=d*n;
            if (s>=0)
            {
                d-=s/n;
                s=0;
                full=TRUE;
            }
            mr_shift(_MIPP_ mr_mip->w2,d,mr_mip->w2);
        }
        p/=2;
    }
    MR_OUT
    return FALSE;
}

void nres_premult(_MIPD_ big x,int k,big w)
{ /* multiply n-residue by small ordinary integer */
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    int sign=0;
    if (k==0)
    {
        zero(w);
        return;
    }
    if (k<0)
    {
        k=-k;
        sign=1;
    }
    if (mr_mip->ERNUM) return;

    MR_IN(102)

    if (k<=6)
    {
        switch (k)
        {
        case 1: copy(x,w);
                break;
        case 2: nres_modadd(_MIPP_ x,x,w);
                break;
        case 3:
                nres_modadd(_MIPP_ x,x,mr_mip->w0);
                nres_modadd(_MIPP_ x,mr_mip->w0,w);
                break;
        case 4:
                nres_modadd(_MIPP_ x,x,w);
                nres_modadd(_MIPP_ w,w,w);
                break;
        case 5:
                nres_modadd(_MIPP_ x,x,mr_mip->w0);
                nres_modadd(_MIPP_ mr_mip->w0,mr_mip->w0,mr_mip->w0);
                nres_modadd(_MIPP_ x,mr_mip->w0,w);
                break;
        case 6:
                nres_modadd(_MIPP_ x,x,w);
                nres_modadd(_MIPP_ w,w,mr_mip->w0);
                nres_modadd(_MIPP_ w,mr_mip->w0,w);
                break;
        }
        if (sign==1) nres_negate(_MIPP_ w,w);
        MR_OUT
        return;
    }

    mr_pmul(_MIPP_ x,(mr_small)k,mr_mip->w0);
    divide(_MIPP_ mr_mip->w0,mr_mip->modulus,mr_mip->modulus);
	copy(mr_mip->w0,w);

    if (sign==1) nres_negate(_MIPP_ w,w);

    MR_OUT
}

BOOL nres_sqroot(_MIPD_ big x,big w)
{ /* w=sqrt(x) mod p. This depends on p being prime! */
    int t,js;

#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    if (mr_mip->ERNUM) return FALSE;

    copy(x,w);
    if (size(w)==0) return TRUE;

    MR_IN(100)

    redc(_MIPP_ w,w);   /* get it back into normal form */

    if (size(w)==1) /* square root of 1 is 1 */
    {
        nres(_MIPP_ w,w);
        MR_OUT
        return TRUE;
    }

    if (size(w)==4) /* square root of 4 is 2 */
    {
        convert(_MIPP_ 2,w);
        nres(_MIPP_ w,w);
        MR_OUT
        return TRUE;
    }

    if (jack(_MIPP_ w,mr_mip->modulus)!=1)
    { /* Jacobi test */
        zero(w);
        MR_OUT
        return FALSE;
    }

    js=mr_mip->pmod8%4-2;     /* 1 mod 4 or 3 mod 4 prime? */

    incr(_MIPP_ mr_mip->modulus,js,mr_mip->w10);
    subdiv(_MIPP_ mr_mip->w10,4,mr_mip->w10);    /* (p+/-1)/4 */

    if (js==1)
    { /* 3 mod 4 primes - do a quick and dirty sqrt(x)=x^(p+1)/4 mod p */
        nres(_MIPP_ w,mr_mip->w2);
        copy(mr_mip->one,w);
        forever
        { /* Simple Right-to-Left exponentiation */

            if (mr_mip->user!=NULL) (*mr_mip->user)();
            if (subdiv(_MIPP_ mr_mip->w10,2,mr_mip->w10)!=0)
                nres_modmult(_MIPP_ w,mr_mip->w2,w);
            if (mr_mip->ERNUM || size(mr_mip->w10)==0) break;
            nres_modmult(_MIPP_ mr_mip->w2,mr_mip->w2,mr_mip->w2);
        }

 /*     nres_moddiv(_MIPP_ mr_mip->one,w,mr_mip->w11);
        nres_modadd(_MIPP_ mr_mip->w11,w,mr_mip->w3);
        nres_lucas(_MIPP_ mr_mip->w3,mr_mip->w10,w,w);
        nres_modadd(_MIPP_ mr_mip->w11,mr_mip->one,mr_mip->w11);
        nres_moddiv(_MIPP_ w,mr_mip->w11,w); */
    }
    else
    { /* 1 mod 4 primes */
        for (t=1; ;t++)
        { /* t=1.5 on average */
            if (t==1) copy(w,mr_mip->w4);
            else
            {
                premult(_MIPP_ w,t,mr_mip->w4);
                divide(_MIPP_ mr_mip->w4,mr_mip->modulus,mr_mip->modulus);
                premult(_MIPP_ mr_mip->w4,t,mr_mip->w4);
                divide(_MIPP_ mr_mip->w4,mr_mip->modulus,mr_mip->modulus);
            }

            decr(_MIPP_ mr_mip->w4,4,mr_mip->w1);
            if (jack(_MIPP_ mr_mip->w1,mr_mip->modulus)==js) break;
            if (mr_mip->ERNUM) break;
        }

        decr(_MIPP_ mr_mip->w4,2,mr_mip->w3);
        nres(_MIPP_ mr_mip->w3,mr_mip->w3);
        nres_lucas(_MIPP_ mr_mip->w3,mr_mip->w10,w,w); /* heavy lifting done here */
        if (t!=1)
        {
            convert(_MIPP_ t,mr_mip->w11);
            nres(_MIPP_ mr_mip->w11,mr_mip->w11);
            nres_moddiv(_MIPP_ w,mr_mip->w11,w);
        }
    }

    MR_OUT
    return TRUE;
}

void nres_lucas(_MIPD_ big p,big r,big vp,big v)
{
    int i,nb;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    if (mr_mip->ERNUM) return;

    MR_IN(107)

    if (size(r)==0)
    {
        zero(vp);
        convert(_MIPP_ 2,v);
        nres(_MIPP_ v,v);
        MR_OUT
        return;
    }
    if (size(r)==1 || size(r)==(-1))
    { /* note - sign of r doesn't matter */
        convert(_MIPP_ 2,vp);
        nres(_MIPP_ vp,vp);
        copy(p,v);
        MR_OUT
        return;
    }

    copy(p,mr_mip->w3);

    convert(_MIPP_ 2,mr_mip->w4);
    nres(_MIPP_ mr_mip->w4,mr_mip->w4);     /* w4=2 */

    copy(mr_mip->w4,mr_mip->w8);
    copy(mr_mip->w3,mr_mip->w9);

    copy(r,mr_mip->w1);
    insign(PLUS,mr_mip->w1);
    decr(_MIPP_ mr_mip->w1,1,mr_mip->w1);

#ifndef MR_ALWAYS_BINARY
    if (mr_mip->base==mr_mip->base2)
    {
#endif
        nb=logb2(_MIPP_ mr_mip->w1);
        for (i=nb-1;i>=0;i--)
        {
            if (mr_mip->user!=NULL) (*mr_mip->user)();

            if (mr_testbit(_MIPP_ mr_mip->w1,i))
            {
                nres_modmult(_MIPP_ mr_mip->w8,mr_mip->w9,mr_mip->w8);
                nres_modsub(_MIPP_ mr_mip->w8,mr_mip->w3,mr_mip->w8);
                nres_modmult(_MIPP_ mr_mip->w9,mr_mip->w9,mr_mip->w9);
                nres_modsub(_MIPP_ mr_mip->w9,mr_mip->w4,mr_mip->w9);

            }
            else
            {
                nres_modmult(_MIPP_ mr_mip->w9,mr_mip->w8,mr_mip->w9);
                nres_modsub(_MIPP_ mr_mip->w9,mr_mip->w3,mr_mip->w9);
                nres_modmult(_MIPP_ mr_mip->w8,mr_mip->w8,mr_mip->w8);
                nres_modsub(_MIPP_ mr_mip->w8,mr_mip->w4,mr_mip->w8);
            }
        }

#ifndef MR_ALWAYS_BINARY
    }
    else
    {
        expb2(_MIPP_ logb2(_MIPP_ mr_mip->w1)-1,mr_mip->w2);

        while (!mr_mip->ERNUM && size(mr_mip->w2)!=0)
        { /* use binary method */
            if (mr_compare(mr_mip->w1,mr_mip->w2)>=0)
            { /* vp=v*vp-p, v=v*v-2 */
                nres_modmult(_MIPP_ mr_mip->w8,mr_mip->w9,mr_mip->w8);
                nres_modsub(_MIPP_ mr_mip->w8,mr_mip->w3,mr_mip->w8);
                nres_modmult(_MIPP_ mr_mip->w9,mr_mip->w9,mr_mip->w9);
                nres_modsub(_MIPP_ mr_mip->w9,mr_mip->w4,mr_mip->w9);
                subtract(_MIPP_ mr_mip->w1,mr_mip->w2,mr_mip->w1);
            }
            else
            { /* v=v*vp-p, vp=vp*vp-2 */
                nres_modmult(_MIPP_ mr_mip->w9,mr_mip->w8,mr_mip->w9);
                nres_modsub(_MIPP_ mr_mip->w9,mr_mip->w3,mr_mip->w9);
                nres_modmult(_MIPP_ mr_mip->w8,mr_mip->w8,mr_mip->w8);
                nres_modsub(_MIPP_ mr_mip->w8,mr_mip->w4,mr_mip->w8);
            }
            subdiv(_MIPP_ mr_mip->w2,2,mr_mip->w2);
        }
    }
#endif

    copy(mr_mip->w9,v);
    if (v!=vp) copy(mr_mip->w8,vp);
    MR_OUT

}

int remain(_MIPD_ big x,int n)
{ /* return integer remainder when x divided by n */
    int r;
    mr_lentype sx;
    if (mr_mip->ERNUM) return FALSE;

    MR_IN(88);

    sx=(x->len&MR_MSBIT);

    if (n==2 && MR_REMAIN(mr_mip->base,2)==0)
    { /* fast odd/even check if base is even */
        MR_OUT
        if ((int)MR_REMAIN(x->w[0],2)==0) return 0;
        else
        {
            if (sx==0) return 1;
            else       return (-1);
        }
    }
    if (n==8 && MR_REMAIN(mr_mip->base,8)==0)
    { /* fast check */
        MR_OUT
        r=(int)MR_REMAIN(x->w[0],8);
        if (sx!=0) r=-r;
        return r;
    }

    copy(x,mr_mip->w0);
    r=subdiv(_MIPP_ mr_mip->w0,n,mr_mip->w0);
    MR_OUT
    return r;
}


static mr_small qdiv(mr_large u,mr_large v)
{ /* fast division - small quotient expected.  */
    mr_large lq,x=u;
    x-=v;
    if (x<v) return 1;
    x-=v;
    if (x<v) return 2;
    x-=v;
    if (x<v) return 3;
    x-=v;
    if (x<v) return 4;
    x-=v;
    if (x<v) return 5;
    x-=v;
    if (x<v) return 6;
    x-=v;
    if (x<v) return 7;
    x-=v;
    if (x<v) return 8;

/* do it the hard way! */

    lq=8+MR_DIV(x,v);
    if (lq>=MAXBASE) return 0;
    return (mr_small)lq;
}

void mad(_MIPD_ big x,big y,big z,big w,big q,big r)
{ /* Multiply, Add and Divide; q=(x*y+z)/w remainder r   *
   * returns remainder only if w=q, quotient only if q=r *
   * add done only if x, y and z are distinct.           */

    BOOL check;
    if (mr_mip->ERNUM) return;

    MR_IN(24)
    if (w==r)
    {
        mr_berror(_MIPP_ MR_ERR_BAD_PARAMETERS);
        MR_OUT
        return;
    }
    check=mr_mip->check;
    mr_mip->check=OFF;           /* turn off some error checks */

    multiply(_MIPP_ x,y,mr_mip->w0);
    if (x!=z && y!=z) add(_MIPP_ mr_mip->w0,z,mr_mip->w0);

    divide(_MIPP_ mr_mip->w0,w,q);
    if (q!=r) copy(mr_mip->w0,r);
    mr_mip->check=check;
    MR_OUT
}

int xgcd(_MIPD_ big x,big y,big xd,big yd,big z)
{ /* greatest common divisor by Euclids method  *
   * extended to also calculate xd and yd where *
   *      z = x.xd + y.yd = gcd(x,y)            *
   * if xd, yd not distinct, only xd calculated *
   * z only returned if distinct from xd and yd *
   * xd will always be positive, yd negative    */

    int s,n,iter;
    mr_small r,a,b,c,d;
    mr_small q,m,sr;

    union doubleword uu,vv;
    mr_large u,v,lr;

    BOOL last,dplus=TRUE;
    big t;

    if (mr_mip->ERNUM) return 0;

    MR_IN(30)


    copy(x,mr_mip->w1);
    copy(y,mr_mip->w2);
    s=exsign(mr_mip->w1);
    insign(PLUS,mr_mip->w1);
    insign(PLUS,mr_mip->w2);
    convert(_MIPP_ 1,mr_mip->w3);
    zero(mr_mip->w4);
    last=FALSE;
    a=b=c=d=0;
    iter=0;

    while (size(mr_mip->w2)!=0)
    {
        if (b==0)
        { /* update mr_mip->w1 and mr_mip->w2 */

            divide(_MIPP_ mr_mip->w1,mr_mip->w2,mr_mip->w5);
            t=mr_mip->w1,mr_mip->w1=mr_mip->w2,mr_mip->w2=t;    /* swap(mr_mip->w1,mr_mip->w2) */
            multiply(_MIPP_ mr_mip->w4,mr_mip->w5,mr_mip->w0);
            add(_MIPP_ mr_mip->w3,mr_mip->w0,mr_mip->w3);
            t=mr_mip->w3,mr_mip->w3=mr_mip->w4,mr_mip->w4=t;    /* swap(xd,yd) */
            iter++;

        }
        else
        {

 /* printf("a= %I64u b= %I64u c= %I64u  d= %I64u \n",a,b,c,d);   */

            mr_pmul(_MIPP_ mr_mip->w1,c,mr_mip->w5);   /* c*w1 */
            mr_pmul(_MIPP_ mr_mip->w1,a,mr_mip->w1);   /* a*w1 */
            mr_pmul(_MIPP_ mr_mip->w2,b,mr_mip->w0);   /* b*w2 */
            mr_pmul(_MIPP_ mr_mip->w2,d,mr_mip->w2);   /* d*w2 */

            if (!dplus)
            {
                mr_psub(_MIPP_ mr_mip->w0,mr_mip->w1,mr_mip->w1); /* b*w2-a*w1 */
                mr_psub(_MIPP_ mr_mip->w5,mr_mip->w2,mr_mip->w2); /* c*w1-d*w2 */
            }
            else
            {
                mr_psub(_MIPP_ mr_mip->w1,mr_mip->w0,mr_mip->w1); /* a*w1-b*w2 */
                mr_psub(_MIPP_ mr_mip->w2,mr_mip->w5,mr_mip->w2); /* d*w2-c*w1 */
            }
            mr_pmul(_MIPP_ mr_mip->w3,c,mr_mip->w5);
            mr_pmul(_MIPP_ mr_mip->w3,a,mr_mip->w3);
            mr_pmul(_MIPP_ mr_mip->w4,b,mr_mip->w0);
            mr_pmul(_MIPP_ mr_mip->w4,d,mr_mip->w4);

            if (a==0) copy(mr_mip->w0,mr_mip->w3);
            else      mr_padd(_MIPP_ mr_mip->w3,mr_mip->w0,mr_mip->w3);
            mr_padd(_MIPP_ mr_mip->w4,mr_mip->w5,mr_mip->w4);
        }
        if (mr_mip->ERNUM || size(mr_mip->w2)==0) break;


        n=(int)mr_mip->w1->len;
        if (n==1)
        {
            last=TRUE;
            u=mr_mip->w1->w[0];
            v=mr_mip->w2->w[0];
        }
        else
        {
            m=mr_mip->w1->w[n-1]+1;
            if (mr_mip->base==0)
            {
 /* use double length type if available */
                if (n>2 && m!=0)
                { /* squeeze out as much significance as possible */
                    uu.h[MR_TOP]=muldvm(mr_mip->w1->w[n-1],mr_mip->w1->w[n-2],m,&sr);
                    uu.h[MR_BOT]=muldvm(sr,mr_mip->w1->w[n-3],m,&sr);
                    vv.h[MR_TOP]=muldvm(mr_mip->w2->w[n-1],mr_mip->w2->w[n-2],m,&sr);
                    vv.h[MR_BOT]=muldvm(sr,mr_mip->w2->w[n-3],m,&sr);
                }
                else
                {
                    uu.h[MR_TOP]=mr_mip->w1->w[n-1];
                    uu.h[MR_BOT]=mr_mip->w1->w[n-2];
                    vv.h[MR_TOP]=mr_mip->w2->w[n-1];
                    vv.h[MR_BOT]=mr_mip->w2->w[n-2];
                    if (n==2) last=TRUE;
                }

                u=uu.d;
                v=vv.d;

            }
            else
            {
                if (n>2)
                { /* squeeze out as much significance as possible */
                    u=muldiv(mr_mip->w1->w[n-1],mr_mip->base,mr_mip->w1->w[n-2],m,&sr);
                    u=u*mr_mip->base+muldiv(sr,mr_mip->base,mr_mip->w1->w[n-3],m,&sr);
                    v=muldiv(mr_mip->w2->w[n-1],mr_mip->base,mr_mip->w2->w[n-2],m,&sr);
                    v=v*mr_mip->base+muldiv(sr,mr_mip->base,mr_mip->w2->w[n-3],m,&sr);
                }
                else
                {
                    u=(mr_large)mr_mip->base*mr_mip->w1->w[n-1]+mr_mip->w1->w[n-2];
                    v=(mr_large)mr_mip->base*mr_mip->w2->w[n-1]+mr_mip->w2->w[n-2];
                    last=TRUE;
                }
            }
        }

        dplus=TRUE;
        a=1; b=0; c=0; d=1;

        forever
        { /* work only with most significant piece */
            if (last)
            {
                if (v==0) break;
                q=qdiv(u,v);
                if (q==0) break;
            }
            else
            {
                if (dplus)
                {
                    if ((mr_small)(v-c)==0 || (mr_small)(v+d)==0) break;

                    q=qdiv(u+a,v-c);

                    if (q==0) break;

                    if (q!=qdiv(u-b,v+d)) break;
                }
                else
                {
                    if ((mr_small)(v+c)==0 || (mr_small)(v-d)==0) break;
                    q=qdiv(u-a,v+c);
                    if (q==0) break;
                    if (q!=qdiv(u+b,v-d)) break;
                }
            }

            if (q==1)
            {
                if ((mr_small)(b+d) >= MAXBASE) break;
                r=a+c;  a=c; c=r;
                r=b+d;  b=d; d=r;
                lr=u-v; u=v; v=lr;
            }
            else
            {
                if (q>=MR_DIV(MAXBASE-b,d)) break;
                r=a+q*c;  a=c; c=r;
                r=b+q*d;  b=d; d=r;
                lr=u-q*v; u=v; v=lr;
            }
            iter++;
            dplus=!dplus;
        }
        iter%=2;

    }

    if (s==MINUS) iter++;
    if (iter%2==1) subtract(_MIPP_ y,mr_mip->w3,mr_mip->w3);

    if (xd!=yd)
    {
        negify(x,mr_mip->w2);
        mad(_MIPP_ mr_mip->w2,mr_mip->w3,mr_mip->w1,y,mr_mip->w4,mr_mip->w4);
        copy(mr_mip->w4,yd);
    }
    copy(mr_mip->w3,xd);
    if (z!=xd && z!=yd) copy(mr_mip->w1,z);

    MR_OUT
    return (size(mr_mip->w1));
}

int invmodp(_MIPD_ big x,big y,big z)
{
    int gcd;

    MR_IN(213);
    gcd=xgcd(_MIPP_ x,y,z,z,z);
    MR_OUT
    return gcd;
}

int nres_moddiv(_MIPD_ big x,big y,big w)
{ /* Modular division using n-residues w=x/y mod n */
    int gcd;
    if (mr_mip->ERNUM) return 0;

    MR_IN(85)

    if (x==y)
    { /* Illegal parameter usage */
        mr_berror(_MIPP_ MR_ERR_BAD_PARAMETERS);
        MR_OUT

        return 0;
    }
    redc(_MIPP_ y,mr_mip->w6);
    gcd=invmodp(_MIPP_ mr_mip->w6,mr_mip->modulus,mr_mip->w6);

    if (gcd!=1) zero(w); /* fails silently and returns 0 */
    else
    {
        nres(_MIPP_ mr_mip->w6,mr_mip->w6);
        nres_modmult(_MIPP_ x,mr_mip->w6,w);
    /*    mad(_MIPP_ x,mr_mip->w6,x,mr_mip->modulus,mr_mip->modulus,w); */
    }
    MR_OUT
    return gcd;
}

void ecurve_double(_MIPD_ epoint *p)
{ /* double epoint on active ecurve */

    if (mr_mip->ERNUM) return;

    if (p->marker==MR_EPOINT_INFINITY)
    { /* 2 times infinity == infinity ! */
        return;
    }
    if (mr_mip->coord==MR_AFFINE)
    { /* 2 sqrs, 1 mul, 1 div */
        if (size(p->Y)==0)
        { /* set to point at infinity */
            epoint_set(_MIPP_ NULL,NULL,0,p);
            return;
        }

        nres_modmult(_MIPP_ p->X,p->X,mr_mip->w8);    /* w8=x^2   */
        nres_premult(_MIPP_ mr_mip->w8,3,mr_mip->w8); /* w8=3*x^2 */
        if (mr_abs(mr_mip->Asize) == MR_TOOBIG)
            nres_modadd(_MIPP_ mr_mip->w8,mr_mip->A,mr_mip->w8);
        else
        {
            convert(_MIPP_ mr_mip->Asize,mr_mip->w2);
            nres(_MIPP_ mr_mip->w2,mr_mip->w2);
            nres_modadd(_MIPP_ mr_mip->w8,mr_mip->w2,mr_mip->w8);
        }                                     /* w8=3*x^2+A */
        nres_premult(_MIPP_ p->Y,2,mr_mip->w6);      /* w6=2y */
        if (nres_moddiv(_MIPP_ mr_mip->w8,mr_mip->w6,mr_mip->w8)>1)
        {
            epoint_set(_MIPP_ NULL,NULL,0,p);
            mr_berror(_MIPP_ MR_ERR_COMPOSITE_MODULUS);
            return;
        }

/* w8 is slope m on exit */

        nres_modmult(_MIPP_ mr_mip->w8,mr_mip->w8,mr_mip->w2); /* w2=m^2 */
        nres_premult(_MIPP_ p->X,2,mr_mip->w1);
        nres_modsub(_MIPP_ mr_mip->w2,mr_mip->w1,mr_mip->w1); /* w1=m^2-2x */

        nres_modsub(_MIPP_ p->X,mr_mip->w1,mr_mip->w2);
        nres_modmult(_MIPP_ mr_mip->w2,mr_mip->w8,mr_mip->w2);
        nres_modsub(_MIPP_ mr_mip->w2,p->Y,p->Y);
        copy(mr_mip->w1,p->X);

        return;
    }

    if (size(p->Y)==0)
    { /* set to point at infinity */
        epoint_set(_MIPP_ NULL,NULL,0,p);
        return;
    }

    convert(_MIPP_ 1,mr_mip->w1);
    if (mr_abs(mr_mip->Asize) < MR_TOOBIG)
    {
        if (mr_mip->Asize!=0)
        {
            if (p->marker==MR_EPOINT_NORMALIZED)
				nres(_MIPP_ mr_mip->w1,mr_mip->w6);
            else nres_modmult(_MIPP_ p->Z,p->Z,mr_mip->w6);
        }

        if (mr_mip->Asize==(-3))
        { /* a is -3. Goody. 4 sqrs, 4 muls */
            nres_modsub(_MIPP_ p->X,mr_mip->w6,mr_mip->w3);
            nres_modadd(_MIPP_ p->X,mr_mip->w6,mr_mip->w8);
            nres_modmult(_MIPP_ mr_mip->w3,mr_mip->w8,mr_mip->w3);
            nres_modadd(_MIPP_ mr_mip->w3,mr_mip->w3,mr_mip->w8);
            nres_modadd(_MIPP_ mr_mip->w8,mr_mip->w3,mr_mip->w8);
        }
        else
        { /* a is small */
            if (mr_mip->Asize!=0)
            { /* a is non zero! */
                nres_modmult(_MIPP_ mr_mip->w6,mr_mip->w6,mr_mip->w3);
                nres_premult(_MIPP_ mr_mip->w3,mr_mip->Asize,mr_mip->w3);
            }
            nres_modmult(_MIPP_ p->X,p->X,mr_mip->w1);
            nres_modadd(_MIPP_ mr_mip->w1,mr_mip->w1,mr_mip->w8);
            nres_modadd(_MIPP_ mr_mip->w8,mr_mip->w1,mr_mip->w8);
            if (mr_mip->Asize!=0) nres_modadd(_MIPP_ mr_mip->w8,mr_mip->w3,mr_mip->w8);
        }
    }
    else
    { /* a is not special */
        if (p->marker==MR_EPOINT_NORMALIZED) nres(_MIPP_ mr_mip->w1,mr_mip->w6);
        else nres_modmult(_MIPP_ p->Z,p->Z,mr_mip->w6);

        nres_modmult(_MIPP_ mr_mip->w6,mr_mip->w6,mr_mip->w3);
        nres_modmult(_MIPP_ mr_mip->w3,mr_mip->A,mr_mip->w3);
        nres_modmult(_MIPP_ p->X,p->X,mr_mip->w1);
        nres_modadd(_MIPP_ mr_mip->w1,mr_mip->w1,mr_mip->w8);
        nres_modadd(_MIPP_ mr_mip->w8,mr_mip->w1,mr_mip->w8);
        nres_modadd(_MIPP_ mr_mip->w8,mr_mip->w3,mr_mip->w8);
    }

/* w8 contains numerator of slope 3x^2+A.z^4  *
 * denominator is now placed in Z             */

    nres_modmult(_MIPP_ p->Y,p->Y,mr_mip->w2);
    nres_modmult(_MIPP_ p->X,mr_mip->w2,mr_mip->w3);
    nres_modadd(_MIPP_ mr_mip->w3,mr_mip->w3,mr_mip->w3);
    nres_modadd(_MIPP_ mr_mip->w3,mr_mip->w3,mr_mip->w3);
    nres_modmult(_MIPP_ mr_mip->w8,mr_mip->w8,p->X);
    nres_modsub(_MIPP_ p->X,mr_mip->w3,p->X);
    nres_modsub(_MIPP_ p->X,mr_mip->w3,p->X);

    if (p->marker==MR_EPOINT_NORMALIZED)
        copy(p->Y,p->Z);
    else nres_modmult(_MIPP_ p->Z,p->Y,p->Z);
    nres_modadd(_MIPP_ p->Z,p->Z,p->Z);

    nres_modadd(_MIPP_ mr_mip->w2,mr_mip->w2,mr_mip->w7);
    nres_modmult(_MIPP_ mr_mip->w7,mr_mip->w7,mr_mip->w2);
    nres_modadd(_MIPP_ mr_mip->w2,mr_mip->w2,mr_mip->w2);
    nres_modsub(_MIPP_ mr_mip->w3,p->X,mr_mip->w3);
    nres_modmult(_MIPP_ mr_mip->w8,mr_mip->w3,p->Y);
    nres_modsub(_MIPP_ p->Y,mr_mip->w2,p->Y);

    p->marker=MR_EPOINT_GENERAL;
    return;
}

void nres_div2(_MIPD_ big x,big w)
{

    MR_IN(198)
    copy(x,mr_mip->w1);
    if (remain(_MIPP_ mr_mip->w1,2)!=0)
        add(_MIPP_ mr_mip->w1,mr_mip->modulus,mr_mip->w1);
    subdiv(_MIPP_ mr_mip->w1,2,mr_mip->w1);
    copy(mr_mip->w1,w);

    MR_OUT
}

static BOOL ecurve_padd(_MIPD_ epoint *p,epoint *pa)
{ /* primitive add two epoints on the active ecurve - pa+=p;   *
   * note that if p is normalized, its Z coordinate isn't used */

    if (mr_mip->coord==MR_AFFINE)
    {  /* 1 sqr, 1 mul, 1 div */
        nres_modsub(_MIPP_ p->Y,pa->Y,mr_mip->w8);
        nres_modsub(_MIPP_ p->X,pa->X,mr_mip->w6);
        if (size(mr_mip->w6)==0)
        { /* divide by 0 */
            if (size(mr_mip->w8)==0)
            { /* should have doubled ! */
                return FALSE;
            }
            else
            { /* point at infinity */
                epoint_set(_MIPP_ NULL,NULL,0,pa);
                return TRUE;
            }
        }
        if (nres_moddiv(_MIPP_ mr_mip->w8,mr_mip->w6,mr_mip->w8)>1)
        {
            epoint_set(_MIPP_ NULL,NULL,0,pa);
            mr_berror(_MIPP_ MR_ERR_COMPOSITE_MODULUS);
            return TRUE;
        }

        nres_modmult(_MIPP_ mr_mip->w8,mr_mip->w8,mr_mip->w2); /* w2=m^2 */
        nres_modsub(_MIPP_ mr_mip->w2,p->X,mr_mip->w1); /* w1=m^2-x1-x2 */
        nres_modsub(_MIPP_ mr_mip->w1,pa->X,mr_mip->w1);


        nres_modsub(_MIPP_ pa->X,mr_mip->w1,mr_mip->w2);
        nres_modmult(_MIPP_ mr_mip->w2,mr_mip->w8,mr_mip->w2);
        nres_modsub(_MIPP_ mr_mip->w2,pa->Y,pa->Y);
        copy(mr_mip->w1,pa->X);

        pa->marker=MR_EPOINT_NORMALIZED;
        return TRUE;
    }

    if (p->marker!=MR_EPOINT_NORMALIZED)
    {
        nres_modmult(_MIPP_ p->Z,p->Z,mr_mip->w6);
        nres_modmult(_MIPP_ pa->X,mr_mip->w6,mr_mip->w1);
        nres_modmult(_MIPP_ mr_mip->w6,p->Z,mr_mip->w6);
        nres_modmult(_MIPP_ pa->Y,mr_mip->w6,mr_mip->w8);
    }
    else
    {
        copy(pa->X,mr_mip->w1);
        copy(pa->Y,mr_mip->w8);
    }
    if (pa->marker==MR_EPOINT_NORMALIZED)
        copy(mr_mip->one,mr_mip->w6);
    else nres_modmult(_MIPP_ pa->Z,pa->Z,mr_mip->w6);

    nres_modmult(_MIPP_ p->X,mr_mip->w6,mr_mip->w4);
    if (pa->marker!=MR_EPOINT_NORMALIZED)
        nres_modmult(_MIPP_ mr_mip->w6,pa->Z,mr_mip->w6);
    nres_modmult(_MIPP_ p->Y,mr_mip->w6,mr_mip->w5);
    nres_modsub(_MIPP_ mr_mip->w1,mr_mip->w4,mr_mip->w1);
    nres_modsub(_MIPP_ mr_mip->w8,mr_mip->w5,mr_mip->w8);

/* w8 contains the numerator of the slope */

    if (size(mr_mip->w1)==0)
    {
        if (size(mr_mip->w8)==0)
        { /* should have doubled ! */
           return FALSE;
        }
        else
        { /* point at infinity */
            epoint_set(_MIPP_ NULL,NULL,0,pa);
            return TRUE;
        }
    }
    nres_modadd(_MIPP_ mr_mip->w4,mr_mip->w4,mr_mip->w6);
    nres_modadd(_MIPP_ mr_mip->w1,mr_mip->w6,mr_mip->w4);
    nres_modadd(_MIPP_ mr_mip->w5,mr_mip->w5,mr_mip->w6);
    nres_modadd(_MIPP_ mr_mip->w8,mr_mip->w6,mr_mip->w5);

    if (p->marker!=MR_EPOINT_NORMALIZED)
    {
        if (pa->marker!=MR_EPOINT_NORMALIZED)
            nres_modmult(_MIPP_ pa->Z,p->Z,mr_mip->w3);
        else
            copy(p->Z,mr_mip->w3);
        nres_modmult(_MIPP_ mr_mip->w3,mr_mip->w1,pa->Z);
    }
    else
    {
        if (pa->marker!=MR_EPOINT_NORMALIZED)
            nres_modmult(_MIPP_ pa->Z,mr_mip->w1,pa->Z);
        else
            copy(mr_mip->w1,pa->Z);
    }
    nres_modmult(_MIPP_ mr_mip->w1,mr_mip->w1,mr_mip->w6);
    nres_modmult(_MIPP_ mr_mip->w1,mr_mip->w6,mr_mip->w1);
    nres_modmult(_MIPP_ mr_mip->w6,mr_mip->w4,mr_mip->w6);
    nres_modmult(_MIPP_ mr_mip->w8,mr_mip->w8,mr_mip->w4);

    nres_modsub(_MIPP_ mr_mip->w4,mr_mip->w6,pa->X);
    nres_modsub(_MIPP_ mr_mip->w6,pa->X,mr_mip->w6);
    nres_modsub(_MIPP_ mr_mip->w6,pa->X,mr_mip->w6);
    nres_modmult(_MIPP_ mr_mip->w8,mr_mip->w6,mr_mip->w2);
    nres_modmult(_MIPP_ mr_mip->w1,mr_mip->w5,mr_mip->w1);
    nres_modsub(_MIPP_ mr_mip->w2,mr_mip->w1,mr_mip->w5);

/* divide by 2 */

    nres_div2(_MIPP_ mr_mip->w5,pa->Y);

    pa->marker=MR_EPOINT_GENERAL;
    return TRUE;
}

int ecurve_add(_MIPD_ epoint *p,epoint *pa)
{  /* pa=pa+p; */
    if (mr_mip->ERNUM) return MR_OVER;

    MR_IN(94)

    if (p==pa)
    {
        ecurve_double(_MIPP_ pa);
        MR_OUT
        if (pa->marker==MR_EPOINT_INFINITY) return MR_OVER;
        return MR_DOUBLE;
    }
    if (pa->marker==MR_EPOINT_INFINITY)
    {
        epoint_copy(p,pa);
        MR_OUT
        return MR_ADD;
    }
    if (p->marker==MR_EPOINT_INFINITY)
    {
        MR_OUT
        return MR_ADD;
    }

    if (!ecurve_padd(_MIPP_ p,pa))
    {
        ecurve_double(_MIPP_ pa);
        MR_OUT
        return MR_DOUBLE;
    }
    MR_OUT
    if (pa->marker==MR_EPOINT_INFINITY) return MR_OVER;
    return MR_ADD;
}


void epoint_copy(epoint *a,epoint *b)
{
    if (a==b || b==NULL) return;

    copy(a->X,b->X);
    copy(a->Y,b->Y);
    if (a->marker==MR_EPOINT_GENERAL) copy(a->Z,b->Z);
    b->marker=a->marker;
    return;
}

BOOL epoint_norm(_MIPD_ epoint *p)
{ /* normalise a point */

    if (mr_mip->coord==MR_AFFINE) return TRUE;
    if (p->marker!=MR_EPOINT_GENERAL) return TRUE;

    if (mr_mip->ERNUM) return FALSE;

    MR_IN(117)

    copy(mr_mip->one,mr_mip->w8);

    if (nres_moddiv(_MIPP_ mr_mip->w8,p->Z,mr_mip->w8)>1) /* 1/Z  */
    {
        epoint_set(_MIPP_ NULL,NULL,0,p);
        mr_berror(_MIPP_ MR_ERR_COMPOSITE_MODULUS);
        MR_OUT
        return FALSE;
    }

    nres_modmult(_MIPP_ mr_mip->w8,mr_mip->w8,mr_mip->w1);/* 1/ZZ */
    nres_modmult(_MIPP_ p->X,mr_mip->w1,p->X);            /* X/ZZ */
    nres_modmult(_MIPP_ mr_mip->w1,mr_mip->w8,mr_mip->w1);/* 1/ZZZ */
    nres_modmult(_MIPP_ p->Y,mr_mip->w1,p->Y);            /* Y/ZZZ */

    copy(mr_mip->one,p->Z);

    p->marker=MR_EPOINT_NORMALIZED;
    MR_OUT

    return TRUE;
}

BOOL zzn2_qr(_MIPD_ zzn2 *u)
{
    int j;

    if (mr_mip->ERNUM) return FALSE;
    if (zzn2_iszero(u)) return TRUE;
    if (size(u->b)==0) return TRUE;

    if (mr_mip->qnr==-1 && size(u->a)==0) return TRUE;


    MR_IN(203)

    nres_modmult(_MIPP_ u->b,u->b,mr_mip->w1);
    if (mr_mip->qnr==-2) nres_modadd(_MIPP_ mr_mip->w1,mr_mip->w1,mr_mip->w1);
    nres_modmult(_MIPP_ u->a,u->a,mr_mip->w2);
    nres_modadd(_MIPP_ mr_mip->w1,mr_mip->w2,mr_mip->w1);
    redc(_MIPP_ mr_mip->w1,mr_mip->w1);
    j=jack(_MIPP_ mr_mip->w1,mr_mip->modulus);

    MR_OUT
    if (j==1) return TRUE;
    return FALSE;
}

int jack(_MIPD_ big a,big n)
{ /* find jacobi symbol (a/n), for positive odd n */
    big w;
    int nm8,onm8,t;
    if (mr_mip->ERNUM || size(a)==0 || size(n) <1) return 0;
    MR_IN(3)

    t=1;
    copy(n,mr_mip->w2);
    nm8=remain(_MIPP_ mr_mip->w2,8);
    if (nm8%2==0)
    {
        MR_OUT
        return 0;
    }

    if (size(a)<0)
    {
        if (nm8%4==3) t=-1;
        negify(a,mr_mip->w1);
    }
    else copy(a,mr_mip->w1);

    while (size(mr_mip->w1)!=0)
    {
        while (remain(_MIPP_ mr_mip->w1,2)==0)
        {
            subdiv(_MIPP_ mr_mip->w1,2,mr_mip->w1);
            if (nm8==3 || nm8==5) t=-t;
        }
        if (mr_compare(mr_mip->w1,mr_mip->w2)<0)
        {
            onm8=nm8;
            w=mr_mip->w1; mr_mip->w1=mr_mip->w2; mr_mip->w2=w;
            nm8=remain(_MIPP_ mr_mip->w2,8);
            if (onm8%4==3 && nm8%4==3) t=-t;
        }
        mr_psub(_MIPP_ mr_mip->w1,mr_mip->w2,mr_mip->w1);
        subdiv(_MIPP_ mr_mip->w1,2,mr_mip->w1);

        if (nm8==3 || nm8==5) t=-t;
    }

    MR_OUT
    if (size(mr_mip->w2)==1) return t;
    return 0;
}

miracl *mr_first_alloc()
{
    return (miracl *)calloc(1,sizeof(miracl));
}


int recode(_MIPD_ big e,int t,int w,int i)
{ /* recode exponent for Comb method */
    int j,r;
    r=0;
    for (j=w-1;j>=0;j--)
    {
        r<<=1;
        r|=mr_testbit(_MIPP_ e,i+j*t);
    }
    return r;
}


/* routines to support sliding-windows exponentiation *
 * in various contexts */

int mr_testbit(_MIPD_ big x,int n)
{ /* return value of n-th bit of big */
    if ((x->w[n/mr_mip->lg2b] & ((mr_small)1<<(n%mr_mip->lg2b))) >0) return 1;
    return 0;
}


int logb2(_MIPD_ big x)
{ /* returns number of bits in x */
    int xl,lg2;
    mr_small top;
    if (mr_mip->ERNUM || size(x)==0) return 0;

    MR_IN(49)
    if (mr_mip->base==mr_mip->base2)
    {
        xl=(int)(x->len&MR_OBITS);
        lg2=mr_mip->lg2b*(xl-1);
        top=x->w[xl-1];
        while (top>=1)
        {
            lg2++;
            top/=2;
        }
    }
    else
    {
        copy(x,mr_mip->w0);
        insign(PLUS,mr_mip->w0);
        lg2=0;
        while (mr_mip->w0->len>1)
        {
            mr_sdiv(_MIPP_ mr_mip->w0,mr_mip->base2,mr_mip->w0);
            lg2+=mr_mip->lg2b;
        }

        while (mr_mip->w0->w[0]>=1)
        {
            lg2++;
            mr_mip->w0->w[0]/=2;
        }
    }
    MR_OUT
    return lg2;
}

void absol(flash x,flash y)
{ /* y=abs(x) */
    copy(x,y);
    y->len&=MR_OBITS;
}

int hamming(_MIPD_ big x)
{
    int h;
    if (mr_mip->ERNUM) return 0;
    MR_IN(148);
    h=0;
    copy(x,mr_mip->w1);
    absol(mr_mip->w1,mr_mip->w1);
    while (size(mr_mip->w1)!=0)
        h+=subdiv(_MIPP_ mr_mip->w1,2,mr_mip->w1);

    MR_OUT
    return h;
}


int mr_window(_MIPD_ big x,int i,int *nbs,int * nzs,int window_size)
{ /* returns sliding window value, max. of 5 bits,         *
   * (Note from version 5.23 this can be changed by        *
   * setting parameter window_size. This can be            *
   * a useful space-saver) starting at i-th bit of big x.  *
   * nbs is number of bits processed, nzs is the number of *
   * additional trailing zeros detected. Returns valid bit *
   * pattern 1x..x1 with no two adjacent 0's. So 10101     *
   * will return 21 with nbs=5, nzs=0. 11001 will return 3,*
   * with nbs=2, nzs=2, having stopped after the first 11..*/
    int j,r,w;
    w=window_size;

/* check for leading 0 bit */

    *nbs=1;
    *nzs=0;
    if (!mr_testbit(_MIPP_ x,i)) return 0;

/* adjust window size if not enough bits left */

    if (i-w+1<0) w=i+1;

    r=1;
    for (j=i-1;j>i-w;j--)
    { /* accumulate bits. Abort if two 0's in a row */
        (*nbs)++;
        r*=2;
        if (mr_testbit(_MIPP_ x,j)) r+=1;
        if (r%4==0)
        { /* oops - too many zeros - shorten window */
            r/=4;
            *nbs-=2;
            *nzs=2;
            break;
        }
    }
    if (r%2==0)
    { /* remove trailing 0 */
        r/=2;
        *nzs=1;
        (*nbs)--;
    }
    return r;
}



void bytes_to_big(_MIPD_ int len,const char *ptr,big x)
{ /* convert len bytes into a big           *
   * The first byte is the Most significant */
    int i,j,m,n,r;
    unsigned int dig;
    unsigned char ch;
    mr_small wrd;
    if (mr_mip->ERNUM) return;
    MR_IN(140);

    zero(x);

    if (len<=0)
    {
        MR_OUT
        return;
    }
/* remove leading zeros.. */

    while (*ptr==0)
    {
        ptr++; len--;
        if (len==0)
        {
            MR_OUT
            return;
        }
    }
    if (mr_mip->base==0)
    { /* pack bytes directly into big */
        m=MIRACL/8;
        n=len/m;

        r=len%m;
		wrd=(mr_small)0;
        if (r!=0)
        {
            n++;
            for (j=0;j<r;j++) {wrd<<=8; wrd|=MR_TOBYTE(*ptr++); }
        }
        x->len=n;
        if (n>mr_mip->nib && mr_mip->check)
        {
            mr_berror(_MIPP_ MR_ERR_OVERFLOW);
            MR_OUT
            return;
        }
        if (r!=0)
        {
            n--;
            x->w[n]=wrd;
        }

        for (i=n-1;i>=0;i--)
        {
            for (j=0;j<m;j++) { wrd<<=8; wrd|=MR_TOBYTE(*ptr++); }
            x->w[i]=wrd;
        }
        mr_lzero(x);     /* needed */
    }
    else
    {
        for (i=0;i<len;i++)
        {
            if (mr_mip->ERNUM) break;

            premult(_MIPP_ x,256,x);
            ch=MR_TOBYTE(ptr[i]);
            dig=ch;
            incr(_MIPP_ x,(int)dig,x);
        }
    }
    MR_OUT
}


mr_small prepare_monty(_MIPD_ big n)
{ /* prepare Montgomery modulus */
    if (mr_mip->ERNUM) return (mr_small)0;
/* Is it set-up already? */
    if (size(mr_mip->modulus)!=0)
        if (mr_compare(n,mr_mip->modulus)==0) return mr_mip->ndash;

    MR_IN(80)

    if (size(n)<=2)
    {
        mr_berror(_MIPP_ MR_ERR_BAD_MODULUS);
        MR_OUT
        return (mr_small)0;
    }

    zero(mr_mip->w6);
    zero(mr_mip->w15);

/* set a small negative QNR (on the assumption that n is prime!) */
/* These defaults can be over-ridden                             */

/* Did you know that for p=2 mod 3, -3 is a QNR? */

    mr_mip->pmod8=remain(_MIPP_ n,8);

    switch (mr_mip->pmod8)
    {
    case 0:
    case 1:
    case 2:
    case 4:
    case 6:
        mr_mip->qnr=0;  /* none defined */
        break;
    case 3:
        mr_mip->qnr=-1;
        break;
    case 5:
        mr_mip->qnr=-2;
        break;
    case 7:
        mr_mip->qnr=-1;
        break;
    }
	mr_mip->pmod9=remain(_MIPP_ n,9);

	mr_mip->NO_CARRY=FALSE;
	if (n->w[n->len-1]>>M4 < 5) mr_mip->NO_CARRY=TRUE;


    mr_mip->MONTY=ON;

    convert(_MIPP_ 1,mr_mip->one);
    if (!mr_mip->MONTY)
    { /* Montgomery arithmetic is turned off */
        copy(n,mr_mip->modulus);
        mr_mip->ndash=0;
        MR_OUT
        return (mr_small)0;
    }
        mr_mip->w6->len=2;
        mr_mip->w6->w[0]=0;
        mr_mip->w6->w[1]=1;    /* w6 = base */
        mr_mip->w15->len=1;
        mr_mip->w15->w[0]=n->w[0];  /* w15 = n mod base */
        if (invmodp(_MIPP_ mr_mip->w15,mr_mip->w6,mr_mip->w14)!=1)
        { /* problems */
            mr_berror(_MIPP_ MR_ERR_BAD_MODULUS);
            MR_OUT
            return (mr_small)0;
        }

    mr_mip->ndash=mr_mip->base-mr_mip->w14->w[0]; /* = N' mod b */
    copy(n,mr_mip->modulus);
    mr_mip->check=OFF;
    mr_shift(_MIPP_ mr_mip->modulus,(int)mr_mip->modulus->len,mr_mip->pR);
    mr_mip->check=ON;
    nres(_MIPP_ mr_mip->one,mr_mip->one);
    MR_OUT

    return mr_mip->ndash;
}

BOOL mr_notint(flash x)
{ /* returns TRUE if x is Flash */
    return FALSE;
}

void mr_shift(_MIPD_ big x,int n,big w)
{ /* set w=x.(mr_base^n) by shifting */
    mr_lentype s;
    int i,bl;
    mr_small *gw=w->w;
    if (mr_mip->ERNUM) return;
    copy(x,w);
    if (w->len==0 || n==0) return;
    MR_IN(33)

    if (mr_notint(w)) mr_berror(_MIPP_ MR_ERR_INT_OP);
    s=(w->len&(MR_MSBIT));
    bl=(int)(w->len&(MR_OBITS))+n;
    if (bl<=0)
    {
        zero(w);
        MR_OUT
        return;
    }
    if (bl>mr_mip->nib && mr_mip->check) mr_berror(_MIPP_ MR_ERR_OVERFLOW);
    if (mr_mip->ERNUM)
    {
        MR_OUT
        return;
    }
    if (n>0)
    {
        for (i=bl-1;i>=n;i--)
            gw[i]=gw[i-n];
        for (i=0;i<n;i++)
            gw[i]=0;
    }
    else
    {
        n=(-n);
        for (i=0;i<bl;i++)
            gw[i]=gw[i+n];
        for (i=0;i<n;i++)
            gw[bl+i]=0;
    }
    w->len=(bl|s);
    MR_OUT
}

void zero(flash x)
{ /* set big/flash number to zero */
    int i,n;
    mr_small *g;
    if (x==NULL) return;
    n=(x->len&MR_OBITS);
    g=x->w;

    for (i=0;i<n;i++)
        g[i]=0;

    x->len=0;
}


void zzn2_zero(zzn2 *w)
{
    zero(w->a);
    zero(w->b);
}

void zzn2_from_int(_MIPD_ int i,zzn2 *w)
{
    if (mr_mip->ERNUM) return;

    MR_IN(156)
    if (i==1)
    {
        copy(mr_mip->one,w->a);
    }
    else
    {
        convert(_MIPP_ i,mr_mip->w1);
        nres(_MIPP_ mr_mip->w1,w->a);
    }
    zero(w->b);
    MR_OUT
}

void zzn2_from_ints(_MIPD_ int i,int j,zzn2 *w)
{
    if (mr_mip->ERNUM) return;

    MR_IN(168)
    convert(_MIPP_ i,mr_mip->w1);
    nres(_MIPP_ mr_mip->w1,w->a);
    convert(_MIPP_ j,mr_mip->w1);
    nres(_MIPP_ mr_mip->w1,w->b);

    MR_OUT
}

void zzn2_from_zzns(big x,big y,zzn2 *w)
{
    copy(x,w->a);
    copy(y,w->b);
}

void zzn2_from_bigs(_MIPD_ big x,big y, zzn2 *w)
{
    if (mr_mip->ERNUM) return;

    MR_IN(166)
    nres(_MIPP_ x,w->a);
    nres(_MIPP_ y,w->b);
    MR_OUT
}

void zzn2_from_zzn(big x,zzn2 *w)
{
    copy(x,w->a);
    zero(w->b);
}

void zzn2_from_big(_MIPD_ big x, zzn2 *w)
{
    if (mr_mip->ERNUM) return;

    MR_IN(167)
    nres(_MIPP_ x,w->a);
    zero(w->b);
    MR_OUT
}


void zzn2_copy(zzn2 *x,zzn2 *w)
{
    if (x==w) return;
    copy(x->a,w->a);
    copy(x->b,w->b);
}


BOOL zzn2_iszero(zzn2 *x)
{
    if (size(x->a)==0 && size(x->b)==0) return TRUE;
    return FALSE;
}

void nres_complex(_MIPD_ big a,big b,big r,big i)
{
    if (mr_mip->ERNUM) return;
	MR_IN(225)

	if (mr_mip->NO_CARRY && mr_mip->qnr==-1)
	{ /* if modulus is small enough we can ignore carries, and use simple addition and subtraction */
	  /* recall that Montgomery reduction can cope as long as product is less than pR */
			mr_padd(_MIPP_ a,b,mr_mip->w1);
			mr_padd(_MIPP_ a,mr_mip->modulus,mr_mip->w2);
			mr_psub(_MIPP_ mr_mip->w2,b,mr_mip->w2);
			mr_padd(_MIPP_ a,a,r);
		nres_modmult(_MIPP_ r,b,i);
		nres_modmult(_MIPP_ mr_mip->w1,mr_mip->w2,r);
	}
	else
	{
		nres_modadd(_MIPP_ a,b,mr_mip->w1);
		nres_modsub(_MIPP_ a,b,mr_mip->w2);

		if (mr_mip->qnr==-2)
			nres_modsub(_MIPP_ mr_mip->w2,b,mr_mip->w2);

		nres_modmult(_MIPP_ a,b,i);
		nres_modmult(_MIPP_ mr_mip->w1,mr_mip->w2,r);

		if (mr_mip->qnr==-2)
			nres_modadd(_MIPP_ r,i,r);

		nres_modadd(_MIPP_ i,i,i);
	}
	MR_OUT
}

void zzn2_sqr(_MIPD_ zzn2 *x,zzn2 *w)
{
    if (mr_mip->ERNUM) return;
    MR_IN(210)

	nres_complex(_MIPP_ x->a,x->b,w->a,w->b);

    MR_OUT
}

void nres_double_modadd(_MIPD_ big x,big y,big w)
{

        if (mr_mip->ERNUM) return;
        MR_IN(153)

        mr_padd(_MIPP_ x,y,w);
        if (mr_compare(w,mr_mip->pR)>=0)
            mr_psub(_MIPP_ w,mr_mip->pR,w);

        MR_OUT
}

void nres_double_modsub(_MIPD_ big x,big y,big w)
{

        if (mr_mip->ERNUM) return;
        MR_IN(154)

        if (mr_compare(x,y)>=0)
            mr_psub(_MIPP_ x,y,w);
        else
        {
            mr_psub(_MIPP_ y,x,w);
            mr_psub(_MIPP_ mr_mip->pR,w,w);
        }

        MR_OUT
}

void nres_lazy(_MIPD_ big a0,big a1,big b0,big b1,big r,big i)
{
    if (mr_mip->ERNUM) return;

    mr_mip->check=OFF;

        MR_IN(151)
        multiply(_MIPP_ a0,b0,mr_mip->w0);
        multiply(_MIPP_ a1,b1,mr_mip->w5);

	if (mr_mip->NO_CARRY && mr_mip->qnr==-1)
	{ /* if modulus is small enough we can ignore carries, and use simple addition and subtraction */
			mr_padd(_MIPP_ mr_mip->w0,mr_mip->w5,mr_mip->w6);
			mr_padd(_MIPP_ a0,a1,mr_mip->w1);
			mr_padd(_MIPP_ b0,b1,mr_mip->w2);
	}
	else
	{
		nres_double_modadd(_MIPP_ mr_mip->w0,mr_mip->w5,mr_mip->w6);  /* w6 =  a0.b0+a1.b1 */
		if (mr_mip->qnr==-2)
          nres_double_modadd(_MIPP_ mr_mip->w5,mr_mip->w5,mr_mip->w5);
		nres_modadd(_MIPP_ a0,a1,mr_mip->w1);
		nres_modadd(_MIPP_ b0,b1,mr_mip->w2);
    }
	nres_double_modsub(_MIPP_ mr_mip->w0,mr_mip->w5,mr_mip->w0);  /* r = a0.b0+D.a1.b1 */
        redc(_MIPP_ mr_mip->w0,r);
        multiply(_MIPP_ mr_mip->w1,mr_mip->w2,mr_mip->w0);           /* w0=(a0+a1)*(b0+b1) */

	if (mr_mip->NO_CARRY && mr_mip->qnr==-1)
	{
			mr_psub(_MIPP_ mr_mip->w0,mr_mip->w6,mr_mip->w0);
	}
	else
		nres_double_modsub(_MIPP_ mr_mip->w0,mr_mip->w6,mr_mip->w0); /* (a0+a1)*(b0+b1) - w6 */
        redc(_MIPP_ mr_mip->w0,i);
        MR_OUT

    mr_mip->check=ON;

}

void zzn2_mul(_MIPD_ zzn2 *x,zzn2 *y,zzn2 *w)
{
    if (mr_mip->ERNUM) return;
	if (x==y) {zzn2_sqr(_MIPP_ x,w); return; }
    MR_IN(162)
 /* Uses w1, w2, and w5 */

    if (zzn2_iszero(x) || zzn2_iszero(y)) zzn2_zero(w);
    else
    {
        if (x->a->len!=0 && x->b->len!=0 && y->a->len!=0 && y->b->len!=0)
            nres_lazy(_MIPP_ x->a,x->b,y->a,y->b,w->a,w->b);
        else
        {
            nres_modmult(_MIPP_ x->a,y->a,mr_mip->w1);
            nres_modmult(_MIPP_ x->b,y->b,mr_mip->w2);
            nres_modadd(_MIPP_ x->a,x->b,mr_mip->w5);
            nres_modadd(_MIPP_ y->a,y->b,w->b);
            nres_modmult(_MIPP_ w->b,mr_mip->w5,w->b);
            nres_modsub(_MIPP_ w->b,mr_mip->w1,w->b);
            nres_modsub(_MIPP_ w->b,mr_mip->w2,w->b);
            nres_modsub(_MIPP_ mr_mip->w1,mr_mip->w2,w->a);
            if (mr_mip->qnr==-2)
                nres_modsub(_MIPP_ w->a,mr_mip->w2,w->a);
        }
    }
    MR_OUT
}

BOOL epoint_comp(_MIPD_ epoint *a,epoint *b)
{
    BOOL result;
    if (mr_mip->ERNUM) return FALSE;
    if (a==b) return TRUE;
    if (a->marker==MR_EPOINT_INFINITY)
    {
        if (b->marker==MR_EPOINT_INFINITY) return TRUE;
        else return FALSE;
    }
    if (b->marker==MR_EPOINT_INFINITY)
        return FALSE;

    if (mr_mip->coord==MR_AFFINE)
    {
        if (mr_compare(a->X,b->X)==0 && mr_compare(a->Y,b->Y)==0) result=TRUE;
        else result=FALSE;
        return result;
    }

    if (mr_mip->coord==MR_PROJECTIVE)
    {
        MR_IN(105)
        if (a->marker!=MR_EPOINT_GENERAL)
            copy(mr_mip->one,mr_mip->w1);
        else copy(a->Z,mr_mip->w1);

        if (b->marker!=MR_EPOINT_GENERAL)
            copy(mr_mip->one,mr_mip->w2);
        else copy(b->Z,mr_mip->w2);

        nres_modmult(_MIPP_ mr_mip->w1,mr_mip->w1,mr_mip->w3); /* Za*Za */
        nres_modmult(_MIPP_ mr_mip->w2,mr_mip->w2,mr_mip->w4); /* Zb*Zb */

        nres_modmult(_MIPP_ a->X,mr_mip->w4,mr_mip->w5); /* Xa*Zb*Zb */
        nres_modmult(_MIPP_ b->X,mr_mip->w3,mr_mip->w6); /* Xb*Za*Za */

        if (mr_compare(mr_mip->w5,mr_mip->w6)!=0) result=FALSE;
        else
        {
            nres_modmult(_MIPP_ mr_mip->w1,mr_mip->w3,mr_mip->w3);
            nres_modmult(_MIPP_ mr_mip->w2,mr_mip->w4,mr_mip->w4);

            nres_modmult(_MIPP_ a->Y,mr_mip->w4,mr_mip->w5);
            nres_modmult(_MIPP_ b->Y,mr_mip->w3,mr_mip->w6);

            if (mr_compare(mr_mip->w5,mr_mip->w6)!=0) result=FALSE;
            else result=TRUE;
        }
        MR_OUT
        return result;
    }
    return FALSE;
}

BOOL zzn2_compare(zzn2 *x,zzn2 *y)
{
    if (mr_compare(x->a,y->a)==0 && mr_compare(x->b,y->b)==0) return TRUE;
    return FALSE;
}

int mr_compare(big x,big y)
{  /* compare x and y: =1 if x>y  =-1 if x<y *
    *  =0 if x=y                             */
    int m,n,sig;
    mr_lentype sx,sy;
    if (x==y) return 0;
    sx=(x->len&MR_MSBIT);
    sy=(y->len&MR_MSBIT);
    if (sx==0) sig=PLUS;
    else       sig=MINUS;
    if (sx!=sy) return sig;
    m=(int)(x->len&MR_OBITS);
    n=(int)(y->len&MR_OBITS);
    if (m>n) return sig;
    if (m<n) return -sig;
    while (m>0)
    { /* check digit by digit */
        m--;
        if (x->w[m]>y->w[m]) return sig;
        if (x->w[m]<y->w[m]) return -sig;
    }
    return 0;
}

void nres_negate(_MIPD_ big x, big w)
{
	if (size(x)==0)
	{
		zero(w);
		return;
	}
        if (mr_mip->ERNUM) return;

        MR_IN(92)
        mr_psub(_MIPP_ mr_mip->modulus,x,w);
        MR_OUT

}

void nres_modadd(_MIPD_ big x,big y,big w)
{ /* modular addition */
        if (mr_mip->ERNUM) return;

        MR_IN(90)
        mr_padd(_MIPP_ x,y,w);
        if (mr_compare(w,mr_mip->modulus)>=0) mr_psub(_MIPP_ w,mr_mip->modulus,w);

        MR_OUT
}

void nres_modsub(_MIPD_ big x,big y,big w)
{ /* modular subtraction */
        if (mr_mip->ERNUM) return;

        MR_IN(91)

        if (mr_compare(x,y)>=0)
            mr_psub(_MIPP_ x,y,w);
        else
        {
            mr_psub(_MIPP_ y,x,w);
            mr_psub(_MIPP_ mr_mip->modulus,w,w);
        }

        MR_OUT

}

void nres_modmult(_MIPD_ big x,big y,big w)
{ /* Modular multiplication using n-residues w=x*y mod n */
    if ((x==NULL || x->len==0) && x==w) return;
    if ((y==NULL || y->len==0) && y==w) return;
    if (y==NULL || x==NULL || x->len==0 || y->len==0)
    {
        zero(w);
        return;
    }
        if (mr_mip->ERNUM) return;

        MR_IN(83)

        mr_mip->check=OFF;
        multiply(_MIPP_ x,y,mr_mip->w0);
        redc(_MIPP_ mr_mip->w0,w);
        mr_mip->check=ON;
        MR_OUT

}


void zzn2_add(_MIPD_ zzn2 *x,zzn2 *y,zzn2 *w)
{
    if (mr_mip->ERNUM) return;
    MR_IN(159)
    nres_modadd(_MIPP_ x->a,y->a,w->a);
    nres_modadd(_MIPP_ x->b,y->b,w->b);
    MR_OUT
}

void zzn2_sadd(_MIPD_ zzn2 *x,big y,zzn2 *w)
{
    if (mr_mip->ERNUM) return;
    MR_IN(169)
    nres_modadd(_MIPP_ x->a,y,w->a);
    MR_OUT
}

void zzn2_conj(_MIPD_ zzn2 *x,zzn2 *w)
{
    MR_IN(158)
    if (mr_mip->ERNUM) return;
    zzn2_copy(x,w);
    nres_negate(_MIPP_ w->b,w->b);
    MR_OUT
}


void zzn2_inv(_MIPD_ zzn2 *w)
{
    if (mr_mip->ERNUM) return;
    MR_IN(163)
    nres_modmult(_MIPP_ w->a,w->a,mr_mip->w1);
    nres_modmult(_MIPP_ w->b,w->b,mr_mip->w2);
    nres_modadd(_MIPP_ mr_mip->w1,mr_mip->w2,mr_mip->w1);

    if (mr_mip->qnr==-2)
        nres_modadd(_MIPP_ mr_mip->w1,mr_mip->w2,mr_mip->w1);
    redc(_MIPP_ mr_mip->w1,mr_mip->w6);

    invmodp(_MIPP_ mr_mip->w6,mr_mip->modulus,mr_mip->w6);

    nres(_MIPP_ mr_mip->w6,mr_mip->w6);

    nres_modmult(_MIPP_ w->a,mr_mip->w6,w->a);
    nres_negate(_MIPP_ mr_mip->w6,mr_mip->w6);
    nres_modmult(_MIPP_ w->b,mr_mip->w6,w->b);
    MR_OUT
}

/* divide zzn2 by 2 */

void zzn2_div2(_MIPD_ zzn2 *w)
{
    if (mr_mip->ERNUM) return;
    MR_IN(173)

    nres_div2(_MIPP_ w->a,w->a);
    nres_div2(_MIPP_ w->b,w->b);

    MR_OUT
}

void zzn2_smul(_MIPD_ zzn2 *x,big y,zzn2 *w)
{
    if (mr_mip->ERNUM) return;
    MR_IN(161)
    if (size(x->a)!=0) nres_modmult(_MIPP_ x->a,y,w->a);
    else zero(w->a);
    if (size(x->b)!=0) nres_modmult(_MIPP_ x->b,y,w->b);
    else zero(w->b);
    MR_OUT
}

void zzn2_timesi(_MIPD_ zzn2 *u)
{
    if (mr_mip->ERNUM) return;
    MR_IN(164)
    copy(u->a,mr_mip->w1);
    nres_negate(_MIPP_ u->b,u->a);
    if (mr_mip->qnr==-2)
        nres_modadd(_MIPP_ u->a,u->a,u->a);

    copy(mr_mip->w1,u->b);
    MR_OUT
}


void zzn2_txx(_MIPD_ zzn2 *u)
{
  /* multiply w by t^2 where x^2-t is irreducible polynomial for ZZn4

   for p=5 mod 8 t=sqrt(sqrt(-2)), qnr=-2
   for p=3 mod 8 t=sqrt(1+sqrt(-1)), qnr=-1
   for p=7 mod 8 and p=2,3 mod 5 t=sqrt(2+sqrt(-1)), qnr=-1 */
    zzn2 t;
    if (mr_mip->ERNUM) return;
    MR_IN(196)

    switch (mr_mip->pmod8)
    {
    case 5:
        zzn2_timesi(_MIPP_ u);
        break;
    case 3:
        t.a=mr_mip->w3;
        t.b=mr_mip->w4;
        zzn2_copy(u,&t);
        zzn2_timesi(_MIPP_ u);
        zzn2_add(_MIPP_ u,&t,u);
        break;
    case 7:
        t.a=mr_mip->w3;
        t.b=mr_mip->w4;
        zzn2_copy(u,&t);
        zzn2_timesi(_MIPP_ u);
        zzn2_add(_MIPP_ u,&t,u);
        zzn2_add(_MIPP_ u,&t,u);
        break;
    default: break;
    }
    MR_OUT
}

void expint(_MIPD_ int b,int n,big x)
{ /* sets x=b^n */
    unsigned int bit,un;
    if (mr_mip->ERNUM) return;
    convert(_MIPP_ 1,x);
    if (n==0) return;

    MR_IN(50)

    if (n<0)
    {
        mr_berror(_MIPP_ MR_ERR_NEG_POWER);
        MR_OUT
        return;
    }
    if (b==2) expb2(_MIPP_ n,x);
    else
    {
        bit=1;
        un=(unsigned int)n;
        while (un>=bit) bit<<=1;
        bit>>=1;
        while (bit>0)
        { /* ltr method */
            multiply(_MIPP_ x,x,x);
            if ((bit&un)!=0) premult(_MIPP_ x,b,x);
            bit>>=1;
        }
    }
    MR_OUT
}

void expb2(_MIPD_ int n,big x)
{ /* sets x=2^n */
    int r,p;
    int i;
    miracl *mr_mip=get_mip();
    if (mr_mip->ERNUM) return;
    convert(_MIPP_ 1,x);
    if (n==0) return;

    MR_IN(149)

    if (n<0)
    {
        mr_berror(_MIPP_ MR_ERR_NEG_POWER);
        MR_OUT
        return;
    }
    r=n/mr_mip->lg2b;
    p=n%mr_mip->lg2b;

    if (mr_mip->base==mr_mip->base2)
    {
        mr_shift(_MIPP_ x,r,x);
        x->w[x->len-1]=mr_shiftbits(x->w[x->len-1],p);
    }
    else
    {
        for (i=1;i<=r;i++)
            mr_pmul(_MIPP_ x,mr_mip->base2,x);
        mr_pmul(_MIPP_ x,mr_shiftbits((mr_small)1,p),x);
    }
    MR_OUT
}

mr_small sgcd(mr_small x,mr_small y)
{ /* integer GCD, returns GCD of x and y */
    mr_small r;
    if (y==(mr_small)0) return x;
    while ((r=MR_REMAIN(x,y))!=(mr_small)0)
        x=y,y=r;
    return y;
}

BOOL subdivisible(_MIPD_ big x,int n)
{
    if (remain(_MIPP_ x,n)==0) return TRUE;
    else                return FALSE;
}

void powmod(_MIPD_ big x,big y,big n,big w)
{ /* fast powmod, using Montgomery's method internally */

    mr_small norm;
    BOOL mty;
    if (mr_mip->ERNUM) return;

    MR_IN(18)

    mty=TRUE;

    if (mr_mip->base!=mr_mip->base2)
    {
        if (size(n)<2 || sgcd(n->w[0],mr_mip->base)!=1) mty=FALSE;
    }
    else
        if (subdivisible(_MIPP_ n,2)) mty=FALSE;

    if (!mty)
    { /* can't use Montgomery */
        copy(y,mr_mip->w1);
        copy(x,mr_mip->w3);
        zero(w);
        if (size(mr_mip->w3)==0)
        {
            MR_OUT
            return;
        }
        convert(_MIPP_ 1,w);
        if (size(mr_mip->w1)==0)
        {
            MR_OUT
            return;
        }
        if (size(mr_mip->w1)<0) mr_berror(_MIPP_ MR_ERR_NEG_POWER);
        if (w==n)           mr_berror(_MIPP_ MR_ERR_BAD_PARAMETERS) ;
        if (mr_mip->ERNUM)
        {
            MR_OUT
            return;
        }

        norm=normalise(_MIPP_ n,n);
        divide(_MIPP_ mr_mip->w3,n,n);
        forever
        {
            if (mr_mip->user!=NULL) (*mr_mip->user)();

            if (subdiv(_MIPP_ mr_mip->w1,2,mr_mip->w1)!=0)
                mad(_MIPP_ w,mr_mip->w3,mr_mip->w3,n,n,w);
            if (mr_mip->ERNUM || size(mr_mip->w1)==0) break;
            mad(_MIPP_ mr_mip->w3,mr_mip->w3,mr_mip->w3,n,n,mr_mip->w3);
        }
        if (norm!=1)
        {
            mr_sdiv(_MIPP_ n,norm,n);
            divide(_MIPP_ w,n,n);
        }
    }
    else
    { /* optimized code for odd moduli */
        prepare_monty(_MIPP_ n);
        nres(_MIPP_ x,mr_mip->w3);
        nres_powmod(_MIPP_ mr_mip->w3,y,w);
        redc(_MIPP_ w,w);
    }

    MR_OUT
}

void nres_powmod(_MIPD_ big x,big y,big w)
{  /*  calculates w=x^y mod z, using m-residues       *
    *  See "Analysis of Sliding Window Techniques for *
    *  Exponentiation, C.K. Koc, Computers. Math. &   *
    *  Applic. Vol. 30 pp17-24 1995. Uses work-space  *
    *  variables for pre-computed table. */
    int i,j,k,t,nb,nbw,nzs,n;
    big table[16];
    if (mr_mip->ERNUM) return;

    copy(y,mr_mip->w1);
    copy(x,mr_mip->w3);

    MR_IN(84)
    zero(w);
    if (size(x)==0)
    {
       if (size(mr_mip->w1)==0)
       { /* 0^0 = 1 */
           copy(mr_mip->one,w);
       }
       MR_OUT
       return;
    }

    copy(mr_mip->one,w);
    if (size(mr_mip->w1)==0)
    {
        MR_OUT
        return;
    }

    if (size(mr_mip->w1)<0) mr_berror(_MIPP_ MR_ERR_NEG_POWER);

    if (mr_mip->ERNUM)
    {
        MR_OUT
        return;
    }
    if (mr_mip->base==mr_mip->base2)
    { /* build a table. Up to 5-bit sliding windows. Windows with
       * two adjacent 0 bits simply won't happen */
        table[0]=mr_mip->w3; table[1]=mr_mip->w4; table[2]=mr_mip->w5; table[3]=mr_mip->w14;
        table[4]=NULL;  table[5]=mr_mip->w6; table[6]=mr_mip->w15; table[7]=mr_mip->w8;
        table[8]=NULL;  table[9]=NULL;  table[10]=mr_mip->w9; table[11]=mr_mip->w10;
        table[12]=NULL; table[13]=mr_mip->w11; table[14]=mr_mip->w12; table[15]=mr_mip->w13;

        nres_modmult(_MIPP_ mr_mip->w3,mr_mip->w3,mr_mip->w2);  /* x^2 */
        n=15;
        j=0;
        do
        { /* pre-computations */
            t=1; k=j+1;
            while (table[k]==NULL) {k++; t++;}
            copy(table[j],table[k]);
            for (i=0;i<t;i++) nres_modmult(_MIPP_ table[k],mr_mip->w2,table[k]);
            j=k;
        } while (j<n);

        nb=logb2(_MIPP_ mr_mip->w1);
        copy(mr_mip->w3,w);
		if (nb>1) for (i=nb-2;i>=0;)
        { /* Left to Right method */

            if (mr_mip->user!=NULL) (*mr_mip->user)();

            n=mr_window(_MIPP_ mr_mip->w1,i,&nbw,&nzs,5);

            for (j=0;j<nbw;j++)
                    nres_modmult(_MIPP_ w,w,w);
            if (n>0)
                nres_modmult(_MIPP_ w,table[n/2],w);

            i-=nbw;
            if (nzs)
            {
                for (j=0;j<nzs;j++)
                    nres_modmult(_MIPP_ w,w,w);
                i-=nzs;
            }
        }
    }
    else
    {
        copy(mr_mip->w3,mr_mip->w2);
        forever
        { /* "Russian peasant" Right-to-Left exponentiation */

            if (mr_mip->user!=NULL) (*mr_mip->user)();

            if (subdiv(_MIPP_ mr_mip->w1,2,mr_mip->w1)!=0)
                nres_modmult(_MIPP_ w,mr_mip->w2,w);
            if (mr_mip->ERNUM || size(mr_mip->w1)==0) break;
            nres_modmult(_MIPP_ mr_mip->w2,mr_mip->w2,mr_mip->w2);
        }
    }
    MR_OUT
}

int powltr(_MIPD_ int x, big y, big n, big w)
{
    mr_small norm;
    BOOL clean_up,mty;
    if (mr_mip->ERNUM) return 0;

    MR_IN(71)
    mty=TRUE;

    if (mr_mip->base!=mr_mip->base2)
    {
        if (size(n)<2 || sgcd(n->w[0],mr_mip->base)!=1) mty=FALSE;
    }
    else
    {
        if (subdivisible(_MIPP_ n,2)) mty=FALSE;
    }

/* Is Montgomery modulus in use... */

    clean_up=FALSE;
    if (mty)
    { /* still a chance to use monty... */
       if (size(mr_mip->modulus)!=0)
       { /* somebody else is using it */
           if (mr_compare(n,mr_mip->modulus)!=0) mty=FALSE;
       }
       else
       { /* its unused, so use it, but clean up after */
           clean_up=TRUE;
       }
    }
    if (!mty)
    { /* can't use Montgomery! */
        copy(y,mr_mip->w1);
        zero(w);
        if (x==0)
        {
            MR_OUT
            return 0;
        }
        convert(_MIPP_ 1,w);
        if (size(mr_mip->w1)==0)
        {
            MR_OUT
            return 1;
        }

        if (size(mr_mip->w1)<0) mr_berror(_MIPP_ MR_ERR_NEG_POWER);
        if (w==n)               mr_berror(_MIPP_ MR_ERR_BAD_PARAMETERS) ;
        if (mr_mip->ERNUM)
        {
            MR_OUT
            return 0;
        }

        norm=normalise(_MIPP_ n,n);

        expb2(_MIPP_ logb2(_MIPP_ mr_mip->w1)-1,mr_mip->w2);
        while (size(mr_mip->w2)!=0)
        { /* Left to Right binary method */

            if (mr_mip->user!=NULL) (*mr_mip->user)();
            if (mr_mip->ERNUM) break;
            mad(_MIPP_ w,w,w,n,n,w);
            if (mr_compare(mr_mip->w1,mr_mip->w2)>=0)
            {
                premult(_MIPP_ w,x,w);
                divide(_MIPP_ w,n,n);
                subtract(_MIPP_ mr_mip->w1,mr_mip->w2,mr_mip->w1);
            }
            subdiv(_MIPP_ mr_mip->w2,2,mr_mip->w2);
        }
        if (norm!=1)
        {
#ifdef MR_FP_ROUNDING
            mr_sdiv(_MIPP_ n,norm,mr_invert(norm),n);
#else
            mr_sdiv(_MIPP_ n,norm,n);
#endif
            divide(_MIPP_ w,n,n);
        }
    }
    else
    { /* optimized code for odd moduli */
        prepare_monty(_MIPP_ n);
        nres_powltr(_MIPP_ x,y,w);
        redc(_MIPP_ w,w);
        if (clean_up) kill_monty(_MIPPO_ );
    }
    MR_OUT
    return (size(w));
}

void kill_monty(_MIPDO_ )
{
    zero(mr_mip->modulus);
}

void nres_powltr(_MIPD_ int x,big y,big w)
{ /* calculates w=x^y mod z using Left to Right Method   */
  /* uses only n^2 modular squarings, because x is small */
  /* Note: x is NOT an nresidue */
    int i,nb;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif

    if (mr_mip->ERNUM) return;
    copy(y,mr_mip->w1);

    MR_IN(86)

    zero(w);
    if (x==0)
    {
        if (size(mr_mip->w1)==0)
        { /* 0^0 = 1 */
            copy(mr_mip->one,w);
        }
        MR_OUT
        return;
    }

    copy(mr_mip->one,w);
    if (size(mr_mip->w1)==0)
    {
        MR_OUT
        return;
    }
    if (size(mr_mip->w1)<0) mr_berror(_MIPP_ MR_ERR_NEG_POWER);
    if (mr_mip->ERNUM)
    {
        MR_OUT
        return;
    }

#ifndef MR_ALWAYS_BINARY
    if (mr_mip->base==mr_mip->base2)
    {
#endif
        nb=logb2(_MIPP_ mr_mip->w1);
        convert(_MIPP_ x,w);
        nres(_MIPP_ w,w);
        if (nb>1) for (i=nb-2;i>=0;i--)
        { /* Left to Right binary method */

            if (mr_mip->user!=NULL) (*mr_mip->user)();

            nres_modmult(_MIPP_ w,w,w);
            if (mr_testbit(_MIPP_ mr_mip->w1,i))
            { /* this is quick */
                premult(_MIPP_ w,x,w);
                divide(_MIPP_ w,mr_mip->modulus,mr_mip->modulus);
            }
        }
#ifndef MR_ALWAYS_BINARY
    }
    else
    {
        expb2(_MIPP_ logb2(_MIPP_ mr_mip->w1)-1,mr_mip->w2);
        while (size(mr_mip->w2)!=0)
        { /* Left to Right binary method */

            if (mr_mip->user!=NULL) (*mr_mip->user)();
            if (mr_mip->ERNUM) break;
            nres_modmult(_MIPP_ w,w,w);
            if (mr_compare(mr_mip->w1,mr_mip->w2)>=0)
            {
                premult(_MIPP_ w,x,w);
                divide(_MIPP_ w,mr_mip->modulus,mr_mip->modulus);
                subtract(_MIPP_ mr_mip->w1,mr_mip->w2,mr_mip->w1);
            }
            subdiv(_MIPP_ mr_mip->w2,2,mr_mip->w2);
        }
    }
#endif
    if (size(w)<0) add(_MIPP_ w,mr_mip->modulus,w);
    MR_OUT
    return;
}

void powmod2(_MIPD_ big x,big y,big a,big b,big n,big w)
{/* w=x^y.a^b mod n */
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    if (mr_mip->ERNUM) return;

    MR_IN(79)

    prepare_monty(_MIPP_ n);
    nres(_MIPP_ x,mr_mip->w2);
    nres(_MIPP_ a,mr_mip->w4);
    nres_powmod2(_MIPP_ mr_mip->w2,y,mr_mip->w4,b,w);
    redc(_MIPP_ w,w);

    MR_OUT
}


int mr_window2(_MIPD_ big x,big y,int i,int *nbs,int *nzs)
{ /* two bit window for double exponentiation */
    int r,w;
    BOOL a,b,c,d;
    w=2;
    *nbs=1;
    *nzs=0;

/* check for two leading 0's */
    a=mr_testbit(_MIPP_ x,i); b=mr_testbit(_MIPP_ y,i);

    if (!a && !b) return 0;
    if (i<1) w=1;

    if (a)
    {
        if (b) r=3;
        else   r=2;
    }
    else r=1;
    if (w==1) return r;

    c=mr_testbit(_MIPP_ x,i-1); d=mr_testbit(_MIPP_ y,i-1);

    if (!c && !d)
    {
        *nzs=1;
        return r;
    }

    *nbs=2;
    r*=4;
    if (c)
    {
        if (d) r+=3;
        else   r+=2;
    }
    else r+=1;
    return r;
}

int cotstr(_MIPD_ flash x,char *string)
{  /*  output a big number in base IOBASE  */
    mr_small newb,oldb,b;
    int ipt;
    if (mr_mip->ERNUM) return 0;

    MR_IN(77)
    newb=mr_mip->IOBASE;
    oldb=mr_mip->apbase;
    b=mr_mip->base;
    mr_setbase(_MIPP_ newb);   /* temporarily change base ... */
    mr_mip->check=OFF;

    cbase(_MIPP_ x,b,mr_mip->w5);   /* number may get bigger !     */
    mr_mip->check=ON;

    ipt=otstr(_MIPP_ mr_mip->w5,string); /*..... and output number  */
    zero(mr_mip->w5);
    mr_setbase(_MIPP_ oldb);
    MR_OUT
    return ipt;
}

int numdig(_MIPD_ big x)
{  /* returns number of digits in x */
    int nd;

    if (x->len==0) return 0;

    nd=(int)(x->len&(MR_OBITS))*mr_mip->pack;
    while (getdig(_MIPP_ x,nd)==0)
        nd--;
    return nd;
}

int otstr(_MIPD_ flash x,char *string)
{  /*  output a big number  */
    int s,i,n,ch,rp,nd,m;
    BOOL check;
    mr_lentype lx;
    BOOL done;
    if (mr_mip->ERNUM) return 0;

    MR_IN(75)
    if (mr_mip->apbase==0 || mr_mip->apbase>256)
    {
        mr_berror(_MIPP_ MR_ERR_BASE_TOO_BIG);
        MR_OUT
        return 0;
    }

    if (!mr_mip->active)
    {
        mr_berror(_MIPP_ MR_ERR_NO_MIRSYS);
        MR_OUT
        return 0;
    }
    n=0;
    s=exsign(x);
    insign(PLUS,x);
    lx = x->len;
    if (lx==0 && mr_mip->apbase<=60)
    {

        if (!mr_mip->fout)
        {
            string[0]='0';
            string[1]='\0';
        }
        else
        {
            fputc('0',mr_mip->otfile);
            fputc('\n',mr_mip->otfile);
        }
        MR_OUT
        return 1;
    }
    rp=0;
    if (s==MINUS && mr_mip->apbase<=60)
    {
        if (!mr_mip->fout) string[n]='-';
        else fputc('-',mr_mip->otfile);
        n++;
    }
    copy(x,mr_mip->w6);
    done=TRUE;

    forever
    {
        nd=numdig(_MIPP_ mr_mip->w6);
        m=nd;
		if (mr_mip->apbase==64)
		{ /* add leading zeros to base64 */
			while (m%4!=0) m++;
		}
        if (rp>m) m=rp;
        for (i=m;i>0;i--)
        {
            if (!mr_mip->fout && string==mr_mip->IOBUFF && n>=mr_mip->IOBSIZ-5)
            {
                mr_berror(_MIPP_ MR_ERR_IO_OVERFLOW);
                MR_OUT
                return n;
            }
            if (i>nd && mr_mip->apbase!=64) ch='0';
            else
            {
                ch=getdig(_MIPP_ mr_mip->w6,i);
                check=mr_mip->check;
                mr_mip->check=OFF;
                putdig(_MIPP_ 0,mr_mip->w6,i);
              /*  mr_mip->check=mr_mip->check; Nasty stupid bug! */
				mr_mip->check=check;
                if (mr_mip->apbase<=60)
                { /* convert number to character */
                    ch+=48;
                    if (ch>=58) ch+=7;
                    if (ch>=91) ch+=6;
                }
                if (mr_mip->apbase==64)
                {
                    if (ch<26) ch+=65;
                    if (ch>=26 && ch<52) ch+=71;
                    if (ch>=52 && ch<62) ch-=4;
                    if (ch==62) ch='+';
                    if (ch==63) ch='/';
                }
            }
            if (i<rp && mr_mip->apbase<=60 && ch=='0' && size(mr_mip->w6)==0) break;
            if (!mr_mip->fout) string[n]=MR_TOBYTE(ch);
            else fputc(MR_TOBYTE(ch),mr_mip->otfile);
            n++;
        }
        if (done) break;
    }
/*
    if (mr_mip->apbase==64)
    {
        while (n%3!=0)
        {
#ifndef MR_NO_FILE_IO
            if (!mr_mip->fout) string[n]='=';
            else fputc('=',mr_mip->otfile);
#else
            string[n]='=';
#endif
            n++;
        }

    }
*/
/* Append a trailing 0 - it may be printable ascii text */
    if (!mr_mip->fout) string[n]='\0';
    else if (mr_mip->apbase<=60 || mr_mip->apbase==64) fputc('\n',mr_mip->otfile);
    insign(s,x);
    MR_OUT
    return n;
}

void nres_powmod2(_MIPD_ big x,big y,big a,big b,big w)
{ /* finds w = x^y.a^b mod n. Fast for some cryptosystems */
    int i,j,nb,nb2,nbw,nzs,n;
    big table[16];
    if (mr_mip->ERNUM) return;
    copy(y,mr_mip->w1);
    copy(x,mr_mip->w2);
    copy(b,mr_mip->w3);
    copy(a,mr_mip->w4);
    zero(w);
    if (size(mr_mip->w2)==0 || size(mr_mip->w4)==0) return;

    MR_IN(99)

    copy(mr_mip->one,w);
    if (size(mr_mip->w1)==0 && size(mr_mip->w3)==0)
    {
        MR_OUT
        return;
    }
    if (size(mr_mip->w1)<0 || size(mr_mip->w3)<0) mr_berror(_MIPP_ MR_ERR_NEG_POWER);
    if (mr_mip->ERNUM)
    {
        MR_OUT
        return;
    }

#ifndef MR_ALWAYS_BINARY

    if (mr_mip->base==mr_mip->base2)
    { /* uses 2-bit sliding window. This is 25% faster! */
#endif
        nres_modmult(_MIPP_ mr_mip->w2,mr_mip->w4,mr_mip->w5);
        nres_modmult(_MIPP_ mr_mip->w2,mr_mip->w2,mr_mip->w12);
        nres_modmult(_MIPP_ mr_mip->w4,mr_mip->w4,mr_mip->w13);
        nres_modmult(_MIPP_ mr_mip->w4,mr_mip->w13,mr_mip->w14);
        nres_modmult(_MIPP_ mr_mip->w2,mr_mip->w13,mr_mip->w6);
        nres_modmult(_MIPP_ mr_mip->w6,mr_mip->w4,mr_mip->w15);
        nres_modmult(_MIPP_ mr_mip->w4,mr_mip->w12,mr_mip->w8);
        nres_modmult(_MIPP_ mr_mip->w2,mr_mip->w12,mr_mip->w9);
        nres_modmult(_MIPP_ mr_mip->w4,mr_mip->w9,mr_mip->w10);
        nres_modmult(_MIPP_ mr_mip->w14,mr_mip->w12,mr_mip->w11);
        nres_modmult(_MIPP_ mr_mip->w9,mr_mip->w13,mr_mip->w12);
        nres_modmult(_MIPP_ mr_mip->w10,mr_mip->w13,mr_mip->w13);

        table[0]=NULL; table[1]=mr_mip->w4;  table[2]=mr_mip->w2;   table[3]=mr_mip->w5;
        table[4]=NULL; table[5]=mr_mip->w14; table[6]=mr_mip->w6;   table[7]=mr_mip->w15;
        table[8]=NULL; table[9]=mr_mip->w8;  table[10]=mr_mip->w9;  table[11]=mr_mip->w10;
        table[12]=NULL;table[13]=mr_mip->w11;table[14]=mr_mip->w12; table[15]=mr_mip->w13;
        nb=logb2(_MIPP_ mr_mip->w1);
        if ((nb2=logb2(_MIPP_ mr_mip->w3))>nb) nb=nb2;

        for (i=nb-1;i>=0;)
        {
            if (mr_mip->user!=NULL) (*mr_mip->user)();

            n=mr_window2(_MIPP_ mr_mip->w1,mr_mip->w3,i,&nbw,&nzs);
            for (j=0;j<nbw;j++)
                nres_modmult(_MIPP_ w,w,w);
            if (n>0) nres_modmult(_MIPP_ w,table[n],w);
            i-=nbw;
            if (nzs)
            {
                nres_modmult(_MIPP_ w,w,w);
                i--;
            }
        }
#ifndef MR_ALWAYS_BINARY
    }
    else
    {
        nres_modmult(_MIPP_ mr_mip->w2,mr_mip->w4,mr_mip->w5);

        if (mr_compare(mr_mip->w1,mr_mip->w3)>=0)
             expb2(_MIPP_ logb2(_MIPP_ mr_mip->w1)-1,mr_mip->w6);
        else expb2(_MIPP_ logb2(_MIPP_ mr_mip->w3)-1,mr_mip->w6);
        while (size(mr_mip->w6)!=0)
        {
            if (mr_mip->user!=NULL) (*mr_mip->user)();
            if (mr_mip->ERNUM) break;
            nres_modmult(_MIPP_ w,w,w);
            if (mr_compare(mr_mip->w1,mr_mip->w6)>=0)
            {
                if (mr_compare(mr_mip->w3,mr_mip->w6)>=0)
                {
                     nres_modmult(_MIPP_ w,mr_mip->w5,w);
                     subtract(_MIPP_ mr_mip->w3,mr_mip->w6,mr_mip->w3);
                }

                else nres_modmult(_MIPP_ w,mr_mip->w2,w);
                subtract(_MIPP_ mr_mip->w1,mr_mip->w6,mr_mip->w1);
            }
            else
            {
                if (mr_compare(mr_mip->w3,mr_mip->w6)>=0)
                {
                     nres_modmult(_MIPP_ w,mr_mip->w4,w);
                     subtract(_MIPP_ mr_mip->w3,mr_mip->w6,mr_mip->w3);
                }
            }
            subdiv(_MIPP_ mr_mip->w6,2,mr_mip->w6);
        }
    }
#endif
    MR_OUT
}

void power(_MIPD_ big x,long n,big z,big w)
{ /* raise big number to int power  w=x^n *
   * (mod z if z and w distinct)          */
    mr_small norm;

    copy(x,mr_mip->w5);
    zero(w);
    if(mr_mip->ERNUM || size(mr_mip->w5)==0) return;
    convert(_MIPP_ 1,w);
    if (n==0L) return;

    MR_IN(17)

    if (n<0L)
    {
        mr_berror(_MIPP_ MR_ERR_NEG_POWER);
        MR_OUT
        return;
    }

    if (w==z) forever
    { /* "Russian peasant" exponentiation */
        if (n%2!=0L)
             multiply(_MIPP_ w,mr_mip->w5,w);
        n/=2L;
        if (mr_mip->ERNUM || n==0L) break;
        multiply(_MIPP_ mr_mip->w5,mr_mip->w5,mr_mip->w5);
    }
    else
    {
        norm=normalise(_MIPP_ z,z);
        divide(_MIPP_ mr_mip->w5,z,z);
        forever
        {
            if (mr_mip->user!=NULL) (*mr_mip->user)();

            if (n%2!=0L) mad(_MIPP_ w,mr_mip->w5,mr_mip->w5,z,z,w);
            n/=2L;
            if (mr_mip->ERNUM || n==0L) break;
            mad(_MIPP_ mr_mip->w5,mr_mip->w5,mr_mip->w5,z,z,mr_mip->w5);
        }
        if (norm!=1)
        {
            mr_sdiv(_MIPP_ z,norm,z);
            divide(_MIPP_ w,z,z);
        }
    }

    MR_OUT
}

void redc(_MIPD_ big x,big y)
{ /* Montgomery's REDC function p. 520 */
  /* also used to convert n-residues back to normal form */
    mr_small carry,delay_carry,m,ndash,*w0g,*mg;

    int i,j,rn,rn2;
    big w0,modulus;
    if (mr_mip->ERNUM) return;

    MR_IN(82)

    w0=mr_mip->w0;        /* get these into local variables (for inline assembly) */
    modulus=mr_mip->modulus;
    ndash=mr_mip->ndash;

    copy(x,w0);
    if (!mr_mip->MONTY)
    {
/*#ifdef MR_CELL
        mod256(_MIPP_ w0,w0);
#else */
        divide(_MIPP_ w0,modulus,modulus);
/* #endif */
        copy(w0,y);
        MR_OUT
        return;
    }
    delay_carry=0;
    rn=(int)modulus->len;
    rn2=rn+rn;
    if (mr_mip->base==0)
    {
      mg=modulus->w;
      w0g=w0->w;
      for (i=0;i<rn;i++)
      {
       /* inline - substitutes for loop below */
/*        muldvd(w0->w[i],ndash,0,&m);    Note that after this time   */
        m=ndash*w0->w[i];
        carry=0;                       /* around the loop, w0[i]=0    */

        for (j=0;j<rn;j++)
        {
            muldvd2(m,modulus->w[j],&carry,&w0->w[i+j]);
        }
        w0->w[rn+i]+=delay_carry;
        if (w0->w[rn+i]<delay_carry) delay_carry=1;
        else delay_carry=0;
        w0->w[rn+i]+=carry;
        if (w0->w[rn+i]<carry) delay_carry=1;
      }

    }
    else for (i=0;i<rn;i++)
    {
        muldiv(w0->w[i],ndash,0,mr_mip->base,&m);
        carry=0;
        for (j=0;j<rn;j++)
        {
          carry=muldiv(modulus->w[j],m,w0->w[i+j]+carry,mr_mip->base,&w0->w[i+j]);
        }
        w0->w[rn+i]+=(delay_carry+carry);
        delay_carry=0;
        if (w0->w[rn+i]>=mr_mip->base)
        {
            w0->w[rn+i]-=mr_mip->base;
            delay_carry=1;
        }
    }
    w0->w[rn2]=delay_carry;
    w0->len=rn2+1;
    mr_shift(_MIPP_ w0,(-rn),w0);
    mr_lzero(w0);

    if (mr_compare(w0,modulus)>=0) mr_psub(_MIPP_ w0,modulus,w0);
    copy(w0,y);
    MR_OUT
}


void mr_track(_MIPDO_ )
{ /* track course of program execution *
   * through the MIRACL routines       */

    int i;
    for (i=0;i<mr_mip->depth;i++) fputc('-',stdout);
    fputc('>',stdout);
    mputs(names[mr_mip->trace[mr_mip->depth]]);
    fputc('\n',stdout);
}

void mr_lzero(big x)
{  /*  strip leading zeros from big number  */
    mr_lentype s;
    int m;
    s=(x->len&(MR_MSBIT));
    m=(int)(x->len&(MR_OBITS));
    while (m>0 && x->w[m-1]==0)
        m--;
    x->len=m;
    if (m>0) x->len|=s;
}


mr_small mr_setbase(_MIPD_ mr_small nb)
{  /* set base. Pack as many digits as  *
    * possible into each computer word  */
    mr_small temp;
    BOOL fits;
    int bits;
    fits=FALSE;
    bits=MIRACL;
    while (bits>1)
    {
        bits/=2;
        temp=((mr_small)1<<bits);
        if (temp==nb)
        {
            fits=TRUE;
            break;
        }
        if (temp<nb || (bits%2)!=0) break;
    }
    if (fits)
    {
        mr_mip->apbase=nb;
        mr_mip->pack=MIRACL/bits;
        mr_mip->base=0;
        return 0;
    }
    mr_mip->apbase=nb;
    mr_mip->pack=1;
    mr_mip->base=nb;
    if (mr_mip->base==0) return 0;
    temp=MR_DIV(MAXBASE,nb);
    while (temp>=nb)
    {
        temp=MR_DIV(temp,nb);
        mr_mip->base*=nb;
        mr_mip->pack++;
    }
    return 0;
}

void *memalloc(_MIPD_ int num)
{
    return mr_alloc(_MIPP_ mr_big_reserve(num,mr_mip->nib-1),1);
}

void memkill(_MIPD_ char *mem,int len)
{
    if (mem==NULL) return;
    memset(mem,0,mr_big_reserve(len,mr_mip->nib-1));
    mr_free(mem);
}


void set_io_buffer_size(_MIPD_ int len)
{
    int i;
    if (len<0) return;
    MR_IN(142)
    for (i=0;i<mr_mip->IOBSIZ;i++) mr_mip->IOBUFF[i]=0;
    mr_free(mr_mip->IOBUFF);
    if (len==0)
    {
        MR_OUT
        return;
    }
    mr_mip->IOBSIZ=len;
    mr_mip->IOBUFF=(char *)mr_alloc(_MIPP_ len+1,1);
    mr_mip->IOBUFF[0]='\0';
    MR_OUT
}

void muldvd2(mr_small a,mr_small b,mr_small *c,mr_small *rp)
{
    union doubleword dble;
    dble.d=(mr_large)a*b+*c+*rp;
    *rp=dble.h[MR_BOT];
    *c=dble.h[MR_TOP];
}


BOOL nres_multi_inverse(_MIPD_ int m,big *x,big *w)
{ /* find w[i]=1/x[i] mod n, for i=0 to m-1 *
   * x and w MUST be distinct               */
    int i;
    if (m==0) return TRUE;
    if (m<0) return FALSE;
    MR_IN(118)

    if (x==w)
    {
        mr_berror(_MIPP_ MR_ERR_BAD_PARAMETERS);
        MR_OUT
        return FALSE;
    }

    if (m==1)
    {
        copy(mr_mip->one,w[0]);
        nres_moddiv(_MIPP_ w[0],x[0],w[0]);
        MR_OUT
        return TRUE;
    }

    convert(_MIPP_ 1,w[0]);
    copy(x[0],w[1]);
    for (i=2;i<m;i++)
        nres_modmult(_MIPP_ w[i-1],x[i-1],w[i]);

    nres_modmult(_MIPP_ w[m-1],x[m-1],mr_mip->w6);  /* y=x[0]*x[1]*x[2]....x[m-1] */
    if (size(mr_mip->w6)==0)
    {
        mr_berror(_MIPP_ MR_ERR_DIV_BY_ZERO);
        MR_OUT
        return FALSE;
    }

    redc(_MIPP_ mr_mip->w6,mr_mip->w6);
    redc(_MIPP_ mr_mip->w6,mr_mip->w6);

    invmodp(_MIPP_ mr_mip->w6,mr_mip->modulus,mr_mip->w6);

/* Now y=1/y */

    copy(x[m-1],mr_mip->w5);
    nres_modmult(_MIPP_ w[m-1],mr_mip->w6,w[m-1]);

    for (i=m-2;;i--)
    {
        if (i==0)
        {
            nres_modmult(_MIPP_ mr_mip->w5,mr_mip->w6,w[0]);
            break;
        }
        nres_modmult(_MIPP_ w[i],mr_mip->w5,w[i]);
        nres_modmult(_MIPP_ w[i],mr_mip->w6,w[i]);
        nres_modmult(_MIPP_ mr_mip->w5,x[i],mr_mip->w5);
    }

    MR_OUT
    return TRUE;
}

BOOL epoint_multi_norm(_MIPD_ int m,big *work,epoint **p)
{ /* Normalise an array of points of length m<MR_MAX_M_T_S - requires a workspace array of length m */
    int i;
	BOOL inf=FALSE;
    big w[MR_MAX_M_T_S];
    if (mr_mip->coord==MR_AFFINE) return TRUE;
    if (mr_mip->ERNUM) return FALSE;
    if (m>MR_MAX_M_T_S) return FALSE;

    MR_IN(190)

    for (i=0;i<m;i++)
    {
        if (p[i]->marker==MR_EPOINT_NORMALIZED) w[i]=mr_mip->one;
        else w[i]=p[i]->Z;
		if (p[i]->marker==MR_EPOINT_INFINITY) {inf=TRUE; break;} /* whoops, one of them is point at infinity */
    }

	if (inf)
	{
		for (i=0;i<m;i++) epoint_norm(_MIPP_ p[i]);
		MR_OUT
		return TRUE;
	}

    if (!nres_multi_inverse(_MIPP_ m,w,work))
    {
       MR_OUT
       return FALSE;
    }

    for (i=0;i<m;i++)
    {
        copy(mr_mip->one,p[i]->Z);
        p[i]->marker=MR_EPOINT_NORMALIZED;
        nres_modmult(_MIPP_ work[i],work[i],mr_mip->w1);
        nres_modmult(_MIPP_ p[i]->X,mr_mip->w1,p[i]->X);    /* X/ZZ */
        nres_modmult(_MIPP_ mr_mip->w1,work[i],mr_mip->w1);
        nres_modmult(_MIPP_ p[i]->Y,mr_mip->w1,p[i]->Y);    /* Y/ZZZ */
    }
    MR_OUT
    return TRUE;
}

int ecurve_mult(_MIPD_ big e,epoint *pa,epoint *pt)
{ /* pt=e*pa; */
    int i,j,n,nb,nbs,nzs,nadds;
    epoint *table[MR_ECC_STORE_N];
    big work[MR_ECC_STORE_N];

    char *mem;
    char *mem1;

    epoint *p;
    int ce,ch;
    if (mr_mip->ERNUM) return 0;

    MR_IN(95)
    if (size(e)==0)
    { /* multiplied by 0 */
        epoint_set(_MIPP_ NULL,NULL,0,pt);
        MR_OUT
        return 0;
    }
    copy(e,mr_mip->w9);
/*    epoint_norm(_MIPP_ pa); */
    epoint_copy(pa,pt);

    if (size(mr_mip->w9)<0)
    { /* pt = -pt */
        negify(mr_mip->w9,mr_mip->w9);
        epoint_negate(_MIPP_ pt);
    }

    if (size(mr_mip->w9)==1)
    {
        MR_OUT
        return 0;
    }

    premult(_MIPP_ mr_mip->w9,3,mr_mip->w10);      /* h=3*e */

    if (mr_mip->base==mr_mip->base2)
    {

        mem=(char *)ecp_memalloc(_MIPP_ MR_ECC_STORE_N);
        mem1=(char *)memalloc(_MIPP_ MR_ECC_STORE_N);

        for (i=0;i<=MR_ECC_STORE_N-1;i++)
        {
            table[i]=epoint_init_mem(_MIPP_ mem,i);
            work[i]=mirvar_mem(_MIPP_ mem1,i);
        }

        epoint_copy(pt,table[0]);
        epoint_copy(table[0],table[MR_ECC_STORE_N-1]);
        ecurve_double(_MIPP_ table[MR_ECC_STORE_N-1]);
     /*   epoint_norm(_MIPP_ table[MR_ECC_STORE_N-1]); */

        for (i=1;i<MR_ECC_STORE_N-1;i++)
        { /* precomputation */
            epoint_copy(table[i-1],table[i]);
            ecurve_add(_MIPP_ table[MR_ECC_STORE_N-1],table[i]);
        }
        ecurve_add(_MIPP_ table[MR_ECC_STORE_N-2],table[MR_ECC_STORE_N-1]);

        epoint_multi_norm(_MIPP_ MR_ECC_STORE_N,work,table);

        nb=logb2(_MIPP_ mr_mip->w10);
        nadds=0;
        epoint_set(_MIPP_ NULL,NULL,0,pt);
        for (i=nb-1;i>=1;)
        { /* add/subtract */
            if (mr_mip->user!=NULL) (*mr_mip->user)();
            n=mr_naf_window(_MIPP_ mr_mip->w9,mr_mip->w10,i,&nbs,&nzs,MR_ECC_STORE_N);
            for (j=0;j<nbs;j++)
                ecurve_double(_MIPP_ pt);
            if (n>0) {ecurve_add(_MIPP_ table[n/2],pt); nadds++;}
            if (n<0) {ecurve_sub(_MIPP_ table[(-n)/2],pt); nadds++;}
            i-=nbs;
            if (nzs)
            {
                for (j=0;j<nzs;j++) ecurve_double(_MIPP_ pt);
                i-=nzs;
            }
        }

        ecp_memkill(_MIPP_ mem,MR_ECC_STORE_N);
        memkill(_MIPP_ mem1,MR_ECC_STORE_N);

    }
    else
    {
        mem=(char *)ecp_memalloc(_MIPP_ 1);
        p=epoint_init_mem(_MIPP_ mem,0);
        epoint_norm(_MIPP_ pt);
        epoint_copy(pt,p);

        nadds=0;
        expb2(_MIPP_ logb2(_MIPP_ mr_mip->w10)-1,mr_mip->w11);
        mr_psub(_MIPP_ mr_mip->w10,mr_mip->w11,mr_mip->w10);
        subdiv(_MIPP_ mr_mip->w11,2,mr_mip->w11);
        while (size(mr_mip->w11) > 1)
        { /* add/subtract method */
            if (mr_mip->user!=NULL) (*mr_mip->user)();

            ecurve_double(_MIPP_ pt);
            ce=mr_compare(mr_mip->w9,mr_mip->w11); /* e(i)=1? */
            ch=mr_compare(mr_mip->w10,mr_mip->w11); /* h(i)=1? */
            if (ch>=0)
            {  /* h(i)=1 */
                if (ce<0) {ecurve_add(_MIPP_ p,pt); nadds++;}
                mr_psub(_MIPP_ mr_mip->w10,mr_mip->w11,mr_mip->w10);
            }
            if (ce>=0)
            {  /* e(i)=1 */
                if (ch<0) {ecurve_sub(_MIPP_ p,pt); nadds++;}
                mr_psub(_MIPP_ mr_mip->w9,mr_mip->w11,mr_mip->w9);
            }
            subdiv(_MIPP_ mr_mip->w11,2,mr_mip->w11);
        }
        ecp_memkill(_MIPP_ mem,1);
    }
    MR_OUT
    return nadds;
}


void ecp_memkill(_MIPD_ char *mem,int num)
{
    if (mem==NULL) return;

    if (mr_mip->coord==MR_AFFINE)
        memset(mem,0,mr_ecp_reserve_a(num,mr_mip->nib-1));
    else
        memset(mem,0,mr_ecp_reserve(num,mr_mip->nib-1));

    mr_free(mem);
}

int ecurve_sub(_MIPD_ epoint *p,epoint *pa)
{
    int r;
    if (mr_mip->ERNUM) return MR_OVER;

    MR_IN(104)

    if (p==pa)
    {
        epoint_set(_MIPP_ NULL,NULL,0,pa);
        MR_OUT
        return MR_OVER;
    }
    if (p->marker==MR_EPOINT_INFINITY)
    {
        MR_OUT
        return MR_ADD;
    }

    epoint_negate(_MIPP_ p);
    r=ecurve_add(_MIPP_ p,pa);
    epoint_negate(_MIPP_ p);

    MR_OUT
    return r;
}

int mr_naf_window(_MIPD_ big x,big x3,int i,int *nbs,int *nzs,int store)
{ /* returns sliding window value, using fractional windows   *
   * where "store" precomputed values are precalulated and    *
   * stored. Scanning starts at the i-th bit of  x. nbs is    *
   * the number of bits processed. nzs is number of           *
   * additional trailing zeros detected. x and x3 (which is   *
   * 3*x) are combined to produce the NAF (non-adjacent       *
   * form). So if x=11011(27) and x3 is 1010001, the LSB is   *
   * ignored and the value 100T0T (32-4-1=27) processed,      *
   * where T is -1. Note x.P = (3x-x)/2.P. This value will    *
   * return +7, with nbs=4 and nzs=1, having stopped after    *
   * the first 4 bits. If it goes too far, it must backtrack  *
   * Note in an NAF non-zero elements are never side by side, *
   * so 10T10T won't happen. NOTE: return value n zero or     *
   * odd, -21 <= n <= +21     */

    int nb,j,r,biggest;

 /* get first bit */
    nb=mr_testbit(_MIPP_ x3,i)-mr_testbit(_MIPP_ x,i);

    *nbs=1;
    *nzs=0;
    if (nb==0) return 0;
    if (i==0) return nb;

    biggest=2*store-1;

    if (nb>0) r=1;
    else      r=(-1);

    for (j=i-1;j>0;j--)
    {
        (*nbs)++;
        r*=2;
        nb=mr_testbit(_MIPP_ x3,j)-mr_testbit(_MIPP_ x,j);
        if (nb>0) r+=1;
        if (nb<0) r-=1;
        if (abs(r)>biggest) break;
    }

    if (r%2!=0 && j!=0)
    { /* backtrack */
        if (nb>0) r=(r-1)/2;
        if (nb<0) r=(r+1)/2;
        (*nbs)--;
    }

    while (r%2==0)
    { /* remove trailing zeros */
        r/=2;
        (*nzs)++;
        (*nbs)--;
    }
    return r;
}

void epoint_negate(_MIPD_ epoint *p)
{ /* negate a point */
    if (mr_mip->ERNUM) return;
    if (p->marker==MR_EPOINT_INFINITY) return;

    MR_IN(121)
    if (size(p->Y)!=0) mr_psub(_MIPP_ mr_mip->modulus,p->Y,p->Y);
    MR_OUT
}

static BOOL ecurve_add_sub(_MIPD_ epoint *P,epoint *Q,epoint *PP,epoint *PM)
{
    big t1,t2,lam;

    if (mr_mip->ERNUM) return FALSE;

    if (P->marker==MR_EPOINT_GENERAL || Q->marker==MR_EPOINT_GENERAL)
    {
        mr_berror(_MIPP_ MR_ERR_BAD_PARAMETERS);
        MR_OUT
        return FALSE;
    }

    if (mr_compare(P->X,Q->X)==0)
    { /* P=Q or P=-Q - shouldn't happen */
        epoint_copy(P,PP);
        ecurve_add(_MIPP_ Q,PP);
        epoint_copy(P,PM);
        ecurve_sub(_MIPP_ Q,PM);

        MR_OUT
        return TRUE;
    }

    t1= mr_mip->w10;
    t2= mr_mip->w11;
    lam = mr_mip->w13;

    copy(P->X,t2);
    nres_modsub(_MIPP_ t2,Q->X,t2);

    redc(_MIPP_ t2,t2);
    invmodp(_MIPP_ t2,mr_mip->modulus,t2);
    nres(_MIPP_ t2,t2);

    nres_modadd(_MIPP_ P->X,Q->X,PP->X);
    copy(PP->X,PM->X);

    copy(P->Y,t1);
    nres_modsub(_MIPP_ t1,Q->Y,t1);
    copy(t1,lam);
    nres_modmult(_MIPP_ lam,t2,lam);
    copy(lam,t1);
    nres_modmult(_MIPP_ t1,t1,t1);
    nres_modsub(_MIPP_ t1,PP->X,PP->X);
    copy(Q->X,PP->Y);
    nres_modsub(_MIPP_ PP->Y,PP->X,PP->Y);
    nres_modmult(_MIPP_ PP->Y,lam,PP->Y);
    nres_modsub(_MIPP_ PP->Y,Q->Y,PP->Y);

    copy(P->Y,t1);
    nres_modadd(_MIPP_ t1,Q->Y,t1);
    copy(t1,lam);
    nres_modmult(_MIPP_ lam,t2,lam);
    copy(lam,t1);
    nres_modmult(_MIPP_ t1,t1,t1);
    nres_modsub(_MIPP_ t1,PM->X,PM->X);
    copy(Q->X,PM->Y);
    nres_modsub(_MIPP_ PM->Y,PM->X,PM->Y);
    nres_modmult(_MIPP_ PM->Y,lam,PM->Y);
    nres_modadd(_MIPP_ PM->Y,Q->Y,PM->Y);

    PP->marker=MR_EPOINT_NORMALIZED;
    PM->marker=MR_EPOINT_NORMALIZED;

    return TRUE;
}

void mr_addbit(_MIPD_ big x,int n)
{ /* add 2^n to positive x - where you know that bit is zero. Use with care! */
    mr_lentype m=n/mr_mip->lg2b;
    x->w[m]+=mr_shiftbits((mr_small)1,n%mr_mip->lg2b);
    if (x->len<m+1) x->len=m+1;
}

void bigbits(_MIPD_ int n,big x)
{ /* sets x as random < 2^n */
    mr_small r;
    mr_lentype wlen;
    zero(x);
    if (mr_mip->ERNUM || n<=0) return;

    MR_IN(150)

    expb2(_MIPP_ n,mr_mip->w1);
    wlen=mr_mip->w1->len;
    do
    {
        r=brand(_MIPPO_ );
        if (mr_mip->base==0) x->w[x->len++]=r;
        else                 x->w[x->len++]=MR_REMAIN(r,mr_mip->base);
    } while (x->len<wlen);
    if (mr_mip->base==mr_mip->base2)
    {

    x->w[wlen-1]=MR_REMAIN(x->w[wlen-1],mr_mip->w1->w[wlen-1]);
    mr_lzero(x);

    }
    else
    {
        divide(_MIPP_ x,mr_mip->w1,mr_mip->w1);
    }

    MR_OUT
}

mr_small brand(_MIPDO_ )
{ /* Marsaglia & Zaman random number generator */
    int i,k;
    mr_unsign32 pdiff,t;
    mr_small r;
    if (mr_mip->lg2b>32)
    { /* underlying type is > 32 bits. Assume <= 64 bits */
        mr_mip->rndptr+=2;
        if (mr_mip->rndptr<NK-1)
        {
            r=(mr_small)mr_mip->ira[mr_mip->rndptr];
            r=mr_shiftbits(r,mr_mip->lg2b-32);
            r+=(mr_small)mr_mip->ira[mr_mip->rndptr+1];
            return r;
        }
    }
    else
    {
        mr_mip->rndptr++;
        if (mr_mip->rndptr<NK) return (mr_small)mr_mip->ira[mr_mip->rndptr];
    }
    mr_mip->rndptr=0;
    for (i=0,k=NK-NJ;i<NK;i++,k++)
    { /* calculate next NK values */
        if (k==NK) k=0;
        t=mr_mip->ira[k];
        pdiff=t - mr_mip->ira[i] - mr_mip->borrow;
        if (pdiff<t) mr_mip->borrow=0;
        if (pdiff>t) mr_mip->borrow=1;
        mr_mip->ira[i]=pdiff;
    }
    if (mr_mip->lg2b>32)
    { /* double up */
        r=(mr_small)mr_mip->ira[0];
        r=mr_shiftbits(r,mr_mip->lg2b-32);
        r+=(mr_small)mr_mip->ira[1];
        return r;
    }
    else return (mr_small)(mr_mip->ira[0]);
}

void bigrand(_MIPD_ big w,big x)
{  /*  generate a big random number 0<=x<w  */
    int m;
    mr_small r;
    if (mr_mip->ERNUM) return;

    MR_IN(20)

 /*   decr(_MIPP_ w,2,w);  */
    m=0;
    zero(mr_mip->w0);

    do
    { /* create big rand piece by piece */
        m++;
        mr_mip->w0->len=m;
        r=brand(_MIPPO_ );
        if (mr_mip->base==0) mr_mip->w0->w[m-1]=r;
        else                 mr_mip->w0->w[m-1]=MR_REMAIN(r,mr_mip->base);
    } while (mr_compare(mr_mip->w0,w)<0);
    mr_lzero(mr_mip->w0);
    divide(_MIPP_ mr_mip->w0,w,w);

    copy(mr_mip->w0,x);
 /*   incr(_MIPP_ x,2,x);
    if (w!=x) incr(_MIPP_ w,2,w); */
    MR_OUT
}

int big_to_bytes(_MIPD_ int max,big x,char *ptr,BOOL justify)
{ /* convert positive big into octet string */
    int i,j,r,m,n,len,start;
    unsigned int dig;
    unsigned char ch;
    mr_small wrd;

    if (mr_mip->ERNUM || max<0) return 0;

	if (max==0 && justify) return 0;
	if (size(x)==0)
	{
		if (justify)
		{
			for (i=0;i<max;i++) ptr[i]=0;
			return max;
		}
		return 0;
	}

    MR_IN(141);

    mr_lzero(x);        /* should not be needed.... */
    if (mr_mip->base==0)
    {
        m=MIRACL/8;
        n=(int)(x->len&MR_OBITS);
        n--;
        len=n*m;
        wrd=x->w[n]; /* most significant */
        r=0;
        while (wrd!=(mr_small)0) { r++; wrd>>=8; len++;}
        r%=m;

        if (max>0 && len>max)
        {
            mr_berror(_MIPP_ MR_ERR_TOO_BIG);
            MR_OUT
            return 0;
        }

        if (justify)
        {
            start=max-len;
            for (i=0;i<start;i++) ptr[i]=0;
        }
        else start=0;

        if (r!=0)
        {
            wrd=x->w[n--];
            for (i=r-1;i>=0;i--)
            {
                ptr[start+i]=(char)(wrd&0xFF);
                wrd>>=8;
            }
        }

        for (i=r;i<len;i+=m)
        {
            wrd=x->w[n--];
            for (j=m-1;j>=0;j--)
            {
                ptr[start+i+j]=(char)(wrd&0xFF);
                wrd>>=8;
            }
        }
    }
    else
    {
        copy(x,mr_mip->w1);
        for (len=0;;len++)
        {
            if (mr_mip->ERNUM) break;

            if (size(mr_mip->w1)==0)
            {
                if (justify)
                {
                   if (len==max) break;
                }
                else break;
            }

            if (max>0 && len>=max)
            {
                mr_berror(_MIPP_ MR_ERR_TOO_BIG);
                MR_OUT
                return 0;
            }
            dig=(unsigned int)subdiv(_MIPP_ mr_mip->w1,256,mr_mip->w1);
            ch=MR_TOBYTE(dig);
            for (i=len;i>0;i--) ptr[i]=ptr[i-1];
            ptr[0]=MR_TOBYTE(ch);
        }
    }
    MR_OUT
    if (justify) return max;
    else         return len;
}

void bigdig(_MIPD_ int n,int b,big x)
{ /* generate random number n digits long *
   * to "printable" base b                */
    if (mr_mip->ERNUM) return;

    MR_IN(19)

    if (b<2 || b>256)
    {
        mr_berror(_MIPP_ MR_ERR_BASE_TOO_BIG);
        MR_OUT
        return;
    }

    do
    { /* repeat if x too small */
        expint(_MIPP_ b,n,mr_mip->w1);
        bigrand(_MIPP_ mr_mip->w1,x);
        subdiv(_MIPP_ mr_mip->w1,b,mr_mip->w1);
    } while (!mr_mip->ERNUM && mr_compare(x,mr_mip->w1)<0);

    MR_OUT
}

void mr_jsf(_MIPD_ big k0,big k1,big u0p,big u0m,big u1p,big u1m)
{
    int j,u0,u1,d0,d1,l0,l1;
    if (mr_mip->ERNUM) return;

    MR_IN(191)

    d0=d1=0;

    convert(_MIPP_ 1,mr_mip->w1);
    copy(k0,mr_mip->w2);
    copy(k1,mr_mip->w3);
    zero(u0p); zero(u0m); zero(u1p); zero(u1m);

    j=0;
    while (!mr_mip->ERNUM)
    {
        if (size(mr_mip->w2)==0 && d0==0 && size(mr_mip->w3)==0 && d1==0) break;
        l0=remain(_MIPP_ mr_mip->w2,8);
        l0=(l0+d0)&0x7;
        l1=remain(_MIPP_ mr_mip->w3,8);
        l1=(l1+d1)&0x7;

        if (l0%2==0) u0=0;
        else
        {
            u0=2-(l0%4);
            if ((l0==3 || l0==5) && l1%4==2) u0=-u0;
        }
        if (l1%2==0) u1=0;
        else
        {
            u1=2-(l1%4);
            if ((l1==3 || l1==5) && l0%4==2) u1=-u1;
        }
        if (mr_mip->base==mr_mip->base2)
        {
            if (u0>0) mr_addbit(_MIPP_ u0p,j);
            if (u0<0) mr_addbit(_MIPP_ u0m,j);
            if (u1>0) mr_addbit(_MIPP_ u1p,j);
            if (u1<0) mr_addbit(_MIPP_ u1m,j);
        }
        else
        {
            if (u0>0) add(_MIPP_ u0p,mr_mip->w1,u0p);
            if (u0<0) add(_MIPP_ u0m,mr_mip->w1,u0m);
            if (u1>0) add(_MIPP_ u1p,mr_mip->w1,u1p);
            if (u1<0) add(_MIPP_ u1m,mr_mip->w1,u1m);
        }

        if (d0+d0==1+u0) d0=1-d0;
        if (d1+d1==1+u1) d1=1-d1;

        subdiv(_MIPP_ mr_mip->w2,2,mr_mip->w2);
        subdiv(_MIPP_ mr_mip->w3,2,mr_mip->w3);

        if (mr_mip->base==mr_mip->base2)
            j++;
        else
            premult(_MIPP_ mr_mip->w1,2,mr_mip->w1);
    }
    MR_OUT
    return;
}

int epoint_get(_MIPD_ epoint* p,big x,big y)
{ /* Get point co-ordinates in affine, normal form       *
   * (converted from projective, Montgomery form)        *
   * if x==y, supplies x only. Return value is Least     *
   * Significant Bit of y (useful for point compression) */

    int lsb;
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif

    if (p->marker==MR_EPOINT_INFINITY)
    {
        zero(x);
        zero(y);
        return 0;
    }
    if (mr_mip->ERNUM) return 0;

    MR_IN(98)

    if (!epoint_norm(_MIPP_ p))
    { /* not possible ! */
        MR_OUT
        return (-1);
    }

    redc(_MIPP_ p->X,x);
    redc(_MIPP_ p->Y,mr_mip->w1);

    if (x!=y) copy(mr_mip->w1,y);
    lsb=remain(_MIPP_ mr_mip->w1,2);
    MR_OUT
    return lsb;
}

void strong_bigrand(_MIPD_ csprng *rng,big w,big x)
{
	int i, m;
	mr_small r;
    unsigned int ran;
    unsigned int ch;

    if (mr_mip->ERNUM) return;
    MR_IN(20)

	m = 0;
    zero(mr_mip->w1);

    do
    {
		m++;
		mr_mip->w1->len=m;
		for (r = 0, i = 0; i < sizeof(mr_small); i++) {
			ch=(unsigned char)strong_rng(rng);
			ran=ch;
			r = (r << 8) ^ ran;
		}
        if (mr_mip->base==0) mr_mip->w1->w[m-1]=r;
        else                 mr_mip->w1->w[m-1]=MR_REMAIN(r,mr_mip->base);
    } while (mr_compare(mr_mip->w1,w)<0);
	mr_lzero(mr_mip->w1);
    divide(_MIPP_ mr_mip->w1,w,w);

    copy(mr_mip->w1,x);
    MR_OUT
}

static mr_unsign32 sbrand(csprng *rng)
{ /* Marsaglia & Zaman random number generator */
    int i,k;
    mr_unsign32 pdiff,t;
    rng->rndptr++;
    if (rng->rndptr<NK) return rng->ira[rng->rndptr];
    rng->rndptr=0;
    for (i=0,k=NK-NJ;i<NK;i++,k++)
    { /* calculate next NK values */
        if (k==NK) k=0;
        t=rng->ira[k];
        pdiff=t - rng->ira[i] - rng->borrow;
        if (pdiff<t) rng->borrow=0;
        if (pdiff>t) rng->borrow=1;
        rng->ira[i]=pdiff;
    }
    return rng->ira[0];
}

epoint* epoint_init(_MIPDO_ )
{ /* initialise epoint to general point at infinity. */
    epoint *p;
    char *ptr;
    if (mr_mip->ERNUM) return NULL;

    MR_IN(96)

/* Create space for whole structure in one heap access */

    p=(epoint *)mr_alloc(_MIPP_ mr_esize(mr_mip->nib-1),1);

    ptr=(char *)p+sizeof(epoint);
    p->X=mirvar_mem(_MIPP_ ptr,0);
    p->Y=mirvar_mem(_MIPP_ ptr,1);
    p->Z=mirvar_mem(_MIPP_ ptr,2);
    p->marker=MR_EPOINT_INFINITY;

    MR_OUT

    return p;
}

void ecurve_multn(_MIPD_ int n,big *y,epoint **x,epoint *w)
{ /* pt=e[o]*p[0]+e[1]*p[1]+ .... e[n-1]*p[n-1]   */
    int i,j,k,m,nb,ea;
    epoint **G;
    if (mr_mip->ERNUM) return;

    MR_IN(114)

    m=1<<n;
    G=(epoint **)mr_alloc(_MIPP_ m,sizeof(epoint*));

    for (i=0,k=1;i<n;i++)
    {
        for (j=0; j < (1<<i) ;j++)
        {
            G[k]=epoint_init(_MIPPO_ );
            epoint_copy(x[i],G[k]);
            if (j!=0) ecurve_add(_MIPP_ G[j],G[k]);
            k++;
        }
    }

    nb=0;
    for (j=0;j<n;j++) if ((k=logb2(_MIPP_ y[j])) > nb) nb=k;

    epoint_set(_MIPP_ NULL,NULL,0,w);            /* w=0 */

    if (mr_mip->base==mr_mip->base2)
    {
        for (i=nb-1;i>=0;i--)
        {
            if (mr_mip->user!=NULL) (*mr_mip->user)();
            ea=0;
            k=1;
            for (j=0;j<n;j++)
            {
                if (mr_testbit(_MIPP_ y[j],i)) ea+=k;
                k<<=1;
            }
            ecurve_double(_MIPP_ w);
            if (ea!=0) ecurve_add(_MIPP_ G[ea],w);
        }
    }
    else mr_berror(_MIPP_ MR_ERR_NOT_SUPPORTED);

    for (i=1;i<m;i++) epoint_free(G[i]);
    mr_free(G);
    MR_OUT
}

void epoint_free(epoint *p)
{ /* clean up point */

    if (p==NULL) return;
    zero(p->X);
    zero(p->Y);
#ifndef MR_AFFINE_ONLY
    if (p->marker==MR_EPOINT_GENERAL) zero(p->Z);
#endif
    mr_free(p);
}

static void fill_pool(csprng *rng)
{ /* hash down output of RNG to re-fill the pool */
    int i;
    sha256 sh;
    shs256_init(&sh);
    for (i=0;i<128;i++) shs256_process(&sh,sbrand(rng));
    shs256_hash(&sh,rng->pool);
    rng->pool_ptr=0;
}

int strong_rng(csprng *rng)
{
    int r;
    r=rng->pool[rng->pool_ptr++];
    if (rng->pool_ptr>=MR_HASH_BYTES) fill_pool(rng);
    return r;
}

void strong_bigdig(_MIPD_ csprng *rng,int n,int b,big x)
{ /* generate random number n digits long *
   * to "printable" base b                */
    if (mr_mip->ERNUM) return;

    MR_IN(19)

    if (b<2 || b>256)
    {
        mr_berror(_MIPP_ MR_ERR_BASE_TOO_BIG);
        MR_OUT
        return;
    }

    do
    { /* repeat if x too small */
        expint(_MIPP_ b,n,mr_mip->w2);
        strong_bigrand(_MIPP_ rng,mr_mip->w2,x);
        subdiv(_MIPP_ mr_mip->w2,b,mr_mip->w2);
    } while (!mr_mip->ERNUM && mr_compare(x,mr_mip->w2)<0);

    MR_OUT
}

void ecurve_mult2(_MIPD_ big e,epoint *p,big ea,epoint *pa,epoint *pt)
{ /* pt=e*p+ea*pa; */
    int e1,h1,e2,h2,bb;
    epoint *p1,*p2,*ps[2];
    char *mem;

    if (mr_mip->ERNUM) return;

    MR_IN(103)

    if (size(e)==0)
    {
        ecurve_mult(_MIPP_ ea,pa,pt);
        MR_OUT
        return;
    }
    mem=(char *)ecp_memalloc(_MIPP_ 4);
    p2=epoint_init_mem(_MIPP_ mem,0);
    p1=epoint_init_mem(_MIPP_ mem,1);
    ps[0]=epoint_init_mem(_MIPP_ mem,2);
    ps[1]=epoint_init_mem(_MIPP_ mem,3);

    epoint_copy(pa,p2);
    copy(ea,mr_mip->w9);
    if (size(mr_mip->w9)<0)
    { /* p2 = -p2 */
        negify(mr_mip->w9,mr_mip->w9);
        epoint_negate(_MIPP_ p2);
    }

    epoint_copy(p,p1);
    copy(e,mr_mip->w12);
    if (size(mr_mip->w12)<0)
    { /* p1= -p1 */
        negify(mr_mip->w12,mr_mip->w12);
        epoint_negate(_MIPP_ p1);
    }

    epoint_set(_MIPP_ NULL,NULL,0,pt);            /* pt=0 */
    ecurve_add_sub(_MIPP_ p1,p2,ps[0],ps[1]);     /* ps[0]=p1+p2, ps[1]=p1-p2 */

    mr_jsf(_MIPP_ mr_mip->w9,mr_mip->w12,mr_mip->w10,mr_mip->w9,mr_mip->w13,mr_mip->w12);

/*    To use a simple NAF instead, substitute this for the JSF
        premult(_MIPP_ mr_mip->w9,3,mr_mip->w10);      3*ea
        premult(_MIPP_ mr_mip->w12,3,mr_mip->w13);     3*e
*/

    if (mr_mip->base==mr_mip->base2)
    {
        if (mr_compare(mr_mip->w10,mr_mip->w13)>=0) bb=logb2(_MIPP_ mr_mip->w10)-1;
        else                                        bb=logb2(_MIPP_ mr_mip->w13)-1;

        while (bb>=0) /* for the simple NAF, this should be 1 */
        {
            if (mr_mip->user!=NULL) (*mr_mip->user)();
            ecurve_double(_MIPP_ pt);

            e1=h1=e2=h2=0;
            if (mr_testbit(_MIPP_ mr_mip->w9,bb)) e2=1;
            if (mr_testbit(_MIPP_ mr_mip->w10,bb)) h2=1;
            if (mr_testbit(_MIPP_ mr_mip->w12,bb)) e1=1;
            if (mr_testbit(_MIPP_ mr_mip->w13,bb)) h1=1;

            if (e1!=h1)
            {
                if (e2==h2)
                {
                    if (h1==1) ecurve_add(_MIPP_ p1,pt);
                    else       ecurve_sub(_MIPP_ p1,pt);
                }
                else
                {
                    if (h1==1)
                    {
                        if (h2==1) ecurve_add(_MIPP_ ps[0],pt);
                        else       ecurve_add(_MIPP_ ps[1],pt);
                    }
                    else
                    {
                        if (h2==1) ecurve_sub(_MIPP_ ps[1],pt);
                        else       ecurve_sub(_MIPP_ ps[0],pt);
                    }
                }
            }
            else if (e2!=h2)
            {
                if (h2==1) ecurve_add(_MIPP_ p2,pt);
                else       ecurve_sub(_MIPP_ p2,pt);
            }
            bb-=1;
        }
    }
    else
    {
         if (mr_compare(mr_mip->w10,mr_mip->w13)>=0)
              expb2(_MIPP_ logb2(_MIPP_ mr_mip->w10)-1,mr_mip->w11);
         else expb2(_MIPP_ logb2(_MIPP_ mr_mip->w13)-1,mr_mip->w11);

        while (size(mr_mip->w11) > 0)    /* for the NAF, this should be 1 */
        { /* add/subtract method */
            if (mr_mip->user!=NULL) (*mr_mip->user)();

            ecurve_double(_MIPP_ pt);

            e1=h1=e2=h2=0;
            if (mr_compare(mr_mip->w9,mr_mip->w11)>=0)
            { /* e1(i)=1? */
                e2=1;
                mr_psub(_MIPP_ mr_mip->w9,mr_mip->w11,mr_mip->w9);
            }
            if (mr_compare(mr_mip->w10,mr_mip->w11)>=0)
            { /* h1(i)=1? */
                h2=1;
                mr_psub(_MIPP_ mr_mip->w10,mr_mip->w11,mr_mip->w10);
            }
            if (mr_compare(mr_mip->w12,mr_mip->w11)>=0)
            { /* e2(i)=1? */
                e1=1;
                mr_psub(_MIPP_ mr_mip->w12,mr_mip->w11,mr_mip->w12);
            }
            if (mr_compare(mr_mip->w13,mr_mip->w11)>=0)
            { /* h2(i)=1? */
                h1=1;
                mr_psub(_MIPP_ mr_mip->w13,mr_mip->w11,mr_mip->w13);
            }

            if (e1!=h1)
            {
                if (e2==h2)
                {
                    if (h1==1) ecurve_add(_MIPP_ p1,pt);
                    else       ecurve_sub(_MIPP_ p1,pt);
                }
                else
                {
                    if (h1==1)
                    {
                        if (h2==1) ecurve_add(_MIPP_ ps[0],pt);
                        else       ecurve_add(_MIPP_ ps[1],pt);
                    }
                    else
                    {
                        if (h2==1) ecurve_sub(_MIPP_ ps[1],pt);
                        else       ecurve_sub(_MIPP_ ps[0],pt);
                    }
                }
            }
            else if (e2!=h2)
            {
                if (h2==1) ecurve_add(_MIPP_ p2,pt);
                else       ecurve_sub(_MIPP_ p2,pt);
            }

            subdiv(_MIPP_ mr_mip->w11,2,mr_mip->w11);
        }
    }
    ecp_memkill(_MIPP_ mem,4);
    MR_OUT
}

mr_small muldiv(mr_small a,mr_small b,mr_small c,mr_small m,mr_small *rp)
{
    mr_small q;
    mr_large ldres,p=(mr_large)a*b+c;
    q=(mr_small)(MR_LROUND(p/m));
    *rp=(mr_small)(p-(mr_large)q*m);
    return q;
}


void sha3_init(sha3 *sh,int olen)
{
    int i,j;
    for (i=0;i<5;i++)
		for (j=0;j<5;j++)
			sh->S[i][j]=0;    /* 5x5x8 bytes = 200 bytes of state */
    sh->length=0;
	sh->len=olen;
	sh->rate=200-2*olen; /* number of bytes consumed in one gulp. Note that some bytes in the
	                        state ("capacity") are not touched. Gulps are smaller for larger digests.
							Important that olen<rate */
}

void sha3_process(sha3 *sh,int byte)
{
	int cnt=(int)(sh->length%sh->rate);
	int i,j,b=cnt%8;
	cnt/=8;
	i=cnt%5; j=cnt/5;  /* process by columns! */
	sh->S[i][j]^=((mr_unsign64)byte<<(8*b));
	sh->length++;
	if (sh->length%sh->rate==0) sha_transform(sh);
}

void sha3_hash(sha3 *sh,char *hash)
{ /* pad message and finish - supply digest */
	int i,j,k,m=0;
	mr_unsign64 el;
	int q=sh->rate-(sh->length%sh->rate);
	if (q==1) sha3_process(sh,0x86);
	else
	{
		sha3_process(sh,0x06);
		while (sh->length%sh->rate!=sh->rate-1) sha3_process(sh,0x00);
		sha3_process(sh,0x80); /* this will force a final transform */
	}

/* extract by columns - assuming here digest size < rate */
	for (j=0;j<5;j++)
	    for (i=0;i<5;i++)
		{
			el=sh->S[i][j];
			for (k=0;k<8;k++)
			{
				hash[m++]=(el&0xff);
				if (m>=sh->len) return;
				el>>=8;
			}
		}
}

static MR_WORD pack(const MR_BYTE *b)
{ /* pack bytes into a 32-bit Word */
    return ((MR_WORD)b[3]<<24)|((MR_WORD)b[2]<<16)|((MR_WORD)b[1]<<8)|(MR_WORD)b[0];
}

static void unpack(MR_WORD a,MR_BYTE *b)
{ /* unpack bytes from a word */
    b[0]=MR_TOBYTE(a);
    b[1]=MR_TOBYTE(a>>8);
    b[2]=MR_TOBYTE(a>>16);
    b[3]=MR_TOBYTE(a>>24);
}

static MR_BYTE bmul(MR_BYTE x,MR_BYTE y)
{ /* x.y= AntiLog(Log(x) + Log(y)) */
    if (x && y) return ptab[(ltab[x]+ltab[y])%255];
    else return 0;
}

static MR_WORD SubByte(MR_WORD a)
{
    MR_BYTE b[4];
    unpack(a,b);
    b[0]=fbsub[b[0]];
    b[1]=fbsub[b[1]];
    b[2]=fbsub[b[2]];
    b[3]=fbsub[b[3]];
    return pack(b);
}

static MR_BYTE product(MR_WORD x,MR_WORD y)
{ /* dot product of two 4-byte arrays */
    MR_BYTE xb[4],yb[4];
    unpack(x,xb);
    unpack(y,yb);
    return bmul(xb[0],yb[0])^bmul(xb[1],yb[1])^bmul(xb[2],yb[2])^bmul(xb[3],yb[3]);
}

static MR_WORD InvMixCol(MR_WORD x)
{ /* matrix Multiplication */
    MR_WORD y,m;
    MR_BYTE b[4];

    m=pack(InCo);
    b[3]=product(m,x);
    m=ROTL24(m);
    b[2]=product(m,x);
    m=ROTL24(m);
    b[1]=product(m,x);
    m=ROTL24(m);
    b[0]=product(m,x);
    y=pack(b);
    return y;
}

void aes_reset(aes *a,int mode,char *iv)
{ /* reset mode, or reset iv */
    int i;
    a->mode=mode;
    for (i=0;i<4*NB;i++)
        a->f[i]=0;
    if (mode!=MR_ECB && iv!=NULL)
    {
        for (i=0;i<4*NB;i++)
            a->f[i]=iv[i];
    }
}

void aes_getreg(aes *a,char *ir)
{
    int i;
    for (i=0;i<4*NB;i++) ir[i]=a->f[i];
}

BOOL aes_init(aes* a,int mode,int nk,char *key,char *iv)
{ /* Key=nk bytes */
  /* currently NB.nk = 16, 24 or 32          */
  /* Key Scheduler. Create expanded encryption key */
    int i,j,k,N,nr;
    MR_WORD CipherKey[8];

    nk/=4;

    if (nk!=4 && nk!=6 && nk!=8) return FALSE;

  /* nr is number of rounds */
    nr=6+nk;

    a->Nk=nk; a->Nr=nr;

    aes_reset(a,mode,iv);

    N=NB*(nr+1);

    for (i=j=0;i<nk;i++,j+=4)
    {
        CipherKey[i]=pack((MR_BYTE *)&key[j]);
    }
    for (i=0;i<nk;i++) a->fkey[i]=CipherKey[i];
    for (j=nk,k=0;j<N;j+=nk,k++)
    {
        a->fkey[j]=a->fkey[j-nk]^SubByte(ROTL24(a->fkey[j-1]))^rco[k];
        if (nk<=6)
        {
            for (i=1;i<nk && (i+j)<N;i++)
                a->fkey[i+j]=a->fkey[i+j-nk]^a->fkey[i+j-1];
        }
        else
        {
            for (i=1;i<4 && (i+j)<N;i++)
                a->fkey[i+j]=a->fkey[i+j-nk]^a->fkey[i+j-1];
            if ((j+4)<N) a->fkey[j+4]=a->fkey[j+4-nk]^SubByte(a->fkey[j+3]);
            for (i=5;i<nk && (i+j)<N;i++)
                a->fkey[i+j]=a->fkey[i+j-nk]^a->fkey[i+j-1];
        }
    }

 /* now for the expanded decrypt key in reverse order */

    for (j=0;j<NB;j++) a->rkey[j+N-NB]=a->fkey[j];
    for (i=NB;i<N-NB;i+=NB)
    {
        k=N-NB-i;
        for (j=0;j<NB;j++) a->rkey[k+j]=InvMixCol(a->fkey[i+j]);
    }
    for (j=N-NB;j<N;j++) a->rkey[j-N+NB]=a->fkey[j];

    return TRUE;
}

void aes_ecb_encrypt(aes *a,MR_BYTE *buff)
{
    int i,j,k;
    MR_WORD p[4],q[4],*x,*y,*t;

#ifdef AES_NI_SUPPORT
	__m128i ky,m = _mm_loadu_si128((__m128i *) buff);
	ky = _mm_loadu_si128((__m128i *) &a->fkey[0]);
    m = _mm_xor_si128       (m, ky);
	k=NB;
	for (i=1;i<a->Nr;i++)
	{
		ky=_mm_loadu_si128((__m128i *) &a->fkey[k]);
		m =_mm_aesenc_si128(m, ky);
		k+=4;
	}
	ky=_mm_loadu_si128((__m128i *) &a->fkey[k]);
    m=_mm_aesenclast_si128(m, ky);

    _mm_storeu_si128((__m128i *)buff, m);
#else

    for (i=j=0;i<NB;i++,j+=4)
    {
        p[i]=pack((MR_BYTE *)&buff[j]);
        p[i]^=a->fkey[i];
    }

    k=NB;
    x=p; y=q;

/* State alternates between x and y */
    for (i=1;i<a->Nr;i++)
    { /* Nr is number of rounds. May be odd. */
#ifndef MR_SMALL_AES
        y[0]=a->fkey[k]^ftable[MR_TOBYTE(x[0])]^
             ftable1[MR_TOBYTE(x[1]>>8)]^
             ftable2[MR_TOBYTE(x[2]>>16)]^
             ftable3[x[3]>>24];
        y[1]=a->fkey[k+1]^ftable[MR_TOBYTE(x[1])]^
             ftable1[MR_TOBYTE(x[2]>>8)]^
             ftable2[MR_TOBYTE(x[3]>>16)]^
             ftable3[x[0]>>24];
        y[2]=a->fkey[k+2]^ftable[MR_TOBYTE(x[2])]^
             ftable1[MR_TOBYTE(x[3]>>8)]^
             ftable2[MR_TOBYTE(x[0]>>16)]^
             ftable3[x[1]>>24];
        y[3]=a->fkey[k+3]^ftable[MR_TOBYTE(x[3])]^
             ftable1[MR_TOBYTE(x[0]>>8)]^
             ftable2[MR_TOBYTE(x[1]>>16)]^
             ftable3[x[2]>>24];
#else
        y[0]=a->fkey[k]^ftable[MR_TOBYTE(x[0])]^
             ROTL8(ftable[MR_TOBYTE(x[1]>>8)])^
             ROTL16(ftable[MR_TOBYTE(x[2]>>16)])^
             ROTL24(ftable[x[3]>>24]);
        y[1]=a->fkey[k+1]^ftable[MR_TOBYTE(x[1])]^
             ROTL8(ftable[MR_TOBYTE(x[2]>>8)])^
             ROTL16(ftable[MR_TOBYTE(x[3]>>16)])^
             ROTL24(ftable[x[0]>>24]);
        y[2]=a->fkey[k+2]^ftable[MR_TOBYTE(x[2])]^
             ROTL8(ftable[MR_TOBYTE(x[3]>>8)])^
             ROTL16(ftable[MR_TOBYTE(x[0]>>16)])^
             ROTL24(ftable[x[1]>>24]);
        y[3]=a->fkey[k+3]^ftable[MR_TOBYTE(x[3])]^
             ROTL8(ftable[MR_TOBYTE(x[0]>>8)])^
             ROTL16(ftable[MR_TOBYTE(x[1]>>16)])^
             ROTL24(ftable[x[2]>>24]);
#endif
        k+=4;
        t=x; x=y; y=t;      /* swap pointers */
    }

/* Last Round */

    y[0]=a->fkey[k]^(MR_WORD)fbsub[MR_TOBYTE(x[0])]^
         ROTL8((MR_WORD)fbsub[MR_TOBYTE(x[1]>>8)])^
         ROTL16((MR_WORD)fbsub[MR_TOBYTE(x[2]>>16)])^
         ROTL24((MR_WORD)fbsub[x[3]>>24]);
    y[1]=a->fkey[k+1]^(MR_WORD)fbsub[MR_TOBYTE(x[1])]^
         ROTL8((MR_WORD)fbsub[MR_TOBYTE(x[2]>>8)])^
         ROTL16((MR_WORD)fbsub[MR_TOBYTE(x[3]>>16)])^
         ROTL24((MR_WORD)fbsub[x[0]>>24]);
    y[2]=a->fkey[k+2]^(MR_WORD)fbsub[MR_TOBYTE(x[2])]^
         ROTL8((MR_WORD)fbsub[MR_TOBYTE(x[3]>>8)])^
         ROTL16((MR_WORD)fbsub[MR_TOBYTE(x[0]>>16)])^
         ROTL24((MR_WORD)fbsub[x[1]>>24]);
    y[3]=a->fkey[k+3]^(MR_WORD)fbsub[MR_TOBYTE(x[3])]^
         ROTL8((MR_WORD)fbsub[MR_TOBYTE(x[0]>>8)])^
         ROTL16((MR_WORD)fbsub[MR_TOBYTE(x[1]>>16)])^
         ROTL24((MR_WORD)fbsub[x[2]>>24]);

    for (i=j=0;i<NB;i++,j+=4)
    {
        unpack(y[i],(MR_BYTE *)&buff[j]);
        x[i]=y[i]=0;   /* clean up stack */
    }
#endif
}

void aes_ecb_decrypt(aes *a,MR_BYTE *buff)
{
    int i,j,k;
    MR_WORD p[4],q[4],*x,*y,*t;

#ifdef AES_NI_SUPPORT
	__m128i ky,m = _mm_loadu_si128((__m128i *) buff);
	ky = _mm_loadu_si128((__m128i *) &a->rkey[0]);
    m = _mm_xor_si128       (m, ky);
	k=NB;
	for (i=1;i<a->Nr;i++)
	{
		ky=_mm_loadu_si128((__m128i *) &a->rkey[k]);
		m =_mm_aesdec_si128    (m, ky);
		k+=4;
	}
	ky=_mm_loadu_si128((__m128i *) &a->rkey[k]);
    m=_mm_aesdeclast_si128(m, ky);

    _mm_storeu_si128((__m128i *)buff, m);
#else

    for (i=j=0;i<NB;i++,j+=4)
    {
        p[i]=pack((MR_BYTE *)&buff[j]);
        p[i]^=a->rkey[i];
    }

    k=NB;
    x=p; y=q;

/* State alternates between x and y */
    for (i=1;i<a->Nr;i++)
    { /* Nr is number of rounds. May be odd. */
#ifndef MR_SMALL_AES
        y[0]=a->rkey[k]^rtable[MR_TOBYTE(x[0])]^
             rtable1[MR_TOBYTE(x[3]>>8)]^
             rtable2[MR_TOBYTE(x[2]>>16)]^
             rtable3[x[1]>>24];
        y[1]=a->rkey[k+1]^rtable[MR_TOBYTE(x[1])]^
             rtable1[MR_TOBYTE(x[0]>>8)]^
             rtable2[MR_TOBYTE(x[3]>>16)]^
             rtable3[x[2]>>24];
        y[2]=a->rkey[k+2]^rtable[MR_TOBYTE(x[2])]^
             rtable1[MR_TOBYTE(x[1]>>8)]^
             rtable2[MR_TOBYTE(x[0]>>16)]^
             rtable3[x[3]>>24];
        y[3]=a->rkey[k+3]^rtable[MR_TOBYTE(x[3])]^
             rtable1[MR_TOBYTE(x[2]>>8)]^
             rtable2[MR_TOBYTE(x[1]>>16)]^
             rtable3[x[0]>>24];
#else
        y[0]=a->rkey[k]^rtable[MR_TOBYTE(x[0])]^
             ROTL8(rtable[MR_TOBYTE(x[3]>>8)])^
             ROTL16(rtable[MR_TOBYTE(x[2]>>16)])^
             ROTL24(rtable[x[1]>>24]);
        y[1]=a->rkey[k+1]^rtable[MR_TOBYTE(x[1])]^
             ROTL8(rtable[MR_TOBYTE(x[0]>>8)])^
             ROTL16(rtable[MR_TOBYTE(x[3]>>16)])^
             ROTL24(rtable[x[2]>>24]);
        y[2]=a->rkey[k+2]^rtable[MR_TOBYTE(x[2])]^
             ROTL8(rtable[MR_TOBYTE(x[1]>>8)])^
             ROTL16(rtable[MR_TOBYTE(x[0]>>16)])^
             ROTL24(rtable[x[3]>>24]);
        y[3]=a->rkey[k+3]^rtable[MR_TOBYTE(x[3])]^
             ROTL8(rtable[MR_TOBYTE(x[2]>>8)])^
             ROTL16(rtable[MR_TOBYTE(x[1]>>16)])^
             ROTL24(rtable[x[0]>>24]);
#endif
        k+=4;
        t=x; x=y; y=t;      /* swap pointers */
    }

/* Last Round */
    y[0]=a->rkey[k]^(MR_WORD)rbsub[MR_TOBYTE(x[0])]^
         ROTL8((MR_WORD)rbsub[MR_TOBYTE(x[3]>>8)])^
         ROTL16((MR_WORD)rbsub[MR_TOBYTE(x[2]>>16)])^
         ROTL24((MR_WORD)rbsub[x[1]>>24]);
    y[1]=a->rkey[k+1]^(MR_WORD)rbsub[MR_TOBYTE(x[1])]^
         ROTL8((MR_WORD)rbsub[MR_TOBYTE(x[0]>>8)])^
         ROTL16((MR_WORD)rbsub[MR_TOBYTE(x[3]>>16)])^
         ROTL24((MR_WORD)rbsub[x[2]>>24]);
    y[2]=a->rkey[k+2]^(MR_WORD)rbsub[MR_TOBYTE(x[2])]^
         ROTL8((MR_WORD)rbsub[MR_TOBYTE(x[1]>>8)])^
         ROTL16((MR_WORD)rbsub[MR_TOBYTE(x[0]>>16)])^
         ROTL24((MR_WORD)rbsub[x[3]>>24]);
    y[3]=a->rkey[k+3]^(MR_WORD)rbsub[MR_TOBYTE(x[3])]^
         ROTL8((MR_WORD)rbsub[MR_TOBYTE(x[2]>>8)])^
         ROTL16((MR_WORD)rbsub[MR_TOBYTE(x[1]>>16)])^
         ROTL24((MR_WORD)rbsub[x[0]>>24]);

    for (i=j=0;i<NB;i++,j+=4)
    {
        unpack(y[i],(MR_BYTE *)&buff[j]);
        x[i]=y[i]=0;   /* clean up stack */
    }
#endif
}

mr_unsign32 aes_encrypt(aes* a,char *buff)
{
    int j,bytes;
    char st[16];
    mr_unsign32 fell_off;

/* Supported Modes of Operation */

    fell_off=0;
    switch (a->mode)
    {
    case MR_ECB:
        aes_ecb_encrypt(a,(MR_BYTE *)buff);
        return 0;
    case MR_CBC:
        for (j=0;j<4*NB;j++) buff[j]^=a->f[j];
        aes_ecb_encrypt(a,(MR_BYTE *)buff);
        for (j=0;j<4*NB;j++) a->f[j]=buff[j];
        return 0;

    case MR_CFB1:
    case MR_CFB2:
    case MR_CFB4:
        bytes=a->mode-MR_CFB1+1;
        for (j=0;j<bytes;j++) fell_off=(fell_off<<8)|a->f[j];
        for (j=0;j<4*NB;j++) st[j]=a->f[j];
        for (j=bytes;j<4*NB;j++) a->f[j-bytes]=a->f[j];
        aes_ecb_encrypt(a,(MR_BYTE *)st);
        for (j=0;j<bytes;j++)
        {
            buff[j]^=st[j];
            a->f[16-bytes+j]=buff[j];
        }
        return fell_off;

    case MR_OFB1:
    case MR_OFB2:
    case MR_OFB4:
    case MR_OFB8:
    case MR_OFB16:

        bytes=a->mode-MR_OFB1+1;
        aes_ecb_encrypt(a,(MR_BYTE *)(a->f));
        for (j=0;j<bytes;j++) buff[j]^=a->f[j];
        return 0;

    case MR_PCFB1:   /* error propagating CFB */
    case MR_PCFB2:
    case MR_PCFB4:
        bytes=a->mode-MR_PCFB1+1;
        for (j=0;j<bytes;j++) fell_off=(fell_off<<8)|a->f[j];
        for (j=0;j<4*NB;j++) st[j]=a->f[j];
        for (j=bytes;j<4*NB;j++) a->f[j-bytes]=a->f[j];
        aes_ecb_encrypt(a,(MR_BYTE *)st);
        for (j=0;j<bytes;j++)
        {
            buff[j]^=st[j];
            a->f[16-bytes+j]=buff[j]^st[16-bytes+j];
        }
        return fell_off;

    default:
        return 0;
    }
}

mr_unsign32 aes_decrypt(aes *a,char *buff)
{
    int j,bytes;
    char st[16];
    mr_unsign32 fell_off;

   /* Supported modes of operation */
    fell_off=0;
    switch (a->mode)
    {
    case MR_ECB:
        aes_ecb_decrypt(a,(MR_BYTE *)buff);
        return 0;
    case MR_CBC:
        for (j=0;j<4*NB;j++)
        {
            st[j]=a->f[j];
            a->f[j]=buff[j];
        }
        aes_ecb_decrypt(a,(MR_BYTE *)buff);
        for (j=0;j<4*NB;j++)
        {
            buff[j]^=st[j];
            st[j]=0;
        }
        return 0;
    case MR_CFB1:
    case MR_CFB2:
    case MR_CFB4:
        bytes=a->mode-MR_CFB1+1;
        for (j=0;j<bytes;j++) fell_off=(fell_off<<8)|a->f[j];
        for (j=0;j<4*NB;j++) st[j]=a->f[j];
        for (j=bytes;j<4*NB;j++) a->f[j-bytes]=a->f[j];
        aes_ecb_encrypt(a,(MR_BYTE *)st);
        for (j=0;j<bytes;j++)
        {
            a->f[16-bytes+j]=buff[j];
            buff[j]^=st[j];
        }
        return fell_off;
    case MR_OFB1:
    case MR_OFB2:
    case MR_OFB4:
    case MR_OFB8:
    case MR_OFB16:
        bytes=a->mode-MR_OFB1+1;
        aes_ecb_encrypt(a,(MR_BYTE *)(a->f));
        for (j=0;j<bytes;j++) buff[j]^=a->f[j];
        return 0;

    case MR_PCFB1:   /* error propagating CFB */
    case MR_PCFB2:
    case MR_PCFB4:

        bytes=a->mode-MR_PCFB1+1;
        for (j=0;j<bytes;j++) fell_off=(fell_off<<8)|a->f[j];
        for (j=0;j<4*NB;j++) st[j]=a->f[j];
        for (j=bytes;j<4*NB;j++) a->f[j-bytes]=a->f[j];
        aes_ecb_encrypt(a,(MR_BYTE *)st);
        for (j=0;j<bytes;j++)
        {
            a->f[16-bytes+j]=buff[j]^st[16-bytes+j];
            buff[j]^=st[j];
        }
        return fell_off;
    default:
        return 0;
    }
}

void aes_end(aes *a)
{ /* clean up */
    int i;
    for (i=0;i<NB*(a->Nr+1);i++)
        a->fkey[i]=a->rkey[i]=0;
    for (i=0;i<4*NB;i++)
        a->f[i]=0;
}
