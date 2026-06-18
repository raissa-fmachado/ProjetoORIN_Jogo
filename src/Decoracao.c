#include <stdlib.h>

#include "raylib/raylib.h"

#include "Decoracao.h"
#include "Animacao.h"
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

        decoracao->animacaoPrincipal.quantidadeQuadros = 3;
        decoracao->animacaoPrincipal.quadroAtual = 0;
        decoracao->animacaoPrincipal.contadorTempoQuadro = 0;
        decoracao->animacaoPrincipal.pararNoUltimoQuadro = false;
        decoracao->animacaoPrincipal.executarUmaVez = false;
        decoracao->animacaoPrincipal.finalizada = false;

        criarQuadrosAnimacao(
            &decoracao->animacaoPrincipal,
            3
        );

        inicializarQuadrosAnimacao(
            decoracao->animacaoPrincipal.quadros,
            3,
            250,
            0, 0,
            33, 60,
            1,
            false,
            (Rectangle){0}
        );

        decoracao->animacoes[0] = &decoracao->animacaoPrincipal;
        decoracao->quantidadeAnimacoes = 1;

        break;

    case TIPO_DECORACAO_GIRASSOL:

        decoracao->textura = &rm.texturaDecoracoes;

        decoracao->animacaoPrincipal.quantidadeQuadros = 2;
        decoracao->animacaoPrincipal.quadroAtual = 0;
        decoracao->animacaoPrincipal.contadorTempoQuadro = 0;
        decoracao->animacaoPrincipal.pararNoUltimoQuadro = false;
        decoracao->animacaoPrincipal.executarUmaVez = false;
        decoracao->animacaoPrincipal.finalizada = false;

        criarQuadrosAnimacao(
            &decoracao->animacaoPrincipal,
            2
        );

        inicializarQuadrosAnimacao(
            decoracao->animacaoPrincipal.quadros,
            2,
            300,
            0, 61,
            34, 76,
            1,
            false,
            (Rectangle){0}
        );

        decoracao->animacoes[0] = &decoracao->animacaoPrincipal;
        decoracao->quantidadeAnimacoes = 1;

        break;

    case TIPO_DECORACAO_PALMEIRA:

        decoracao->textura = &rm.texturaDecoracoes;

        decoracao->animacaoPrincipal.quantidadeQuadros = 1;
        criarQuadrosAnimacao(
            &decoracao->animacaoPrincipal,
            1
        );

        decoracao->animacaoPrincipal.quadros[0].fonte =
            (Rectangle){1, 226, 72, 125};

        decoracao->animacoes[0] = &decoracao->animacaoPrincipal;
        decoracao->quantidadeAnimacoes = 1;

        break;

    case TIPO_DECORACAO_ARBUSTO:

        decoracao->textura = &rm.texturaDecoracoes;

        decoracao->animacaoPrincipal.quantidadeQuadros = 1;
        criarQuadrosAnimacao(
            &decoracao->animacaoPrincipal,
            1
        );

        decoracao->animacaoPrincipal.quadros[0].fonte =
            (Rectangle){1, 188, 50, 37};

        decoracao->animacoes[0] = &decoracao->animacaoPrincipal;
        decoracao->quantidadeAnimacoes = 1;

        break;

    case TIPO_DECORACAO_TOTEM:

        decoracao->textura = &rm.texturaDecoracoes;

        decoracao->animacaoPrincipal.quantidadeQuadros = 1;
        criarQuadrosAnimacao(
            &decoracao->animacaoPrincipal,
            1
        );

        decoracao->animacaoPrincipal.quadros[0].fonte =
            (Rectangle){1, 138, 50, 49};

        decoracao->animacoes[0] = &decoracao->animacaoPrincipal;
        decoracao->quantidadeAnimacoes = 1;

        break;

    default:
        decoracao->textura = NULL;
        break;
    }

    return decoracao;
}

void destruirDecoracao(Decoracao *decoracao)
{
    if (decoracao != NULL)
    {
        for (int i = 0; i < decoracao->quantidadeAnimacoes; i++)
        {
            destruirQuadrosAnimacao(
                decoracao->animacoes[i]
            );
        }

        free(decoracao);
    }
}

void atualizarDecoracao(Decoracao *decoracao, float delta)
{
    if (decoracao == NULL)
    {
        return;
    }

    if (decoracao->quantidadeAnimacoes > 0)
    {
        atualizarAnimacao(
            decoracao->animacoes[0],
            delta
        );
    }
}

void desenharDecoracao(Decoracao *decoracao)
{
    if (decoracao == NULL || decoracao->textura == NULL)
    {
        return;
    }

    QuadroAnimacao *qa =
        getQuadroAtualAnimacao(
            decoracao->animacoes[0]
        );

    DrawTexturePro(
        *decoracao->textura,
        qa->fonte,
        decoracao->ret,
        (Vector2){0, 0},
        0.0f,
        WHITE
    );
}