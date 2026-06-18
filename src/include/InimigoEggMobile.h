/**
 * @file InimigoEggMobile.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções do Inimigo (Egg Mobile).
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo inimigo (Egg Mobile).
 */
InimigoEggMobile *criarInimigoEggMobile(
    Rectangle ret,
    Color cor
);

/**
 * @brief Destroi um inimigo (Egg Mobile).
 */
void destruirInimigoEggMobile(
    InimigoEggMobile *inimigo
);

/**
 * @brief Atualiza um inimigo (Egg Mobile).
 */
void atualizarInimigoEggMobile(
    InimigoEggMobile *inimigo,
    GameWorld *gw,
    float delta
);

/**
 * @brief Desenha um inimigo (Egg Mobile).
 */
void desenharInimigoEggMobile(
    InimigoEggMobile *inimigo
);

/**
 * @brief Aplica dano ao Egg Mobile.
 */
void causarDanoInimigoEggMobile(
    InimigoEggMobile *inimigo
);

/**
 * @brief Verifica se o Egg Mobile foi derrotado.
 */
bool inimigoEggMobileDerrotado(
    InimigoEggMobile *inimigo
);

/**
 * @brief Obtém o quadro de animação atual do Egg Mobile.
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoEggMobile(
    InimigoEggMobile *inimigo
);