/**
 * @file InimigoEggMobile.c
 * @brief Implementação do Inimigo (Egg Mobile).
 */

#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "InimigoEggMobile.h"
#include "Macros.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void desenharQuadroAnimacaoInimigoEggMobile(
    InimigoEggMobile *inimigo,
    QuadroAnimacao *qa,
    Color tonalidade);

static Animacao *getAnimacaoAtualInimigoEggMobile(
    InimigoEggMobile *inimigo);

static const bool MOSTRAR_RETANGULOS = true;

/**
 * @brief Cria um novo inimigo (Egg Mobile).
 */
InimigoEggMobile *criarInimigoEggMobile(
    Rectangle ret,
    Color cor)
{

    InimigoEggMobile *novoInimigo =
        (InimigoEggMobile *)malloc(
            sizeof(InimigoEggMobile));

    novoInimigo->ret = ret;
    novoInimigo->vel = (Vector2){120, 0};

    novoInimigo->cor = cor;

    novoInimigo->vida = 8;

    novoInimigo->ativo = true;
    novoInimigo->olhandoParaDireita = false;
    novoInimigo->invulneravel = false;

    novoInimigo->contadorEstado = 0.0f;
    novoInimigo->contadorInvulnerabilidade = 0.0f;

    novoInimigo->estado =
        ESTADO_INIMIGO_EGGMOBILE_VOANDO;

    /* =========================
       ANIMAÇÃO VOANDO
       ========================= */

    novoInimigo->animacaoVoando.quantidadeQuadros = 1;
    novoInimigo->animacaoVoando.quadroAtual = 0;
    novoInimigo->animacaoVoando.contadorTempoQuadro = 0;
    novoInimigo->animacaoVoando.pararNoUltimoQuadro = false;
    novoInimigo->animacaoVoando.executarUmaVez = false;
    novoInimigo->animacaoVoando.finalizada = false;

    criarQuadrosAnimacao(
        &novoInimigo->animacaoVoando,
        1);

    /* AJUSTAR DEPOIS */
    novoInimigo->animacaoVoando.quadros[0].fonte =
        (Rectangle){1, 83, 64, 56};

    novoInimigo->animacaoVoando.quadros[0].duracao =
        1000;

    novoInimigo->animacaoVoando.quadros[0].retColisao =
        (Rectangle){0, 0, 64, 56};

    /* =========================
       ANIMAÇÃO DANO
       ========================= */

    novoInimigo->animacaoDano.quantidadeQuadros = 1;
    novoInimigo->animacaoDano.quadroAtual = 0;
    novoInimigo->animacaoDano.contadorTempoQuadro = 0;
    novoInimigo->animacaoDano.pararNoUltimoQuadro = false;
    novoInimigo->animacaoDano.executarUmaVez = false;
    novoInimigo->animacaoDano.finalizada = false;

    criarQuadrosAnimacao(
        &novoInimigo->animacaoDano,
        1);

    novoInimigo->animacaoDano.quadros[0].fonte =
        (Rectangle){0, 0, 64, 64};

    novoInimigo->animacaoDano.quadros[0].duracao =
        100;

    novoInimigo->animacaoDano.quadros[0].retColisao =
        (Rectangle){0, 0, 64, 64};

    /* =========================
       ANIMAÇÃO DERROTADO
       ========================= */

    novoInimigo->animacaoDerrotado.quantidadeQuadros = 1;
    novoInimigo->animacaoDerrotado.quadroAtual = 0;
    novoInimigo->animacaoDerrotado.contadorTempoQuadro = 0;
    novoInimigo->animacaoDerrotado.pararNoUltimoQuadro = true;
    novoInimigo->animacaoDerrotado.executarUmaVez = true;
    novoInimigo->animacaoDerrotado.finalizada = false;

    criarQuadrosAnimacao(
        &novoInimigo->animacaoDerrotado,
        1);

    novoInimigo->animacaoDerrotado.quadros[0].fonte =
        (Rectangle){0, 0, 64, 64};

    novoInimigo->animacaoDerrotado.quadros[0].duracao =
        1000;

    novoInimigo->animacaoDerrotado.quadros[0].retColisao =
        (Rectangle){0, 0, 64, 64};

    /* =========================
       ESTADOS
       ========================= */

    novoInimigo->animacoes[ESTADO_INIMIGO_EGGMOBILE_VOANDO] =
        &novoInimigo->animacaoVoando;

    novoInimigo->animacoes[ESTADO_INIMIGO_EGGMOBILE_DANO] =
        &novoInimigo->animacaoDano;

    novoInimigo->animacoes[ESTADO_INIMIGO_EGGMOBILE_DERROTADO] =
        &novoInimigo->animacaoDerrotado;

    novoInimigo->quantidadeAnimacoes = 3;

    return novoInimigo;
}

/**
 * @brief Destroi um inimigo (Egg Mobile).
 */
void destruirInimigoEggMobile(
    InimigoEggMobile *inimigo)
{
    if (inimigo != NULL)
    {
        destruirQuadrosAnimacao(
            &inimigo->animacaoVoando);

        destruirQuadrosAnimacao(
            &inimigo->animacaoDano);

        destruirQuadrosAnimacao(
            &inimigo->animacaoDerrotado);

        free(inimigo);
    }
}

/**
 * @brief Atualiza um inimigo (Egg Mobile).
 */
void atualizarInimigoEggMobile(
    InimigoEggMobile *inimigo,
    GameWorld *gw,
    float delta)
{
    if (!inimigo->ativo)
        return;

    atualizarAnimacao(
        getAnimacaoAtualInimigoEggMobile(
            inimigo),
        delta);

    switch (inimigo->estado)
    {

    case ESTADO_INIMIGO_EGGMOBILE_VOANDO:

        inimigo->ret.x +=
            inimigo->vel.x * delta;

        if (inimigo->ret.x < 100)
        {
            inimigo->vel.x = 120;
            inimigo->olhandoParaDireita = true;
        }

        if (inimigo->ret.x > 900)
        {
            inimigo->vel.x = -120;
            inimigo->olhandoParaDireita = false;
        }

        break;

    case ESTADO_INIMIGO_EGGMOBILE_DANO:

        inimigo->contadorEstado += delta;

        if (inimigo->contadorEstado >= 0.3f)
        {
            inimigo->contadorEstado = 0;

            if (inimigo->vida > 0)
            {
                inimigo->estado =
                    ESTADO_INIMIGO_EGGMOBILE_VOANDO;
            }
            else
            {
                inimigo->estado =
                    ESTADO_INIMIGO_EGGMOBILE_DERROTADO;
            }
        }

        break;

    case ESTADO_INIMIGO_EGGMOBILE_DERROTADO:

        inimigo->ativo = false;

        break;
    }

    if (inimigo->invulneravel)
    {
        inimigo->contadorInvulnerabilidade += delta;

        if (inimigo->contadorInvulnerabilidade >= 1.0f)
        {
            inimigo->invulneravel = false;
            inimigo->contadorInvulnerabilidade = 0;
        }
    }
}

/**
 * @brief Desenha um inimigo (Egg Mobile).
 */
void desenharInimigoEggMobile(
    InimigoEggMobile *inimigo)
{
    if (!inimigo->ativo)
        return;

    Color cor = WHITE;

    if (inimigo->invulneravel)
    {
        if (((int)(GetTime() * 10)) % 2 == 0)
        {
            cor = Fade(WHITE, 0.3f);
        }
    }

    desenharQuadroAnimacaoInimigoEggMobile(
        inimigo,
        getQuadroAnimacaoAtualInimigoEggMobile(
            inimigo),
        cor);

    if (MOSTRAR_RETANGULOS)
    {
        DrawRectangleRec(
            inimigo->ret,
            Fade(inimigo->cor, 0.5f));

        DrawRectangleLines(
            inimigo->ret.x,
            inimigo->ret.y,
            inimigo->ret.width,
            inimigo->ret.height,
            BLACK);
    }
}

/**
 * @brief Obtém o quadro atual.
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoEggMobile(
    InimigoEggMobile *inimigo)
{
    return getQuadroAtualAnimacao(
        getAnimacaoAtualInimigoEggMobile(
            inimigo));
}

static void desenharQuadroAnimacaoInimigoEggMobile(
    InimigoEggMobile *inimigo,
    QuadroAnimacao *qa,
    Color tonalidade)
{
    if (qa != NULL)
    {
        DrawTexturePro(
            rm.texturaChefao,
            (Rectangle){
                qa->fonte.x,
                qa->fonte.y,
                inimigo->olhandoParaDireita
                    ? -qa->fonte.width
                    : qa->fonte.width,
                qa->fonte.height},
            inimigo->ret,
            (Vector2){0, 0},
            0.0f,
            tonalidade);
    }
}

static Animacao *getAnimacaoAtualInimigoEggMobile(
    InimigoEggMobile *inimigo)
{
    return inimigo->animacoes[inimigo->estado];
}