#version 330 core

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;

uniform sampler2D tex_buzz; 

in vec3 vtx_pos; // [-1, 1]
in vec2 vtx_uv; // [0, 1]

out vec4 frag_color;

#define NUM_STAR 100.

// Amanda: Added the below definitions
in vec2 fragCoord;              /* screen space coordinate */

#define Time (iTime*1.0)            
#define Gravity 0.7             /* gravity */
#define DURATION 6.             /* duration of a shooting star */

const vec2 g = vec2(.0, -Gravity); /* gravity */

// return random vec2 between 0 and 1
vec2 hash2d(float t)
{
    t += 1.;
    float x = fract(sin(t * 674.3) * 453.2);
    float y = fract(sin((t + x) * 714.3) * 263.2);

    return vec2(x, y);
}

vec3 renderParticle(vec2 uv, vec2 pos, float brightness, vec3 color)
{
    float d = length(uv - pos);
    return brightness / d * color;
}

vec3 renderStars(vec2 uv)
{
    vec3 fragColor = vec3(0.0);

    float t = iTime;
    for(float i = 0.; i < NUM_STAR; i++)
    {
        vec2 pos = hash2d(i) * 2. - 1.; // map to [-1, 1]
        float brightness = .0015;
        brightness *= sin(1.5 * t + i) * .5 + .5; // flicker
        vec3 color = vec3(0.15, 0.71, 0.92);

        fragColor += renderParticle(uv, pos, brightness, color);
    }

    return fragColor;
}

/////////////////////////////////////////////////////
//// Simulate the motion of a single particle by programming ballistic motion
//// In this function, you are asked to calculate the position of the particle using the expression of ballistic motion.
//// The function takes the initial position, initial velocity, and time t as input, and returns the particle's current location.
/////////////////////////////////////////////////////

vec2 moveParticle(vec2 initPos, vec2 initVel, float t)
{
    vec2 currentPos = initPos;

    /* your implementation starts */
    currentPos += initVel * t + 0.5 * g * pow(t, 2);
    /* your implementation ends */

    return currentPos;
}

/////////////////////////////////////////////////////
//// Putting simulation and rendering together in one function call
//// In this function, you will practice to combine the animaiton and rendering functions together 
//// by calling moveParticle() and renderParticle() you have implemented to calculate the fragment color.
//// The idea is to update the particle's current position with moveParticle() first, 
//// and then use this position as an input for renderParticles() to calculate the fragment color.
//// After implementing both Step 3A and 3B, you want to testify its correctness by uncommenting Step 3 in mainImage(). 
//// The expected result is the animation of a single particle that moves along a ballistic trajectory.
/////////////////////////////////////////////////////

vec3 simSingleParticle(vec2 fragPos, vec2 initPos, vec2 initVel, float t, float brightness, vec3 color)
{
    vec3 fragColor = vec3(0.0);

    /* your implementation starts */
    vec2 currentPos = moveParticle(initPos, initVel, t);
    fragColor = renderParticle(fragPos, currentPos, brightness, color);
    /* your implementation ends */

    return fragColor;
}

void main()
{
    // Data for the shooting star
    vec2 initVel = vec2(1.5, 2.5);
    float t = mod(Time, DURATION);
    float brightness = .005;

    float period = floor(Time / DURATION);
    float rnd = hash2d(period).x;
    // Random y position for the shooting star
    vec2 initPos = vec2(-1, -1 * hash2d(period).y);
    // Pick a color randomly each DURATION period: blue, yellow, or white
    vec3 color;
    if(rnd < 0.33)
        color = vec3(0.15, 0.71, 0.92); // blue
    else if(rnd < 0.66)
        color = vec3(1.0, 0.984, 0.0); // yellow
    else
        color = vec3(1.0); // white
    
    vec3 outputColor = renderStars(vtx_pos.xy);
    // Shooting star
    outputColor += simSingleParticle(vtx_pos.xy, initPos, initVel, t, brightness, color);

    // vec2 uv = vec2(vtx_uv.x, -vtx_uv.y);
    // vec3 buzzColor = texture(tex_buzz, uv).xyz;

    // only stars, no buzz
    frag_color = vec4(outputColor, 1.0);
}