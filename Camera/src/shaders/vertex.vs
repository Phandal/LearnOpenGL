#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 vertexColor;
out vec2 TexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 perspectiveMatrix;

void main(){
  gl_Position = perspectiveMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
  vertexColor = aColor;
  TexCoord = aTexCoord;
}
