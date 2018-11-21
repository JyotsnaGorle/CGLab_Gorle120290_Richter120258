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

// float value for alpha in the equation
uniform float shininess;

// float value between 0 and 1
uniform float rho;

// color of light white
uniform vec3 lightColor;

// float value between 0 and 1
uniform float lightIntensity;

// uniform for mode switch 1 or 2
uniform float modeSwitch;

// final color of the elements
out vec4 out_Color;



void main() {
// calculating the ambient color
  vec3 ambient =  pass_Color;

// beta function
  vec3 phi = lightIntensity * lightColor;
  float light = length(lightSource - frag_pos);

  vec3 beta = (phi/(4.0 * 3.14)) / pow(light,2);

// diffusion function
  vec3 lightDir = normalize(lightSource - frag_pos);
  float diffuseCoffeicient = max(dot(pass_Normal, lightDir), 0.0);
  // need to get value of rho
  vec3 diffuse = (diffuseCoffeicient * diffuseColor) * rho / 3.14f;

// speculative light
	vec3 viewDir = normalize(cameraPos - frag_pos);
	vec3 halfway = normalize(lightDir + viewDir);
	float shine = 4.0f * shininess;
	float specularCoffecient = pow(max(dot(pass_Normal, halfway), 0.0), shine);
	vec3 specular = speculativeColor * specularCoffecient;

// for Toon shading
	float intensity = dot(lightDir, pass_Normal);
	vec4 color = vec4(1.0,0.5,0.5,1.0);
		if (intensity > 0.95) {
			color = vec4(1.0,0.5,0.5,1.0);
		}
		if (intensity > 0.5) {
			color = vec4(0.6,0.3,0.3,1.0);
		}
		if (intensity > 0.25) {
			color = vec4(0.4,0.2,0.2,1.0);
		}	
		else {
			color = vec4(0.2,0.1,0.1,1.0);
		}	

// calulate the final color

  vec3 finalColor = ambient + beta * (diffuse + specular);
  if(modeSwitch == 1.0) {
	out_Color = vec4(finalColor, 1.0);
  } 
  else if(modeSwitch == 2.0) {
	 out_Color = color;
  }
}
