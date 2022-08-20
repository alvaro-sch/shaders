#version 460 core

out vec4 frag_color;

uniform vec2 res;
uniform vec2 mouse;
uniform float time; 

float plot(vec2 st) {    
    return smoothstep(0.02, 0.0, abs(st.y - st.x));
}

void main() {
    vec2 pixel = gl_FragCoord.xy / res;

    vec3 color = vec3(pixel.x);

    float gp = plot(pixel);
    color = (1.0 - gp) * color + gp * vec3(0.0, 1.0, 0.0);

    frag_color = vec4(color, 1.0);
}
