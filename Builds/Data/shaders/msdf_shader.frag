uniform sampler2D tex;
uniform vec4 color;
 

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    vec4 Sample = texture2D(tex, gl_TexCoord[0].st);

    float r = Sample.r;
    float g = Sample.g;
    float b = Sample.b;

    float median = max(min(r, g), min(max(r, g), b));
    float signed_dist = median - 0.5;
    float d = fwidth(signed_dist);
    float opacity = smoothstep(-d, d, signed_dist);

    gl_FragColor = vec4(color.rgb, opacity * color.a);
}