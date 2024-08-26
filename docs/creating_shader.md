## How to create simple shader (Step by Step)

# Create very simple vertex and fragment shader

```
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.3f, 0.1f, 0.8f, 1.f);\n"
    "}\0";
```

# 1) Creating VBO
```
GLuint VBO; // Create VBO to store our buffer data in
glGenBuffers(1, &VBO); // Generate buffer ID
glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind buffer to GL_ARRAY_BUFFER
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Apply vertices array to the buffer
```

# 2) Creating Vertex Shader
```
GLuint vertexShader;
vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create new vertex shader object

// Compile vertex source shader
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
glCompileShader(vertexShader);

```

# 3) Creating Fragment Shader
```

unsigned int fragmentShader;
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

// Compile fragment shader
glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
glCompileShader(fragmentShader);

```
# 4) Creating Program Shader
```
// Create shader program
unsigned int shaderProgram;
shaderProgram = glCreateProgram();

// Link vertex and fragment to our program
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);

// Use and cleanup
glUseProgram(shaderProgram);
glDeleteShader(vertexShader);
glDeleteShader(fragmentShader); 
```
