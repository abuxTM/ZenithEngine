#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform float vignetteIntensity = 0.9;
uniform float vignetteInner = 0.5;

void main() {
  vec3 color = texture(screenTexture, TexCoord).rgb;

  // Calculate vignette effect
  float dist = length(TexCoord - vec2(0.5, 0.5));
  float vignette = smoothstep(vignetteInner, vignetteIntensity, dist);

  color *= 1.0 - vignette; // Darken edges

  FragColor = vec4(color, 1.0);
}

