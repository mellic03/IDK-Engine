#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
  vec3 FragPos; vec4 FragPosLightSpace;
  vec3 SurfaceNormal;
  vec2 TexCoords;
  
  vec3 DIR_TangentLightPositions[];
  vec3 DIR_TangentViewPositions[];
  vec3 DIR_TangentFragPositions[];

  vec3 POINT_TangentLightPositions[];
  vec3 POINT_TangentViewPositions[];
  vec3 POINT_TangentFragPositions[];

  vec3 SPOT_TangentLightPositions[];
  vec3 SPOT_TangentViewPositions[];
  vec3 SPOT_TangentFragPositions[];

} gs_in[];

out vec2 TextureCoords; 


vec4 explode(vec4 position, vec3 normal)
{
  float magnitude = 2.0;
  vec3 direction = normal * magnitude; 
  return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}

void main() {    
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    TextureCoords = gs_in[0].TexCoords;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    TextureCoords = gs_in[1].TexCoords;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    TextureCoords = gs_in[2].TexCoords;
    EmitVertex();
    EndPrimitive();
}  