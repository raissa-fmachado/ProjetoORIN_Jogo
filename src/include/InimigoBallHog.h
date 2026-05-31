/**
 * @file InimigoBallHog.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções do Inimigo (BallHog).
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo inimigo (ballhog).
 */
InimigoBallHog *criarInimigoBallHog( Rectangle ret, Color cor );

/**
 * @brief Destroi um inimigo (ballhog).
 */
void destruirInimigoBallHog( InimigoBallHog *inimigo );

/**
 * @brief Atualiza um inimigo (ballhog).
 */
void atualizarInimigoBallHog( InimigoBallHog *inimigo, GameWorld *gw, float delta );

/**
 * @brief Desenha um inimigo (ballhog).
 */
void desenharInimigoBallHog( InimigoBallHog *inimigo );

/**
 * @brief Obtém o quadro de animação atual de um inimigo (ballhog).
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoBallHog( InimigoBallHog *inimigo );