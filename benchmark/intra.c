#include <stdint.h>
void intra(int16_t ref_top[50261][17], int16_t ref_left[50261][17], int16_t dst[50261][8][8]) {

    int16_t top_right[50261];
    int16_t bottom_left[50261];
    int16_t hor[8][50261];
    int16_t acc[8][50261];

    for (int i = 0; i < 50261; i++) {
        top_right[i] = ref_top[i][9];
        bottom_left[i] = ref_left[i][9];
    }

    for (int j = 0; j < 8; ++j) {
        for (int i = 0; i < 50261; i++) {
            hor[j][i] = (ref_left[i][j + 1] << 3) + 8;
            acc[j][i] = ref_top[i][j + 1] << 3;
        }
    }

    for (int j = 0; j < 8; ++j) {
        for (int k = 0; k < 8; ++k) {
            for (int i = 0; i < 50261; i++) {
                hor[j][i] += top_right[i] - ref_left[i][j + 1];
                acc[k][i] += bottom_left[i] - ref_top[i][k + 1];
                dst[i][j][k] = (hor[j][i] + acc[k][i]) >> (3 + 1);
            }
        }
    }
}

// Simulated compute: 298994
// Simulated memory: 416784