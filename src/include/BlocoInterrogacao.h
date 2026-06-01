/**
 * @file BlocoInterrogacao.h
 * @brief Bloco de interrogação: bata por baixo para soltar anéis.
 */
#pragma once

#include "Tipos.h"

BlocoInterrogacao *criarBlocoInterrogacao( Rectangle ret, int aneisParaSoltar );
void destruirBlocoInterrogacao( BlocoInterrogacao *b );
void atualizarBlocoInterrogacao( BlocoInterrogacao *b, float delta );
void desenharBlocoInterrogacao( BlocoInterrogacao *b );
void ativarBlocoInterrogacao( BlocoInterrogacao *b );