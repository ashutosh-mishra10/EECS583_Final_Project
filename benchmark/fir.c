void fir(float src[65568], float coeff[32], float dst[65536]) {
    float acc[65536];
    for (int sample_idx = 0; sample_idx < 65536; sample_idx++) {
        // Vectorized set
        // count: 65536/8192=8
        // latency.compute: set_FP32 latency (32) * count (8) = 256
        acc[sample_idx] = 0;
    }
    for (int coeff_idx = 0; coeff_idx < 32; coeff_idx++) {
        for (int sample_idx = 0; sample_idx < 65536; sample_idx++) {
            // Vectorized load
            // count: 32*65536/8192=256
            // stride: 0
            // cache line: stride(0) * 8192 / 64 = 0 [but we consider 1]
            // latency.dataaccess: (cache line (1) * memAccessLatency (2)) * count (256) = 512
            // latency.compute: load_FP32 latency (32) * count (256) = 8192
            float coeff_temp = coeff[coeff_idx];

            // Vectorized load
            // count: 32*65536/8192=256
            // stride: 1
            // cache line: stride(1) * 8192 / 64 = 128
            // latency.dataaccess: (cache line (128) * memAccessLatency (2)) * count (256) = 65536
            // latency.compute: load_FP32 latency (32) * count (256) = 8192
            float src_temp = src[sample_idx + coeff_idx];

            // Vectorized multiplication
            // count: 32*65536/8192=256
            // latency.compute: mult_FP32 latency (346) * count (256) = 88576

            float mult_val = coeff_temp * src_temp;
            // Vectorized addition
            // count: 32*65536/8192=256
            // latency.compute: add_FP32 latency (351) * count (256) = 89856
            acc[sample_idx] = acc[sample_idx] + mult_val;
        }
    }
    int other_idx = 0;
    for (int sample_idx = 0; sample_idx < 65536; sample_idx++) {
        // Vectorized store
        // count: 65536/8192=8
        // stride: 1
        // cache line: stride(1) * 8192 / 64 = 128
        // latency.dataaccess: (cache line (128) * memAccessLatency (2)) * count (8) = 2048
        // latency.compute: store_FP32 latency (32) * count (8) = 256
        dst[sample_idx] = acc[other_idx];
        other_idx += 1;
    }
}

// total latency.compute (calculated): 195328
// total latency.compute (simulated): 195328

// total latency.memory (calculated): 68096
// total latency.memory (simulated): 26220