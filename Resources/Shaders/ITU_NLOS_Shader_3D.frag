#version 330
in vec3 fragWorldPos;
in vec4 fragColor;

out vec4 finalColor;

vec3 jet(float t) {
    return vec3(
        clamp(1.5 - abs(4.0 * t - 3.0), 0.0, 1.0), // Red
        clamp(1.5 - abs(4.0 * t - 2.0), 0.0, 1.0), // Green
        clamp(1.5 - abs(4.0 * t - 1.0), 0.0, 1.0)  // Blue
    );
}

float hash(vec2 p) {
    p = fract(p * vec2(127.1, 311.7));
    return fract(sin(dot(p, vec2(129.5, 393.2))) * 43758.5453123);
}

// Box-Muller Transform to get Gaussian Noise N(0, sigma)
float getGaussianNoise(vec2 p, float sigma) {
    float u1 = hash(p);
    float u2 = hash(p + 0.1); 
    float z = sqrt(-2.0 * log(u1)) * cos(6.28318530718 * u2);
    return z * sigma;
}

void main() {
    // Constant parameters
    float T_Power = 10.0; // dBm
    float frequency = 2.4; // GHz
    float T_Gain = 1.0;
    float R_Gain = 1.0;
    float sigma = 4.0;

    // Path Loss Model Parameters
    float alpha;
    float beta;
    float gamma;
    float max_distance;


    alpha = 2.39;
    beta = 30.13;
    gamma = 2.40;
    max_distance = 30.0;

    // Precalculate logarithmic components constant for all calculations
    float log10f = log(frequency) / log(10.0);

    // We want the absolute maximum received power at this fragment's position
    float maxRPower = -9999.0; 
    float targetDistance = 0.0; 

    // Loop through all active transmitters
    for (int i = 0; i < u_transmitterCount; i++) {
        float d = max(distance(fragWorldPos, u_BS_Positions[i]), 0.001);

        float log10d = log(d) / log(10.0);
        float L = 10.0 * alpha * log10d + beta + 10.0 * gamma * log10f;

        float R_Power = T_Power + T_Gain - L + R_Gain;

        // Track the dominant transmitter signal
        if (R_Power > maxRPower) {
            maxRPower = R_Power;
            targetDistance = d; // Track distance of the dominant station for opacity falloff
        }
    }

    // Add environmental Gaussian shadowing noise based on fragment space
    float noise = getGaussianNoise(fragWorldPos.xy, sigma);
    maxRPower += noise;

    // Normalizing signal to map to heat colors
    float minDBM = -90.0;
    float maxDBM = -30.0;
    float range = maxDBM - minDBM;
    float normalized = clamp((maxRPower - minDBM) / range, 0.0, 1.0);

    // Opacity calculation based on the distance to the dominant transmitter
    float opacity;
    if(targetDistance < max_distance - 1.0) {
        opacity = 1.0;
    }
    else if(targetDistance < max_distance + 1.0) {
        opacity = smoothstep(max_distance + 1.0, max_distance - 1.0, targetDistance);
    }
    else {
        opacity = 0.0;
    }

    finalColor = vec4(jet(normalized), opacity);
}
