#version 330
layout (location = 0) in vec3 vsiPosition;
layout (location = 1) in vec3 vsiColor;

out vec4 vsoColor;

uniform mat4 projectionMatrix, modelViewMatrix;
out vec3 vsoNormal;

void main(void) {
  vsoNormal = (transpose(inverse(modelViewMatrix))  * vec4(vsiPosition, 1.0)).xyz;
  gl_Position = projectionMatrix * modelViewMatrix * vec4(vsiPosition.xyz, 1.0);
  vsoColor = vec4(vsiColor, 1.0);  
}
