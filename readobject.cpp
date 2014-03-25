#include "readobject.h"
#include "readfile.h"

ReadObject::ReadObject() {

}


Cotxe* ReadObject::readCar(QString fichero, GLfloat mida, GLfloat xorig, GLfloat yorig, GLfloat zorig,
                           double xrot, double yrot, double zrot,
                           float xdirec, float ydirec, float zdirec) {

    vector<Cara> cares;
    vector<point4> vertexs;
    Cotxe* cotxe = new Cotxe(vertexs, cares,mida, xorig, yorig, zorig, xrot, yrot, zrot,xdirec, ydirec, zdirec);

    readObj(fichero, cotxe);
    return cotxe;
}


void ReadObject::readObj(QString filename, Objecte *obj) {

    FILE *fp = fopen(filename.toLocal8Bit(),"rb");
    Objecte *currentObj = NULL;
    vector<Cara> caresTmp;
    vector<point4> vertexsTmp;
    char objType[25];
    if (!fp)
    {
        cout << "No puc obrir el fitxer " << endl;
    }
    else {

        int vindexAct = 0;//
        int vindexUlt = 0;//

        while (true)
        {
            char *comment_ptr = fetch_line (fp);

            if (comment_ptr == (char *) -1) {
                /* end-of-file */
                if(dynamic_cast<Cotxe*>(obj)){
                    if(!strcmp (objType, "Roda_Esquerra_Posterior_Untitled")) {
                        currentObj = new Roda(vertexsTmp, caresTmp);
                        ((Roda*)currentObj)->type = ESQUERRA_POSTERIOR;
                    }else if(!strcmp (objType, "Roda_Dreta_Posterior_04")){
                        currentObj = new Roda(vertexsTmp, caresTmp);
                        ((Roda*)currentObj)->type = DRETA_POSTERIOR;
                    }else if(!strcmp (objType, "Roda_Esquerra_Davantera_02")){
                        currentObj = new Roda(vertexsTmp, caresTmp);
                        ((Roda*)currentObj)->type = ESQUERRA_DAVANTERA;
                    }else if(!strcmp (objType, "Roda_Dreta_Davantera_03")){
                        currentObj = new Roda(vertexsTmp, caresTmp);
                        ((Roda*)currentObj)->type = DRETA_DAVANTERA;
                    }else if(!strcmp (objType, "Carrosseria_00")){
                        currentObj = new Carrosseria(vertexsTmp, caresTmp);
                    }else{
                        break;
                    }
                    ((Cotxe*)obj)->addComponent(currentObj);
                }else if(dynamic_cast<Obstacle*>(obj)) {
                    obj = new Obstacle(vertexsTmp, caresTmp);
                }
                break;
            }

            /* did we get a comment? */
            if (comment_ptr) {
                //make_comment (comment_ptr);
                continue;
            }

            /* if we get here, the line was not a comment */
            int nwords = fetch_words();

            /* skip empty lines */
            if (nwords == 0)
                continue;

            char *first_word = words[0];

            if (!strcmp (first_word, "v"))
            {
                if (nwords < 4)
                {
                    fprintf (stderr, "Too few coordinates");//: '%s'", str_orig);
                    exit (-1);
                }
                QString sx(words[1]);
                QString sy(words[2]);
                QString sz(words[3]);
                double x = sx.toDouble();
                double y = sy.toDouble();
                double z = sz.toDouble();

                if (nwords == 5)
                {
                  QString sw(words[4]);
                  double w = sw.toDouble();
                  x/=w;
                  y/=w;
                  z/=w;
                }
                // S'afegeix el vertex a l'objecte
                point4 pTmp = point4(x, y, z, 1);
                vertexsTmp.push_back(pTmp);
                vindexAct++;

            }
            else if (!strcmp (first_word, "vn")) {
            }
            else if (!strcmp (first_word, "vt")) {
            }
            else if (!strcmp (first_word, "f")) {
                // S'afegeix la cara a l'objecte
                this->buildFace(&words[1], nwords-1, &caresTmp, &vertexsTmp, vindexUlt);
            }

            // added
            else if (!strcmp (first_word, "mtllib")) {
                //read_mtllib (&words[1], nwords-1, matlib, filename);
            }
            else if (!strcmp (first_word, "usemtl")) {
                //int size = strlen(words[1])-1;
                //while (size && (words[1][size]=='\n' || words[1][size]=='\r') ) words[1][size--]=0;
                //currentMaterial = matlib.index(words[1]);
            }
            else if (!strcmp (first_word, "o")) {
                if(dynamic_cast<Cotxe*>(obj)) {
                    if(!vertexsTmp.empty()){
                        if(!strcmp (objType, "Roda_Esquerra_Posterior_Untitled")) {
                            currentObj = new Roda(vertexsTmp, caresTmp);
                            ((Roda*)currentObj)->type = ESQUERRA_POSTERIOR;
                        }else if(!strcmp (objType, "Roda_Dreta_Posterior_04")){
                            currentObj = new Roda(vertexsTmp, caresTmp);
                            ((Roda*)currentObj)->type = DRETA_POSTERIOR;
                        }else if(!strcmp (objType, "Roda_Esquerra_Davantera_02")){
                            currentObj = new Roda(vertexsTmp, caresTmp);
                            ((Roda*)currentObj)->type = ESQUERRA_DAVANTERA;
                        }else if(!strcmp (objType, "Roda_Dreta_Davantera_03")){
                            currentObj = new Roda(vertexsTmp, caresTmp);
                            ((Roda*)currentObj)->type = DRETA_DAVANTERA;
                        }else if(!strcmp (objType, "Carrosseria_00")){
                            currentObj = new Carrosseria(vertexsTmp, caresTmp);
                        }else{
                            strcpy(objType,words[1]);
                            vindexUlt = vindexAct;
                            continue;
                        }
                        ((Cotxe*)obj)->addComponent(currentObj);
                        caresTmp.clear();
                        vertexsTmp.clear();
                    }
                }
                strcpy(objType,words[1]);
                //cada nou objecte s'actualitza aquest Ã­ndex
                vindexUlt = vindexAct;
            }
            // fadded
            else {
                //fprintf (stderr, "Do not recognize: '%s'\n", str_orig);
            }
            //free(words)
        }
    }
}


void ReadObject::buildFace ( char **words, int nwords, vector<Cara> *cares, vector<point4> *vertexs, int vindexUlt) {
    Cara face;
    for (int i = 0; i < nwords; i++)
    {
        int vindex;
        int nindex;
        int tindex;

        if ((words[i][0]>='0')&&(words[i][0]<='9'))
        {
            get_indices (words[i], &vindex, &tindex, &nindex);

#if 0
            printf ("vtn: %d %d %d\n", vindex, tindex, nindex);
#endif

            /* store the vertex index */

            if (vindex > 0)       /* indices are from one, not zero */
                face.idxVertices.push_back(vindex - 1 - vindexUlt);
            else if (vindex < 0)  /* negative indices mean count backwards */
                face.idxVertices.push_back(vertexs->size() + vindex - vindexUlt);
            else
            {
                fprintf (stderr, "Zero indices not allowed");//: '%s'\n", str_orig);
                exit (-1);
            }
        }
    }
    face.color = vec4(1.0, 0.0, 0.0, 1.0);
    cares->push_back(face);
}
