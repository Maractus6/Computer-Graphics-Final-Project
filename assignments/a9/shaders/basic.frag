#version 330 core

/*default camera matrices. do not modify.*/
layout(std140) uniform camera
{
    mat4 projection;	/*camera's projection matrix*/
    mat4 view;			/*camera's view matrix*/
    mat4 pvm;			/*camera's projection*view*model matrix*/
    mat4 ortho;			/*camera's ortho projection matrix*/
    vec4 position;		/*camera's position in world space*/
};

/* set light ubo. do not modify.*/
struct light
{
	ivec4 att; 
	vec4 pos; // position
	vec4 dir;
	vec4 amb; // ambient intensity
	vec4 dif; // diffuse intensity
	vec4 spec; // specular intensity
	vec4 atten;
	vec4 r;
};
layout(std140) uniform lights
{
	vec4 amb;
	ivec4 lt_att; // lt_att[0] = number of lights
	light lt[4];
};

/*input variables*/
in vec3 vtx_normal; // vtx normal in world space
in vec3 vtx_position; // vtx position in world space
in vec3 vtx_model_position; // vtx position in model space
in vec4 vtx_color;
in vec2 vtx_uv;
in vec3 vtx_tangent;

uniform vec3 ka;            /* object material ambient */
uniform vec3 kd;            /* object material diffuse */
uniform vec3 ks;            /* object material specular */
uniform float shininess;    /* object material shininess */

uniform sampler2D tex_color;   /* texture sampler for color */
uniform sampler2D tex_normal;   /* texture sampler for normal vector */

/*output variables*/
out vec4 frag_color;




vec3 shading_texture_with_phong(light li, vec3 e, vec3 p, vec3 s, vec3 n)
{
    vec3 l = normalize(li.pos.xyz - p);
    vec3 v = normalize(e - p);   
    vec3 r = reflect(-l, n);                 

    vec3 ambient = ka * li.amb.xyz;
    float diff = max(dot(n, l), 0.0);
    vec3 diffuse = kd * li.dif.xyz * diff;
    float spec = pow(max(dot(r, v), 0.0), shininess);
    vec3 specular = ks * li.spec.xyz * spec;

    return s * (ambient + diffuse) + specular;
}


void main()
{
    vec3 eye_pos = position.xyz;
    vec3 frag_pos = vtx_position;

    vec3 Nmap = normalize(texture(tex_normal, vtx_uv).rgb * 2.0 - 1.0);

    vec3 B = normalize(cross(vtx_normal, vtx_tangent));
    vec3 tangent = normalize(vtx_tangent);
    mat3 TBN = mat3(tangent, B, vtx_normal);
    
    vec3 N = normalize(TBN * Nmap);

    vec3 texture_color = texture(tex_color, vtx_uv).rgb;

    //add lighting together
    vec3 color = vec3(0.0);
    for(int i = 0; i < lt_att[0]; i++)
    {
        color += shading_texture_with_phong(lt[i], eye_pos, frag_pos, texture_color, N);
    }

    // Clamp together
    frag_color = vec4(clamp(color, 0.0, 1.0), 1.0);
}
