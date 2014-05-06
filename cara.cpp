#include "cara.h"
#include <cmath>

Cara::Cara() 
{
  normal.x=0.0;
  normal.y=0.0;
  normal.z=0.0;
}

Cara::Cara(int i1, int i2, int i3, int i4)
{
  idxVertices.push_back(i1);
  idxVertices.push_back(i2);
  idxVertices.push_back(i3);
  if (i4!=-1) idxVertices.push_back(i4);
  normal.x=0.0;
  normal.y=0.0;
  normal.z=0.0;
}


vec4 Cara::calculaNormal(vector<Vertices> &v) {
    ////////////////////////////////////////////////////////////////////////////////////
    ///////[0,1,2] = [B,C,A]
    //2
    vec4 vectorACx_ax =  v[idxVertices[2]].x - v[idxVertices[1]].x;
    vec4 vectorACy_ay =  v[idxVertices[2]].y - v[idxVertices[1]].y;
    vec4 vectorACz_az =  v[idxVertices[2]].z - v[idxVertices[1]].z;

    //0
    vec4 vectorBCx_bx =  v[idxVertices[0]].x - v[idxVertices[1]].x;
    vec4 vectorBCy_by =  v[idxVertices[0]].y - v[idxVertices[1]].y;
    vec4 vectorBCz_bz =  v[idxVertices[0]].z - v[idxVertices[1]].z;



    //|(a,b,c)|  = sqrt(a²+b²+c²)
    normal.x = vectorACy_ay*vectorBCz_bz - vectorACz_az * vectorBCy_by;//A x B
    normal.y = vectorACz_az*vectorBCx_bx - vectorACx_ax * vectorBCz_bz;
    normal.z = vectorACx_ax*vectorBCy_by - vectorACy_ay*vectorBCx_bx;
    GLfloat mod = sqrt(normal.x * normal.x +
                       normal.y * normal.y +
                       normal.z * normal.z);
    normal.x /= mod;
    normal.y /= mod;
    normal.z /= mod;

}

