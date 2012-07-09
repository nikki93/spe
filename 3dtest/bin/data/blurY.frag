#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect input;

vec4 blurY(sampler2DRect tex, vec2 pos, float incr)
{
    return texture2DRect(tex, vec2(pos.x, pos.y - 4*incr)) * 0.000215825
         + texture2DRect(tex, vec2(pos.x, pos.y - 3*incr)) * 0.00579007
         + texture2DRect(tex, vec2(pos.x, pos.y - 2*incr)) * 0.059897
         + texture2DRect(tex, vec2(pos.x, pos.y - 1*incr)) * 0.241811
         + texture2DRect(tex, vec2(pos.x, pos.y         )) * 0.384572
         + texture2DRect(tex, vec2(pos.x, pos.y + 1*incr)) * 0.241811
         + texture2DRect(tex, vec2(pos.x, pos.y + 2*incr)) * 0.059897
         + texture2DRect(tex, vec2(pos.x, pos.y + 3*incr)) * 0.00579007
         + texture2DRect(tex, vec2(pos.x, pos.y + 4*incr)) * 0.000215825;
}

void main()
{
    vec2 pos = gl_TexCoord[0].st;
    gl_FragColor = blurY(input, pos, 2.8);
}

