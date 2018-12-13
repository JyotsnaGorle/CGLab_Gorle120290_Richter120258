#version 150

in vec2 pass_TexCoordinates; 

uniform sampler2D ColorTexture;

uniform bool enablehorizontalmirror;
uniform bool enableverticalmirror;
uniform bool enableGrayScale;

out vec4 out_Color;

void main() {
  float posx = pass_TexCoordinates.x;
  float posy = pass_TexCoordinates.y;
  
  if(enablehorizontalmirror) {
	posy = abs(1.0f - posy);
  }
  
  if(enableverticalmirror) {
	posx = abs(1.0f - posx);
  }

  vec4 fragColor = texture(ColorTexture, vec2(posx,posy));
  out_Color = fragColor;

  if(enableGrayScale) {
	float avg = (fragColor.r + fragColor.g + fragColor.b)/3.0;
	out_Color = vec4(avg, avg, avg, 1.0);
  }  
}
