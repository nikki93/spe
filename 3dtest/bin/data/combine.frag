#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect edge;
uniform sampler2DRect paint;
uniform sampler2DRect canvas;

void main()
{
    vec2 coord = gl_TexCoord[0].st;

    vec4 color = texture2DRect(paint, coord);
    if (texture2DRect(edge, coord).r < 0.5)
        color *= 0.52;
    vec4 canvascolor = texture2DRect(canvas, mod(coord, 512.0));

    //gl_FragColor = texture2DRect(edge, coord) * texture2DRect(paint, coord);

    //multiply
    //gl_FragColor = canvascolor*color;

    //color on canvas overlay
    vec4 withcanvas = canvascolor*(canvascolor + 2*color*(1 - canvascolor));
    gl_FragColor = mix(color, withcanvas, 0.4);

    //screen
    //gl_FragColor = 1 - (1 - canvascolor)*(1 - color);
}

