#include "chaine.h"
#include <stdlib.h>     /* abs */
#include <cmath>
#include <complex>

Chaine::Chaine(Roue *pignon, Roue *roue){
    this->pignon = pignon;
    this->roue = roue;

    initPoints();
}

void Chaine::initPoints(){
    cenrtaxe = abs(pignon->origin - roue->origin);
    pas = pignon->r_cyl * 2 * PI / pignon->nb_dents;
    longChaine = sqrt( pow(roue->r_cyl, 2) + pow(cenrtaxe, 2) );
    angleChaine = atan(roue->r_cyl / cenrtaxe);
    epsilonInDent = roue->h_dent * 0.1;

    // TODO https://www.cours-et-exercices.com/2016/03/chaines-de-transmission-expose.html

}
