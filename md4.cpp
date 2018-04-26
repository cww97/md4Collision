#include <ctime>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long LL;
#define uint unsigned int
const uint N = 107;
const uint shift[5] = {3, 7, 11, 19};
inline uint f(const uint &x, const uint &y, const uint &z){
    return (x & y) | ((~x) & z);
}
inline uint g(const uint &x, const uint &y, const uint &z){
    return (x & y) | (x & z) | (y & z);
}
inline uint h(const uint &x, const uint &y, const uint &z){
    return x ^ y ^ z;
}

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
    return (a & (1 << (idx-1)));
}

uint chain[N];
const uint table6[23][5][13] ={
    {}, {}, {}, {}, // a0, d0, c0, b0
    // 0, 1, ==
    {{}, {}, {7}},  // a1
    {{7}, {}, {8, 11}},  // d1
    {{11}, {7, 8}, {26}},  // c1
    {{8, 11, 26}, {7}, {}},  // b1

    {{26}, {8, 11}, {14}},  // a2
    {{14}, {26}, {19,20,21,22}},  // d2
    {{14, 19, 20,22}, {21}, {13, 15}},  // c2
    {{15, 19, 20, 21, 22}, {13, 14}, {17}},  // b2

    {{17, 19, 20, 21}, {13, 14, 15, 22}, {23, 26}},  // a3
    {{17, 20, 23}, {13, 14, 15, 21, 22, 26}, {30}},  // d3
    {{20, 21, 22, 23, 26}, {17, 30}, {32}},          // c3
    {{20, 30, 32}, {21, 22, 26}, {23}},              // b3

    {{23, 26, 32}, {30}, {27, 29}},  // a4
    {{23, 26,30}, {27,29,32}, {}}, // d4
    {{27,29,30}, {23,26}, {19}}, // c4
    {{19,30}, {26,27,29}, {}}, // b4
};

const uint mOnPaper[17] = {
    0x4d7a9c83, 0x56cb927a, 0xb9d5a578, 0x57a7a5ee, 0xde748a3c, 0xdcc366b3, 0xb683a020, 0x3b2a5d9f,
    0xc69d71b3, 0xf9e99198, 0xd79f805e, 0xa63bb2e8, 0x45dd8e31, 0x97e31fe5, 0x2794bf08, 0xb9e8c3e9
};
//--------------------------------head-----------------------------------
inline void init(){
    srand((unsigned)time(NULL));
    //srand(465465436);
    chain[0] = 0x67452301;  // a0
    chain[1] = 0x10325476;  // d0
    chain[2] = 0x98badcfe;  // c0
    chain[3] = 0xefcdab89;  // b0
}

inline uint getM(const uint &idx){
    uint m = myrand();
    chain[idx] = cl(chain[idx-4] + f(chain[idx-1], chain[idx-2], chain[idx-3]) + m, shift[idx%4]);
    for (int i = 0; table6[idx][0][i] != 0; i++){
        chain[idx] ^= bit(chain[idx], table6[idx][0][i]);
    }
    for (int i = 0; table6[idx][1][i] != 0; i++){
        chain[idx] ^= bit(~chain[idx], table6[idx][1][i]);
    }
    for (int i = 0; table6[idx][2][i] != 0; i++){
        chain[idx] ^= bit(chain[idx], table6[idx][2][i]) ^ bit(chain[idx-1], table6[idx][2][i]);
    }
    m = cr(chain[idx], shift[idx%4]) - chain[idx-4] - f(chain[idx-1], chain[idx-2], chain[idx-3]);
    return m;
}

const uint round2idx[] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};
const uint round2shift[] = {3, 5, 9, 13};
const uint round3idx[] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
const uint round3shift[] = {3, 9, 11, 15};
vector <uint> md4(uint* mes){
    for (int i = 4; i < 20; i++){
        chain[i] = cl(chain[i-4] + f(chain[i-1], chain[i-2], chain[i-3]) + mes[i-4], shift[i%4]);
    }
    for (int i = 20; i < 36; i++){
        chain[i] = cl(chain[i-4] + g(chain[i-1], chain[i-2], chain[i-3]) + mes[round2idx[i-20]] + 0x5a827999, round2shift[i%4]);
    }
    for (int i = 36; i < 52; i++){
        chain[i] = cl(chain[i-4] + h(chain[i-1], chain[i-2], chain[i-3]) + mes[round3idx[i-36]] + 0x6ed9eba1, round3shift[i%4]);
    }
    vector<uint>ans;
    for (int i = 0; i < 4; i++){
        ans.push_back(chain[i] + chain[i+48]);
    }
    return ans;
}

bool checkMD4(vector <uint> md4_0, vector <uint> md4_1){
    for (int i = 0; i < 4; i++){
        //printf("md4[%d] = %08x ?= %08x\n", i, md4_0[i], md4_1[i]);
        if (md4_0[i] != md4_1[i]) return false;
    }
    return true;
}

/*
2f0a3578, 13461800, 86a37180, 97894480, e16f1480, 9d51f140, e694557a, 39ce2e7d,
6d5d3580, c99d9c40, 4ff41b20, 39997f7e, 5c261180, 28c21a80, f1804100, 088b92c0,
*/

int main(){
    init();
    uint mes0[N], mes1[N]; //mes0 for M  mes1 for M'
    time_t start_t = time(NULL);
    for (uint cnt = 0; 1;cnt++){
        for (int i = 0; i < 16; i++){
            mes0[i] = mes1[i] = getM(i+4);  // mOnPaper[i];
        }
        mes1[1] += (1<<31);  mes1[2] += (1<<31);  // get M'
        mes1[2] -= (1<<28); mes1[12] -= (1<<16);
        vector <uint> md4_0 = md4(mes0);
        vector <uint> md4_1 = md4(mes1);

        if (checkMD4(md4_0, md4_1)) {
            for (int i = 0; i < 16; i++) printf("%08x, ", mes0[i]);
            puts("");
            break;
        }

        if (cnt % 1000000 == 0) {
            time_t cur_t = time(NULL);
            printf("cnt = %d, time used: %ld s\n", cnt, cur_t - start_t);
        }
    }
    return 0;
}
