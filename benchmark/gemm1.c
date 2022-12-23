
__attribute__((noinline))
float max(float a, float b)
{
    return a > b ? a : b;
}
__attribute__((noinline))
float min(float a, float b)
{
    return a < b ? a : b;
}
void gemm1(float input[64][32], float bias[64], float weights[32][12544], float output[64][12544], float min_tr, float max_tr) {
    for (int n = 0; n < 64; n++) {
        for (int m = 0; m < 12544; m++) {
            float acc = bias[n];
            for (int k = 0; k < 32; k++) {
                acc += input[n][k] * weights[k][m];
            }
            output[n][m] = min(max(acc, min_tr), max_tr);
        }
    }
}