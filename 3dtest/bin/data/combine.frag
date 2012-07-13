#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect edge;
uniform sampler2DRect paint;
uniform sampler2DRect canvas;

void main()
{
    vec2 coord = gl_TexCoord[0].st;

    // color
    vec4 color = texture2DRect(paint, coord);

    // darken if edge
    if (texture2DRect(edge, coord).r < 0.5)
        color *= 0.52;

    // canvas
    vec4 canvascolor = texture2DRect(canvas, mod(coord, 512.0));
    vec4 withcanvas = canvascolor*(canvascolor + 2*color*(1 - canvascolor));

    gl_FragColor = mix(color, withcanvas, 0.5);
}

