uniform sampler2D tex;
uniform vec4 color;
 
void main()
{
    float dist = texture2D(tex, gl_TexCoord[0].st).r;

	const float smoothing = 1.0/16.0;
    float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, dist);

    gl_FragColor = vec4(color.rgb, alpha * color.a);
}