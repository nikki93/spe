uniform mat4 gl_ModelViewMatrix;
uniform mat4 gl_ProjectionMatrix;

attribute vec4 gl_Vertex;

attribute vec4 gl_MultiTexCoord0;

void main()
{
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;
}
