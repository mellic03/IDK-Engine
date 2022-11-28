#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 9) out;

in VS_OUT {
  vec3 normal;
  mat3 nmat;
  mat4 projection, view, model;
} gs_in[];

const float MAGNITUDE = 0.1;
  
uniform mat4 projection;

void GenerateLine(int index)
{
  gl_Position = gl_in[index].gl_Position;
  EmitVertex();
  gl_Position = (gl_in[index].gl_Position + vec4(gs_in[index].nmat * gs_in[index].normal, 0.0) * MAGNITUDE);
  EmitVertex();
  EndPrimitive();
}

void main()
{
  // gl_Position = gl_in[0].gl_Position;
  // EmitVertex();
  // gl_Position = gl_in[1].gl_Position;
  // EmitVertex();
  // gl_Position = gl_in[2].gl_Position;
  // EmitVertex();
  // EndPrimitive();

  GenerateLine(0);
  GenerateLine(1);
  GenerateLine(2);
} 