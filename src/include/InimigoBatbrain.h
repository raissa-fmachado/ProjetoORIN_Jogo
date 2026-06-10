/**
 * @file InimigoBatbrain.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções do Inimigo (Batbrain).
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo inimigo (Batbrain).
 */
InimigoBatbrain *criarInimigoBatbrain(Rectangle ret, Color cor);

/**
 * @brief Destroi um inimigo (Batbrain).
 */
void destruirInimigoBatbrain(InimigoBatbrain *inimigo);

/**
 * @brief Atualiza um inimigo (Batbrain).
 */
void atualizarInimigoBatbrain(InimigoBatbrain *inimigo, GameWorld *gw, float delta);

/**
 * @brief Desenha um inimigo (Batbrain).
 */
void desenharInimigoBatbrain(InimigoBatbrain *inimigo);

/**
 * @brief Obtém o quadro de animação atual de um inimigo (Batbrain).
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoBatbrain(InimigoBatbrain *inimigo);