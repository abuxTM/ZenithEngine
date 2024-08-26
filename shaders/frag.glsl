#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D aTexture;
uniform bool textured = false;
uniform vec3 aColor = vec3(0.2, 0.1, 0.7);

void main() {
  if (textured) {
    FragColor = texture(aTexture, TexCoord);
  } else {
    FragColor = vec4(aColor, 1.0f);
  }
}
