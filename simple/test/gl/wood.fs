#version 430 core
in vec2 TexCoord;
in vec3 ourColor; 

out vec4 FragColor;

uniform sampler2D texture1;

void main()
{
    //FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0f);
    //FragColor = texture(texture1, TexCoord);
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

