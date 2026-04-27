#version 330
in vec3 fragWorldPos;
in vec4 fragColor;

out vec4 finalColor;

uniform vec3 BS_Pos;

vec3 jet(float t) {
    return vec3(
        clamp(1.5 - abs(4.0 * t - 3.0), 0.0, 1.0), // Red
        clamp(1.5 - abs(4.0 * t - 2.0), 0.0, 1.0), // Green
        clamp(1.5 - abs(4.0 * t - 1.0), 0.0, 1.0)  // Blue
    );
}

vec3 heatmapHot(float t) {
    t = clamp(t, 0.0, 1.0);
    return vec3(
        t,                  // Red increases to max at 1.0
        sin(t * 3.14159),   // Green peaks in the middle (0.5)
        1.0 - t             // Blue decreases to 0 at 1.0
    );
}

float hash(vec2 p) {
    p = fract(p * vec2(127.1, 311.7));
    return fract(sin(dot(p, vec2(129.5, 393.2))) * 43758.5453123);
}

// Box-Muller Transform to get Gaussian Noise N(0, sigma)
float getGaussianNoise(vec2 p, float sigma) {
    float u1 = hash(p);
    float u2 = hash(p + 0.1); // Offset to get a second independent random number
    // Box-Muller transform
    float z = sqrt(-2.0 * log(u1)) * cos(6.28318530718 * u2);
    return z * sigma;
}

void main() {
    float T_Power = 10.0; // dBm
    float frequency = 2.4; // GHz
    float T_Gain = 1.0;
    float R_Gain = 1.0;

    // Office LoS
    float alpha = 1.47;
    float beta = 34.17;
    float gamma = 2.08;

    // Office NLoS
    //float alpha = 2.39;
    //float beta = 30.13;
    //float gamma = 2.40;

    float sigma = 4.0; 
    float noise = getGaussianNoise(fragWorldPos.xy, sigma);

    float d = max(distance(fragWorldPos, BS_Pos), 0.001); 

    float log10d = log(d) / log(10.0);
    float log10f = log(frequency) / log(10.0);

    float L = 10.0 * alpha * log10d + beta + 10.0 * gamma * log10f;

    float R_Power = T_Power + T_Gain - L + R_Gain + noise;

    float minDBM = -90.0;
    float maxDBM = -30.0;
    float range = maxDBM - minDBM;

    float normalized = clamp((R_Power - minDBM) / range, 0.0, 1.0);

    finalColor = vec4(jet(normalized), 1.0);
}
