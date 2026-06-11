#version 330

in vec3 fragWorldPos;
in vec4 fragColor;

out vec4 finalColor;

void main()
{
    vec3 n = normalize(cross(dFdx(fragWorldPos), dFdy(fragWorldPos)));

    vec3 weights = abs(n);
 
    float totalWeight = weights.x + weights.y + weights.z;
    weights /= totalWeight;

    float xBright = 0.8;
    float yBright = 0.9;
    float zBright = 1.0;

    float brightness = (weights.x * xBright + weights.y * yBright + weights.z * zBright);

    float gradient = smoothstep(0.0, 3.0, fragWorldPos.z);
    float gradient2 = smoothstep(-0.5, 0.5, fragWorldPos.z);

    float test = mix(0.8, 1.0, gradient);
    float test2 = mix(0.8, 1.0, gradient2);

    finalColor = vec4(fragColor.rgb * brightness * test * test2 , fragColor.a);
}
