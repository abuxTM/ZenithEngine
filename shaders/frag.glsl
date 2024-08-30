#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D aTexture;
uniform bool textured = false;
uniform bool lighting = false;
uniform vec3 aColor = vec3(0.2, 0.1, 0.7);
uniform vec3 lightPos = vec3(0.0, 4.0, 2.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform float ambientIntensity = 0.7;
uniform float time;

// Threshold for alpha discard
const float alphaThreshold = 0.1;

void main() {
    vec4 texColor;
    vec3 resultColor; // Declare resultColor outside conditionals

    if (textured) {
        texColor = texture(aTexture, TexCoord);
        
        // Discard fragments with low alpha
        if (texColor.a < alphaThreshold) {
            discard;
        }

        if (lighting) {
            // Ambient lighting
            vec3 ambient = ambientIntensity * lightColor;

            // Diffuse lighting
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;

            // Combine ambient and diffuse components with texture
            resultColor = (ambient + diffuse) * texColor.rgb;
        } else {
            // Only texture color without lighting
            resultColor = texColor.rgb;
        }
    } else {
        if (lighting) {
            // Ambient lighting
            vec3 ambient = ambientIntensity * lightColor;

            // Diffuse lighting
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;

            // Combine ambient and diffuse components with solid color
            resultColor = (ambient + diffuse) * aColor;
        } else {
            // Only solid color without lighting
            resultColor = aColor;
        }
    }

    FragColor = vec4(resultColor, 1.0f); // Output final color with full alpha
}

