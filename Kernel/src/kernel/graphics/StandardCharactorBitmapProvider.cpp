#include "graphics/StandardCharactorBitmapProvider.h"

static const uint8_t char_bitmap_space[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
};

static const uint8_t char_bitmap_0[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00111100,
    0b01100110,
    0b01101110,
    0b01110110,
    0b01100110,
    0b01100110,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_1[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00011000,
    0b00111000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_2[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00111100,
    0b01100110,
    0b00000110,
    0b00001100,
    0b00011000,
    0b00110000,
    0b01111110,
    0b00000000,
};

static const uint8_t char_bitmap_3[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00111100,
    0b01100110,
    0b00000110,
    0b00011100,
    0b00000110,
    0b01100110,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_4[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00001100,
    0b00011100,
    0b00101100,
    0b01001100,
    0b01111110,
    0b00001100,
    0b00011110,
    0b00000000,
};

static const uint8_t char_bitmap_5[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01111110,
    0b01100000,
    0b01111100,
    0b00000110,
    0b00000110,
    0b01100110,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_6[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00111100,
    0b01100110,
    0b01100000,
    0b01111100,
    0b01100110,
    0b01100110,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_7[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01111110,
    0b00000110,
    0b00001100,
    0b00011000,
    0b00110000,
    0b00110000,
    0b00110000,
    0b00000000,
};

static const uint8_t char_bitmap_8[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00111100,
    0b01100110,
    0b01100110,
    0b00111100,
    0b01100110,
    0b01100110,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_9[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00111100,
    0b01100110,
    0b01100110,
    0b00111110,
    0b00000110,
    0b01100110,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_A[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00011000,
    0b00111100,
    0b01100110,
    0b01100110,
    0b01111110,
    0b01100110,
    0b01100110,
    0b00000000,
};

static const uint8_t char_bitmap_B[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01111100,
    0b01100110,
    0b01100110,
    0b01111100,
    0b01100110,
    0b01100110,
    0b01111100,
    0b00000000,
};

static const uint8_t char_bitmap_C[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00111100,
    0b01100110,
    0b01100000,
    0b01100000,
    0b01100000,
    0b01100110,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_D[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01111000,
    0b01101100,
    0b01100110,
    0b01100110,
    0b01100110,
    0b01101100,
    0b01111000,
    0b00000000,
};

static const uint8_t char_bitmap_E[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01111110,
    0b01100000,
    0b01100000,
    0b01111100,
    0b01100000,
    0b01100000,
    0b01111110,
    0b00000000,
};

static const uint8_t char_bitmap_F[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01111110,
    0b01100000,
    0b01100000,
    0b01111100,
    0b01100000,
    0b01100000,
    0b01100000,
    0b00000000,
};

static const uint8_t char_bitmap_G[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00111100,
    0b01100110,
    0b01100000,
    0b01101110,
    0b01100110,
    0b01100110,
    0b00111110,
    0b00000000,
};

static const uint8_t char_bitmap_H[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01100110,
    0b01100110,
    0b01100110,
    0b01111110,
    0b01100110,
    0b01100110,
    0b01100110,
    0b00000000,
};

static const uint8_t char_bitmap_I[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01111110,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b01111110,
    0b00000000,
};

static const uint8_t char_bitmap_J[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00011110,
    0b00001100,
    0b00001100,
    0b00001100,
    0b00001100,
    0b01101100,
    0b00111000,
    0b00000000,
};

static const uint8_t char_bitmap_K[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01100110,
    0b01101100,
    0b01111000,
    0b01110000,
    0b01111000,
    0b01101100,
    0b01100110,
    0b00000000,
};

static const uint8_t char_bitmap_L[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01100000,
    0b01100000,
    0b01100000,
    0b01100000,
    0b01100000,
    0b01100000,
    0b01111110,
    0b00000000,
};

static const uint8_t char_bitmap_M[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01100011,
    0b01110111,
    0b01111111,
    0b01101011,
    0b01100011,
    0b01100011,
    0b01100011,
    0b00000000,
};

static const uint8_t char_bitmap_N[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01100110,
    0b01110110,
    0b01111110,
    0b01111110,
    0b01101110,
    0b01100110,
    0b01100110,
    0b00000000,
};

static const uint8_t char_bitmap_O[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00111100,
    0b01100110,
    0b01100110,
    0b01100110,
    0b01100110,
    0b01100110,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_P[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01111100,
    0b01100110,
    0b01100110,
    0b01111100,
    0b01100000,
    0b01100000,
    0b01100000,
    0b00000000,
};

static const uint8_t char_bitmap_Q[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00111100,
    0b01100110,
    0b01100110,
    0b01100110,
    0b01101110,
    0b00111100,
    0b00001110,
    0b00000000,
};

static const uint8_t char_bitmap_R[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01111100,
    0b01100110,
    0b01100110,
    0b01111100,
    0b01111000,
    0b01101100,
    0b01100110,
    0b00000000,
};

static const uint8_t char_bitmap_S[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00111100,
    0b01100110,
    0b01100000,
    0b00111100,
    0b00000110,
    0b01100110,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_T[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01111110,
    0b01011010,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_U[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01100110,
    0b01100110,
    0b01100110,
    0b01100110,
    0b01100110,
    0b01100110,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_V[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01100110,
    0b01100110,
    0b01100110,
    0b01100110,
    0b01100110,
    0b00111100,
    0b00011000,
    0b00000000,
};

static const uint8_t char_bitmap_W[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01100011,
    0b01100011,
    0b01100011,
    0b01101011,
    0b01111111,
    0b01110111,
    0b01100011,
    0b00000000,
};

static const uint8_t char_bitmap_X[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01100110,
    0b01100110,
    0b00111100,
    0b00011000,
    0b00111100,
    0b01100110,
    0b01100110,
    0b00000000,
};

static const uint8_t char_bitmap_Y[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01100110,
    0b01100110,
    0b01100110,
    0b00111100,
    0b00011000,
    0b00011000,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_Z[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01111110,
    0b00000110,
    0b00001100,
    0b00011000,
    0b00110000,
    0b01100000,
    0b01111110,
    0b00000000,
};

static const uint8_t char_bitmap_a[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00111100,
    0b00000110,
    0b00111110,
    0b01100110,
    0b00111110,
    0b00000000,
};

static const uint8_t char_bitmap_b[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01100000,
    0b01100000,
    0b01111100,
    0b01100110,
    0b01100110,
    0b01100110,
    0b01111100,
    0b00000000,
};

static const uint8_t char_bitmap_c[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00111100,
    0b01100110,
    0b01100000,
    0b01100110,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_d[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000110,
    0b00000110,
    0b00111110,
    0b01100110,
    0b01100110,
    0b01100110,
    0b00111110,
    0b00000000,
};

static const uint8_t char_bitmap_e[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00111100,
    0b01100110,
    0b01111110,
    0b01100000,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_f[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00001110,
    0b00011011,
    0b00011000,
    0b00111100,
    0b00011000,
    0b00011000,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_g[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00111110,
    0b01100110,
    0b01100110,
    0b00111110,
    0b00000110,
    0b01111100,
};

static const uint8_t char_bitmap_h[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01100000,
    0b01100000,
    0b01111100,
    0b01100110,
    0b01100110,
    0b01100110,
    0b01100110,
    0b00000000,
};

static const uint8_t char_bitmap_i[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00011000,
    0b00000000,
    0b00111000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_j[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000110,
    0b00000000,
    0b00000110,
    0b00000110,
    0b00000110,
    0b01100110,
    0b01100110,
    0b00111100,
};

static const uint8_t char_bitmap_k[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01100000,
    0b01100000,
    0b01100110,
    0b01101100,
    0b01111000,
    0b01101100,
    0b01100110,
    0b00000000,
};

static const uint8_t char_bitmap_l[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00111000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_m[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b01101100,
    0b01111110,
    0b01111110,
    0b01101100,
    0b01101100,
    0b00000000,
};

static const uint8_t char_bitmap_n[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b01111100,
    0b01100110,
    0b01100110,
    0b01100110,
    0b01100110,
    0b00000000,
};

static const uint8_t char_bitmap_o[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00111100,
    0b01100110,
    0b01100110,
    0b01100110,
    0b00111100,
    0b00000000,
};

static const uint8_t char_bitmap_p[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b01111100,
    0b01100110,
    0b01100110,
    0b01111100,
    0b01100000,
    0b01100000,
};

static const uint8_t char_bitmap_q[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00111110,
    0b01100110,
    0b01100110,
    0b00111110,
    0b00000110,
    0b00000110,
};

static const uint8_t char_bitmap_r[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b01101100,
    0b01110110,
    0b01100000,
    0b01100000,
    0b01100000,
    0b00000000,
};

static const uint8_t char_bitmap_s[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00111110,
    0b01100000,
    0b00111100,
    0b00000110,
    0b01111100,
    0b00000000,
};

static const uint8_t char_bitmap_t[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00001000,
    0b00011000,
    0b01111110,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00001110,
    0b00000000,
};

static const uint8_t char_bitmap_u[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b01100110,
    0b01100110,
    0b01100110,
    0b01100110,
    0b00111110,
    0b00000000,
};

static const uint8_t char_bitmap_v[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b01100110,
    0b01100110,
    0b01100110,
    0b00111100,
    0b00011000,
    0b00000000,
};

static const uint8_t char_bitmap_w[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b01100011,
    0b01101011,
    0b01111111,
    0b00110110,
    0b00110110,
    0b00000000,
};

static const uint8_t char_bitmap_x[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b01100110,
    0b00111100,
    0b00011000,
    0b00111100,
    0b01100110,
    0b00000000,
};

static const uint8_t char_bitmap_y[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b01100110,
    0b01100110,
    0b01100110,
    0b00111110,
    0b00000110,
    0b00111100,
};

static const uint8_t char_bitmap_z[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b01111110,
    0b00001100,
    0b00011000,
    0b00110000,
    0b01111110,
    0b00000000,
};

static const uint8_t char_bitmap_dot[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00000000,
};

static const uint8_t char_bitmap_comma[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00110000,
};

static const uint8_t char_bitmap_colon[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00000000,
};

static const uint8_t char_bitmap_semicolon[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00110000,
};

static const uint8_t char_bitmap_exclamation[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00011000,
    0b00000000,
};

static const uint8_t char_bitmap_question[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00111100,
    0b01100110,
    0b00000110,
    0b00001100,
    0b00011000,
    0b00000000,
    0b00011000,
    0b00000000,
};

static const uint8_t char_bitmap_hyphen[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111110,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
};

static const uint8_t char_bitmap_underscore[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b11111111,
};

static const uint8_t char_bitmap_plus[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00011000,
    0b00011000,
    0b00111110,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00000000,
};

static const uint8_t char_bitmap_equal[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00111110,
    0b00000000,
    0b00111110,
    0b00000000,
    0b00000000,
    0b00000000,
};

static const uint8_t char_bitmap_slash[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00000110,
    0b00000110,
    0b00001100,
    0b00011000,
    0b00110000,
    0b01100000,
    0b01100000,
    0b00000000,
};

static const uint8_t char_bitmap_backslash[CHARACTOR_BITMAP_HEIGHT] =
{
    0b01100000,
    0b01100000,
    0b00110000,
    0b00011000,
    0b00001100,
    0b00000110,
    0b00000110,
    0b00000000,
};

static const uint8_t char_bitmap_parenLeft[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00001100,
    0b00011000,
    0b00110000,
    0b00110000,
    0b00110000,
    0b00011000,
    0b00001100,
    0b00000000,
};

static const uint8_t char_bitmap_parenRight[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00110000,
    0b00011000,
    0b00001100,
    0b00001100,
    0b00001100,
    0b00011000,
    0b00110000,
    0b00000000,
};

static const uint8_t* char_bitmap_table[128] = { nullptr };

static void init_table()
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    char_bitmap_table['0'] = char_bitmap_0;
    char_bitmap_table['1'] = char_bitmap_1;
    char_bitmap_table['2'] = char_bitmap_2;
    char_bitmap_table['3'] = char_bitmap_3;
    char_bitmap_table['4'] = char_bitmap_4;
    char_bitmap_table['5'] = char_bitmap_5;
    char_bitmap_table['6'] = char_bitmap_6;
    char_bitmap_table['7'] = char_bitmap_7;
    char_bitmap_table['8'] = char_bitmap_8;
    char_bitmap_table['9'] = char_bitmap_9;
    char_bitmap_table['A'] = char_bitmap_A;
    char_bitmap_table['B'] = char_bitmap_B;
    char_bitmap_table['C'] = char_bitmap_C;
    char_bitmap_table['D'] = char_bitmap_D;
    char_bitmap_table['E'] = char_bitmap_E;
    char_bitmap_table['F'] = char_bitmap_F;
    char_bitmap_table['G'] = char_bitmap_G;
    char_bitmap_table['H'] = char_bitmap_H;
    char_bitmap_table['I'] = char_bitmap_I;
    char_bitmap_table['J'] = char_bitmap_J;
    char_bitmap_table['K'] = char_bitmap_K;
    char_bitmap_table['L'] = char_bitmap_L;
    char_bitmap_table['M'] = char_bitmap_M;
    char_bitmap_table['N'] = char_bitmap_N;
    char_bitmap_table['O'] = char_bitmap_O;
    char_bitmap_table['P'] = char_bitmap_P;
    char_bitmap_table['Q'] = char_bitmap_Q;
    char_bitmap_table['R'] = char_bitmap_R;
    char_bitmap_table['S'] = char_bitmap_S;
    char_bitmap_table['T'] = char_bitmap_T;
    char_bitmap_table['U'] = char_bitmap_U;
    char_bitmap_table['V'] = char_bitmap_V;
    char_bitmap_table['W'] = char_bitmap_W;
    char_bitmap_table['X'] = char_bitmap_X;
    char_bitmap_table['Y'] = char_bitmap_Y;
    char_bitmap_table['Z'] = char_bitmap_Z;
    char_bitmap_table['a'] = char_bitmap_a;
    char_bitmap_table['b'] = char_bitmap_b;
    char_bitmap_table['c'] = char_bitmap_c;
    char_bitmap_table['d'] = char_bitmap_d;
    char_bitmap_table['e'] = char_bitmap_e;
    char_bitmap_table['f'] = char_bitmap_f;
    char_bitmap_table['g'] = char_bitmap_g;
    char_bitmap_table['h'] = char_bitmap_h;
    char_bitmap_table['i'] = char_bitmap_i;
    char_bitmap_table['j'] = char_bitmap_j;
    char_bitmap_table['k'] = char_bitmap_k;
    char_bitmap_table['l'] = char_bitmap_l;
    char_bitmap_table['m'] = char_bitmap_m;
    char_bitmap_table['n'] = char_bitmap_n;
    char_bitmap_table['o'] = char_bitmap_o;
    char_bitmap_table['p'] = char_bitmap_p;
    char_bitmap_table['q'] = char_bitmap_q;
    char_bitmap_table['r'] = char_bitmap_r;
    char_bitmap_table['s'] = char_bitmap_s;
    char_bitmap_table['t'] = char_bitmap_t;
    char_bitmap_table['u'] = char_bitmap_u;
    char_bitmap_table['v'] = char_bitmap_v;
    char_bitmap_table['w'] = char_bitmap_w;
    char_bitmap_table['x'] = char_bitmap_x;
    char_bitmap_table['y'] = char_bitmap_y;
    char_bitmap_table['z'] = char_bitmap_z;
    char_bitmap_table['.'] = char_bitmap_dot;
    char_bitmap_table[','] = char_bitmap_comma;
    char_bitmap_table[':'] = char_bitmap_colon;
    char_bitmap_table[';'] = char_bitmap_semicolon;
    char_bitmap_table['!'] = char_bitmap_exclamation;
    char_bitmap_table['?'] = char_bitmap_question;
    char_bitmap_table['-'] = char_bitmap_hyphen;
    char_bitmap_table['_'] = char_bitmap_underscore;
    char_bitmap_table['+'] = char_bitmap_plus;
    char_bitmap_table['='] = char_bitmap_equal;
    char_bitmap_table['/'] = char_bitmap_slash;
    char_bitmap_table['\\'] = char_bitmap_backslash;
    char_bitmap_table['('] = char_bitmap_parenLeft;
    char_bitmap_table[')'] = char_bitmap_parenRight;
}

const uint8_t* get_char_bitmap(char c)
{
    init_table();
    unsigned char uc = static_cast<unsigned char>(c);
    if (uc < 128) return char_bitmap_table[uc];
    return nullptr;
}