#version 410
out vec4 color;
in float v_charge;
in float v_mass;
flat in vec4 center;
uniform sampler2D tex;
void main()
{

  vec2 N;
  N =gl_PointCoord*2.0-vec2(1.0f);
 float mag = dot(N, N);
if (mag > 1.0f) discard;


  if(v_charge>0.1)
  {
    color=vec4(1.0f,0.0f,0.0f,1.0f);
  }
  else{
    color=vec4(0.3f,1.0f,1.0f,1.0f);
  }


}
