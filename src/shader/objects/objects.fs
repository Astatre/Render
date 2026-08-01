#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec2 TexCoords;
} fs_in;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform int test;

void main(){    
    if( !gl_FrontFacing ) {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    } 
    else {
        if(test == 1){
            if(gl_FragCoord.x < 800) FragColor = texture(texture1, fs_in.TexCoords);
            else FragColor = texture(texture2, fs_in.TexCoords);
        }
        else{
            FragColor = texture(texture1, fs_in.TexCoords);
            }
    }
    
}