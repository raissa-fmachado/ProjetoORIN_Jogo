/**
 * @file GameWorld.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do GameWorld.
 *
 * @copyright Copyright (c) 2026
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "GameWorld.h"
#include "Jogador.h"
#include "Macros.h"
#include "Mapa.h"
#include "Obstaculo.h"
#include "Tipos.h"
#include "ResourceManager.h"
#include "Hud.h"

#include "raylib/raylib.h"

static void desenharFundo(GameWorld *gw);
static void atualizarCamera(GameWorld *gw);
static void inicializar(GameWorld *gw);
static void reiniciar(GameWorld *gw);
static void carregarFase(GameWorld *gw, int fase);
static void destruirFaseAtual(GameWorld *gw);
static bool jogadorChegouAoFim(GameWorld *gw);

/* ─── Duração do fade em segundos ─────────────────────────────────────────── */
#define FADE_DURACAO 1.2f

/* ─── Cor de fundo de cada fase ───────────────────────────────────────────── */
static const Color COR_FUNDO_FASE1 = {36,  0, 180, 255};   /* azul Green Hill  */
static const Color COR_FUNDO_FASE2 = {80, 20,  20, 255};   /* vinho Marble Zone */

/**
 * @brief Cria uma instância alocada dinamicamente da struct GameWorld.
 */
GameWorld *createGameWorld(void)
{
    GameWorld *gw = (GameWorld *)malloc(sizeof(GameWorld));
    gw->mapa    = NULL;
    gw->jogador = NULL;
    inicializar(gw);
    return gw;
}

/**
 * @brief Destrói um objeto GameWorld e suas dependências.
 */
void destroyGameWorld(GameWorld *gw)
{
    if (gw != NULL)
    {
        destruirMapa(gw->mapa);
        destruirJogador(gw->jogador);
        free(gw);
    }
}

/**
 * @brief Lê a entrada do usuário e atualiza o estado do jogo.
 */
void updateGameWorld(GameWorld *gw, float delta)
{
    /* ── música ─────────────────────────────────────────────────────────── */
    Music *musicaAtual = (gw->faseAtual == 1) ? &rm.musicaFase01 : &rm.musicaFase02;

    if (!IsMusicStreamPlaying(*musicaAtual))
        PlayMusicStream(*musicaAtual);
    else
        UpdateMusicStream(*musicaAtual);

    /* ── reinício manual ─────────────────────────────────────────────────── */
    if (IsKeyPressed(KEY_R))
    {
        reiniciar(gw);
        return;
    }

    /* ── fade de saída: espera terminar antes de trocar fase ─────────────── */
    if (gw->fadeSaida)
    {
        gw->fadeContador += delta;
        gw->fadeAlpha = gw->fadeContador / gw->fadeDuracao;
        if (gw->fadeAlpha >= 1.0f)
        {
            gw->fadeAlpha  = 1.0f;
            gw->fadeSaida  = false;
            gw->trocandoFase = true;
        }
        return; /* pausa o jogo durante o fade-out */
    }

    /* ── troca de fase efetiva ───────────────────────────────────────────── */
    if (gw->trocandoFase)
    {
        gw->trocandoFase = false;

        /* para a música atual */
        StopMusicStream(*musicaAtual);

        int proximaFase = (gw->faseAtual == 1) ? 2 : 1;
        carregarFase(gw, proximaFase);

        /* inicia o fade-in */
        gw->fadeEntrada  = true;
        gw->fadeContador = 0.0f;
        gw->fadeAlpha    = 1.0f;
        return;
    }

    /* ── fade de entrada ─────────────────────────────────────────────────── */
    if (gw->fadeEntrada)
    {
        gw->fadeContador += delta;
        gw->fadeAlpha = 1.0f - (gw->fadeContador / gw->fadeDuracao);
        if (gw->fadeAlpha <= 0.0f)
        {
            gw->fadeAlpha   = 0.0f;
            gw->fadeEntrada = false;
        }
        /* o jogo já roda normalmente durante o fade-in */
    }

    /* ── lógica normal do jogo ───────────────────────────────────────────── */
    atualizarMapa(gw->mapa, gw, delta);
    entradaJogador(gw->jogador, delta);
    atualizarJogador(gw->jogador, gw, delta);
    atualizarCamera(gw);

    if (gw->tempoJogo > 0.0f)
    {
        gw->tempoJogo -= delta;
        if (gw->tempoJogo < 0.0f)
            gw->tempoJogo = 0.0f;
    }

    /* ── detecta chegada ao fim da fase ─────────────────────────────────── */
    if (!gw->faseCompleta && !gw->fadeSaida && jogadorChegouAoFim(gw))
    {
        gw->faseCompleta = true;

        /* só transiciona se ainda há uma próxima fase */
        if (gw->faseAtual < 2)
        {
            gw->fadeSaida    = true;
            gw->fadeContador = 0.0f;
            gw->fadeAlpha    = 0.0f;
        }
    }
}

/**
 * @brief Desenha o estado do jogo.
 */
void drawGameWorld(GameWorld *gw)
{
    Color corFundo = (gw->faseAtual == 1) ? COR_FUNDO_FASE1 : COR_FUNDO_FASE2;

    BeginDrawing();
    ClearBackground(corFundo);

    BeginMode2D(gw->camera);
    desenharFundo(gw);
    desenharMapa(gw->mapa);
    desenharJogador(gw->jogador);
    EndMode2D();

    desenharHud(gw);

    /* ── overlay de fade ────────────────────────────────────────────────── */
    if (gw->fadeAlpha > 0.0f)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                      Fade(BLACK, gw->fadeAlpha));
    }

    EndDrawing();
}

/* ─────────────────────────────────────────────────────────────────────────── */
/*  Funções estáticas                                                          */
/* ─────────────────────────────────────────────────────────────────────────── */

static void desenharFundo(GameWorld *gw)
{
    Texture2D fundo = (gw->faseAtual == 1) ? rm.texturaFundo : rm.texturaFundo2;

    int larguraFundo = fundo.width;
    int larguraMapa  = calcularLarguraMapa(gw->mapa);
    int alturaMapa   = calcularAlturaMapa(gw->mapa);
    int repeticoes   = larguraMapa / larguraFundo;

    int deslocamentoParallax =
        (int)((gw->camera.target.x / (float)larguraMapa) * 200);

    for (int i = 0; i <= repeticoes; i++)
    {
        DrawTexture(fundo,
                    larguraFundo * i - deslocamentoParallax,
                    alturaMapa - fundo.height,
                    WHITE);
    }
}

static void atualizarCamera(GameWorld *gw)
{
    Jogador  *j = gw->jogador;
    Camera2D *c = &gw->camera;

    c->offset.x = GetScreenWidth()  / 2;
    c->offset.y = GetScreenHeight() / 2;

    c->target.x = roundf(j->ret.x + j->ret.width  / 2.0f);
    c->target.y = roundf(j->ret.y + j->ret.height / 2.0f);

    int minX = GetScreenWidth()  / 2;
    int maxX = calcularLarguraMapa(gw->mapa) - GetScreenWidth()  / 2;
    int maxY = calcularAlturaMapa(gw->mapa)  - GetScreenHeight() / 2;

    if      (c->target.x < minX) c->target.x = minX;
    else if (c->target.x > maxX) c->target.x = maxX;

    if (c->target.y > maxY) c->target.y = maxY;
}

/**
 * @brief Carrega os dados de uma fase específica sem reiniciar pontuação.
 */
static void carregarFase(GameWorld *gw, int fase)
{
    destruirFaseAtual(gw);

    gw->faseAtual    = fase;
    gw->faseCompleta = false;
    gw->tempoJogo    = 599.0f;

    const char *caminhoMapa = (fase == 1)
        ? "resources/mapas/mapa01.txt"
        : "resources/mapas/mapa02.txt";

    gw->mapa = carregarMapa(caminhoMapa);
    gw->jogador = criarJogador(
        GetScreenWidth() / 2 + 144,
        calcularAlturaMapa(gw->mapa) - 196,
        96, 96);

    gw->camera = (Camera2D){
        .offset   = {0},
        .target   = {0},
        .rotation = 0.0f,
        .zoom     = 1.0f
    };
}

static void destruirFaseAtual(GameWorld *gw)
{
    if (gw->mapa)    { destruirMapa(gw->mapa);       gw->mapa    = NULL; }
    if (gw->jogador) { destruirJogador(gw->jogador); gw->jogador = NULL; }
}

static void inicializar(GameWorld *gw)
{
    gw->gravidade = 900;
    gw->pontuacao = 0;

    /* fade */
    gw->fadeDuracao  = FADE_DURACAO;
    gw->fadeContador = 0.0f;
    gw->fadeAlpha    = 1.0f;   /* começa preto e faz fade-in */
    gw->fadeEntrada  = true;
    gw->fadeSaida    = false;
    gw->trocandoFase = false;
    gw->faseCompleta = false;

    carregarFase(gw, 1);
}

static void reiniciar(GameWorld *gw)
{
    Music *musicaAtual = (gw->faseAtual == 1) ? &rm.musicaFase01 : &rm.musicaFase02;
    if (IsMusicStreamPlaying(*musicaAtual))
        StopMusicStream(*musicaAtual);

    gw->pontuacao = 0;

    gw->fadeAlpha    = 1.0f;
    gw->fadeEntrada  = true;
    gw->fadeSaida    = false;
    gw->trocandoFase = false;
    gw->fadeContador = 0.0f;

    carregarFase(gw, 1);
}

/**
 * @brief Retorna true quando o jogador passa da borda direita do mapa.
 *        Usa a mesma lógica de câmera: margem de 1 tile (48px) além do fim.
 */
static bool jogadorChegouAoFim(GameWorld *gw)
{
    int fimMapa = calcularLarguraMapa(gw->mapa);
    return (gw->jogador->ret.x + gw->jogador->ret.width) >= (fimMapa - 48);
}