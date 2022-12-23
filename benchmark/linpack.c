void linpack(float *in_a, float *in_b, float *in_c, float *out) {
    for (int i = 0; i < 65536; i++) {
        out[i] = in_c[i] + in_a[0] * in_b[i];
    }
}

// Simulated compute: 6600
// Simulated dataaccess: 17051