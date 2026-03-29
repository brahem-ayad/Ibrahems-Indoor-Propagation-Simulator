#version 330

in vec3 fragWorldPos;
in vec4 fragColor;

out vec4 finalColor;

void main()
{
    vec3 n = normalize(cross(dFdx(fragWorldPos), dFdy(fragWorldPos)));

    float brightness = 0.35
        + max( n.z,  0.0) * 0.80
        + max(-n.z,  0.0) * 0.10
        + max( n.y,  0.0) * 0.50
        + max(-n.y,  0.0) * 0.50
        + max( n.x,  0.0) * 0.30
        + max(-n.x,  0.0) * 0.30;

    finalColor = vec4(fragColor.rgb * brightness, fragColor.a);
}
