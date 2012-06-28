#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect input;

void main()
{
    vec2 pos = vec2(gl_TexCoord[0].s, 768 - gl_TexCoord[0].t);
    gl_FragColor = texture2DRect(input, pos);
}

