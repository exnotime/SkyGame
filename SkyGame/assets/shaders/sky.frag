#version 300 es
precision highp float;
in vec3 Pos;

layout(location = 0) out vec4 FragmentColor;
#define MOD3 vec3(443.8975,397.2973, 491.1871)
vec3 hash32(vec2 p)
{
	vec3 p3 = fract(vec3(p.xyx) * MOD3);
    p3 += dot(p3, p3.yxz+19.19);
    return fract(vec3((p3.x + p3.y)*p3.z, (p3.x+p3.z)*p3.y, (p3.y+p3.z)*p3.x));
}
vec3 ditherRGB(vec3 c, vec2 seed){
	return c + hash32(seed) / 255.0;
}

void main(){
	const vec3 horizon = vec3(0.2,0.4,0.7);
	const vec3 zenith = vec3(0.1, 0.2,0.5);
	const float timeOfDay = 0.1;
	const vec3 sunPos = vec3(0.4, -1.0, -0.1) * -2.0;

	vec3 normal = normalize(Pos);
	vec3 sun = normalize(sunPos);
	float NdotS = dot(normal, sun);

	float light = 0.8 * pow(max(0.001, NdotS), 360.0);

	float curve = 0.25 * (2.0 - sunPos.y);
	vec3 sky = mix(horizon, zenith, curve);
	sky = sky * (1.0 + NdotS) + zenith * -NdotS;
	sky *= (1.0 - timeOfDay);
	sky *= clamp(sunPos.y + 2.0, 0.0, 1.0);
	sky = ditherRGB(sky, gl_FragCoord.xy - vec2(0.5));
	FragmentColor = vec4(sky + light, 1.0);
}