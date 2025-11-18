#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D iChannel0;
void main()
{
    // trivial pass-through (fast). Replace with your effect after testing.
    FragColor = texture(iChannel0, TexCoords);
}