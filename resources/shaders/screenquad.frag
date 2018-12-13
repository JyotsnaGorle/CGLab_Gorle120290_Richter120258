#version 150

in vec2 pass_TexCoordinates; 

uniform sampler2D ColorTexture;

uniform bool enablehorizontalmirror;
uniform bool enableverticalmirror;

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

  out_Color = texture(ColorTexture, vec2(posx,posy));
}
