/**
 * @file InimigoBatbrain.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Inimigo (Batbrain).
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "Inimigo.h"
#include "InimigoBatbrain.h"
#include "Macros.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void desenharQuadroAnimacaoInimigoBatbrain(InimigoBatbrain *inimigo, QuadroAnimacao *qa, Color tonalidade);
static void desenharQuadroAnimacaoInimigoBatbrainMorrendo(InimigoBatbrain *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade);
static Animacao *getAnimacaoAtualInimigoBatbrain(InimigoBatbrain *inimigo);

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria um novo Inimigo (BATBRAIN).
 */
InimigoBatbrain *criarInimigoBatbrain(Rectangle ret, Color cor)
{

    InimigoBatbrain *novoInimigo = (InimigoBatbrain *)malloc(sizeof(InimigoBatbrain));

    novoInimigo->ret = ret;
    novoInimigo->posInicial = (Vector2){
        ret.x,
        ret.y};
    novoInimigo->mergulhando = false;
    novoInimigo->velocidadeMergulho = 180.0f;
    novoInimigo->distanciaAtivacao = 140.0f;
    novoInimigo->alturaRetorno = ret.y;

    novoInimigo->vel = (Vector2){0};
    novoInimigo->cor = cor;

    novoInimigo->velAndando = 60;
    novoInimigo->velMaxQueda = 600;

    novoInimigo->estado = ESTADO_INIMIGO_BATBRAIN_PARADO;
    novoInimigo->ativo = true;
    novoInimigo->olhandoParaDireita = false;

    int quantidadeAnimacoes = 4;

    novoInimigo->animacaoParado.quantidadeQuadros = 1;
    novoInimigo->animacaoParado.quadroAtual = 0;
    novoInimigo->animacaoParado.contadorTempoQuadro = 0;
    novoInimigo->animacaoParado.pararNoUltimoQuadro = false;
    novoInimigo->animacaoParado.executarUmaVez = false;
    novoInimigo->animacaoParado.finalizada = false;
    novoInimigo->animacaoAndando.quantidadeQuadros = 3;
    novoInimigo->animacaoAndando.quadroAtual = 0;
    novoInimigo->animacaoAndando.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoAndando.pararNoUltimoQuadro = false;
    novoInimigo->animacaoAndando.executarUmaVez = false;
    novoInimigo->animacaoAndando.finalizada = false;

    criarQuadrosAnimacao(
        &novoInimigo->animacaoParado,
        1);

    novoInimigo->animacaoParado.quadros[0].fonte =
        (Rectangle){1, 113, 16, 24};

    novoInimigo->animacaoParado.quadros[0].duracao = 1000;

    novoInimigo->animacaoParado.quadros[0].retColisao =
        (Rectangle){2, 2, 16, 24};

    criarQuadrosAnimacao(&novoInimigo->animacaoAndando, novoInimigo->animacaoAndando.quantidadeQuadros);
    inicializarQuadrosAnimacao(
        novoInimigo->animacaoAndando.quadros,
        novoInimigo->animacaoAndando.quantidadeQuadros,
        250,        // duração padrão para todos os quadros
        1, 113,     // início
        16, 24,     // dimensões
        1,          // separação
        false,      // de trás para frente
        (Rectangle){// retângulo de colisão padrão para cada quadro
                    2, 2, 28, 28});

    novoInimigo->animacaoAndando.quadros[0].fonte =
        (Rectangle){18, 113, 32, 32};

    novoInimigo->animacaoAndando.quadros[1].fonte =
        (Rectangle){51, 113, 32, 24};

    novoInimigo->animacaoAndando.quadros[2].fonte =
        (Rectangle){84, 113, 24, 32};

    for (int i = 0; i < novoInimigo->animacaoAndando.quantidadeQuadros; i++)
    {
        novoInimigo->animacaoAndando.quadros[i].retColisao =
            (Rectangle){2, 2, 44, 30};
    }

    novoInimigo->animacaoMorrendo.quantidadeQuadros = 4;
    novoInimigo->animacaoMorrendo.quadroAtual = 0;
    novoInimigo->animacaoMorrendo.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoMorrendo.pararNoUltimoQuadro = false;
    novoInimigo->animacaoMorrendo.executarUmaVez = true;
    novoInimigo->animacaoMorrendo.finalizada = false;
    criarQuadrosAnimacao(&novoInimigo->animacaoMorrendo, novoInimigo->animacaoMorrendo.quantidadeQuadros);
    inicializarQuadrosAnimacao(
        novoInimigo->animacaoMorrendo.quadros,
        novoInimigo->animacaoMorrendo.quantidadeQuadros,
        100,           // duração padrão para todos os quadros
        169, 1,        // início
        32, 32,        // dimensões
        1,             // separação
        false,         // de trás para frente
        (Rectangle){0} // retângulo de colisão padrão para cada quadro
    );

    novoInimigo->animacoes[ESTADO_INIMIGO_BATBRAIN_PARADO] =
        &novoInimigo->animacaoParado;

    novoInimigo->animacoes[ESTADO_INIMIGO_BATBRAIN_MERGULHANDO] =
        &novoInimigo->animacaoAndando;

    novoInimigo->animacoes[ESTADO_INIMIGO_BATBRAIN_SUBINDO] =
        &novoInimigo->animacaoAndando;

    novoInimigo->animacoes[ESTADO_INIMIGO_BATBRAIN_MORRENDO] =
        &novoInimigo->animacaoMorrendo;
    quantidadeAnimacoes++;

    novoInimigo->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoInimigo;
}

/**
 * @brief Destroi um inimigo (BATBRAIN).
 */
void destruirInimigoBatbrain(InimigoBatbrain *inimigo)
{
    if (inimigo != NULL)
    {
        destruirQuadrosAnimacao(&inimigo->animacaoParado);
        destruirQuadrosAnimacao(&inimigo->animacaoAndando);
        destruirQuadrosAnimacao(&inimigo->animacaoMorrendo);

        free(inimigo);
    }
}

/**
 * @brief Atualiza um inimigo (BATBRAIN).
 */
void atualizarInimigoBatbrain(InimigoBatbrain *inimigo, GameWorld *gw, float delta)
{
    if (!inimigo->ativo)
        return;

    atualizarAnimacao(
        getAnimacaoAtualInimigoBatbrain(inimigo),
        delta);

    switch (inimigo->estado)
    {
    case ESTADO_INIMIGO_BATBRAIN_PARADO:
    {
        float dx = gw->jogador->ret.x - inimigo->ret.x;

        if (fabsf(dx) < inimigo->distanciaAtivacao)
        {
            inimigo->olhandoParaDireita = dx > 0;
            inimigo->estado = ESTADO_INIMIGO_BATBRAIN_MERGULHANDO;
        }

        break;
    }

    case ESTADO_INIMIGO_BATBRAIN_MERGULHANDO:

        inimigo->ret.x +=
            (inimigo->olhandoParaDireita ? 1 : -1) *
            inimigo->velocidadeMergulho * delta;

        inimigo->ret.y += 120.0f * delta;

        if (inimigo->ret.y >= inimigo->posInicial.y + 80)
        {
            inimigo->estado = ESTADO_INIMIGO_BATBRAIN_SUBINDO;
        }

        break;

    case ESTADO_INIMIGO_BATBRAIN_SUBINDO:

        inimigo->ret.y -= 80.0f * delta;

        if (inimigo->ret.y <= inimigo->posInicial.y)
        {
            inimigo->ret.y = inimigo->posInicial.y;
            inimigo->estado = ESTADO_INIMIGO_BATBRAIN_PARADO;
        }

        break;

    case ESTADO_INIMIGO_BATBRAIN_MORRENDO:

        if (inimigo->animacaoMorrendo.finalizada)
            inimigo->ativo = false;

        break;
    }
}

/**
 * @brief Desenha um inimigo (BATBRAIN).
 */
void desenharInimigoBatbrain(InimigoBatbrain *inimigo)
{
    if (!inimigo->ativo)
        return;

    if (inimigo->estado == ESTADO_INIMIGO_BATBRAIN_MORRENDO)
    {
        desenharQuadroAnimacaoInimigoBatbrainMorrendo(
            inimigo,
            getQuadroAtualAnimacao(&inimigo->animacaoMorrendo),
            2.0f,
            WHITE);
    }
    else
    {
        QuadroAnimacao *qa =
            getQuadroAnimacaoAtualInimigoBatbrain(inimigo);

        desenharQuadroAnimacaoInimigoBatbrain(
            inimigo,
            qa,
            WHITE);
    }

    if (MOSTRAR_RETANGULOS)
    {
        DrawRectangleRec(inimigo->ret, Fade(inimigo->cor, 0.5f));
        DrawRectangleLines(
            inimigo->ret.x,
            inimigo->ret.y,
            inimigo->ret.width,
            inimigo->ret.height,
            BLACK);
    }
}

/**
 * @brief Obtém o quadro de animação atual de um inimigo (BATBRAIN).
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoBatbrain(InimigoBatbrain *inimigo)
{
    return getQuadroAtualAnimacao(getAnimacaoAtualInimigoBatbrain(inimigo));
}

static void desenharQuadroAnimacaoInimigoBatbrain(InimigoBatbrain *inimigo, QuadroAnimacao *qa, Color tonalidade)
{

    if (qa != NULL)
    {
        DrawTexturePro(
            rm.texturaBadniks,
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

        if (MOSTRAR_RETANGULOS)
        {
            float xDesenho = inimigo->olhandoParaDireita
                                 ? inimigo->ret.x + inimigo->ret.width - qa->retColisao.x - qa->retColisao.width
                                 : inimigo->ret.x + qa->retColisao.x;
            float yDesenho = inimigo->ret.y + qa->retColisao.y;
            DrawRectangle(xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade(GREEN, 0.5f));
        }
    }
}

static void desenharQuadroAnimacaoInimigoBatbrainMorrendo(InimigoBatbrain *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade)
{

    if (qa != NULL)
    {

        DrawTexturePro(
            rm.texturaBadniks,
            qa->fonte,
            (Rectangle){
                inimigo->ret.x,
                inimigo->ret.y,
                qa->fonte.width * escala,
                qa->fonte.height * escala},
            (Vector2){0},
            0.0f,
            tonalidade);
    }
}

static Animacao *getAnimacaoAtualInimigoBatbrain(InimigoBatbrain *inimigo)
{
    return inimigo->animacoes[inimigo->estado];
}