#include <ctime>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long LL;
#define uint unsigned int
const uint N = 22;
const uint shift[] = {3, 7, 11, 19};
inline uint f(const uint &x, const uint &y, const uint &z){return (x & y) | ((!x) & z);}
inline uint g(const uint &x, const uint &y, const uint &z){return (x & y) | (x & z) | (y & z);}
inline uint h(const uint &x, const uint &y, const uint &z){return x ^ y ^ z;}

#define LEN 32
inline uint cl(const uint &a, const uint &x){ // circle left
    return (a>>(LEN-x)) | (a<<x);
}
inline uint cr(const uint &a, const uint &x){ // circle right
    return (a<<(LEN-x)) | (a>>x);
}
inline uint myrand(){
	return cl(rand(), 7) ^ cl(rand(), 17);
}
inline uint bit(const uint &a, const uint &idx){
    return (a & (1 << (idx-1))) >> (idx-1);
}
inline uint xbit(const uint &a, const uint &idx){
    return (a & (1 << (idx-1)));
}


struct Node{
    uint a, b, c, d;
    Node(){}
    Node(uint _a, uint _b, uint _c, uint _d):a(_a), b(_b), c(_c), d(_d){}
} chains[N], _chains[N];
#define a(x) chains[x].a
#define b(x) chains[x].b
#define c(x) chains[x].c
#define d(x) chains[x].d
#define _a(x) _chains[x].a
#define _b(x) _chains[x].b
#define _c(x) _chains[x].c
#define _d(x) _chains[x].d
//--------------------------------head-----------------------------------

inline void init(){
    srand((unsigned)time(NULL));
    chains[0] = Node(0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476);
}

inline uint get_m0(){
    uint m0 = myrand();
    a(1) = cl( a(0) + f(b(0), c(0), d(0)) + m0, shift[0]);
    //printf("    m0 = %08x, a1 = %08x\n", m0, a(1));
    uint a_1_7 = bit(a(1), 7);
    uint b_0_7 = bit(b(0), 7);
    if (a_1_7 != b_0_7){
        a(1) ^= cl((a_1_7 ^ b_0_7), 6);
        m0 = cr(a(1), shift[0]) - a(0) - f(b(0), c(0), d(0));
    }
    //printf("--> m0 = %08x, a1 = %08x, %d == %d\n", m0, a(1), bit(a(1), 7), bit(b(0), 7));
    return m0;
}

inline uint get_m1(){
    uint m = myrand();
    d(1) = cl( d(0) + f(a(1), b(0), c(0)) + m, shift[1]);
    uint d1_7 = bit(d(1), 7);
    uint d1_8 = bit(d(1), 8), a1_8 = bit(a(1), 8);
    uint d1_11 = bit(d(1), 11), a1_11 = bit(a(1), 11);
    // change
    d(1) ^= (d1_7 << 6);
    d(1) ^= cl((d1_8 ^ a1_8), 7);
    d(1) ^= cl((d1_11 ^ a1_11), 10);
    m = cr(d(1), shift[1]) - d(0) - f(a(1), b(0), c(0));
    return m1;
}

inline uint get_m2(){
    uint m2 = myrand();
    c(1) = cl(c(0) + f(d(1), a(1), b(0)) + m2, shift[2]);
    //printf("    m2 = %x, c1 = %x\n", m2, c(1));
    uint c1_7 = bit(c(1), 7);
    uint c1_8 = bit(c(1), 8);
    uint c1_11 = bit(c(1), 11);
    uint c1_26 = bit(c(1), 26), d1_26 = bit(d(1), 26);

    c(1) ^= ((!c1_7) << 6);
    c(1) ^= ((!c1_8) << 7);
    c(1) ^= (c1_11 << 10);
    c(1) ^= cl((c1_26 ^ d1_26), 10);
    m2 = cr(c(1), shift[2]) - c(0) - f(d(1), a(1), b(0));
    //printf("--> m1 = %x, d1 = %x\n", m1, d(1));
    return m2;
}

inline uint get_m3(){
    uint m = myrand();
    b(1) = cl(b(0) + f(c(1), d(1), a(1)) + m, shift[3]);
    //printf("    m2 = %x, c1 = %x\n", m2, c(1));
    uint b1_7 = bit(b(1), 7);
    uint b1_8 = bit(b(1), 8);
    uint b1_11 = bit(b(1), 11);
    uint b1_26 = bit(b(1), 26);

    // b1_7
    b(1) ^= ((!b1_7) << 6); // b1_7 = 1
    b(1) ^= (b1_8 << 7);    // b1_8 = 0
    b(1) ^= (b1_11 << 10);  // b1_11 = 0
    b(1) ^= (b1_11 << 10);  // b1_26 = 0
    m = cr(b(1), shift[3]) - (b(0) + f(c(1), d(1), a(1)));
    //printf("--> m1 = %x, d1 = %x\n", m1, d(1));
    return m;
}

//------------------------------------------

inline uint get_m10(){
	uint m = myrand();
    c(3) = cl(c(2) + f(d(3), a(3), b(2)) + m, shift[2]);
    //printf("    m2 = %x, c1 = %x\n", m2, c(1));
    uint c3_17 = bit(c(3), 17);
    uint c3_20 = bit(c(3), 20);
    uint c3_21 = bit(c(3), 21);
    uint c3_22 = bit(c(3), 22);
    uint c3_23 = bit(c(3), 23);
    uint c3_26 = bit(c(3), 26);
    uint c3_30 = bit(c(3), 30);
    uint c3_32 = bit(c(3), 32);uint d3_32 = bit(d(3), 32);
    //
    c(3) ^= ((!c3_17) << 16); // c3_17 = 1
    c(3) ^= (c3_20 << 19);    // b3_20 = 0
    c(3) ^= (c3_21 << 20);  // c3_21 = 0
    c(3) ^= (c3_22 << 21);  // c3_22 = 0
    c(3) ^= (c3_23 << 22);  // c3_23 = 0
    c(3) ^= (c3_26 << 25);  // c3_26 = 0
    c(3) ^= ((!c3_30) << 29);  // c3_30 = 1
    c(3) ^= ((c3_32 ^ d3_32) << 31);  // c3_32 = d3_32
    m = cr(c(3), shift[2]) - (c(2) + f(d(3), a(3), b(2)));
    //printf("--> m1 = %x, d1 = %x\n", m1, d(1));
    return m;
} 

inline uint get_m11(){
	uint m = myrand();
    b(3) = cl(b(2) + f(c(3), d(3), a(3)) + m, shift[3]);
    //printf("    m2 = %x, c1 = %x\n", m2, c(1));
    uint b3_20 = bit(b(3), 20);
    uint b3_21 = bit(b(3), 21);
    uint b3_22 = bit(b(3), 22);
    uint b3_23 = bit(b(3), 23); uint c3_23 = bit(c(3), 23);
    uint b3_26 = bit(b(3), 26);
    uint b3_30 = bit(b(3), 30);
    uint b3_32 = bit(b(3), 32);
    //

    b(3) ^= (b3_20 << 19);    // b3_20 = 0
    b(3) ^= ((!b3_21) << 20);  // b3_21 = 1
    b(3) ^= ((!b3_22) << 21); // b3_22 = 1
    b(3) ^= ((b3_23 ^ c3_23 ) << 22);  // b3_23 = c3_23
    b(3) ^= ((!c3_26) << 25);  // b3_26 = 1
    b(3) ^= (b3_30 << 29);  // b3_30 = 0
    b(3) ^= (b3_32 << 31);  // b3_32 = 0
    m = cr(b(3), shift[3]) - (b(2) + f(c(3), d(3), a(3)));
    //printf("--> m1 = %x, d1 = %x\n", m1, d(1));
    return m;
} 

inline uint get_m12(){
	uint m = myrand();
    a(4) = cl(a(3) + f(b(3), c(3), d(3)) + m, shift[0]);
    //printf("    m2 = %x, c1 = %x\n", m2, c(1));
    uint a4_23 = bit(a(4), 23);
    uint a4_26 = bit(a(4), 26);
    uint a4_27 = bit(a(4), 27); uint b3_27 = bit(b(3), 27);
    uint a4_29 = bit(a(4), 29); uint b3_29 = bit(b(3), 29);
    uint a4_30 = bit(a(4), 30);
    uint a4_32 = bit(a(4), 32);
    //

    a(4) ^= (a4_23 << 22);    // a4_23 = 0
    a(4) ^= (a4_26 << 25);  // a4_26 = 0
    a(4) ^= ((a4_27 ^ b3_27) << 26); // a4_27 = b3_27
    a(4) ^= ((a4_29 ^ b3_29) << 28);  // a4_29 = b3_29
    a(4) ^= ((!a4_30) << 29);  // a4_30 = 1
    a(4) ^= (a4_32 << 31);  // a4_32 = 0
    m = cr(a(4), shift[0]) - (a(3) + f(b(3), c(3), d(3)));
    //printf("--> m1 = %x, d1 = %x\n", m1, d(1));
    return m;
} 

inline uint get_m13(){
	uint m = myrand();
    d(4) = cl(d(3) + f(a(4), b(3), c(3)) + m, shift[1]);
    //printf("    m2 = %x, c1 = %x\n", m2, c(1));

    d(4) ^= xbit(d(4), 23);    // d4_23 = 0
    d(4) ^= xbit(d(4), 26);  // d4_26 = 0
    d(4) ^= xbit(~d(4), 27); // d4_27 = 1
    d(4) ^= xbit(~d(4), 29);  // d4_29 = 1
    d(4) ^= xbit(d(4), 30);  // d4_30 = 0
    d(4) ^= xbit(~d(4), 32);  // d4_32 = 1
    m = cr(d(4), shift[1]) - (d(3) + f(a(4), b(3), c(3));
    //printf("--> m1 = %x, d1 = %x\n", m1, d(1));
    return m;
}

inline uint get_m14(){
	uint m = myrand();
    c(4) = cl(c(3) + f(d(4), a(4), b(3)) + m, shift[2]);
    //printf("    m2 = %x, c1 = %x\n", m2, c(1));

    c(4) ^= xbit(c(4), 19) ^ xbit(d(4), 19);    // c4_19 = d4_19
    c(4) ^= xbit(~c(4), 23);  // c4_23 = 1
    c(4) ^= xbit(~c(4), 26);  // c4_26 = 1
    c(4) ^= xbit(c(4), 27); // c4_27 = 0
    c(4) ^= xbit(c(4), 29);  // c4_29 = 0
    c(4) ^= xbit(c(4), 30);  // c4_30 = 0

    m = cr(c(4), shift[2]) - (c(3) + f(d(4), a(4), b(3));
    //printf("--> m1 = %x, d1 = %x\n", m1, d(1));
    return m;
}

inline uint get_m15(){
	uint m = myrand();
    b(4) = cl(b(3) + f(c(4), d(4), a(4)) + m, shift[3]);
    //printf("    m2 = %x, c1 = %x\n", m2, c(1));

    b(4) ^= xbit(b(4), 19);    // b4_19 = 0
    b(4) ^= xbit(~b(4), 26);  // b4_26 = c4_26 =  1
    b(4) ^= xbit(~b(4), 27); // b4_27 = 1
    b(4) ^= xbit(~b(4), 29);  // b4_29 = 1
    b(4) ^= xbit(b(4), 30);  // b4_30 = 0

    m = cr(b(4), shift[3]) - (b(3) + f(c(4), d(4), a(4));
    //printf("--> m1 = %x, d1 = %x\n", m1, d(1));
    return m;
}


int main(){
    init();
    uint mes0[N], mes1[N];//mes0 for M'  mes1 for M
    for (uint cnt = 3; cnt--;){
        int top = -1;
        mes0[++top] = mes1[top] = get_m0();
        mes0[++top] = mes1[top] = get_m1(); mes1[top] += (1<<31);
        mes0[++top] = mes1[top] = get_m2(); mes1[top] += (1<<31) - (1<<28);
        mes0[++top] = mes1[top] = get_m3(); 
        
        
        mes0[++top] = mes1[top] = get_m10(); 
        mes0[++top] = mes1[top] = get_m11();
        mes0[++top] = mes1[top] = get_m12(); mes1[top] -= (1<<16);
        mes0[++top] = mes1[top] = get_m13();
        mes0[++top] = mes1[top] = get_m14();
        mes0[++top] = mes1[top] = get_m15();
        //mes0[++top] = get_m4();

        top++;
        for (int i = 0; i < top; i++){
        	printf("i = %d m = %08x, m' = %08x\n", i, mes0[i], mes1[i]);
        }
        puts("-----------------");
    }
    return 0;
}
