#version 120

uniform vec4 lightPosition;
uniform float heightFactor;
uniform int width; // width of the texture in pixels
uniform int height; // height of the texture in pixels
uniform sampler2D texture; // OpenGL texture id 

varying vec3 Normal; // to store the computed normal vector of the vertex
varying vec3 ToLight; // the vector from the vertex to the light source
varying vec4 PositionVCS; // the position of the vertex in Viewing Coord. Sys.
varying vec2 texture_coordinate; // texture coordinate of the vertex 

float getHeight(vec4 v) {
    vec3 p = v.rgb;
    return (0.2126*p.r + 0.7152*p.g  + 0.0722*p.b)*heightFactor;
}

vec3 prepare(float vx, float vz) {
    float yp = getHeight(texture2D(texture, texture_coordinate));

    vec4 tex = texture2D(texture,vec2(texture_coordinate.x + vx/width, texture_coordinate.y + vz/height));
    float yt = getHeight(tex) - yp;

    return normalize(vec3(vx, yt, vz));
}

vec4 n_pixel(float vx, float vz) {
    return texture2D(texture,vec2(texture_coordinate.x + vx/width, texture_coordinate.y + vz/height));
}

vec3 crossN(vec3 l, vec3 r) {
    return cross(normalize(l), normalize(r));
}

void main() {
	texture_coordinate = vec2(gl_MultiTexCoord0);

    vec3 Up = prepare(0, -1),
         Down = prepare(0, +1),
         Right = prepare(+1, 0),
         Left = prepare(-1, 0),
         UpLeft = prepare(-1, -1),
         DownLeft = prepare(-1, +1),
         UpRight = prepare(+1, -1),
         DownRight = prepare(+1, +1);

    Normal = normalize((
            crossN(Down  , DownRight) +
            crossN(DownRight , Right)  +
            crossN(Right  , UpRight) +
            crossN(UpRight , Up)  +
            crossN(Up  , UpLeft) +
            crossN(UpLeft , Left)  +
            crossN(Left  , DownLeft) +
            crossN(DownLeft , Down)
            )/8.0);

    // smoothing
	vec4 point = (
            n_pixel(0,-1) +
            n_pixel(0,1) +
            n_pixel(1,0) +
            n_pixel(-1,0) +
            n_pixel(-1,-1) +
            n_pixel(-1,1) +
            n_pixel(1,-1) +
            n_pixel(1,1)
         )/8.0;

	gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.x, getHeight(point), gl_Vertex.z, 1.0);

	ToLight = normalize(vec3(lightPosition.x-gl_Position.x, lightPosition.y-gl_Position.y, lightPosition.z-gl_Position.z));

	PositionVCS = normalize(gl_ModelViewMatrix * gl_Position);
}
