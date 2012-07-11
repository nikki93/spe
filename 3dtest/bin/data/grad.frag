#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect input;

float avg(sampler2DRect tex, vec2 pos)
{
    vec4 c = texture2DRect(tex, pos);
    return (c.r + c.g + c.b) / 3.0;
}
vec2 sobel_color(sampler2DRect tex, vec2 pos, float incr)
{
    return vec2(
        // left
        - avg(tex, vec2(pos.x - incr, pos.y       )) * 2
        - avg(tex, vec2(pos.x - incr, pos.y - incr))
        - avg(tex, vec2(pos.x - incr, pos.y + incr))
        // right
        + avg(tex, vec2(pos.x + incr, pos.y       )) * 2
        + avg(tex, vec2(pos.x + incr, pos.y - incr))
        + avg(tex, vec2(pos.x + incr, pos.y + incr)),

        // up
        - avg(tex, vec2(pos.x,        pos.y - incr)) * 2
        - avg(tex, vec2(pos.x - incr, pos.y - incr))
        - avg(tex, vec2(pos.x + incr, pos.y - incr))
        // down
        + avg(tex, vec2(pos.x,        pos.y + incr)) * 2
        + avg(tex, vec2(pos.x - incr, pos.y + incr))
        + avg(tex, vec2(pos.x + incr, pos.y + incr))
    );
}

vec2 sobel_depth(sampler2DRect tex, vec2 pos, float incr)
{
    return vec2(
        // left
        - texture2DRect(tex, vec2(pos.x - incr, pos.y       )).a * 2
        - texture2DRect(tex, vec2(pos.x - incr, pos.y - incr)).a
        - texture2DRect(tex, vec2(pos.x - incr, pos.y + incr)).a
        // right
        + texture2DRect(tex, vec2(pos.x + incr, pos.y       )).a * 2
        + texture2DRect(tex, vec2(pos.x + incr, pos.y - incr)).a
        + texture2DRect(tex, vec2(pos.x + incr, pos.y + incr)).a,

        // up
        - texture2DRect(tex, vec2(pos.x,        pos.y - incr)).a * 2
        - texture2DRect(tex, vec2(pos.x - incr, pos.y - incr)).a
        - texture2DRect(tex, vec2(pos.x + incr, pos.y - incr)).a
        // down
        + texture2DRect(tex, vec2(pos.x,        pos.y + incr)).a * 2
        + texture2DRect(tex, vec2(pos.x - incr, pos.y + incr)).a
        + texture2DRect(tex, vec2(pos.x + incr, pos.y + incr)).a
    );
}

void main()
{
    vec2 pos = gl_TexCoord[0].st;

    vec2 col = sobel_color(input, pos, 1);
    vec2 dpt = sobel_depth(input, pos, 3);

    float coll = length(col);
    float dptl = length(dpt);

    if (coll > dptl)
        gl_FragColor = vec4(0.5*col/coll + 0.5, 0, 1);
    else
        gl_FragColor = vec4(0.5*dpt/dptl + 0.5, 0, 1);
}

