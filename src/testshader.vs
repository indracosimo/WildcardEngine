#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoords;
out vec2 TexCoords;
void main()
{
    TexCoords = aTexCoords;
    // Must write gl_Position to avoid "must write to gl_Position" link errors
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
}