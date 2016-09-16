@vertex
#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vert_normal;
layout(location = 2) in vec2 vert_uv;

out vec3 vertNormal;
out vec2 uv;
out vec3 vertexPos;
out vec4 lightSpace;

uniform mat4 MVP;
uniform mat4 lightMVP;
uniform mat4 worldMat;

void main () {
    gl_Position = MVP * vec4(pos, 1.0);
    vertexPos = (worldMat * vec4(pos, 1.0)).xyz;
    lightSpace = lightMVP * vec4(pos, 1.0);
    vertNormal = (worldMat * vec4(vert_normal, 0)).xyz;
    uv = vert_uv;
};

@fragment

#version 330

uniform sampler2D diffuseTex;
uniform sampler2D shadowMap;
uniform vec3 camPos;
uniform mat4 worldMat;

in vec3 vertNormal;
in vec2 uv;
in vec3 vertexPos;
in vec4 lightSpace;

out vec4 frag_color;

float getShadowFactor(vec4 lightSpacePos) {
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    float depth = texture(shadowMap, projCoords.xy).x;
    float bias = 0.005;
    if (depth < projCoords.z - bias)
        return 0.1;
    else
        return 1.0;
}

void main() {
    vec3 normal = normalize(vertNormal);
    vec3 lightColor = vec3(1);
    
    float roughness = 0.1;
    roughness = roughness * roughness;
    vec3 eyeDir = normalize(camPos - vertexPos);
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
    frag_color = texture(diffuseTex, uv);
    frag_color.rgb *= (specular * diffuse) * getShadowFactor(lightSpace);
    frag_color.rgb = pow(frag_color.rgb, vec3(1.0/2.2));
}
