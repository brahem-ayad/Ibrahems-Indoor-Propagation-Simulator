#version 330

// Standard Raylib Mesh Input Layout Locations
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;
layout (location = 3) in vec4 vertexColor;

// Output attributes going into our fragment shader stage
out vec3 fragWorldPos;
out vec2 fragTexCoord;
out vec4 fragColor;

// Uniform transformation matrices automatically provided by Raylib
uniform mat4 mvp;
uniform mat4 matModel; // <-- Added this to get real world positioning

void main() {
    fragWorldPos = vec3(matModel * vec4(vertexPosition, 1.0));
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;

    // Standard camera matrix transformation projection
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
