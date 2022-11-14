#shader vertex

#version 330 core
layout (location = 0) in vec4 pos;   // the position variable has attribute position 0
out vec4 color;
void main()
{
  gl_Position = pos;
  color = vec4(0.5, 0.8, 0.6, 1);
}


#shader fragment 

#version 330 core
in vec4 color;
out vec4 FragColor;
void main()
{
  FragColor = color;
}