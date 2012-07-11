varying vec3 V;
varying vec3 N;
varying float depth;

void main() 
{
    gl_Position = ftransform();

    // lighting
    V = (gl_ModelViewMatrix * gl_Vertex).xyz;
    N = gl_NormalMatrix * gl_Normal;

    // depth
    depth = (-V.z - 2) / 400.0;
}
