#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D inputTexture;

void main()
{
  vec2 tsize = textureSize(inputTexture, 0);

  float offset_x = (1.0 / tsize.x);
  float offset_y = (1.0 / tsize.y);
  vec2 offsets[4] = vec2[](
    vec2( 0,         offset_y),
    vec2( 0,        -offset_y),
    vec2(-offset_x,  0),
    vec2( offset_x,  0)
  );

  float ImageKernel[4] = float[](
    1.0/4.0, // top-left
    1.0/4.0, // top-right
    1.0/4.0, // bottom-left
    1.0/4.0  // bottom-right    
  );

  vec3 sampleTex[4];
  for(int i = 0; i < 4; i++)
    sampleTex[i] = vec3(texture(inputTexture, TexCoords.st + offsets[i]));

  vec3 RenderWithKernel = vec3(0.0);

  for(int i = 0; i < 4; i++)
    RenderWithKernel += sampleTex[i] * ImageKernel[i];


  FragColor = vec4(RenderWithKernel, 1.0);
  // FragColor = texture(inputTexture, TexCoords);
}
