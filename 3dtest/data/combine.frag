#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect edge;
uniform sampler2DRect paint;

void main()
{
    vec2 paintpos = gl_TexCoord[0].st;
    vec2 edgepos = vec2(gl_TexCoord[0].s, 768 - gl_TexCoord[0].t);
    gl_FragColor = texture2DRect(edge, edgepos) * texture2DRect(paint, paintpos);
}

