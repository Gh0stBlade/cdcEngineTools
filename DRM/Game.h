#ifndef GAME_H
#define GAME_H

#if TR7
#define GAME_NAME "Tomb Raider: Legend"
#elif TRAE
#define GAME_NAME "Tomb Raider: Anniversary"
#elif TR8
#define GAME_NAME "Tomb Raider: Underworld"
#else
#error "Unsupported Game!"
#endif
#endif