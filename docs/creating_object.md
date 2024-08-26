## Creating a simple object using vertices

# 1) Creating VBO

```
GLuint VBO;
glGenBuffers(1, &VBO); // Generate buffer ID
glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind buffer to GL_ARRAY_BUFFER
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Apply vertices array to the buffer

```

# 2) Creating VAO
```
GLuint VAO;
glGenVertexArrays(1, &VAO); // Generate new VAO-ID

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
```

# 3) Drawing
```
shader.use();
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 3);
```

## Creating new mesh

simply copy triangle.h and triangle.cpp and just change vertices
