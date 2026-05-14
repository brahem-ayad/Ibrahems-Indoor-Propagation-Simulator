#version 330

in vec3 fragWorldPos;
in vec4 fragColor;

out vec4 finalColor;

uniform vec3 cameraPos;

void main() {
    vec3 viewDir = normalize(cameraPos);
    vec3 pointDir = normalize(fragWorldPos);

    float alignment = dot(viewDir, pointDir);

    float intensity = (1.0 - alignment) * 0.25; 

    intensity = clamp(1.0 - intensity, 0.0, 1.0);

    finalColor = vec4(fragColor.rgb, intensity);
}
