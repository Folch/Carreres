#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec4 vPosition;
IN vec4 vNormal;
uniform mat4 model_view;
uniform mat4 projection;

uniform vec3 Ia_global;
uniform vec3 ka_global;


OUT vec4 n;
OUT vec4 position;

OUT vec4 iag;
OUT vec4 kag;


void main(){
    gl_Position = projection*model_view*vPosition;

    n = normalize(vNormal);
    position = vPosition;

    iag = vec4(Ia_global, 1.0f);
    kag = vec4(ka_global, 1.0f);
}





