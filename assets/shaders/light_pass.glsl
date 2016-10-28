@vertex
#version 330

layout(location = 0) in vec2 pos;

out vec2 uv;

void main () {
    gl_Position = vec4(pos, 0.0, 1.0);
    uv = (pos + vec2(1)) / 2;
};

@fragment

#version 330

uniform sampler2D position_tex;
uniform sampler2D diffuse_tex;
uniform sampler2D normal_tex;

uniform vec3 cam_pos;

in vec2 uv;

out vec4 frag_color;

void main() {
    vec3 normal = texture(normal_tex, uv).xyz;
    vec3 position = texture(position_tex, uv).xyz;
    vec3 lightColor = vec3(1);

    float roughness = 0.1;
    roughness = roughness * roughness;
    vec3 eyeDir = normalize(cam_pos - position);
    vec3 Ln = vec3(0, 1, 0);
    vec3 h = normalize(Ln + eyeDir);
    float NdotL = dot(normal, Ln);
    float k = 0.3;

    vec3 specular = vec3(0.0);
    if (NdotL > 0.0) 
    {
        float cook = 0.0;
        float NdotV = dot(normal, eyeDir);
        float VdotH = dot(eyeDir, h);
        float NdotH = dot(normal, h);
        float fl = 0.8;
        
        // Fresnel
        float fresnel = fl + (1.0 - fl) * pow((1.0 - VdotH), 5.0);

        // Roughness
        float NdotHsq = NdotH * NdotH;
        float r1 = 1.0 / (3.14 * roughness * pow(NdotH, 4.0));
        float r2 = (NdotHsq - 1.0) / (roughness * NdotHsq);
        float microfacetDist = 0.0;
        if (NdotL > 0.0 && NdotV > 0.0)
            microfacetDist = r1 * exp(r2);

        // GGX
        float GA = 1.0;
        float GB = (2.0 * NdotH * NdotV) / VdotH;
        float GC = (2.0 * NdotH * NdotL) / VdotH;
        float GGX = min(GA, min(GB, GC));

        cook = (fresnel * microfacetDist * GGX) / (3.14 * NdotL * NdotV);
        specular = lightColor * (NdotL * (k + cook * (1.0 - k)));
    }

    vec3 diffuse = lightColor * clamp(NdotL, 0.1f, 1);
    frag_color = texture(diffuse_tex, uv);
    frag_color.rgb *= (specular * diffuse);
    frag_color.rgb = pow(frag_color.rgb, vec3(1.0/2.2));
}