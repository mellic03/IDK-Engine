#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
  vec3 normal;
  mat3 nmat;
  mat4 projection, view, model;
} vs_out;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0); 
  mat3 normalMatrix = mat3(transpose(inverse(view * model)));
  vs_out.normal = aNormal;
  vs_out.nmat = normalMatrix;
  vs_out.view = view;
  vs_out.model = model;

}