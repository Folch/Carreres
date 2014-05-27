#if __VERSION__<130
#define IN varying
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

IN mat4 view;
IN vec4 n;
IN vec4 position;

IN vec4 iag;
IN vec4 kag;

uniform tipusLlum llum_puntual;
uniform tipusLlum llum_direccional;
uniform tipusLlum llum_spot;

uniform tipusMaterial material;


vec4 getColor(tipusLlum light, tipusMaterial mat, vec4 v, vec4 dir) {
    if(light.on == 0) return vec4(0.0f);

    float d = length(dir);
    vec4 l = normalize(dir);
    vec4 h = (l+v) / length(l+v);

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

    vec4 pos = vec4(gl_FragCoord.xyz,1.0f);
    vec4 v = normalize(view*pos);
    vec4 dir_puntual = llum_puntual.posicio-position;
    vec4 dir_directional = llum_direccional.dir;
    vec4 dir_spot = llum_spot.posicio-position;


    gl_FragColor = iag * kag +
          (getColor(llum_puntual, material, v, dir_puntual) +
           getColor(llum_direccional, material, v, dir_directional) +
           getSpotColor(llum_spot, material, v, dir_spot));

    gl_FragColor = normalize(gl_FragColor);
    gl_FragColor.w = 1.0f;

}




