/**
 * @file Hud.h
 * @brief Declarações do HUD (anéis, pontuação, tempo e vidas) no estilo Sonic 1.
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Desenha o HUD completo na tela (fora do BeginMode2D).
 */
void desenharHud( GameWorld *gw );