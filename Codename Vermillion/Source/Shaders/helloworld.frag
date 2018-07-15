uniform sampler2D tex;
uniform vec4 color;
 
void main()
{
    vec4 texColor = vec4( texture2D(tex,gl_TexCoord[0].st).r );
    gl_FragColor = color * texColor;
}