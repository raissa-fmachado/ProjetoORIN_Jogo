#include <stdlib.h>

#include "raylib/raylib.h"

#include "Decoracao.h"
#include "ResourceManager.h"

extern ResourceManager rm;

Decoracao *criarDecoracao(TipoDecoracao tipo)
{
    Decoracao *decoracao = malloc(sizeof(Decoracao));

    decoracao->tipo = tipo;
    decoracao->ret = (Rectangle){0};
    decoracao->quantidadeAnimacoes = 0;

    switch (tipo)
    {
    case TIPO_DECORACAO_FLOR:
        decoracao->textura = &rm.texturaDecoracoes;
        decoracao->fonte = (Rectangle){0, 0, 33, 60};
        break;

    case TIPO_DECORACAO_PALMEIRA:
        decoracao->textura = &rm.texturaDecoracoes;
        decoracao->fonte = (Rectangle){48, 0, 96, 128};
        break;

    case TIPO_DECORACAO_ARBUSTO:
        decoracao->textura = &rm.texturaDecoracoes;
        decoracao->fonte = (Rectangle){144, 0, 64, 48};
        break;

    default:
        decoracao->textura = NULL;
        decoracao->fonte = (Rectangle){0};
        break;
    }

    return decoracao;
}

void destruirDecoracao(Decoracao *decoracao)
{
    if (decoracao != NULL)
    {
        free(decoracao);
    }
}

void atualizarDecoracao(Decoracao *decoracao, float delta)
{
    (void)decoracao;
    (void)delta;
}

void desenharDecoracao(Decoracao *decoracao)
{
    if (decoracao == NULL || decoracao->textura == NULL)
    {
        return;
    }

    DrawTexturePro(
        *decoracao->textura,
        decoracao->fonte,
        decoracao->ret,
        (Vector2){0, 0},
        0.0f,
        WHITE);
}