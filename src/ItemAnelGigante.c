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

static void desenharQuadroAnimacaoItemAnelGigante(ItemAnelGigante *item, QuadroAnimacao *qa, Color tonalidade);
static Animacao *getAnimacaoAtualItemAnelGigante(ItemAnelGigante *item);
static QuadroAnimacao *getQuadroAnimacaoAtualItemAnelGigante(ItemAnelGigante *item);

static const bool MOSTRAR_RETANGULOS = false;

ItemAnelGigante *criarItemAnelGigante(Rectangle ret, Color cor)
{
    ItemAnelGigante *novoItem = (ItemAnelGigante *)malloc(sizeof(ItemAnelGigante));

    novoItem->ret = ret;
    novoItem->cor = cor;
    novoItem->estado = ESTADO_ITEM_ANEL_GIGANTE_SURGINDO;
    novoItem->ativo = true;

    novoItem->animacaoSurgindo.quantidadeQuadros = 7;
    novoItem->animacaoSurgindo.quadroAtual = 0;
    novoItem->animacaoSurgindo.contadorTempoQuadro = 0.0f;
    novoItem->animacaoSurgindo.pararNoUltimoQuadro = false;
    novoItem->animacaoSurgindo.executarUmaVez = true;
    novoItem->animacaoSurgindo.finalizada = false;

    criarQuadrosAnimacao(&novoItem->animacaoSurgindo, novoItem->animacaoSurgindo.quantidadeQuadros);

    inicializarQuadrosAnimacao(
        novoItem->animacaoSurgindo.quadros,
        novoItem->animacaoSurgindo.quantidadeQuadros,
        70,
        1, 247,
        64, 64,
        1,
        false,
        (Rectangle){0});

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
        1,
        false,
        (Rectangle){0});

    novoItem->animacoes[ESTADO_ITEM_ANEL_GIGANTE_SURGINDO] = &novoItem->animacaoSurgindo;
    novoItem->animacoes[ESTADO_ITEM_ANEL_GIGANTE_PARADO] = &novoItem->animacaoParado;
    novoItem->animacoes[ESTADO_ITEM_ANEL_GIGANTE_COLETADO] = &novoItem->animacaoColetando;
    novoItem->quantidadeAnimacoes = 2;

    return novoItem;
}

void destruirItemAnelGigante(ItemAnelGigante *item)
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

void atualizarItemAnelGigante(ItemAnelGigante *item, float delta)
{
    if (item->ativo)
    {
        Animacao *anim = getAnimacaoAtualItemAnelGigante(item);
        atualizarAnimacao(anim, delta);

        if (item->estado == ESTADO_ITEM_ANEL_GIGANTE_SURGINDO &&
            anim->finalizada)
        {
            item->estado = ESTADO_ITEM_ANEL_GIGANTE_PARADO;

            item->animacaoParado.quadroAtual = 0;
            item->animacaoParado.contadorTempoQuadro = 0;
        }

        if (item->estado == ESTADO_ITEM_ANEL_GIGANTE_COLETADO && anim->finalizada)
        {
            item->ativo = false;
        }
    }
}

void desenharItemAnelGigante(ItemAnelGigante *item)
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

QuadroAnimacao *getQuadroAnimacaoAtualItemAnelGigante(ItemAnelGigante *item)
{
    return getQuadroAtualAnimacao(getAnimacaoAtualItemAnelGigante(item));
}

static void desenharQuadroAnimacaoItemAnelGigante(ItemAnelGigante *item, QuadroAnimacao *qa, Color tonalidade)
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

static Animacao *getAnimacaoAtualItemAnelGigante(ItemAnelGigante *item)
{
    return item->animacoes[item->estado];
}