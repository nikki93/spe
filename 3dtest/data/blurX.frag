#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect input;

vec4 blurX(sampler2DRect tex, vec2 pos, float incr)
{
    return texture2DRect(tex, vec2(pos.x - 4*incr, pos.y)) * 0.000215825
         + texture2DRect(tex, vec2(pos.x - 3*incr, pos.y)) * 0.00579007
         + texture2DRect(tex, vec2(pos.x - 2*incr, pos.y)) * 0.059897
         + texture2DRect(tex, vec2(pos.x - 1*incr, pos.y)) * 0.241811
         + texture2DRect(tex, vec2(pos.x         , pos.y)) * 0.384572
         + texture2DRect(tex, vec2(pos.x + 1*incr, pos.y)) * 0.241811
         + texture2DRect(tex, vec2(pos.x + 2*incr, pos.y)) * 0.059897
         + texture2DRect(tex, vec2(pos.x + 3*incr, pos.y)) * 0.00579007
         + texture2DRect(tex, vec2(pos.x + 4*incr, pos.y)) * 0.000215825;
}

void main()
{
    vec2 pos = vec2(gl_TexCoord[0].s, gl_TexCoord[0].t);
    gl_FragColor = blurX(input, pos, 1);
}

