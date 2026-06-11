/**
 * @file ItemEscudo.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções do Item (Escudo).
 */

#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo item (Escudo).
 */
ItemEscudo *criarItemEscudo(Rectangle ret, Color cor);

/**
 * @brief Destroi um item (Escudo).
 */
void destruirItemEscudo(ItemEscudo *item);

/**
 * @brief Atualiza um item (Escudo).
 */
void atualizarItemEscudo(ItemEscudo *item, float delta);

/**
 * @brief Desenha um item (Escudo).
 */
void desenharItemEscudo(ItemEscudo *item);

/**
 * @brief Obtém o quadro de animação atual do Escudo.
 */
QuadroAnimacao *getQuadroAnimacaoAtualItemEscudo(ItemEscudo *item);