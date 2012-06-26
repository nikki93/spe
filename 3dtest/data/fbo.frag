#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect fbo;

vec4 sobel(sampler2DRect tex, vec2 pos, float incr)
{
	return 4*texture2DRect(tex, pos)
		- texture2DRect(tex, vec2(pos.x - incr, pos.y))
		- texture2DRect(tex, vec2(pos.x + incr, pos.y))
		- texture2DRect(tex, vec2(pos.x, pos.y - incr))
		- texture2DRect(tex, vec2(pos.x, pos.y + incr));
}

void main()
{
    vec2 pos = vec2(gl_TexCoord[0].s, 768 - gl_TexCoord[0].t);
    vec4 e = sobel(fbo, pos, 1);
    if (length(e) > 0.1)
        gl_FragColor = vec4(0, 0, 0, 1);
    else
        gl_FragColor = vec4(1, 1, 1, 1);
}
