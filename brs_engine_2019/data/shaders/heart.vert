//uniform float time;
//uniform vec2 resolution;

void main(void)
{
//  texcoord = gl_MultiTexCoord0.st;
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
