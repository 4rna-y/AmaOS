#include "graphics/StandardCharactorBitmapProvider.h"

static inline const uint8_t* getCharBitmap(char c)
{
    unsigned char uc = (unsigned char)c;
    if (uc < 128) 
    {
        return charBitmapTable[uc];
    }
    return nullptr;
}

static const uint8_t *charBitmapTable[128] =
{
    [0]  = nullptr, [1]  = nullptr, [2]  = nullptr, [3]  = nullptr,
    [4]  = nullptr, [5]  = nullptr, [6]  = nullptr, [7]  = nullptr,
    [8]  = nullptr, [9]  = nullptr, [10] = nullptr, [11] = nullptr,
    [12] = nullptr, [13] = nullptr, [14] = nullptr, [15] = nullptr,
    [16] = nullptr, [17] = nullptr, [18] = nullptr, [19] = nullptr,
    [20] = nullptr, [21] = nullptr, [22] = nullptr, [23] = nullptr,
    [24] = nullptr, [25] = nullptr, [26] = nullptr, [27] = nullptr,
    [28] = nullptr, [29] = nullptr, [30] = nullptr, [31] = nullptr,
    [' ']  = charBitmap_space,
    ['0']  = charBitmap_0,
    ['1']  = charBitmap_1,
    ['2']  = charBitmap_2,
    ['3']  = charBitmap_3,
    ['4']  = charBitmap_4,
    ['5']  = charBitmap_5,
    ['6']  = charBitmap_6,
    ['7']  = charBitmap_7,
    ['8']  = charBitmap_8,
    ['9']  = charBitmap_9,
    ['.']  = charBitmap_dot,
    [',']  = charBitmap_comma,
    [':']  = charBitmap_colon,
    [';']  = charBitmap_semicolon,
    ['!']  = charBitmap_exclamation,
    ['?']  = charBitmap_question,
    ['-']  = charBitmap_hyphen,
    ['_']  = charBitmap_underscore,
    ['+']  = charBitmap_plus,
    ['=']  = charBitmap_equal,
    ['/']  = charBitmap_slash,
    ['\\'] = charBitmap_backslash,
    ['(']  = charBitmap_parenLeft,
    [')']  = charBitmap_parenRight,
    ['A']  = charBitmap_A,
    ['B']  = charBitmap_B,
    ['C']  = charBitmap_C,
    ['D']  = charBitmap_D,
    ['E']  = charBitmap_E,
    ['F']  = charBitmap_F,
    ['G']  = charBitmap_G,
    ['H']  = charBitmap_H,
    ['I']  = charBitmap_I,
    ['J']  = charBitmap_J,
    ['K']  = charBitmap_K,
    ['L']  = charBitmap_L,
    ['M']  = charBitmap_M,
    ['N']  = charBitmap_N,
    ['O']  = charBitmap_O,
    ['P']  = charBitmap_P,
    ['Q']  = charBitmap_Q,
    ['R']  = charBitmap_R,
    ['S']  = charBitmap_S,
    ['T']  = charBitmap_T,
    ['U']  = charBitmap_U,
    ['V']  = charBitmap_V,
    ['W']  = charBitmap_W,
    ['X']  = charBitmap_X,
    ['Y']  = charBitmap_Y,
    ['Z']  = charBitmap_Z,
    ['a']  = charBitmap_a,
    ['b']  = charBitmap_b,
    ['c']  = charBitmap_c,
    ['d']  = charBitmap_d,
    ['e']  = charBitmap_e,
    ['f']  = charBitmap_f,
    ['g']  = charBitmap_g,
    ['h']  = charBitmap_h,
    ['i']  = charBitmap_i,
    ['j']  = charBitmap_j,
    ['k']  = charBitmap_k,
    ['l']  = charBitmap_l,
    ['m']  = charBitmap_m,
    ['n']  = charBitmap_n,
    ['o']  = charBitmap_o,
    ['p']  = charBitmap_p,
    ['q']  = charBitmap_q,
    ['r']  = charBitmap_r,
    ['s']  = charBitmap_s,
    ['t']  = charBitmap_t,
    ['u']  = charBitmap_u,
    ['v']  = charBitmap_v,
    ['w']  = charBitmap_w,
    ['x']  = charBitmap_x,
    ['y']  = charBitmap_y,
    ['z']  = charBitmap_z,
    [127] = nullptr,
};

static const uint8_t charBitmap_space[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_0[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_1[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_2[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_3[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_4[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_5[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_6[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_7[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_8[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_9[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_A[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_B[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_C[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_D[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_E[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_F[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_G[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_H[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_I[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_J[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_K[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_L[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_M[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_N[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_O[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_P[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_Q[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_R[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_S[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_T[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_U[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_V[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_W[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_X[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_Y[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_Z[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_a[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_b[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_c[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_d[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_e[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_f[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00001100,
    0b00011110,
    0b00001100,
    0b00001100,
    0b00001100,
    0b00001100,
    0b00011110,
    0b00000000,
};

static const uint8_t charBitmap_g[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_h[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_i[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_j[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_k[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_l[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_m[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_n[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_o[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_p[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_q[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_r[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_s[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_t[CHARACTOR_BITMAP_HEIGHT] =
{
    0b00001000,
    0b00001000,
    0b00111110,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00000110,
    0b00000000,
};

static const uint8_t charBitmap_u[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_v[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_w[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_x[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_y[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_z[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_dot[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_comma[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_colon[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_semicolon[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_exclamation[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_question[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_hyphen[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_underscore[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_plus[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_equal[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_slash[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_backslash[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_parenLeft[CHARACTOR_BITMAP_HEIGHT] =
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

static const uint8_t charBitmap_parenRight[CHARACTOR_BITMAP_HEIGHT] =
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