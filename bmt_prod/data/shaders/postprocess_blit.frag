uniform sampler2D tex0;

void main(void)
{
    vec2 p = gl_TexCoord[0].xy;
    gl_FragColor = gl_Color * texture2D(tex0, p);
}
