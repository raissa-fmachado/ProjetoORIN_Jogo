/**
 * @file Decoracao.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções da Decoração.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria uma nova decoração.
 */
Decoracao *criarDecoracao( TipoDecoracao tipo );

/**
 * @brief Destroi uma decoração.
 */
void destruirDecoracao( Decoracao *decoracao );

/**
 * @brief Atualiza uma decoração.
 */
void atualizarDecoracao( Decoracao *decoracao, float delta );

/**
 * @brief Desenha uma decoração.
 */
void desenharDecoracao( Decoracao *decoracao );