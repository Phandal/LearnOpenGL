#version 330 core

in vec3 vertexColor;
in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D aTexture;
uniform sampler2D bTexture;

void main() {
  fragColor = mix(texture(aTexture, TexCoord), texture(bTexture, vec2(-TexCoord.x, TexCoord.y)), 0.2) * vec4(vertexColor, 1.0f);
}
