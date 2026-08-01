#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
/* negative
void main()
{
    vec3 col = 1 - texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
} 
*/
/* grayscale
void main(){
    FragColor = texture(screenTexture, TexCoords);
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g +0.0722 * FragColor.b;
    FragColor = vec4(average, average, average, 1.0);
    }
*/
/* normal
void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
} 
*/
// kernel effect
const float offset = 1.0 / 3000.0;
void main(){
    vec2 offsets[9] = vec2[](vec2(-offset, offset), vec2( 0.0f,offset), vec2( offset, offset), // top-center // top-left // top-right
                             vec2(-offset, 0.0f),   vec2( 0.0f,0.0f),   vec2( offset, 0.0f), // center-left // center-center // center-right
                             vec2(-offset, -offset),vec2( 0.0f,-offset),vec2( offset, -offset) // bottom-left // bottom-center // bottom-right
    );
    /* example kernel: sharpen kernel
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    */
    
    // blur kernel
    //float kernel[9] = float[](1.0/16, 2.0/16, 1.0/16, 2.0/16, 4.0/16, 2.0/16, 1.0/16, 2.0/16, 1.0/16);

    // edge detection kernel
    //float kernel[9] = float[](1, 1, 1, 1, -8, 1, 1, 1, 1);
    
    // emboss kernel
    //float kernel[9] = float[](-2, -1, 0, -1, 1, 1, 0, 1, 2);  
    
    // normal kernel
    float kernel[9] = float[](0, 0, 0, 0, 1, 0, 0, 0, 0);

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++){
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st +
        offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)col += sampleTex[i] * kernel[i];
    FragColor = vec4(col, 1.0);
}

