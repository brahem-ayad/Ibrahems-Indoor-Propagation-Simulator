#version 330

in vec3 vertexPosition;
in vec4 vertexColor;

uniform mat4 mvp;

out vec3 fragWorldPos;
out vec4 fragColor;

void main()
{
    fragWorldPos = vertexPosition;
    fragColor    = vertexColor;
    gl_Position  = mvp * vec4(vertexPosition, 1.0);
}
