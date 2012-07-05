#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect input;

float avg(sampler2DRect tex, vec2 pos)
{
    vec4 c = texture2DRect(tex, pos);
    return c.a * (c.r + c.g + c.b) / 3.0;
}
vec2 sobel(sampler2DRect tex, vec2 pos, float incr)
{
    return normalize(vec2(
        // left
        - avg(tex, vec2(pos.x - incr, pos.y       )) * 2
        - avg(tex, vec2(pos.x - incr, pos.y - incr))
        - avg(tex, vec2(pos.x - incr, pos.y + incr))
        // right
        + avg(tex, vec2(pos.x + incr, pos.y       )) * 2
        + avg(tex, vec2(pos.x + incr, pos.y - incr))
        + avg(tex, vec2(pos.x + incr, pos.y + incr)),

        // up
        + avg(tex, vec2(pos.x,        pos.y - incr)) * 2
        + avg(tex, vec2(pos.x - incr, pos.y - incr))
        + avg(tex, vec2(pos.x + incr, pos.y - incr))
        // down
        - avg(tex, vec2(pos.x,        pos.y + incr)) * 2
        - avg(tex, vec2(pos.x - incr, pos.y + incr))
        - avg(tex, vec2(pos.x + incr, pos.y + incr))
    ));
}

void main()
{
    vec2 pos = gl_TexCoord[0].st;
    gl_FragColor = vec4(0.5*sobel(input, pos, 1) + 0.5, 0, 1);
}

