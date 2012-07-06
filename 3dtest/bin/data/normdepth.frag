varying vec3 N;
varying float depth;

void main() 
{
    N = normalize(N);
    gl_FragColor = vec4(N.xyz, depth);
}
