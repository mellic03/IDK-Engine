#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec3 gEmission;

in vec3 FragPos;
in vec2 TexCoords;
in vec4 Color;
in vec3 SurfaceNormal;
in mat3 TBN;

struct TerrainMaterial {
  sampler2D   diffuseMap1,   diffuseMap2,   diffuseMap3,   diffuseMap4;
  sampler2D   specularMap1,  specularMap2,  specularMap3,  specularMap4;
  sampler2D   normalMap1,    normalMap2,    normalMap3,    normalMap4;
  float spec_exponent;
};
uniform TerrainMaterial material;


vec4 hash4( vec2 p ) { return fract(sin(vec4( 1.0+dot(p,vec2(37.0,17.0)), 
                                              2.0+dot(p,vec2(11.0,47.0)),
                                              3.0+dot(p,vec2(41.0,29.0)),
                                              4.0+dot(p,vec2(23.0,31.0))))*103.0); }

vec4 textureNoTile( sampler2D samp, in vec2 uv )
{
  vec2 p = floor( uv );
  vec2 f = fract( uv );

  // derivatives (for correct mipmapping)
  vec2 ddx = dFdx( uv );
  vec2 ddy = dFdy( uv );
  
  // voronoi contribution
  vec4 va = vec4( 0.0 );
  float wt = 0.0;
  for( int j=-1; j<=1; j++ )
  for( int i=-1; i<=1; i++ )
  {
    vec2 g = vec2( float(i), float(j) );
    vec4 o = hash4( p + g );
    vec2 r = g - f + o.xy;
    float d = dot(r,r);
    float w = exp(-5.0*d );
    vec4 c = textureGrad( samp, uv + o.zw, ddx, ddy );
    va += w*c;
    wt += w;
  }

  // normalization
  return va/wt;
}


uniform float threshold;
uniform float epsilon;


void main()
{
  gPosition = vec4(FragPos, 1.0);


  vec3 sNormal = normalize(SurfaceNormal);

  float alpha = 1.0 - smoothstep(threshold - epsilon, threshold + epsilon, clamp(sNormal.y, 0.0, 1.0));


  vec4 normal4 = mix( textureNoTile(material.normalMap1, TexCoords),
                      textureNoTile(material.normalMap2, TexCoords), alpha );
                //  Color.b * texture(material.normalMap3, TexCoords);
                //  Color.a * texture(material.normalMap4, TexCoords);
  vec3 normal = normal4.xyz;

  normal = normal * 2.0 - 1.0; 
  normal = normalize(TBN * normal);
  gNormal = vec4(normal, 1.0);
  gNormal = vec4(normal, 1.0);



  gAlbedoSpec.rgb = mix(  textureNoTile(material.diffuseMap1, TexCoords).rgb,
                          textureNoTile(material.diffuseMap2, TexCoords).rgb, alpha );
                    // Color.b * texture(material.diffuseMap3, TexCoords).rgb;
                    // Color.a * texture(material.diffuseMap4, TexCoords).rgb;

  gAlbedoSpec.a = mix(  textureNoTile(material.diffuseMap1, TexCoords).rgb,
                        textureNoTile(material.diffuseMap2, TexCoords).rgb, alpha ).r;
  // gAlbedoSpec.a = 1.0;

  gEmission = vec3(0.0);

}
