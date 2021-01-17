#include <iostream>

#include "codage.h"
#include "decodage.h"

int main(){
    std::cout << "Si vous voulez coder en morse tapez 1 \
    si vous voulez décoder un message en morse tapez 2" << std::endl;
    std::string info;
    std::getline (std::cin, info);

    if ( info == "1" ){ codage(); }
    else if ( info == "2" ) { decodage(); }
    else { printf("La commande saisie n'est pas valide"); }

    return 0;
}