#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif

struct tipusLlum {
    vec4 posicio;
    vec4 dir;
    int on;
    float angle;

    float a,b,c;

    vec3 ambient;
    vec3 specular;
    vec3 diffuse;
};

struct tipusMaterial {
    vec3 ambient;
    vec3 specular;
    vec3 diffuse;

    float shineness;
};

IN vec4 vPosition;
IN vec4 vNormal;
uniform mat4 model_view;
uniform mat4 projection;

uniform tipusLlum llum_puntual;
uniform tipusLlum llum_direccional;
uniform tipusLlum llum_spot;

uniform tipusMaterial material;
uniform vec3 Ia_global;
uniform vec3 ka_global;


OUT mat4 view;
OUT vec4 n;
OUT vec4 position;

OUT vec4 iag;
OUT vec4 kag;


void main(){
    gl_Position = projection*model_view*vPosition;

    n = normalize(vNormal);
    view = model_view;
    position = vPosition;

    iag = vec4(Ia_global, 1.0f);
    kag = vec4(ka_global, 1.0f);
}





