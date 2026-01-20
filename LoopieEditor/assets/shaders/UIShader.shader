[vertex]
#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 lp_Projection;
uniform mat4 model;

void main() {
    gl_Position = lp_Projection * model * vec4(aPos, 0.0, 1.0);
    TexCoords = aTexCoords;
}


[fragment]
#version 460 core

in vec3 vColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vColor, 1.0);
}
