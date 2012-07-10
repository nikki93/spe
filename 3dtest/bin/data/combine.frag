#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect edge;
uniform sampler2DRect paint;
uniform sampler2DRect canvas;

void main()
{
    vec2 coord = gl_TexCoord[0].st;

    //vec4 color = texture2DRect(edge, coord) * texture2DRect(paint, coord);
    //vec4 canvascolor = texture2DRect(canvas, mod(coord, 512.0));

    gl_FragColor = texture2DRect(edge, coord) * texture2DRect(paint, coord);

    //multiply
    //gl_FragColor = canvascolor*color;

    //color on canvas overlay
    //gl_FragColor = canvascolor*(canvascolor + 2*color*(1 - canvascolor));

    //screen
    //gl_FragColor = 1 - (1 - canvascolor)*(1 - color);
}

