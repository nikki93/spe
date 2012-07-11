varying vec3 V;
varying vec3 N;
varying float depth;

void main() 
{
    // lighting
    N = normalize(N);
    V = normalize(V);
    vec3 R = reflect(V, N);
    vec3 L = normalize(vec3(gl_LightSource[0].position));

    vec4 ambient = vec4(0.5, 0.5, 0.5, 1) * 0.2;
    vec4 diffuse = vec4(0.5, 0.5, 0.5, 1) * max(dot(L, N), 0.0);
    vec4 specular = vec4(0.4, 0.4, 0.4, 1) * pow(max(dot(R, L), 0.0), 4);

    // output
    gl_FragColor = vec4((ambient + diffuse + specular).xyz, depth);
}
