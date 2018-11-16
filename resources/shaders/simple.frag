#version 150

// vectors passed from the vertex shader
in  vec3 pass_Normal;
in  vec3 pass_Color;
in  vec3 frag_pos;

// light source 0,0
uniform vec3 lightSource;

// color RGB vec3
uniform vec3 diffuseColor;

// color RGB vec3
uniform vec3 speculativeColor;


// position vector of the camera
uniform vec3 cameraPos;

// float value of 24.0
uniform float shininess;

// final color of the elements
out vec4 out_Color;


void main() {
// calculating the ambient color
  vec3 ambient =  pass_Color;

// beta function
  float lightIntensity = 0.4;
  vec3  lightColor = vec3(0.2f, 0.2f, 0.2f);

  vec3 phi = lightIntensity * lightColor;
  float light = length(lightSource - frag_pos);

  vec3 beta = (phi/(4.0 * 3.14)) / pow(light,2);

// diffusion function
  vec3 lightDir = normalize(lightSource - frag_pos);
  float diffuseCoffeicient = max(dot(pass_Normal, lightDir), 0.0);
  // need to get value of rho
  vec3 diffuse = (diffuseCoffeicient * diffuseColor) / 3.14f;

// speculative light
	vec3 viewDir = normalize(cameraPos - frag_pos);
	vec3 halfway = normalize(lightDir + viewDir);
	float shine = 4.0f * 0.1;
	float specularCoffecient = pow(max(dot(pass_Normal, halfway), 0.0), 16.0);
	vec3 specular = speculativeColor * specularCoffecient;

  vec3 finalColor = ambient + beta * (diffuse + specular);
  out_Color = vec4(finalColor, 1.0);
}
