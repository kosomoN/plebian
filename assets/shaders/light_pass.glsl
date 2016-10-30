@vertex
#version 330

layout(location = 0) in vec3 pos;

uniform mat4 MVP;

void main () {
    gl_Position = MVP * vec4(pos, 1.0);
};

@fragment

#version 330

uniform sampler2D position_tex;
uniform sampler2D diffuse_tex;
uniform sampler2D normal_tex;

uniform vec3 cam_pos;
uniform vec3 light_pos;
uniform vec3 light_intensity;
uniform vec3 attenuation;
uniform vec2 screen_size;

out vec3 frag_color;

void main() {
    vec2 uv = gl_FragCoord.xy / screen_size;

    vec3 normal = texture(normal_tex, uv).xyz;
    vec3 position = texture(position_tex, uv).xyz;

    float roughness = 0.1;
    float metallic = 0.3;

    vec3 frag_to_light = light_pos - position;
    vec3 Ln = normalize(frag_to_light);
    float dist = length(frag_to_light);
    float NdotL = dot(normal, Ln);

    vec3 attenuated_light = light_intensity / (attenuation.x + attenuation.y * dist + attenuation.z * dist * dist);

    vec3 specular = vec3(0.0);
    if (NdotL > 0.0) 
    {
        float cook = 0.0;

        vec3 eye_dir = normalize(cam_pos - position);
        vec3 h = normalize(Ln + eye_dir);
        float NdotV = dot(normal, eye_dir);
        float VdotH = dot(eye_dir, h);
        float NdotH = dot(normal, h);
        
        // Fresnel
        float fl = 0.8;
        float fresnel = fl + (1.0 - fl) * pow((1.0 - VdotH), 5.0);

        // Roughness
        float roughness_sqr = roughness * roughness;

        float NdotHsq = NdotH * NdotH;
        float r1 = 1.0 / (3.14 * roughness_sqr * pow(NdotH, 4.0));
        float r2 = (NdotHsq - 1.0) / (roughness_sqr * NdotHsq);
        float microfacetDist = 0.0;
        if (NdotL > 0.0 && NdotV > 0.0)
            microfacetDist = r1 * exp(r2);

        // GGX
        float GA = 1.0;
        float GB = (2.0 * NdotH * NdotV) / VdotH;
        float GC = (2.0 * NdotH * NdotL) / VdotH;
        float GGX = min(GA, min(GB, GC));

        cook = (fresnel * microfacetDist * GGX) / (3.14 * NdotL * NdotV);
        specular = attenuated_light * (NdotL * (metallic + cook * (1.0 - metallic)));
    }

    vec3 diffuse = attenuated_light * clamp(NdotL, 0.1f, 1);
    frag_color = texture(diffuse_tex, uv).rgb;
    frag_color *= specular * diffuse;
    frag_color = pow(frag_color, vec3(1.0/2.2));
}