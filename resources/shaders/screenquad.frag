#version 150

in vec2 pass_TexCoordinates; 
// uniform for texture sampler
uniform sampler2D ColorTexture;

// uniforms for post-processing
// horizontal flipping
uniform bool enablehorizontalmirror;
// vertical flipping
uniform bool enableverticalmirror;
// grayscale
uniform bool enableGrayScale;
// 3X3 kernel
uniform bool enableKernel;

out vec4 out_Color;
const float offset = 1.0 / 300.0;  
void main() {

// individual texture coordinates
  float posx = pass_TexCoordinates.x;
  float posy = pass_TexCoordinates.y;
 
// data for 3X3 kernel

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        1.0, 2.0, 1.0,
		2.0, 4.0, 2.0,
		1.0, 2.0, 1.0
    );

	vec3 sampleTex[9];
    

  // do horizontal flipping
  if(enablehorizontalmirror) {
	posy = abs(1.0f - posy);
  }
  
  // do vertical flipping
  if(enableverticalmirror) {
	posx = abs(1.0f - posx);
  }

  vec4 fragColor = texture(ColorTexture, vec2(posx,posy));
  out_Color = fragColor;

  // pass greyscale color if enabled
  if(enableGrayScale) {
	float avg = (fragColor.r + fragColor.g + fragColor.b)/3.0;
	out_Color = vec4(avg, avg, avg, 1.0);
  }

  if(enableKernel) {
  // add offset values to the texture coordinates
	for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(ColorTexture, pass_TexCoordinates.xy + offsets[i]));
    }

    vec3 col = vec3(0.0);

   // since my kernel sums upto 16 divide each value by 16 to get it to sum to 1
   for(int i = 0; i < 9; i++) {
	kernel[i] = kernel[i]/16;
   }

   // multiply by kernel weight for the surrounding pixels and current pixel
	for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    out_Color = vec4(col, 1.0);
  }
  
}
