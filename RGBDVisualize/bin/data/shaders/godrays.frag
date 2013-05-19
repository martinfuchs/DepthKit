
//these are our texture names, set in openFrameworks on the shader object after begin()
uniform sampler2DRect image;

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
	// Divide by number of samples and scale by control factor.
	//deltaTexCoord *= 1.0 / numSamples * density;
	deltaTexCoord *= density / numSamples;
	// Store initial sample.
	vec3 color = texture2DRect(image, texCoord).rgb;
	// Set up illumination decay factor.
	float illuminationDecay = 1.0;
	// Evaluate summation from Equation 3 NUM_SAMPLES iterations.
	for (float i = 0.0; i < numSamples; i++)
	{
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
}

