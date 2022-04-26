#version 430

in vec4 vertexColor;
in vec2 texCoord;

out vec4 color;

// pixels da textura
uniform sampler2D tex_buffer;
uniform sampler2D fundo;


void main()
{
   color = texture(tex_buffer, texCoord);
    //color = mix(texture(tex_buffer, TexCoord), texture(fundo, TexCoord), 0.2);
}