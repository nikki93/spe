attribute vec4 gl_Color;

void main()
{
    gl_FrontColor = gl_Color;
    gl_Position = ftransform();
}
