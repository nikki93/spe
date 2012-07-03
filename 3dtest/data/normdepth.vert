varying vec3 N;
varying float depth;

void main() 
{
    gl_Position = ftransform();

    N = gl_NormalMatrix * gl_Normal;
    depth = (-(gl_ModelViewMatrix * gl_Vertex).z - 2) / 400.0;
}
