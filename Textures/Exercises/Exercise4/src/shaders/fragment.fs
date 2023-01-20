#version 330 core

in vec3 vertexColor;
in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D aTexture;
uniform sampler2D bTexture;
uniform float mixLevel;

void main() {
  fragColor = mix(texture(aTexture, TexCoord), texture(bTexture, vec2(-TexCoord.x, TexCoord.y)), mixLevel) * vec4(vertexColor, 1.0f);
}
