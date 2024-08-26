#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;

void main() {
  vec3 color = texture(screenTexture, TexCoord).rgb;

  // Calculate vignette effect
  float dist = length(TexCoord - vec2(0.5, 0.5));
  float vignette = smoothstep(0.5, 0.9, dist);

  color *= 1.0 - vignette; // Darken edges

  FragColor = vec4(color, 1.0);
}

