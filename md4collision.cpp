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



int main(){
    init();
    uint mes0[N], mes1[N];
    for (uint cnt = 3; cnt--;){
        int top = -1;
        mes0[++top] = mes1[top] = get_m0();
        mes0[++top] = mes1[top] = get_m1(); mes1[top] += (1<<31);
        mes0[++top] = mes1[top] = get_m2(); mes1[top] += (1<<31) - (1<<28);
        mes0[++top] = mes1[top] = get_m3();
        //mes0[++top] = get_m4();

        top++;
        for (int i = 0; i < top; i++){
        	printf("i = %d m = %08x, m' = %08x\n", i, mes0[i], mes1[i]);
        }
        puts("-----------------");
    }
    return 0;
}
