/**
 * @file InimigoBatbrain.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Inimigo (Batbrain).
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

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
    novoInimigo->vel = (Vector2){0};
    novoInimigo->cor = cor;

    novoInimigo->velAndando = 100;
    novoInimigo->velMaxQueda = 600;

    novoInimigo->estado = ESTADO_INIMIGO_BATBRAIN_ANDANDO;
    novoInimigo->ativo = true;
    novoInimigo->olhandoParaDireita = false;

    int quantidadeAnimacoes = 0;

    novoInimigo->animacaoAndando.quantidadeQuadros = 6;
    novoInimigo->animacaoAndando.quadroAtual = 0;
    novoInimigo->animacaoAndando.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoAndando.pararNoUltimoQuadro = false;
    novoInimigo->animacaoAndando.executarUmaVez = false;
    novoInimigo->animacaoAndando.finalizada = false;
    criarQuadrosAnimacao(&novoInimigo->animacaoAndando, novoInimigo->animacaoAndando.quantidadeQuadros);
    inicializarQuadrosAnimacao(
        novoInimigo->animacaoAndando.quadros,
        novoInimigo->animacaoAndando.quantidadeQuadros,
        250,        // duração padrão para todos os quadros
        1, 113,     // início
        32, 32,     // dimensões
        1,          // separação
        false,      // de trás para frente
        (Rectangle){// retângulo de colisão padrão para cada quadro
                    2, 2, 28, 28});

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

    novoInimigo->animacoes[ESTADO_INIMIGO_BATBRAIN_ANDANDO] = &novoInimigo->animacaoAndando;
    quantidadeAnimacoes++;
    novoInimigo->animacoes[ESTADO_INIMIGO_BATBRAIN_MORRENDO] = &novoInimigo->animacaoMorrendo;
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
        for (int i = 0; i < inimigo->quantidadeAnimacoes; i++)
        {
            destruirQuadrosAnimacao(inimigo->animacoes[i]);
        }
        free(inimigo);
    }
}

/**
 * @brief Atualiza um inimigo (BATBRAIN).
 */
void atualizarInimigoBatbrain(InimigoBatbrain *inimigo, GameWorld *gw, float delta)
{

    if (inimigo->ativo)
    {

        if (inimigo->estado == ESTADO_INIMIGO_BATBRAIN_ANDANDO)
        {

            Animacao *animacaoAtual = getAnimacaoAtualInimigoBatbrain(inimigo);
            atualizarAnimacao(animacaoAtual, delta);

            Inimigo ini = {
                .objeto = inimigo,
                .tipo = TIPO_INIMIGO_BATBRAIN};

            if (inimigo->olhandoParaDireita)
            {
                inimigo->vel.x = inimigo->velAndando;
            }
            else
            {
                inimigo->vel.x = -inimigo->velAndando;
            }

            // fase X
            inimigo->ret.x += inimigo->vel.x * delta;
            resolverColisaoInimigoObstaculosMapaX(&ini, gw->mapa);

            inimigo->vel.y += gw->gravidade * delta;
            if (inimigo->vel.y > inimigo->velMaxQueda)
            {
                inimigo->vel.y = inimigo->velMaxQueda;
            }

            // fase Y
            inimigo->ret.y += inimigo->vel.y * delta;
            resolverColisaoInimigoObstaculosMapaY(&ini, gw->mapa);
        }
        else if (inimigo->estado == ESTADO_INIMIGO_BATBRAIN_MORRENDO)
        {

            atualizarAnimacao(&inimigo->animacaoMorrendo, delta);

            if (inimigo->animacaoMorrendo.finalizada)
            {
                inimigo->ativo = false;
            }
        }
    }
}

/**
 * @brief Desenha um inimigo (BATBRAIN).
 */
void desenharInimigoBatbrain(InimigoBatbrain *inimigo)
{

    if (inimigo->ativo)
    {

        if (inimigo->estado == ESTADO_INIMIGO_BATBRAIN_ANDANDO)
        {
            QuadroAnimacao *qa = getQuadroAnimacaoAtualInimigoBatbrain(inimigo);
            desenharQuadroAnimacaoInimigoBatbrain(inimigo, qa, WHITE);
        }
        else if (inimigo->estado == ESTADO_INIMIGO_BATBRAIN_MORRENDO)
        {
            desenharQuadroAnimacaoInimigoBatbrainMorrendo(inimigo, getQuadroAtualAnimacao(&inimigo->animacaoMorrendo), 2.0f, WHITE);
        }

        if (MOSTRAR_RETANGULOS)
        {
            DrawRectangleRec(inimigo->ret, Fade(inimigo->cor, 0.5f));
            DrawRectangleLines(inimigo->ret.x, inimigo->ret.y, inimigo->ret.width, inimigo->ret.height, BLACK);
        }
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
                inimigo->olhandoParaDireita ? -qa->fonte.width : qa->fonte.width,
                qa->fonte.height},
            inimigo->ret,
            (Vector2){0},
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