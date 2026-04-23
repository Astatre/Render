#version 330 core
struct Material {
sampler2D diffuse;
sampler2D specular;
sampler2D emission;
float shininess;
};

struct Light {
vec4 lightVector; // w component is 0 for directional light, 1 for point light
vec3 direction; // only used for spotlight
float cutOff; // only used for spotlight 0 for directional light and point light
float outerCutOff; // only used for spotlight
vec3 ambient;
vec3 diffuse;
vec3 specular;

float constant;
float linear;
float quadratic;    
};


out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform Light light;
uniform Material material;



void main(){

vec3 lightDir;
if (light.lightVector.w == 0.0f){ // directional light
    lightDir = normalize(-light.lightVector.xyz);}
else{ // point light
    lightDir = normalize(light.lightVector.xyz - FragPos);
}


// ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    
    // diffuse 
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
    
    if(light.cutOff > 0.0f){ // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    }

    
    // attenuation
    float distance    = length(light.lightVector.xyz - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}