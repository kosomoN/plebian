#include "lights.h"

void PointLight::SetAttenuation(glm::vec3 attenuation_)
{
    attenuation = attenuation_;
    float max_intesity = glm::max(glm::max(intensity.r, intensity.g), intensity.b);
    // quadratic equation solution to where the light's intensity is < 1 / 256
    radius = (-attenuation.y
             + sqrt(attenuation.y * attenuation.y
                    - 4 * attenuation.z * (attenuation.x - 256 * max_intesity)))
             / (2 * attenuation.z);
}
