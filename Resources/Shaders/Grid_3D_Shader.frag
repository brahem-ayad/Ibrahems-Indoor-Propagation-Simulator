#version 330

in vec3 fragWorldPos;
in vec4 fragColor;

uniform vec3 Camera_Target;
uniform vec3 Camera_Position;

out vec4 finalColor;

void main() {
  float dist = distance(fragWorldPos.xy, Camera_Target.xy);

  float radius = distance(Camera_Target, Camera_Position);

  float alpha = 1.0 - clamp(dist/radius, 0.0, 1.0);

  finalColor = vec4(fragColor.rgb, fragColor.a * alpha);
}
