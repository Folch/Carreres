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

OUT vec4 color;

vec4 getColor(tipusLlum light, tipusMaterial mat, vec4 v, vec4 dir) {
    if(light.on == 0) return vec4(0.0f);

    float d = length(dir);
    vec4 l = normalize(dir);
    vec4 h = (l+v) / length(l+v);
    vec4 n = normalize(vNormal);

    vec4 ks = vec4(mat.specular,0);
    vec4 kd = vec4(mat.diffuse,0);
    vec4 ka = vec4(mat.ambient,0);

    vec4 Ia = vec4(light.ambient,1.0f);
    vec4 Is = vec4(light.specular,1.0f);
    vec4 Id = vec4(light.diffuse,1.0f);

    return (1.0f/(light.a*d*d+light.b*d+light.c))*
                      ((kd*Id) * max(dot(l, n),0) +
                      (ks*Is) * max(pow((dot(n,h)), mat.shineness),0) +
                       ka*Ia);
}


vec4 getSpotColor(tipusLlum light, tipusMaterial mat, vec4 v, vec4 dir) {
    float angle = dot(light.dir, normalize(dir));

    if(angle > light.angle)
        return getColor(light, mat, v, dir);

    vec4 Ia = vec4(light.ambient,1.0f);
    vec4 ka = vec4(mat.ambient,1.0f);

    return light.on ==1 ? Ia*ka : vec4(0.0f);
}

void main() {
    gl_Position = projection*model_view*vPosition;

    vec4 v = normalize(model_view*vPosition);
    vec4 dir_puntual = llum_puntual.posicio-vPosition;
    vec4 dir_directional = llum_direccional.dir;
    vec4 dir_spot = llum_spot.posicio-vPosition;

    vec4 iag = vec4(Ia_global, 1.0f);
    vec4 kag = vec4(ka_global, 1.0f);

    color = iag * kag +
          (getColor(llum_puntual, material, v, dir_puntual) +
           getColor(llum_direccional, material, v, dir_directional) +
           getSpotColor(llum_spot, material, v, dir_spot));
}






