uniform float iTime;
in vec2 vtx_uv;
out vec4 frag_color;

// rand and noise reference: https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
// our random or noise func
float rand(vec2 n) {
    return fract(sin(dot(n, vec2(12, 78))) * 43758);
}
float noise(vec2 n) {
    vec2 cell = floor(n);
    vec2 fracv = fract(n);
    // smooth the weights
    vec2 w = fracv * fracv * (3.0 - 2.0 * fracv);

    float a = rand(cell);
    float b = rand(cell + vec2(1.0, 0.0));
    float c = rand(cell + vec2(0.0, 1.0));
    float d = rand(cell + vec2(1.0, 1.0));

    float x1 = mix(a, b, w.x);
    float x2 = mix(c, d, w.x);
    return mix(x1, x2, w.y);
}
void main() {
    vec2 uv = vtx_uv;
    float x = uv.x - 0.5; // center it horizontally
    float y = uv.y;

    // call our noise func
    float n = noise(vec2(x*3.0, y*4.0 - iTime*1.5));
    
    // creating the flame shape
    float shape = (1.0 - y) * exp(-3.0 * abs(x)) + n * 0.4; // n is our flicker
    shape = clamp(shape, 0.0, 1.0);
    shape = smoothstep(0.02, 0.85, shape);

    // mix red -> orange -> yellow
    vec3 color = mix(vec3(1.0, 0.2, 0.0), vec3(1.0, 0.95, 0.2), pow(y, 0.5));
    float alpha = pow(shape, 1.2);

    frag_color = vec4(color * shape, alpha);
}