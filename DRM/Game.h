#ifndef GAME_H
#define GAME_H

#if TR7
#define GAME_NAME "Tomb Raider: Legend"
#elif TRAE
#define GAME_NAME "Tomb Raider: Anniversary"
#elif TR8
#define GAME_NAME "Tomb Raider: Underworld"
#elif TRAS
#define GAME_NAME "Tomb Raider: Ascension"
#else
#error "Unsupported Game!"
#endif
#endif