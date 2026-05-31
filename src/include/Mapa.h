/**
 * @file Mapa.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções do Mapa.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Carrega um mapa a partir de um arquivo usando a textura da fase indicada.
 *        fase = 1 → Green Hill (texturaTerreno)
 *        fase = 2 → Marble Zone (texturaTerreno2)
 */
Mapa *carregarMapaFase( const char *caminhoArquivo, int fase );

/**
 * @brief Carrega um mapa usando a textura da fase 1 (compatibilidade).
 */
Mapa *carregarMapa( const char *caminhoArquivo );

/**
 * @brief Destroi um mapa.
 */
void destruirMapa( Mapa *m );

/**
 * @brief Atualiza um mapa.
 */
void atualizarMapa( Mapa *m, GameWorld *gw, float delta );

/**
 * @brief Desenha um mapa.
 */
void desenharMapa( Mapa *m );

/**
 * @brief Calcula a largura do mapa.
 */
int calcularLarguraMapa( Mapa *m );

/**
 * @brief Calcula a altura do mapa.
 */
int calcularAlturaMapa( Mapa *m );