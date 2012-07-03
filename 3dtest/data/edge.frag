#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect input;

vec4 edge(sampler2DRect tex, vec2 pos, float incr)
{
    return 4*texture2DRect(tex, pos)
        - texture2DRect(tex, vec2(pos.x - incr, pos.y))
        - texture2DRect(tex, vec2(pos.x + incr, pos.y))
        - texture2DRect(tex, vec2(pos.x, pos.y - incr))
        - texture2DRect(tex, vec2(pos.x, pos.y + incr));
}

void main()
{
    vec2 pos = vec2(gl_TexCoord[0].s, gl_TexCoord[0].t);
    vec4 e = edge(input, pos, 1);
    if (length(e.rgb) > 0.1 || e.a > 0.008)
        gl_FragColor = vec4(0, 0, 0, 1);
    else
        gl_FragColor = vec4(1, 1, 1, 1);;
}
