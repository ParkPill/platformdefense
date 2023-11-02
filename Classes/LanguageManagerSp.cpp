#include "LanguageManager.h"

void LanguageManager::setSpanish(){
    strArray.clear();
    strArray.push_back(std::string("NotoSans-Bold.ttf")); // 0
    strArray.push_back(std::string("øUna nueva partida?")); // 1
    strArray.push_back(std::string("MantÈn la pantalla para disparar. °Consigue monedas matando enemigos!")); // 2
    strArray.push_back(std::string("Pulsa el botÛn de la trampa a la derecha. Y colÛcalo en el rect·ngulo.")); // 3
    strArray.push_back(std::string("Si ya has instalado una trampa te aparecer· el botÛn de mejorar.")); // 4
    strArray.push_back(std::string("°El juego se acaba pero que no cunda el p·nico! No pierdes trampas ni monedas. °Consigue m·s monedas y fortalece a˙n m·s tus trampas!")); // 5
    strArray.push_back(std::string("°Punto de control! Puedes continuar desde aquÌ, aunque no completes la partida.")); // 6
    strArray.push_back(std::string("No queda nada m·s por explicar. °Planea tu defensa y supera las 100 oleadas! °VAMOOOOS!")); // 7
    strArray.push_back(std::string("NORMAL: NADA ESPECIAL")); // 8
    strArray.push_back(std::string("FINO: ALGO M¡S R¡PIDO")); // 9
    strArray.push_back(std::string("DIFÕCIL: IGNORA LOS PINCHOS")); // 10
    strArray.push_back(std::string("SANO: UN TOQUECITO")); // 11
    strArray.push_back(std::string("PESADO: LENTO PERO EFECTIVO")); // 12
    strArray.push_back(std::string("FUEGO: NUNCA ARDE")); // 13
    strArray.push_back(std::string("LIGER: R¡PIDO, PERO D…BIL")); // 14
    strArray.push_back(std::string("HIELO: NUNCA SE CONGELA")); // 15
    strArray.push_back(std::string("CIELO: AHORA FUNCIONAN LAS TRAMPAS DEL SUELO")); // 16
    strArray.push_back(std::string("JEFE: °DE ARMAS TOMAR!")); // 17
    strArray.push_back(std::string("El hielo apaga el fuego.")); // 18
    strArray.push_back(std::string("El fuego derrite el hielo.")); // 19
    strArray.push_back(std::string("Los enemigos voladores evitan las trampas del suelo.")); // 20
    strArray.push_back(std::string("Cuidado con los enemigos grandes.")); // 21
    strArray.push_back(std::string("Los enemigos r·pidos reciben menos daÒo del fuego y los pinchos.")); // 22
    strArray.push_back(std::string("°Los enemigos reciben el doble de daÒo al estar quemados!")); // 23
    strArray.push_back(std::string("Es difÌcil congelar a los enemigos r·pidos y pesados.")); // 24
    strArray.push_back(std::string("øSabes cu·ntos enemigos te has cargado? °MÌralo en el men˙ de clasificaciÛn!")); // 25
    strArray.push_back(std::string("°Cuantas menos monedas gastas, mejor clasificaciÛn obtendr·s!")); // 26
    strArray.push_back(std::string("Al mejorar el muro no solo se hace m·s resistente, tambiÈn se rellena su energÌa.")); // 27
    strArray.push_back(std::string("Recibes 1 estrella al matar a 20 enemigos.")); // 28
    strArray.push_back(std::string("El modo r·pido es m·s difÌcil.")); // 29
    strArray.push_back(std::string("°Genial! °PublÌcalo en Facebook!")); // 30
    strArray.push_back(std::string("°Todos deberÌan ver esto!")); // 31
    strArray.push_back(std::string("Esto es para FB. No puedo evitarlo.")); // 32
    strArray.push_back(std::string("No hay otra opciÛn, hay que publicarlo.")); // 33
    strArray.push_back(std::string("SerÌa un crimen no publicar esto.")); // 34
    strArray.push_back(std::string("°Comparte tu experiencia con tus amigos!")); // 35
    strArray.push_back(std::string("°100 oleadas superadas!\n°Se ha publicado tu puntuaciÛn! °El final est· en la oleada 1.000!")); // 36
    strArray.push_back(std::string("°BonificaciÛn diaria!\n°MaÒana m·s! :O")); // 37
    strArray.push_back(std::string("Ardiente: quema a los enemigos que lo pisan")); // 38
    strArray.push_back(std::string("Congelante: quema a los enemigos que lo pisan")); // 39
    strArray.push_back(std::string("Muro: detiene a los enemigos durante un momento")); // 40
    strArray.push_back(std::string("Flask: bomba de un uso")); // 41
    strArray.push_back(std::string("Roca: ralentiza y daÒa a los enemigos")); // 42
    strArray.push_back(std::string("Bomba: bomba de un uso (toca al pasar un segundo tras tocarla)")); // 43
    strArray.push_back(std::string("Fuego: quema gravemente a los enemigos que lo pisan")); // 44
    strArray.push_back(std::string("Pinchos: daÒa a los enemigos que lo pisan")); // 45
    strArray.push_back(std::string("Pincho superior: pinchos que caen del techo")); // 46
    strArray.push_back(std::string("Bola de nieve: lanza bolas de nieve que ralentizan")); // 47
    strArray.push_back(std::string("Mano grande: empuja a los monstruos escaleras abajo")); // 48
    strArray.push_back(std::string("Pinchos de pared: reduce la velocidad y daÒa gravemente a los enemigos")); // 49
    strArray.push_back(std::string("AntiaÈreo: ataca a los enemigos voladores")); // 50
    strArray.push_back(std::string("Inicia nueva partida 5 veces antes de la oleada 50")); // 51
    strArray.push_back(std::string("Llega a la oleada 70\nsin el antiaÈreo")); // 52
    strArray.push_back(std::string("Llega a la oleada 50 sin ninguna trampa")); // 53
    strArray.push_back(std::string("Mata 100.000 enemigos")); // 54
    strArray.push_back(std::string("ObtÈn la recompensa diaria n∫ 15")); // 55
    strArray.push_back(std::string("No se guardar·n las monedas. G·stalas antes de nada.")); // 56
    strArray.push_back(std::string("Los datos no se guardan autom·ticamente. Si eliminas el juego, perder·s todo.")); // 57
    strArray.push_back(std::string("Si cargas los datos, perder·s todas las monedas. øQuieres continuar?")); // 58
    strArray.push_back(std::string("Califica nuestra aplicación!")); // 59
}