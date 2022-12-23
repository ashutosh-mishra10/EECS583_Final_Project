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
void gemm3(float input[64][32], float bias[64], float weights[32][12544], float output[64][12544], float min_tr, float max_tr) {
    for (int m = 0; m < 12544; m++) {
        float acc[64];
        for (int n = 0; n < 64; n++) {
            // Vectorized load
            // count: 64*12544/8192=98
            // stride: 1
            // cache line: stride(1) * 4 * 8192 / 64 = 512
            // latency.dataaccess: (cache line (512) * memAccessLatency (0.43)) * count (98) = 21575.68
            // latency.compute: load_FP32 latency (32) * count (98) = 3136
            acc[n] = bias[n];
        }
        for (int k = 0; k < 32; k++) {
            for (int n = 0; n < 64; n++) {
                // Vectorized load
                // count: 64 * 32 * 12544/8192=3136
                // stride: 32
                // cache line: stride(32) * 4 * 8192 / 64 = 16384 [but we consider 8192]
                // latency.dataaccess: (cache line (8192) * memAccessLatency (0.43)) * count (3136) = 11046748.16
                // latency.compute: load_FP32 latency (32) * count (3136) = 100352
                float input_val = input[n][k];

                // Vectorized load
                // count: 64 * 32 * 12544/8192=3136
                // stride: 0 [but we consider 1]
                // cache line: stride(1) * 8192 * 4 / 64 = 512
                // latency.dataaccess: (cache line (512) * memAccessLatency (0.43)) * count (3136) = 690421.76
                // latency.compute: load_FP32 latency (32) * count (3136) = 100352
                float weight_val = weights[k][m];

                // Vectorized multiplication
                // count: 64 * 32 * 12544/8192=3136
                // latency.compute: mult_FP32 latency (346) * count (3136) = 1085056
                float mult_val = input_val * weight_val;

                // Vectorized addition
                // count: 64 * 32 * 12544/8192=3136
                // latency.compute: add_FP32 latency (351) * count (3136) = 1100736
                acc[n] = acc[n] + mult_val;
            }
        }
        for (int n = 0; n < 64; n++) {
            // Vectorized maximum
            // count: 64*12544/8192=98
            // latency.compute: max_FP32 latency (128) * count (98) = 12544
            float max_val = max(acc[n], min_tr);

            // Vectorized minimum
            // count: 64*12544/8192=98
            // latency.compute: min_FP32 latency (128) * count (98) = 12544
            float min_val = min(max_val, max_tr);

            // Vectorized store
            // count: 64*12544/8192=98
            // stride: 12544
            // cache line: stride(12544) * 4 * 8192 / 64 = 6422528 [but we consider 8192]
            // latency.dataaccess: (cache line (8192) * memAccessLatency (0.43)) * count (98) = 345210.88
            // latency.compute: store_FP32 latency (32) * count (98) = 3136
            output[n][m] = min_val;
        }
    }
}

// total latency.compute (calculated): 2417856
// total latency.memory (calculated): 12103956.48