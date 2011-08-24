uniform float time;
uniform vec2 resolution;
uniform vec4 mouse;
uniform float wave;

void main(void)
{
    float itime = time * 0.001;
    float iwave = abs(wave);

    //the centre point for each blob
    vec2 move1;
    move1.x = cos(itime*sin(iwave/20.0))*0.4;
    move1.y = sin(itime*1.5)*0.4;
    vec2 move2;
    move2.x = cos(itime*2.0)*0.4;
    move2.y = sin(itime*3.0)*0.4;

    //screen coordinates
    vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;

    //radius for each blob
    float r1 =(dot(p-move1,p-move1))*8.0;
    float r2 =(dot(p+move2,p+move2))*16.0;

    //sum the meatballs
    float metaball =(1.0/r1+1.0/r2);
    //alter the cut-off power
    float col = pow(metaball,8.0);

    //set the output color
    gl_FragColor = vec4(col,col,col,1.0);
//    gl_FragColor = vec4(sin(10.0*itime),cos(10.0*itime),col,1.0);
//    gl_FragColor = vec4(col*sin(wave),col*wave,col*sin(itime),1.0);
//    gl_FragColor = vec4(col,col*sin(0.1*iwave),col*sin(itime),1.0);
    gl_FragColor = vec4(sin(col*itime),col*sin(0.1*wave),col,1.0);
}
