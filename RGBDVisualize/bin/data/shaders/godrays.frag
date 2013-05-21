
uniform sampler2DRect image;
uniform sampler2DRect waterbump;

uniform float waterDistortion;
uniform vec2 waterbumpScale;
uniform vec2 waterbumpOffset;
uniform vec2 waterbumpDimensions;

uniform vec2 screenLightPos;
uniform float density;
uniform float decay;
uniform float exposure;
uniform float weight;
uniform float numSamples;

void main (void)
{
	

	
	vec2 texCoord = gl_TexCoord[0].st;
	// Calculate vector from pixel to light source in screen space.
	vec2 deltaTexCoord = (texCoord - screenLightPos);
	
	vec2 waterTexCoord = mod(normalize(deltaTexCoord) * waterDistortion * waterbumpScale + waterbumpOffset, waterbumpDimensions);
	vec2 waterDistort = (texture2DRect(waterbump, waterTexCoord).rg * 2.0 - 1.0) * waterDistortion * 10.0;
	texCoord += waterDistort;
	
	// Divide by number of samples and scale by control factor.
	//deltaTexCoord *= 1.0 / numSamples * density;
	deltaTexCoord *= density / numSamples;
	// Store initial sample.
	vec3 color = texture2DRect(image, texCoord).rgb;
	// Set up illumination decay factor.
	float illuminationDecay = 1.0;
	// Evaluate summation from Equation 3 NUM_SAMPLES iterations.
	for (float i = 0.0; i < numSamples; i++){
		// Step sample location along ray.
		texCoord -= deltaTexCoord;
		// Retrieve sample at new location.
		vec3 sample = texture2DRect(image, texCoord).rgb;
		// Apply sample attenuation scale/decay factors.
		sample *= illuminationDecay * weight;
		// Accumulate combined color.
		color += sample;
		// Update exponential decay factor.
		illuminationDecay *= decay;
	}
	// Output final color with a further scale control factor.
	gl_FragColor = vec4(color * exposure, 1.0);
	//gl_FragColor = texture2DRect(waterbump, waterTexCoord);
}

