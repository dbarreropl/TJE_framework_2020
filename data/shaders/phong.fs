//this var comes from the vertex shader
varying vec2 v_coord;
varying vec3 v_wPos;
varying vec3 v_wNormal;

//the texture passed from the application
uniform sampler2D color_texture;
uniform sampler2D color_spec;


//uniform
uniform vec3 eye_position;
uniform vec3 light_position;
uniform vec3 diffuse_color;
uniform vec3 specular_color;
uniform vec3 ambient_color;

void main()
{	
	
	//read the pixel RGBA from the texture at the position v_coord
	vec4 color_amb = texture2D( color_texture, v_coord );
	vec4 color = texture2D( color_spec, v_coord );

	

	vec3 ka = color_amb.xyz;
	vec3 kd = color.xyz;
	vec3 ks = vec3(color.w);
	float shininess = 13;

    vec3 V = normalize(v_wPos - eye_position);
	vec3 N = normalize(v_wNormal);
	vec3 L = normalize(light_position - v_wPos);
	vec3 R = normalize(reflect(L, N));
	
	vec3 ambient1 = ka * ambient_color;
	vec3 diffuse1 = (dot(L,N) * kd ) * diffuse_color;
    vec3 specular1 = (ks* pow(dot(R, V),shininess)) * specular_color;
    
	vec3 ambient2 = clamp(ambient1, 0.0, 1.0);
	vec3 diffuse2 = clamp(diffuse1, 0.0, 1.0);
	vec3 specular2 = clamp(specular1, 0.0, 1.0);	

	color = vec4((ambient2 + diffuse2 + specular2),0.0);



	//assign the color to the output
	gl_FragColor = color;
}
