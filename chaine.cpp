#include "chaine.h"
#include <stdlib.h>     /* abs */

Chaine::Chaine(Roue *pignon, Roue *roue){
    this->pignon = pignon;
    this->roue = roue;

    initPoints();
}

void Chaine::initPoints(){
    cenrtaxe = abs(pignon->origin - roue->origin);
    pas = pignon->r_cyl * 2 * PI / pignon->nb_dents;

    // TODO https://www.cours-et-exercices.com/2016/03/chaines-de-transmission-expose.html

}
