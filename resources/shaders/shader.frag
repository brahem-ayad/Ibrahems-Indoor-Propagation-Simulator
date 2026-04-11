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

    finalColor = vec4(fragColor.rgb * brightness, fragColor.a);
}
