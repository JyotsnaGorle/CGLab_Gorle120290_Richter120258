#version 150

in  vec2 pass_TexCoordinates; 

uniform sampler2D ColorTexture;

out vec4 out_Color;

void main() {
  out_Color = texture(ColorTexture, pass_TexCoordinates);
}
