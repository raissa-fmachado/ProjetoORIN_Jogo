/**
 * @file TelaInicial.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções da Tela Inicial.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria a tela inicial.
 */
TelaInicial *criarTelaInicial( void );

/**
 * @brief Destroi a tela inicial.
 */
void destruirTelaInicial( TelaInicial *tela );

/**
 * @brief Atualiza a tela inicial.
 */
void atualizarTelaInicial( TelaInicial *tela, float delta );

/**
 * @brief Desenha a tela inicial.
 */
void desenharTelaInicial( TelaInicial *tela );

/**
 * @brief Obtém o quadro atual da animação do Sonic da tela inicial.
 */
QuadroAnimacao *getQuadroAnimacaoAtualTelaInicial( TelaInicial *tela );