#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aFaceNormal;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;

uniform vec3 diffuse;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragColor;

void main()
{
  gl_Position = projection * view * model * transform * vec4(aPos, 1.0);
  fragColor = 2 * diffuse;
}


#shader fragment

#version 330 core

in vec3 fragColor;
out vec4 FragColor;
  
void main()
{
  FragColor = vec4(fragColor, 1);
}