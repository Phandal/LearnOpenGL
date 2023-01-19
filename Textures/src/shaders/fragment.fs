#version 330 core

in vec3 vertexColor;
in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D aTexture;
uniform sampler2D bTexture;

void main() {
  fragColor = mix(texture(aTexture, TexCoord), texture(bTexture, TexCoord), 0.2) * vec4(vertexColor, 1.0f);
}
