
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;

void main()
{
	vec2 uv = v_uv;
	vec3 N = normalize(v_normal);
	
	vec3 light_pos = vec3(0,1000,0);
	vec3 light_color = vec3(1,1,1);
	vec3 L = normalize(light_pos - v_world_position);
	vec3 diffuse = 0.25*max(dot(L,N),0.0 ) * light_color;
	vec3 ambient = 1*light_color;
	
	ambient = clamp(ambient, 0.0, 1.0);
	diffuse = clamp(diffuse, 0.0, 1.0);
	
	vec3 result = ambient + diffuse;
	gl_FragColor = u_color * texture2D( u_texture, uv ) * vec4(result,0);
	//gl_FragColor = u_color * texture2D( u_texture, uv );
}
