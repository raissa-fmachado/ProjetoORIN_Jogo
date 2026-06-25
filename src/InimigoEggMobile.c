/**
 * @file InimigoEggMobile.c
 * @brief Implementação do Inimigo (Egg Mobile).
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

Vector2 obterGanchoEggMobile(InimigoEggMobile *inimigo);

static Animacao *getAnimacaoAtualInimigoEggMobile(
    InimigoEggMobile *inimigo);

const float GANCHO_X = 58;
const float GANCHO_Y = 128;

static const bool MOSTRAR_RETANGULOS = false;

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

    novoInimigo->raioBola = 195;
    novoInimigo->anguloBola = PI / 2;
    novoInimigo->velocidadeAngular = -1.3f;

    novoInimigo->ret = ret;

    novoInimigo->olhandoParaDireita = false;

    novoInimigo->vel = (Vector2){
        novoInimigo->olhandoParaDireita ? 100 : -100,
        0};

    novoInimigo->cor = cor;

    novoInimigo->vida = 1;

    novoInimigo->batalhaIniciada = false;

    novoInimigo->ativo = true;
    novoInimigo->invulneravel = false;
    novoInimigo->arenaAtivada = false;
    novoInimigo->limiteEsquerdoArena = 0;
    novoInimigo->limiteDireitoArena = 0;

    novoInimigo->contadorEstado = 0.0f;
    novoInimigo->contadorInvulnerabilidade = 0.0f;

    novoInimigo->estado =
        ESTADO_INIMIGO_EGGMOBILE_VOANDO;

    /* =========================
       ANIMAÇÃO VOANDO
       ========================= */

    novoInimigo->animacaoVoando.quantidadeQuadros = 2;
    novoInimigo->animacaoVoando.quadroAtual = 0;
    novoInimigo->animacaoVoando.contadorTempoQuadro = 0;
    novoInimigo->animacaoVoando.pararNoUltimoQuadro = false;
    novoInimigo->animacaoVoando.executarUmaVez = false;
    novoInimigo->animacaoVoando.finalizada = false;

    criarQuadrosAnimacao(&novoInimigo->animacaoVoando, novoInimigo->animacaoVoando.quantidadeQuadros);
    inicializarQuadrosAnimacao(
        novoInimigo->animacaoVoando.quadros,
        novoInimigo->animacaoVoando.quantidadeQuadros,
        250,    // duração padrão para todos os quadros
        79, 83,  // início
        77, 72, // dimensões
        1,      // separação
        false,  // de trás para frente
        (Rectangle){0, 10, 115, 85});

    /* =========================
       ANIMAÇÃO BOLA
       ========================= */

    novoInimigo->animacaoBola.quantidadeQuadros = 2;
    novoInimigo->animacaoBola.quadroAtual = 0;
    novoInimigo->animacaoBola.contadorTempoQuadro = 0;
    novoInimigo->animacaoBola.pararNoUltimoQuadro = false;
    novoInimigo->animacaoBola.executarUmaVez = false;
    novoInimigo->animacaoBola.finalizada = false;

    criarQuadrosAnimacao(&novoInimigo->animacaoBola, 2);

    novoInimigo->animacaoBola.quadros[0].fonte = (Rectangle){1, 173, 48, 48};
    novoInimigo->animacaoBola.quadros[0].duracao = 250;

    novoInimigo->animacaoBola.quadros[1].fonte = (Rectangle){50, 173, 48, 48};
    novoInimigo->animacaoBola.quadros[1].duracao = 250;

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
        (Rectangle){235, 83, 77, 72};

    novoInimigo->animacaoDano.quadros[0].duracao =
        250;

    novoInimigo->animacaoDano.quadros[0].retColisao =
        (Rectangle){0, 10, 115, 85};

    /* =========================
       ANIMAÇÃO DERROTADO
       ========================= */

    novoInimigo->animacaoDerrotado.quantidadeQuadros = 11;
    novoInimigo->animacaoDerrotado.quadroAtual = 0;
    novoInimigo->animacaoDerrotado.contadorTempoQuadro = 0;
    novoInimigo->animacaoDerrotado.pararNoUltimoQuadro = true;
    novoInimigo->animacaoDerrotado.executarUmaVez = true;
    novoInimigo->animacaoDerrotado.finalizada = false;

    criarQuadrosAnimacao(&novoInimigo->animacaoDerrotado, novoInimigo->animacaoDerrotado.quantidadeQuadros);
    inicializarQuadrosAnimacao(
        novoInimigo->animacaoDerrotado.quadros,
        novoInimigo->animacaoDerrotado.quantidadeQuadros,
        300,    // duração padrão para todos os quadros
        1, 222, // início
        77, 72, // dimensões
        1,      // separação
        false,  // de trás para frente
        (Rectangle){0, 10, 115, 85});

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
    if (!inimigo->batalhaIniciada)
    {
        inimigo->ret.y +=
            sinf(GetTime() * 2.0f) * 0.2f;

        Vector2 centro =
            obterGanchoEggMobile(inimigo);

        inimigo->posBola.x = centro.x;
        inimigo->posBola.y =
            centro.y + inimigo->raioBola;

        inimigo->retBola = (Rectangle){
            inimigo->posBola.x - 48,
            inimigo->posBola.y - 48,
            96,
            96};

        return;
    }

    if (!inimigo->ativo)
        return;

    atualizarAnimacao(
        getAnimacaoAtualInimigoEggMobile(
            inimigo),
        delta);

    atualizarAnimacao(&inimigo->animacaoBola, delta);

    switch (inimigo->estado)
    {

    case ESTADO_INIMIGO_EGGMOBILE_VOANDO:

        inimigo->ret.x +=
            inimigo->vel.x * delta;

        inimigo->contadorPendulo += delta;

        float amplitude = PI / 2.0f;

        float angulo = PI / 2 +
                       sinf(inimigo->contadorPendulo *
                            inimigo->velocidadeAngular) *
                           amplitude;

        inimigo->anguloBola = angulo;

        if (inimigo->arenaAtivada)
        {
            if (inimigo->ret.x <= inimigo->limiteEsquerdoArena)
            {
                inimigo->ret.x = inimigo->limiteEsquerdoArena;
                inimigo->vel.x = 100;
                inimigo->olhandoParaDireita = true;
            }

            if (inimigo->ret.x + inimigo->ret.width >=
                inimigo->limiteDireitoArena)
            {
                inimigo->ret.x =
                    inimigo->limiteDireitoArena -
                    inimigo->ret.width;

                inimigo->vel.x = -100;
                inimigo->olhandoParaDireita = false;
            }
        }
        else
        {
            if (inimigo->ret.x < 100)
            {
                inimigo->vel.x = 100;
                inimigo->olhandoParaDireita = true;
            }

            if (inimigo->ret.x > 900)
            {
                inimigo->vel.x = -100;
                inimigo->olhandoParaDireita = false;
            }
        }

        Vector2 centro = obterGanchoEggMobile(inimigo);

        inimigo->posBola.x =
            centro.x + cosf(angulo) * inimigo->raioBola;

        inimigo->posBola.y =
            centro.y + sinf(angulo) * inimigo->raioBola;

        inimigo->retBola = (Rectangle){
            inimigo->posBola.x - 48,
            inimigo->posBola.y - 48,
            96,
            96};

        inimigo->retParteInferior = (Rectangle){
            inimigo->ret.x + 10,
            inimigo->ret.y + inimigo->ret.height - 12,
            inimigo->ret.width - 20,
            12};

        break;

    case ESTADO_INIMIGO_EGGMOBILE_DANO:

        inimigo->contadorEstado += delta;

        if (inimigo->contadorEstado >= 0.5f)
        {
            inimigo->contadorEstado = 0;

            if (inimigo->olhandoParaDireita)
            {
                inimigo->vel.x = 120;
            }
            else
            {
                inimigo->vel.x = -120;
            }

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

        inimigo->contadorEstado += delta;

        atualizarAnimacao(
            getAnimacaoAtualInimigoEggMobile(inimigo),
            delta);

        if (inimigo->animacaoDerrotado.finalizada)
        {
            inimigo->ativo = false;
        }

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

Vector2 obterGanchoEggMobile(InimigoEggMobile *inimigo)
{
    Vector2 p;

    float ganchoX = GANCHO_X;

    if (inimigo->olhandoParaDireita)
    {
        ganchoX = inimigo->ret.width - GANCHO_X;
    }

    p.x = inimigo->ret.x + ganchoX;
    p.y = inimigo->ret.y + GANCHO_Y;

    return p;
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

    desenharQuadroAnimacaoInimigoEggMobile(
        inimigo,
        getQuadroAnimacaoAtualInimigoEggMobile(
            inimigo),
        cor);

    if (inimigo->estado != ESTADO_INIMIGO_EGGMOBILE_DERROTADO)
    {
        Vector2 centro = obterGanchoEggMobile(inimigo);

#define NUM_ELOS 6

        Vector2 diferenca = {
            inimigo->posBola.x - centro.x,
            inimigo->posBola.y - centro.y};

        float comprimento =
            sqrtf(diferenca.x * diferenca.x +
                  diferenca.y * diferenca.y);

        Vector2 direcao = {
            diferenca.x / comprimento,
            diferenca.y / comprimento};

        float raioVisualBola = 48.0f;

        Vector2 pontoFimCorrente = {
            inimigo->posBola.x - direcao.x * raioVisualBola,
            inimigo->posBola.y - direcao.y * raioVisualBola};

        float anguloGraus =
            atan2f(diferenca.y, diferenca.x) * RAD2DEG;

        for (int i = 1; i <= NUM_ELOS; i++)
        {
            float t = (float)i / (NUM_ELOS + 0.5);

            Vector2 elo = {
                centro.x + (pontoFimCorrente.x - centro.x) * t,
                centro.y + (pontoFimCorrente.y - centro.y) * t};

            DrawTexturePro(
                rm.texturaChefao,
                (Rectangle){1, 156, 16, 16},
                (Rectangle){elo.x, elo.y, 32, 32},
                (Vector2){16, 16},
                anguloGraus + 90,
                WHITE);
        }

        QuadroAnimacao *qaBola =
            getQuadroAtualAnimacao(&inimigo->animacaoBola);

        DrawTexturePro(
            rm.texturaChefao,
            qaBola->fonte,
            (Rectangle){
                inimigo->posBola.x,
                inimigo->posBola.y,
                96,
                96},
            (Vector2){48, 48},
            0,
            WHITE);
    }

    if (inimigo->invulneravel)
    {
        if (((int)(GetTime() * 10)) % 2 == 0)
        {
            cor = Fade(WHITE, 0.3f);
        }
    }

    if (MOSTRAR_RETANGULOS)
    {
        DrawRectangleRec(
            inimigo->ret,
            Fade(inimigo->cor, 0.5f));

        DrawCircleLines(
            inimigo->posBola.x,
            inimigo->posBola.y,
            40,
            RED);

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

        if (MOSTRAR_RETANGULOS)
        {
            float xDesenho = inimigo->olhandoParaDireita
                                 ? inimigo->ret.x + inimigo->ret.width - qa->retColisao.x - qa->retColisao.width
                                 : inimigo->ret.x + qa->retColisao.x;
            float yDesenho = inimigo->ret.y + qa->retColisao.y;
            DrawRectangle(xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade(RED, 0.5f));
        }
    }
}

static Animacao *getAnimacaoAtualInimigoEggMobile(
    InimigoEggMobile *inimigo)
{
    return inimigo->animacoes[inimigo->estado];
}