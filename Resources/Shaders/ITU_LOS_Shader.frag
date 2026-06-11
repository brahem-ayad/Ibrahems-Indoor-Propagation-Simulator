#version 330

// Input texture coordinates from the floor mesh (ranges from 0.0 to 1.0)
in vec2 fragTexCoord;
in vec3 fragWorldPos;

// The massive master texture grid containing your baked maps
uniform sampler2D Master_Visibility_Polygon;

#define MAX_TRANSMITTERS 32
uniform vec3 u_BS_Positions[MAX_TRANSMITTERS];
uniform int u_transmitterCount;

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
    float u1 = max(hash(p), 1e-6);
    float u2 = hash(p + 0.1); // Offset to get a second independent random number
    // Box-Muller transform
    float z = sqrt(-2.0 * log(u1)) * cos(6.28318530718 * u2);
    return z * sigma;
}

void main() {
    float T_Power = 10.0;
    float frequency = 2.4;
    float T_Gain = 1.0;
    float R_Gain = 1.0;
    float alpha;
    float beta;
    float gamma;
    float max_distance;

    float cols = 8.0;
    float rows = 4.0;

    vec2 cellUV = fragTexCoord / vec2(cols, rows);

    float colIndex = 0.0;
    float rowIndex = 0.0;

    vec4 texelColor;

    float value = -90.0;

    for(int i = 0; i < int(cols); i++){
    for(int j = 0; j < int(rows); j++){

      int k = int(j) * int(cols) + int(i);

      if(k >= u_transmitterCount) continue;

      //colIndex = i;
      //rowIndex = j;

      //cellUV.x += colIndex / cols;
      //cellUV.y += rowIndex / rows;

      vec2 baseUV = fragTexCoord / vec2(cols, rows);
      vec2 cellUV = baseUV + vec2(float(i) / cols, float(j) / rows);

      vec4 tempTexelColor = texture(Master_Visibility_Polygon, cellUV);
      if(tempTexelColor.r > 0.5){
        //texelColor = tempTexelColor;
        alpha = 1.47;
        beta = 34.17;
        gamma = 2.08;
        max_distance = 27;
      }
      else{
        alpha = 2.39;
        beta = 30.13;
        gamma = 2.40;
        max_distance = 30.0;
      }

      float sigma = 4.0;
      float noise = getGaussianNoise(fragWorldPos.xy, sigma);

      float d = max(distance(fragWorldPos, u_BS_Positions[k]), 0.001);

      float log10d = log(d) / log(10.0);
      float log10f = log(frequency) / log(10.0);

      float L = 10.0 * alpha * log10d + beta + 10.0 * gamma * log10f;

      float R_Power = T_Power + T_Gain - L + R_Gain + noise;

      if(R_Power > value){
        value = R_Power;
      }

    }}

      float minDBM = -90.0;
      float maxDBM = -30.0;
      float range = maxDBM - minDBM;

      float normalized = clamp((value - minDBM) / range, 0.0, 1.0);

    finalColor = vec4(jet(normalized), 1.0);
}
