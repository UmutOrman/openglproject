#version 120

uniform int width; // width of the texture in pixels
uniform int height; // height of the texture in pixels
uniform sampler2D texture;
varying vec2 texture_coordinate;

varying vec3 Normal;
varying vec3 ToLight;
varying vec4 PositionVCS;

vec4 n_pixel(float vx, float vz) {
    return texture2D(texture,vec2(texture_coordinate.x + vx/width, texture_coordinate.y + vz/height));
}

void main()
{
    // get the texture color
	vec4 tColor = (
            n_pixel(0,-1) +
            n_pixel(0,1) +
            n_pixel(1,0) +
            n_pixel(-1,0) +
            n_pixel(-1,-1) +
            n_pixel(-1,1) +
            n_pixel(1,-1) +
            n_pixel(1,1)
         )/8.0;


    // apply Phong shading by using the following parameters
    int specExp = 100; // specular exponent
    vec4 ka = vec4(0.25,0.25,0.25,1.0); // reflectance coeff. for ambient
    vec4 kd = vec4(1.0, 1.0, 1.0, 1.0); // reflectance coeff. for diffuse
    vec4 ks = vec4(1.0, 1.0, 1.0, 1.0); // reflectance coeff. for specular 
    vec4 Ia = vec4(0.3,0.3,0.3,1.0);    // light color for ambient
    vec4 Id = vec4(1.0, 1.0, 1.0, 1.0); // light color for diffuse
    vec4 Is = vec4(1.0, 1.0, 1.0, 1.0); // light color for specular

    vec3 V = normalize(vec3(PositionVCS)),
         L = normalize(ToLight);

    vec3 R = normalize(V+L);

    float NL = max(0, dot(Normal, L)),
          NR = max(0, dot(Normal, R));

    vec4 ambient  = Ia * ka,
         diffuse  = kd * Id * NL,
         specular = ks * Is * pow(NR, specExp);
   
    // compute the color using the following equation
    vec3 color = clamp(tColor.xyz * vec3(ambient + diffuse + specular), 0.0, 1.0);
    
    gl_FragColor = vec4(color, 1.0);
}
