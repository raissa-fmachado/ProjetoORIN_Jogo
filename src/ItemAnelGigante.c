/**
 * @file ItemAnelGigante.c
 * @brief Implementação do Item (Anel Gigante).
 */

#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "ItemAnel.h"
#include "Macros.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void desenharQuadroAnimacaoItemAnelGigante(ItemAnel *item, QuadroAnimacao *qa, Color tonalidade);
static Animacao *getAnimacaoAtualItemAnelGigante(ItemAnel *item);
static QuadroAnimacao *getQuadroAnimacaoAtualItemAnelGigante(ItemAnel *item);

static const bool MOSTRAR_RETANGULOS = false;

ItemAnel *criarItemAnelGigante(Rectangle ret, Color cor)
{
    ItemAnel *novoItem = (ItemAnel *)malloc(sizeof(ItemAnel));

    novoItem->ret = ret;
    novoItem->cor = cor;
    novoItem->estado = ESTADO_ITEM_ANEL_PARADO;
    novoItem->ativo = true;

    novoItem->animacaoParado.quantidadeQuadros = 4;
    novoItem->animacaoParado.quadroAtual = 0;
    novoItem->animacaoParado.contadorTempoQuadro = 0.0f;
    novoItem->animacaoParado.pararNoUltimoQuadro = false;
    novoItem->animacaoParado.executarUmaVez = false;
    novoItem->animacaoParado.finalizada = false;

    criarQuadrosAnimacao(&novoItem->animacaoParado, novoItem->animacaoParado.quantidadeQuadros);

    inicializarQuadrosAnimacao(
        novoItem->animacaoParado.quadros,
        novoItem->animacaoParado.quantidadeQuadros,
        140,
        1, 182,
        64, 64,
        2,
        false,
        (Rectangle){0, 0, 64, 64});

    novoItem->animacaoColetando.quantidadeQuadros = 7;
    novoItem->animacaoColetando.quadroAtual = 0;
    novoItem->animacaoColetando.contadorTempoQuadro = 0.0f;
    novoItem->animacaoColetando.pararNoUltimoQuadro = false;
    novoItem->animacaoColetando.executarUmaVez = true;
    novoItem->animacaoColetando.finalizada = false;

    criarQuadrosAnimacao(&novoItem->animacaoColetando, novoItem->animacaoColetando.quantidadeQuadros);

    inicializarQuadrosAnimacao(
        novoItem->animacaoColetando.quadros,
        novoItem->animacaoColetando.quantidadeQuadros,
        70,
        1, 247,
        64, 64,
        2,
        false,
        (Rectangle){0});

    novoItem->animacoes[ESTADO_ITEM_ANEL_PARADO] = &novoItem->animacaoParado;
    novoItem->animacoes[ESTADO_ITEM_ANEL_COLETADO] = &novoItem->animacaoColetando;
    novoItem->quantidadeAnimacoes = 2;

    return novoItem;
}

void destruirItemAnelGigante(ItemAnel *item)
{
    if (item)
    {
        for (int i = 0; i < item->quantidadeAnimacoes; i++)
        {
            destruirQuadrosAnimacao(item->animacoes[i]);
        }
        free(item);
    }
}

void atualizarItemAnelGigante(ItemAnel *item, float delta)
{
    if (item->ativo)
    {
        Animacao *anim = getAnimacaoAtualItemAnelGigante(item);
        atualizarAnimacao(anim, delta);

        if (item->estado == ESTADO_ITEM_ANEL_COLETADO && anim->finalizada)
        {
            item->ativo = false;
        }
    }
}

void desenharItemAnelGigante(ItemAnel *item)
{
    if (item->ativo)
    {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualItemAnelGigante(item);

        desenharQuadroAnimacaoItemAnelGigante(item, qa, WHITE);

        if (MOSTRAR_RETANGULOS)
        {
            DrawRectangleRec(item->ret, Fade(item->cor, 0.5f));
            DrawRectangleLines(item->ret.x, item->ret.y, item->ret.width, item->ret.height, BLACK);
        }
    }
}

QuadroAnimacao *getQuadroAnimacaoAtualItemAnelGigante(ItemAnel *item)
{
    return getQuadroAtualAnimacao(getAnimacaoAtualItemAnelGigante(item));
}

static void desenharQuadroAnimacaoItemAnelGigante(ItemAnel *item, QuadroAnimacao *qa, Color tonalidade)
{
    if (qa)
    {
        DrawTexturePro(
            rm.texturaItens,
            qa->fonte,
            item->ret,
            (Vector2){0},
            0.0f,
            tonalidade);

        if (MOSTRAR_RETANGULOS)
        {
            DrawRectangle(
                item->ret.x + qa->retColisao.x,
                item->ret.y + qa->retColisao.y,
                qa->retColisao.width,
                qa->retColisao.height,
                Fade(GREEN, 0.5f));
        }
    }
}

static Animacao *getAnimacaoAtualItemAnelGigante(ItemAnel *item)
{
    return item->animacoes[item->estado];
}