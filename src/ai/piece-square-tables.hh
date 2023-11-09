#pragma once

// https://www.chessprogramming.org/PeSTO%27s_Evaluation_Function
namespace ai::tables
{
    struct PieceSquareTable
    {
        const int *early_table;
        const int *end_table;
        int phase_value;
    };

    // Opening <piece> Value
    static constexpr const int OQV = 1025;
    static constexpr const int ORV = 477;
    static constexpr const int OBV = 365;
    static constexpr const int OKV = 337;
    static constexpr const int OPV = 82;

    // End <piece> Value
    static constexpr const int EQV = 936;
    static constexpr const int ERV = 512;
    static constexpr const int EBV = 297;
    static constexpr const int EKV = 281;
    static constexpr const int EPV = 94;

    /*
     * Early pawn table:
     *
     *   0,   0,   0,   0,   0,   0,  0,   0,
     *  98, 134,  61,  95,  68, 126, 34, -11,
     *  -6,   7,  26,  31,  65,  56, 25, -20,
     * -14,  13,   6,  21,  23,  12, 17, -23,
     * -27,  -2,  -5,  12,  17,   6, 10, -25,
     * -26,  -4,  -4, -10,   3,   3, 33, -12,
     * -35,  -1, -20, -23, -15,  24, 38, -22,
     *   0,   0,   0,   0,   0,   0,  0,   0,
     */
    static constexpr const int early_white_pawn[64] = {
        0 + OPV,   0 + OPV,   0 + OPV,   0 + OPV,   0 + OPV,   0 + OPV,   0 + OPV,   0 + OPV,   -35 + OPV, -1 + OPV,
        -20 + OPV, -23 + OPV, -15 + OPV, 24 + OPV,  38 + OPV,  -22 + OPV, -26 + OPV, -4 + OPV,  -4 + OPV,  -10 + OPV,
        3 + OPV,   3 + OPV,   33 + OPV,  -12 + OPV, -27 + OPV, -2 + OPV,  -5 + OPV,  12 + OPV,  17 + OPV,  6 + OPV,
        10 + OPV,  -25 + OPV, -14 + OPV, 13 + OPV,  6 + OPV,   21 + OPV,  23 + OPV,  12 + OPV,  17 + OPV,  -23 + OPV,
        -6 + OPV,  7 + OPV,   26 + OPV,  31 + OPV,  65 + OPV,  56 + OPV,  25 + OPV,  -20 + OPV, 98 + OPV,  134 + OPV,
        61 + OPV,  95 + OPV,  68 + OPV,  126 + OPV, 34 + OPV,  -11 + OPV, 0 + OPV,   0 + OPV,   0 + OPV,   0 + OPV,
        0 + OPV,   0 + OPV,   0 + OPV,   0 + OPV,
    };

    static constexpr const int early_black_pawn[64] = {
        0 - OPV,   0 - OPV,   0 - OPV,   0 - OPV,    0 - OPV,   0 - OPV,   0 - OPV,   0 - OPV,   -98 - OPV, -134 - OPV,
        -61 - OPV, -95 - OPV, -68 - OPV, -126 - OPV, -34 - OPV, 11 - OPV,  6 - OPV,   -7 - OPV,  -26 - OPV, -31 - OPV,
        -65 - OPV, -56 - OPV, -25 - OPV, 20 - OPV,   14 - OPV,  -13 - OPV, -6 - OPV,  -21 - OPV, -23 - OPV, -12 - OPV,
        -17 - OPV, 23 - OPV,  27 - OPV,  2 - OPV,    5 - OPV,   -12 - OPV, -17 - OPV, -6 - OPV,  -10 - OPV, 25 - OPV,
        26 - OPV,  4 - OPV,   4 - OPV,   10 - OPV,   -3 - OPV,  -3 - OPV,  -33 - OPV, 12 - OPV,  35 - OPV,  1 - OPV,
        20 - OPV,  23 - OPV,  15 - OPV,  -24 - OPV,  -38 - OPV, 22 - OPV,  0 - OPV,   0 - OPV,   0 - OPV,   0 - OPV,
        0 - OPV,   0 - OPV,   0 - OPV,   0 - OPV,
    };

    /*
     * End pawn table:
     *
     *   0,   0,   0,   0,   0,   0,   0,   0,
     * 178, 173, 158, 134, 147, 132, 165, 187,
     *  94, 100,  85,  67,  56,  53,  82,  84,
     *  32,  24,  13,   5,  -2,   4,  17,  17,
     *  13,   9,  -3,  -7,  -7,  -8,   3,  -1,
     *   4,   7,  -6,   1,   0,  -5,  -1,  -8,
     *  13,   8,   8,  10,  13,   0,   2,  -7,
     *   0,   0,   0,   0,   0,   0,   0,   0,
     */
    static constexpr const int end_white_pawn[64] = {
        0 + EPV,   0 + EPV,   0 + EPV,   0 + EPV,   0 + EPV,   0 + EPV,   0 + EPV,  0 + EPV,  13 + EPV,  8 + EPV,
        8 + EPV,   10 + EPV,  13 + EPV,  0 + EPV,   2 + EPV,   -7 + EPV,  4 + EPV,  7 + EPV,  -6 + EPV,  1 + EPV,
        0 + EPV,   -5 + EPV,  -1 + EPV,  -8 + EPV,  13 + EPV,  9 + EPV,   -3 + EPV, -7 + EPV, -7 + EPV,  -8 + EPV,
        3 + EPV,   -1 + EPV,  32 + EPV,  24 + EPV,  13 + EPV,  5 + EPV,   -2 + EPV, 4 + EPV,  17 + EPV,  17 + EPV,
        94 + EPV,  100 + EPV, 85 + EPV,  67 + EPV,  56 + EPV,  53 + EPV,  82 + EPV, 84 + EPV, 178 + EPV, 173 + EPV,
        158 + EPV, 134 + EPV, 147 + EPV, 132 + EPV, 165 + EPV, 187 + EPV, 0 + EPV,  0 + EPV,  0 + EPV,   0 + EPV,
        0 + EPV,   0 + EPV,   0 + EPV,   0 + EPV,
    };

    static constexpr const int end_black_pawn[64] = {
        0 - EPV,    0 - EPV,    0 - EPV,    0 - EPV,    0 - EPV,    0 - EPV,    0 - EPV,    0 - EPV,
        -178 - EPV, -173 - EPV, -158 - EPV, -134 - EPV, -147 - EPV, -132 - EPV, -165 - EPV, -187 - EPV,
        -94 - EPV,  -100 - EPV, -85 - EPV,  -67 - EPV,  -56 - EPV,  -53 - EPV,  -82 - EPV,  -84 - EPV,
        -32 - EPV,  -24 - EPV,  -13 - EPV,  -5 - EPV,   2 - EPV,    -4 - EPV,   -17 - EPV,  -17 - EPV,
        -13 - EPV,  -9 - EPV,   3 - EPV,    7 - EPV,    7 - EPV,    8 - EPV,    -3 - EPV,   1 - EPV,
        -4 - EPV,   -7 - EPV,   6 - EPV,    -1 - EPV,   0 - EPV,    5 - EPV,    1 - EPV,    8 - EPV,
        -13 - EPV,  -8 - EPV,   -8 - EPV,   -10 - EPV,  -13 - EPV,  0 - EPV,    -2 - EPV,   7 - EPV,
        0 - EPV,    0 - EPV,    0 - EPV,    0 - EPV,    0 - EPV,    0 - EPV,    0 - EPV,    0 - EPV,
    };

    /*
     * Early knight table:
     *
     * -167, -89, -34, -49,  61, -97, -15, -107,
     *  -73, -41,  72,  36,  23,  62,   7,  -17,
     *  -47,  60,  37,  65,  84, 129,  73,   44,
     *   -9,  17,  19,  53,  37,  69,  18,   22,
     *  -13,   4,  16,  13,  28,  19,  21,   -8,
     *  -23,  -9,  12,  10,  19,  17,  25,  -16,
     *  -29, -53, -12,  -3,  -1,  18, -14,  -19,
     * -105, -21, -58, -33, -17, -28, -19,  -23,
     */
    static constexpr const int early_white_knight[64] = {
        -105 + OKV, -21 + OKV, -58 + OKV, -33 + OKV,  -17 + OKV, -28 + OKV, -19 + OKV,  -23 + OKV, -29 + OKV, -53 + OKV,
        -12 + OKV,  -3 + OKV,  -1 + OKV,  18 + OKV,   -14 + OKV, -19 + OKV, -23 + OKV,  -9 + OKV,  12 + OKV,  10 + OKV,
        19 + OKV,   17 + OKV,  25 + OKV,  -16 + OKV,  -13 + OKV, 4 + OKV,   16 + OKV,   13 + OKV,  28 + OKV,  19 + OKV,
        21 + OKV,   -8 + OKV,  -9 + OKV,  17 + OKV,   19 + OKV,  53 + OKV,  37 + OKV,   69 + OKV,  18 + OKV,  22 + OKV,
        -47 + OKV,  60 + OKV,  37 + OKV,  65 + OKV,   84 + OKV,  129 + OKV, 73 + OKV,   44 + OKV,  -73 + OKV, -41 + OKV,
        72 + OKV,   36 + OKV,  23 + OKV,  62 + OKV,   7 + OKV,   -17 + OKV, -167 + OKV, -89 + OKV, -34 + OKV, -49 + OKV,
        61 + OKV,   -97 + OKV, -15 + OKV, -107 + OKV,
    };

    static constexpr const int early_black_knight[64] = {
        167 - OKV, 89 - OKV,   34 - OKV,  49 - OKV,  -61 - OKV, 97 - OKV,  15 - OKV,  107 - OKV, 73 - OKV,  41 - OKV,
        -72 - OKV, -36 - OKV,  -23 - OKV, -62 - OKV, -7 - OKV,  17 - OKV,  47 - OKV,  -60 - OKV, -37 - OKV, -65 - OKV,
        -84 - OKV, -129 - OKV, -73 - OKV, -44 - OKV, 9 - OKV,   -17 - OKV, -19 - OKV, -53 - OKV, -37 - OKV, -69 - OKV,
        -18 - OKV, -22 - OKV,  13 - OKV,  -4 - OKV,  -16 - OKV, -13 - OKV, -28 - OKV, -19 - OKV, -21 - OKV, 8 - OKV,
        23 - OKV,  9 - OKV,    -12 - OKV, -10 - OKV, -19 - OKV, -17 - OKV, -25 - OKV, 16 - OKV,  29 - OKV,  53 - OKV,
        12 - OKV,  3 - OKV,    1 - OKV,   -18 - OKV, 14 - OKV,  19 - OKV,  105 - OKV, 21 - OKV,  58 - OKV,  33 - OKV,
        17 - OKV,  28 - OKV,   19 - OKV,  23 - OKV,
    };

    /*
     * End knight table:
     *
     * -58, -38, -13, -28, -31, -27, -63, -99,
     * -25,  -8, -25,  -2,  -9, -25, -24, -52,
     * -24, -20,  10,   9,  -1,  -9, -19, -41,
     * -17,   3,  22,  22,  22,  11,   8, -18,
     * -18,  -6,  16,  25,  16,  17,   4, -18,
     * -23,  -3,  -1,  15,  10,  -3, -20, -22,
     * -42, -20, -10,  -5,  -2, -20, -23, -44,
     * -29, -51, -23, -15, -22, -18, -50, -64,
     */
    static constexpr const int end_white_knight[64] = {
        -29 + EKV, -51 + EKV, -23 + EKV, -15 + EKV, -22 + EKV, -18 + EKV, -50 + EKV, -64 + EKV, -42 + EKV, -20 + EKV,
        -10 + EKV, -5 + EKV,  -2 + EKV,  -20 + EKV, -23 + EKV, -44 + EKV, -23 + EKV, -3 + EKV,  -1 + EKV,  15 + EKV,
        10 + EKV,  -3 + EKV,  -20 + EKV, -22 + EKV, -18 + EKV, -6 + EKV,  16 + EKV,  25 + EKV,  16 + EKV,  17 + EKV,
        4 + EKV,   -18 + EKV, -17 + EKV, 3 + EKV,   22 + EKV,  22 + EKV,  22 + EKV,  11 + EKV,  8 + EKV,   -18 + EKV,
        -24 + EKV, -20 + EKV, 10 + EKV,  9 + EKV,   -1 + EKV,  -9 + EKV,  -19 + EKV, -41 + EKV, -25 + EKV, -8 + EKV,
        -25 + EKV, -2 + EKV,  -9 + EKV,  -25 + EKV, -24 + EKV, -52 + EKV, -58 + EKV, -38 + EKV, -13 + EKV, -28 + EKV,
        -31 + EKV, -27 + EKV, -63 + EKV, -99 + EKV,
    };

    static constexpr const int end_black_knight[64] = {
        58 - EKV, 38 - EKV, 13 - EKV, 28 - EKV,  31 - EKV,  27 - EKV,  63 - EKV,  99 - EKV,  25 - EKV,  8 - EKV,
        25 - EKV, 2 - EKV,  9 - EKV,  25 - EKV,  24 - EKV,  52 - EKV,  24 - EKV,  20 - EKV,  -10 - EKV, -9 - EKV,
        1 - EKV,  9 - EKV,  19 - EKV, 41 - EKV,  17 - EKV,  -3 - EKV,  -22 - EKV, -22 - EKV, -22 - EKV, -11 - EKV,
        -8 - EKV, 18 - EKV, 18 - EKV, 6 - EKV,   -16 - EKV, -25 - EKV, -16 - EKV, -17 - EKV, -4 - EKV,  18 - EKV,
        23 - EKV, 3 - EKV,  1 - EKV,  -15 - EKV, -10 - EKV, 3 - EKV,   20 - EKV,  22 - EKV,  42 - EKV,  20 - EKV,
        10 - EKV, 5 - EKV,  2 - EKV,  20 - EKV,  23 - EKV,  44 - EKV,  29 - EKV,  51 - EKV,  23 - EKV,  15 - EKV,
        22 - EKV, 18 - EKV, 50 - EKV, 64 - EKV,
    };

    /*
     * Early bishop table:
     *
     * -29,   4, -82, -37, -25, -42,   7,  -8,
     * -26,  16, -18, -13,  30,  59,  18, -47,
     * -16,  37,  43,  40,  35,  50,  37,  -2,
     *  -4,   5,  19,  50,  37,  37,   7,  -2,
     *  -6,  13,  13,  26,  34,  12,  10,   4,
     *   0,  15,  15,  15,  14,  27,  18,  10,
     *   4,  15,  16,   0,   7,  21,  33,   1,
     * -33,  -3, -14, -21, -13, -12, -39, -21,
     */
    static constexpr const int early_white_bishop[64] = {
        -33 + OBV, -3 + OBV,  -14 + OBV, -21 + OBV, -13 + OBV, -12 + OBV, -39 + OBV, -21 + OBV, 4 + OBV,   15 + OBV,
        16 + OBV,  0 + OBV,   7 + OBV,   21 + OBV,  33 + OBV,  1 + OBV,   0 + OBV,   15 + OBV,  15 + OBV,  15 + OBV,
        14 + OBV,  27 + OBV,  18 + OBV,  10 + OBV,  -6 + OBV,  13 + OBV,  13 + OBV,  26 + OBV,  34 + OBV,  12 + OBV,
        10 + OBV,  4 + OBV,   -4 + OBV,  5 + OBV,   19 + OBV,  50 + OBV,  37 + OBV,  37 + OBV,  7 + OBV,   -2 + OBV,
        -16 + OBV, 37 + OBV,  43 + OBV,  40 + OBV,  35 + OBV,  50 + OBV,  37 + OBV,  -2 + OBV,  -26 + OBV, 16 + OBV,
        -18 + OBV, -13 + OBV, 30 + OBV,  59 + OBV,  18 + OBV,  -47 + OBV, -29 + OBV, 4 + OBV,   -82 + OBV, -37 + OBV,
        -25 + OBV, -42 + OBV, 7 + OBV,   -8 + OBV,
    };

    static constexpr const int early_black_bishop[64] = {
        29 - OBV,  -4 - OBV,  82 - OBV,  37 - OBV,  25 - OBV,  42 - OBV,  -7 - OBV,  8 - OBV,   26 - OBV,  -16 - OBV,
        18 - OBV,  13 - OBV,  -30 - OBV, -59 - OBV, -18 - OBV, 47 - OBV,  16 - OBV,  -37 - OBV, -43 - OBV, -40 - OBV,
        -35 - OBV, -50 - OBV, -37 - OBV, 2 - OBV,   4 - OBV,   -5 - OBV,  -19 - OBV, -50 - OBV, -37 - OBV, -37 - OBV,
        -7 - OBV,  2 - OBV,   6 - OBV,   -13 - OBV, -13 - OBV, -26 - OBV, -34 - OBV, -12 - OBV, -10 - OBV, -4 - OBV,
        0 - OBV,   -15 - OBV, -15 - OBV, -15 - OBV, -14 - OBV, -27 - OBV, -18 - OBV, -10 - OBV, -4 - OBV,  -15 - OBV,
        -16 - OBV, 0 - OBV,   -7 - OBV,  -21 - OBV, -33 - OBV, -1 - OBV,  33 - OBV,  3 - OBV,   14 - OBV,  21 - OBV,
        13 - OBV,  12 - OBV,  39 - OBV,  21 - OBV,
    };

    /*
     * End bishop table:
     *
     * -14, -21, -11,  -8, -7,  -9, -17, -24,
     *  -8,  -4,   7, -12, -3, -13,  -4, -14,
     *   2,  -8,   0,  -1, -2,   6,   0,   4,
     *  -3,   9,  12,   9, 14,  10,   3,   2,
     *  -6,   3,  13,  19,  7,  10,  -3,  -9,
     * -12,  -3,   8,  10, 13,   3,  -7, -15,
     * -14, -18,  -7,  -1,  4,  -9, -15, -27,
     * -23,  -9, -23,  -5, -9, -16,  -5, -17,
     */
    static constexpr const int end_white_bishop[64] = {
        -23 + EBV, -9 + EBV,  -23 + EBV, -5 + EBV,  -9 + EBV,  -16 + EBV, -5 + EBV,  -17 + EBV, -14 + EBV, -18 + EBV,
        -7 + EBV,  -1 + EBV,  4 + EBV,   -9 + EBV,  -15 + EBV, -27 + EBV, -12 + EBV, -3 + EBV,  8 + EBV,   10 + EBV,
        13 + EBV,  3 + EBV,   -7 + EBV,  -15 + EBV, -6 + EBV,  3 + EBV,   13 + EBV,  19 + EBV,  7 + EBV,   10 + EBV,
        -3 + EBV,  -9 + EBV,  -3 + EBV,  9 + EBV,   12 + EBV,  9 + EBV,   14 + EBV,  10 + EBV,  3 + EBV,   2 + EBV,
        2 + EBV,   -8 + EBV,  0 + EBV,   -1 + EBV,  -2 + EBV,  6 + EBV,   0 + EBV,   4 + EBV,   -8 + EBV,  -4 + EBV,
        7 + EBV,   -12 + EBV, -3 + EBV,  -13 + EBV, -4 + EBV,  -14 + EBV, -14 + EBV, -21 + EBV, -11 + EBV, -8 + EBV,
        -7 + EBV,  -9 + EBV,  -17 + EBV, -24 + EBV,
    };
    static constexpr const int end_black_bishop[64] = {
        14 - EBV, 21 - EBV, 11 - EBV, 8 - EBV,   7 - EBV,   9 - EBV,   17 - EBV,  24 - EBV,  8 - EBV,   4 - EBV,
        -7 - EBV, 12 - EBV, 3 - EBV,  13 - EBV,  4 - EBV,   14 - EBV,  -2 - EBV,  8 - EBV,   0 - EBV,   1 - EBV,
        2 - EBV,  -6 - EBV, 0 - EBV,  -4 - EBV,  3 - EBV,   -9 - EBV,  -12 - EBV, -9 - EBV,  -14 - EBV, -10 - EBV,
        -3 - EBV, -2 - EBV, 6 - EBV,  -3 - EBV,  -13 - EBV, -19 - EBV, -7 - EBV,  -10 - EBV, 3 - EBV,   9 - EBV,
        12 - EBV, 3 - EBV,  -8 - EBV, -10 - EBV, -13 - EBV, -3 - EBV,  7 - EBV,   15 - EBV,  14 - EBV,  18 - EBV,
        7 - EBV,  1 - EBV,  -4 - EBV, 9 - EBV,   15 - EBV,  27 - EBV,  23 - EBV,  9 - EBV,   23 - EBV,  5 - EBV,
        9 - EBV,  16 - EBV, 5 - EBV,  17 - EBV,
    };

    /*
     * Early rook table:
     *
     *  32,  42,  32,  51, 63,  9,  31,  43,
     *  27,  32,  58,  62, 80, 67,  26,  44,
     *  -5,  19,  26,  36, 17, 45,  61,  16,
     * -24, -11,   7,  26, 24, 35,  -8, -20,
     * -36, -26, -12,  -1,  9, -7,   6, -23,
     * -45, -25, -16, -17,  3,  0,  -5, -33,
     * -44, -16, -20,  -9, -1, 11,  -6, -71,
     * -19, -13,   1,  17, 16,  7, -37, -26,
     */
    static constexpr const int early_white_rook[64] = {
        -19 + ORV, -13 + ORV, 1 + ORV,   17 + ORV,  16 + ORV,  7 + ORV,   -37 + ORV, -26 + ORV, -44 + ORV, -16 + ORV,
        -20 + ORV, -9 + ORV,  -1 + ORV,  11 + ORV,  -6 + ORV,  -71 + ORV, -45 + ORV, -25 + ORV, -16 + ORV, -17 + ORV,
        3 + ORV,   0 + ORV,   -5 + ORV,  -33 + ORV, -36 + ORV, -26 + ORV, -12 + ORV, -1 + ORV,  9 + ORV,   -7 + ORV,
        6 + ORV,   -23 + ORV, -24 + ORV, -11 + ORV, 7 + ORV,   26 + ORV,  24 + ORV,  35 + ORV,  -8 + ORV,  -20 + ORV,
        -5 + ORV,  19 + ORV,  26 + ORV,  36 + ORV,  17 + ORV,  45 + ORV,  61 + ORV,  16 + ORV,  27 + ORV,  32 + ORV,
        58 + ORV,  62 + ORV,  80 + ORV,  67 + ORV,  26 + ORV,  44 + ORV,  32 + ORV,  42 + ORV,  32 + ORV,  51 + ORV,
        63 + ORV,  9 + ORV,   31 + ORV,  43 + ORV,
    };

    static constexpr const int early_black_rook[64] = {
        -32 - ORV, -42 - ORV, -32 - ORV, -51 - ORV, -63 - ORV, -9 - ORV,  -31 - ORV, -43 - ORV, -27 - ORV, -32 - ORV,
        -58 - ORV, -62 - ORV, -80 - ORV, -67 - ORV, -26 - ORV, -44 - ORV, 5 - ORV,   -19 - ORV, -26 - ORV, -36 - ORV,
        -17 - ORV, -45 - ORV, -61 - ORV, -16 - ORV, 24 - ORV,  11 - ORV,  -7 - ORV,  -26 - ORV, -24 - ORV, -35 - ORV,
        8 - ORV,   20 - ORV,  36 - ORV,  26 - ORV,  12 - ORV,  1 - ORV,   -9 - ORV,  7 - ORV,   -6 - ORV,  23 - ORV,
        45 - ORV,  25 - ORV,  16 - ORV,  17 - ORV,  -3 - ORV,  0 - ORV,   5 - ORV,   33 - ORV,  44 - ORV,  16 - ORV,
        20 - ORV,  9 - ORV,   1 - ORV,   -11 - ORV, 6 - ORV,   71 - ORV,  19 - ORV,  13 - ORV,  -1 - ORV,  -17 - ORV,
        -16 - ORV, -7 - ORV,  37 - ORV,  26 - ORV,
    };

    /*
     * End rook table:
     *
     * 13, 10, 18, 15, 12,  12,   8,   5,
     * 11, 13, 13, 11, -3,   3,   8,   3,
     *  7,  7,  7,  5,  4,  -3,  -5,  -3,
     *  4,  3, 13,  1,  2,   1,  -1,   2,
     *  3,  5,  8,  4, -5,  -6,  -8, -11,
     * -4,  0, -5, -1, -7, -12,  -8, -16,
     * -6, -6,  0,  2, -9,  -9, -11,  -3,
     * -9,  2,  3, -1, -5, -13,   4, -20,
     */
    static constexpr const int end_white_rook[64] = {
        -9 + ERV, 2 + ERV,   3 + ERV,  -1 + ERV,  -5 + ERV,  -13 + ERV, 4 + ERV,  -20 + ERV, -6 + ERV, -6 + ERV,
        0 + ERV,  2 + ERV,   -9 + ERV, -9 + ERV,  -11 + ERV, -3 + ERV,  -4 + ERV, 0 + ERV,   -5 + ERV, -1 + ERV,
        -7 + ERV, -12 + ERV, -8 + ERV, -16 + ERV, 3 + ERV,   5 + ERV,   8 + ERV,  4 + ERV,   -5 + ERV, -6 + ERV,
        -8 + ERV, -11 + ERV, 4 + ERV,  3 + ERV,   13 + ERV,  1 + ERV,   2 + ERV,  1 + ERV,   -1 + ERV, 2 + ERV,
        7 + ERV,  7 + ERV,   7 + ERV,  5 + ERV,   4 + ERV,   -3 + ERV,  -5 + ERV, -3 + ERV,  11 + ERV, 13 + ERV,
        13 + ERV, 11 + ERV,  -3 + ERV, 3 + ERV,   8 + ERV,   3 + ERV,   13 + ERV, 10 + ERV,  18 + ERV, 15 + ERV,
        12 + ERV, 12 + ERV,  8 + ERV,  5 + ERV,
    };

    static constexpr const int end_black_rook[64] = {
        -13 - ERV, -10 - ERV, -18 - ERV, -15 - ERV, -12 - ERV, -12 - ERV, -8 - ERV,  -5 - ERV, -11 - ERV, -13 - ERV,
        -13 - ERV, -11 - ERV, 3 - ERV,   -3 - ERV,  -8 - ERV,  -3 - ERV,  -7 - ERV,  -7 - ERV, -7 - ERV,  -5 - ERV,
        -4 - ERV,  3 - ERV,   5 - ERV,   3 - ERV,   -4 - ERV,  -3 - ERV,  -13 - ERV, -1 - ERV, -2 - ERV,  -1 - ERV,
        1 - ERV,   -2 - ERV,  -3 - ERV,  -5 - ERV,  -8 - ERV,  -4 - ERV,  5 - ERV,   6 - ERV,  8 - ERV,   11 - ERV,
        4 - ERV,   0 - ERV,   5 - ERV,   1 - ERV,   7 - ERV,   12 - ERV,  8 - ERV,   16 - ERV, 6 - ERV,   6 - ERV,
        0 - ERV,   -2 - ERV,  9 - ERV,   9 - ERV,   11 - ERV,  3 - ERV,   9 - ERV,   -2 - ERV, -3 - ERV,  1 - ERV,
        5 - ERV,   13 - ERV,  -4 - ERV,  20 - ERV,
    };

    /*
     * Early queen table:
     *
     * -28,   0,  29,  12,  59,  44,  43,  45,
     * -24, -39,  -5,   1, -16,  57,  28,  54,
     * -13, -17,   7,   8,  29,  56,  47,  57,
     * -27, -27, -16, -16,  -1,  17,  -2,   1,
     *  -9, -26,  -9, -10,  -2,  -4,   3,  -3,
     * -14,   2, -11,  -2,  -5,   2,  14,   5,
     * -35,  -8,  11,   2,   8,  15,  -3,   1,
     *  -1, -18,  -9,  10, -15, -25, -31, -50,
     */
    static constexpr const int early_white_queen[64] = {
        -1 + OQV,  -18 + OQV, -9 + OQV,  10 + OQV,  -15 + OQV, -25 + OQV, -31 + OQV, -50 + OQV, -35 + OQV, -8 + OQV,
        11 + OQV,  2 + OQV,   8 + OQV,   15 + OQV,  -3 + OQV,  1 + OQV,   -14 + OQV, 2 + OQV,   -11 + OQV, -2 + OQV,
        -5 + OQV,  2 + OQV,   14 + OQV,  5 + OQV,   -9 + OQV,  -26 + OQV, -9 + OQV,  -10 + OQV, -2 + OQV,  -4 + OQV,
        3 + OQV,   -3 + OQV,  -27 + OQV, -27 + OQV, -16 + OQV, -16 + OQV, -1 + OQV,  17 + OQV,  -2 + OQV,  1 + OQV,
        -13 + OQV, -17 + OQV, 7 + OQV,   8 + OQV,   29 + OQV,  56 + OQV,  47 + OQV,  57 + OQV,  -24 + OQV, -39 + OQV,
        -5 + OQV,  1 + OQV,   -16 + OQV, 57 + OQV,  28 + OQV,  54 + OQV,  -28 + OQV, 0 + OQV,   29 + OQV,  12 + OQV,
        59 + OQV,  44 + OQV,  43 + OQV,  45 + OQV,
    };

    static constexpr const int early_black_queen[64] = {
        28 - OQV,  0 - OQV,   -29 - OQV, -12 - OQV, -59 - OQV, -44 - OQV, -43 - OQV, -45 - OQV, 24 - OQV, 39 - OQV,
        5 - OQV,   -1 - OQV,  16 - OQV,  -57 - OQV, -28 - OQV, -54 - OQV, 13 - OQV,  17 - OQV,  -7 - OQV, -8 - OQV,
        -29 - OQV, -56 - OQV, -47 - OQV, -57 - OQV, 27 - OQV,  27 - OQV,  16 - OQV,  16 - OQV,  1 - OQV,  -17 - OQV,
        2 - OQV,   -1 - OQV,  9 - OQV,   26 - OQV,  9 - OQV,   10 - OQV,  2 - OQV,   4 - OQV,   -3 - OQV, 3 - OQV,
        14 - OQV,  -2 - OQV,  11 - OQV,  2 - OQV,   5 - OQV,   -2 - OQV,  -14 - OQV, -5 - OQV,  35 - OQV, 8 - OQV,
        -11 - OQV, -2 - OQV,  -8 - OQV,  -15 - OQV, 3 - OQV,   -1 - OQV,  1 - OQV,   18 - OQV,  9 - OQV,  -10 - OQV,
        15 - OQV,  25 - OQV,  31 - OQV,  50 - OQV,
    };

    /*
     * End queen table:
     *
     *  -9,  22,  22,  27,  27,  19,  10,  20,
     * -17,  20,  32,  41,  58,  25,  30,   0,
     * -20,   6,   9,  49,  47,  35,  19,   9,
     *   3,  22,  24,  45,  57,  40,  57,  36,
     * -18,  28,  19,  47,  31,  34,  39,  23,
     * -16, -27,  15,   6,   9,  17,  10,   5,
     * -22, -23, -30, -16, -16, -23, -36, -32,
     * -33, -28, -22, -43,  -5, -32, -20, -41,
     */
    static constexpr const int end_white_queen[64] = {
        -33 + EQV, -28 + EQV, -22 + EQV, -43 + EQV, -5 + EQV,  -32 + EQV, -20 + EQV, -41 + EQV, -22 + EQV, -23 + EQV,
        -30 + EQV, -16 + EQV, -16 + EQV, -23 + EQV, -36 + EQV, -32 + EQV, -16 + EQV, -27 + EQV, 15 + EQV,  6 + EQV,
        9 + EQV,   17 + EQV,  10 + EQV,  5 + EQV,   -18 + EQV, 28 + EQV,  19 + EQV,  47 + EQV,  31 + EQV,  34 + EQV,
        39 + EQV,  23 + EQV,  3 + EQV,   22 + EQV,  24 + EQV,  45 + EQV,  57 + EQV,  40 + EQV,  57 + EQV,  36 + EQV,
        -20 + EQV, 6 + EQV,   9 + EQV,   49 + EQV,  47 + EQV,  35 + EQV,  19 + EQV,  9 + EQV,   -17 + EQV, 20 + EQV,
        32 + EQV,  41 + EQV,  58 + EQV,  25 + EQV,  30 + EQV,  0 + EQV,   -9 + EQV,  22 + EQV,  22 + EQV,  27 + EQV,
        27 + EQV,  19 + EQV,  10 + EQV,  20 + EQV,
    };

    static constexpr const int end_black_queen[64] = {
        9 - EQV,   -22 - EQV, -22 - EQV, -27 - EQV, -27 - EQV, -19 - EQV, -10 - EQV, -20 - EQV, 17 - EQV,  -20 - EQV,
        -32 - EQV, -41 - EQV, -58 - EQV, -25 - EQV, -30 - EQV, 0 - EQV,   20 - EQV,  -6 - EQV,  -9 - EQV,  -49 - EQV,
        -47 - EQV, -35 - EQV, -19 - EQV, -9 - EQV,  -3 - EQV,  -22 - EQV, -24 - EQV, -45 - EQV, -57 - EQV, -40 - EQV,
        -57 - EQV, -36 - EQV, 18 - EQV,  -28 - EQV, -19 - EQV, -47 - EQV, -31 - EQV, -34 - EQV, -39 - EQV, -23 - EQV,
        16 - EQV,  27 - EQV,  -15 - EQV, -6 - EQV,  -9 - EQV,  -17 - EQV, -10 - EQV, -5 - EQV,  22 - EQV,  23 - EQV,
        30 - EQV,  16 - EQV,  16 - EQV,  23 - EQV,  36 - EQV,  32 - EQV,  33 - EQV,  28 - EQV,  22 - EQV,  43 - EQV,
        5 - EQV,   32 - EQV,  20 - EQV,  41 - EQV,
    };

    /*
     * Early king table:
     *
     * -65,  23,  16, -15, -56, -34,   2,  13,
     *  29,  -1, -20,  -7,  -8,  -4, -38, -29,
     *  -9,  24,   2, -16, -20,   6,  22, -22,
     * -17, -20, -12, -27, -30, -25, -14, -36,
     * -49,  -1, -27, -39, -46, -44, -33, -51,
     * -14, -14, -22, -46, -44, -30, -15, -27,
     *   1,   7,  -8, -64, -43, -16,   9,   8,
     * -15,  36,  12, -54,   8, -28,  24,  14,
     */
    static constexpr const int early_white_king[64] = {
        -15, 36,  12,  -54, 8,   -28, 24,  14,  1,   7,   -8,  -64, -43, -16, 9,   8,   -14, -14, -22, -46, -44, -30,
        -15, -27, -49, -1,  -27, -39, -46, -44, -33, -51, -17, -20, -12, -27, -30, -25, -14, -36, -9,  24,  2,   -16,
        -20, 6,   22,  -22, 29,  -1,  -20, -7,  -8,  -4,  -38, -29, -65, 23,  16,  -15, -56, -34, 2,   13,
    };

    static constexpr const int early_black_king[64] = {
        65,  -23, -16, 15, 56, 34, -2, -13, -29, 1,  20, 7,  8,  4,   38,  29, 9,  -24, -2,  16,  20, -6,
        -22, 22,  17,  20, 12, 27, 30, 25,  14,  36, 49, 1,  27, 39,  46,  44, 33, 51,  14,  14,  22, 46,
        44,  30,  15,  27, -1, -7, 8,  64,  43,  16, -9, -8, 15, -36, -12, 54, -8, 28,  -24, -14,
    };

    /*
     * End king table:
     *
     * -74, -35, -18, -18, -11,  15,   4, -17,
     * -12,  17,  14,  17,  17,  38,  23,  11,
     *  10,  17,  23,  15,  20,  45,  44,  13,
     *  -8,  22,  24,  27,  26,  33,  26,   3,
     * -18,  -4,  21,  24,  27,  23,   9, -11,
     * -19,  -3,  11,  21,  23,  16,   7,  -9,
     * -27, -11,   4,  13,  14,   4,  -5, -17,
     * -53, -34, -21, -11, -28, -14, -24, -43,
     */
    static constexpr const int end_white_king[64] = {
        -53, -34, -21, -11, -28, -14, -24, -43, -27, -11, 4,  13, 14,  4,   -5,  -17, -19, -3, 11, 21,  23, 16,
        7,   -9,  -18, -4,  21,  24,  27,  23,  9,   -11, -8, 22, 24,  27,  26,  33,  26,  3,  10, 17,  23, 15,
        20,  45,  44,  13,  -12, 17,  14,  17,  17,  38,  23, 11, -74, -35, -18, -18, -11, 15, 4,  -17,
    };

    static constexpr const int end_black_king[64] = {
        74,  35,  18, 18,  11,  -15, -4,  17,  12,  -17, -14, -17, -17, -38, -23, -11, -10, -17, -23, -15, -20, -45,
        -44, -13, 8,  -22, -24, -27, -26, -33, -26, -3,  18,  4,   -21, -24, -27, -23, -9,  11,  19,  3,   -11, -21,
        -23, -16, -7, 9,   27,  11,  -4,  -13, -14, -4,  5,   17,  53,  34,  21,  11,  28,  14,  24,  43,
    };

    static constexpr const int *square_tables[24] = {
        early_white_pawn,   end_white_pawn,     early_black_pawn, end_black_pawn,     early_white_knight,
        end_white_knight,   early_black_knight, end_black_knight, early_white_bishop, end_white_bishop,
        early_black_bishop, end_black_bishop,   early_white_rook, end_white_rook,     early_black_rook,
        end_black_rook,     early_white_queen,  end_white_queen,  early_black_queen,  end_black_queen,
        early_white_king,   end_white_king,     early_black_king, end_black_king,
    };

} // namespace ai::tables