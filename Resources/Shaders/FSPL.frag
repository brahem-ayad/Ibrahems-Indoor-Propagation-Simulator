#version 330
in vec3 fragWorldPos;
in vec4 fragColor;

out vec4 finalColor;

uniform vec2 mousePos;
uniform float frequency;
uniform float TP;
uniform int stepped;

float map(float RP, float TP){
  return (RP+90.0)/(TP+90.0);
}

float step_map(float RP){
  if(RP > -40) return 1.0;
  else if(RP > -50) return 0.9;
  else if(RP > -60) return 0.8;
  else if(RP > -70) return 0.5;
  else if(RP > -80) return 0.1;
  else if(RP > -90) return 0.05;
  else return 0.0;
}

void main() {

  float d = distance(gl_FragCoord.xy, mousePos);

  float FSPL = 20.0 * log(d/1000)/log(10.0) + 20.0 * log(frequency)/log(10.0) + 92.45; // distance in km and frequency in GHz

  float RP = TP - FSPL;

  if(stepped == 1) finalColor = vec4(1.0, 1.0, 1.0, step_map(RP));
  else finalColor = vec4(1.0, 1.0, 1.0, map(RP, TP));
}
