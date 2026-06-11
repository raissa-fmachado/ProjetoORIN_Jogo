#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo item (anel gigante).
 */
ItemAnelGigante *criarItemAnelGigante(Rectangle ret, Color cor);

/**
 * @brief Destroi um item (anel gigante).
 */
void destruirItemAnelGigante(ItemAnelGigante *item);

/**
 * @brief Atualiza um item (anel gigante).
 */
void atualizarItemAnelGigante(ItemAnelGigante *item, float delta);

/**
 * @brief Desenha um item (anel gigante).
 */
void desenharItemAnelGigante(ItemAnelGigante *item);

/**
 * @brief Obtém o quadro de animação atual.
 */
QuadroAnimacao *getQuadroAnimacaoAtualItemAnelGigante(ItemAnelGigante *item);