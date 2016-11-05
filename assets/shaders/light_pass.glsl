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
uniform float inv_sqr_radius;
uniform vec2 screen_size;

out vec3 frag_color;

float SmoothAttenuation(float squared_distance)
{
    float factor = squared_distance * inv_sqr_radius;
    float smooth_factor = clamp(1.0 - factor * factor, 0.0, 1.0);
    return smooth_factor * smooth_factor;
}

float GetAttenuation(vec3 frag_to_light)
{
    float sqr_dist = dot(frag_to_light, frag_to_light);
    float attenuation = 1.0 / (max(sqr_dist, 0.01 * 0.01));

    // fade out at light radius
    attenuation *= SmoothAttenuation(sqr_dist);

    return attenuation;
}
void main() {
    vec2 uv = gl_FragCoord.xy / screen_size;

    vec4 diffuse_map = texture(diffuse_tex, uv);
    vec3 color = diffuse_map.rgb;
    float roughness = diffuse_map.a;

    vec4 normal_map = texture(normal_tex, uv);
    vec3 normal = normal_map.xyz;
    float metallic = normal_map.w;

    vec3 position = texture(position_tex, uv).xyz;

    vec3 frag_to_light = light_pos - position;
    vec3 Ln = normalize(frag_to_light);
    float NdotL = dot(normal, Ln);

    vec3 attenuated_light = light_intensity * GetAttenuation(frag_to_light);

    float specular = 0.0;
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
        if (NdotV > 0.0)
            microfacetDist = r1 * exp(r2);

        // GGX
        float GA = 1.0;
        float GB = (2.0 * NdotH * NdotV) / VdotH;
        float GC = (2.0 * NdotH * NdotL) / VdotH;
        float GGX = min(GA, min(GB, GC));

        specular = (fresnel * microfacetDist * GGX) / (3.14 * NdotL * NdotV);
    }

    frag_color = color * attenuated_light * max(NdotL, 0.0) * ((1 - metallic) + specular * metallic);
    frag_color = pow(frag_color, vec3(1.0/2.2));
}