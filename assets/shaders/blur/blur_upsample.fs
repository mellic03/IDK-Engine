#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D inputTexture0;
uniform sampler2D inputTexture1;



void main()
{
  vec2 tsize = textureSize(inputTexture0, 0);

  float offset_x = 1.0 / tsize.x;
  float offset_y = 1.0 / tsize.y;
  vec2 offsets[9] = vec2[](
    vec2(-offset_x,  offset_y), // top-left
    vec2( 0.0f,      offset_y), // top-center
    vec2( offset_x,  offset_y), // top-right
    vec2(-offset_x,  0.0f),   // center-left
    vec2( 0.0f,      0.0f),   // center-center
    vec2( offset_x,  0.0f),   // center-right
    vec2(-offset_x, -offset_y), // bottom-left
    vec2( 0.0f,     -offset_y), // bottom-center
    vec2( offset_x, -offset_y)  // bottom-right    
  );

  float ImageKernel[9] = float[](
      1.0/16.0, // top-left
      2.0/16.0, // top-center
      1.0/16.0, // top-right
      2.0/16.0,   // center-left
      4.0/16.0,   // center-center
      2.0/16.0,   // center-right
      1.0/16.0, // bottom-left
      2.0/16.0, // bottom-center
      1.0/16.0  // bottom-right    
  );


  vec3 sampleTex[9];
  for(int i = 0; i < 9; i++)
    sampleTex[i] = texture(inputTexture0, TexCoords.st + offsets[i]).rgb + texture(inputTexture1, TexCoords + offsets[i]/2.0).rgb;

  vec3 RenderWithKernel = vec3(0.0);

  for(int i = 0; i < 9; i++)
    RenderWithKernel += sampleTex[i] * ImageKernel[i];


  FragColor = vec4(RenderWithKernel, 1.0);
  // FragColor = texture(inputTexture0, TexCoords) + texture(inputTexture1, TexCoords);
}
