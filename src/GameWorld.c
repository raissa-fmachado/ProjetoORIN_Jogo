/**
 * @file GameWorld.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do GameWorld com transição entre fases,
 *        card de fase estilo Sonic clássico e tela de vitória.
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
#include "InimigoMotobug.h"
#include "InimigoSpikes.h"
#include "InimigoBallHog.h"
#include "InimigoBatbrain.h"
#include "InimigoEggmobile.h"
#include "TelaInicial.h"

#include "raylib/raylib.h"

/* ─────────────────────────────────────────────────────────────────────────── */
/*  Constantes de configuração                                                 */
/* ─────────────────────────────────────────────────────────────────────────── */

#define FADE_DURACAO 1.2f       /* segundos de cada fade              */
#define CARD_DURACAO 3.0f       /* segundos do card de fase           */
#define CARD_SLIDE_VEL 600.0f   /* px/s da animação de slide          */
#define BONUS_ANIM_DURACAO 3.0f /* segundos que a contagem animada leva */
#define BONUS_TEMPO_POR_SEG 10  /* pontos por segundo restante          */
#define BONUS_ANEL 50           /* pontos por anel coletado             */

/* ─────────────────────────────────────────────────────────────────────────── */
/*  Cores de fundo                                                             */
/* ─────────────────────────────────────────────────────────────────────────── */
static const Color COR_FUNDO_FASE1 = {36, 0, 180, 255};
static const Color COR_FUNDO_FASE2 = {80, 20, 20, 255};

/* ─────────────────────────────────────────────────────────────────────────── */
/*  Protótipos internos                                                        */
/* ─────────────────────────────────────────────────────────────────────────── */
static void desenharFundo(GameWorld *gw);
static void atualizarCamera(GameWorld *gw);
static void inicializar(GameWorld *gw);
static void reiniciar(GameWorld *gw);
static void carregarFase(GameWorld *gw, int fase);
static void destruirFaseAtual(GameWorld *gw);
static bool jogadorChegouAoFim(GameWorld *gw);

static void updateTelaCard(GameWorld *gw, float delta);
static void updateTelaVitoria(GameWorld *gw, float delta);
static void updateTelaContinue(GameWorld *gw, float delta);
static void updateTelaGameOver(GameWorld *gw, float delta);
static void drawTelaCard(GameWorld *gw);
static void drawTelaVitoria(GameWorld *gw);
static void drawTelaContinue(GameWorld *gw);
static void drawTelaGameOver(GameWorld *gw);
static void drawFadeOverlay(float alpha);
static void calcularBonusVitoria(GameWorld *gw);
static bool todosInimigosMortos(GameWorld *gw);
static bool jogadorSemVidas(GameWorld *gw);

static void updateTelaInicial(GameWorld *gw, float delta);
static void drawTelaInicial(GameWorld *gw);

/* ═══════════════════════════════════════════════════════════════════════════ */
/*  API pública                                                                */
/* ═══════════════════════════════════════════════════════════════════════════ */

GameWorld *createGameWorld(void)
{
    GameWorld *gw = (GameWorld *)malloc(sizeof(GameWorld));
    gw->mapa = NULL;
    gw->jogador = NULL;
    gw->telaInicial = criarTelaInicial();
    gw->estadoTela = TELA_INICIAL;
    inicializar(gw);
    return gw;
}

void destroyGameWorld(GameWorld *gw)
{
    if (gw != NULL)
    {
        destruirTelaInicial(gw->telaInicial);
        destruirFaseAtual(gw);
        free(gw);
    }
}

/* ─────────────────────────────────────────────────────────────────────────── */
/*  UPDATE                                                                     */
/* ─────────────────────────────────────────────────────────────────────────── */

void updateGameWorld(GameWorld *gw, float delta)
{
    /* ── reinício manual ──────────────────────────────────────────────── */
    if (IsKeyPressed(KEY_R))
    {
        reiniciar(gw);
        return;
    }

    /* ── despacha para a tela certa ──────────────────────────────────── */
    if (gw->estadoTela == TELA_INICIAL)
    {
        atualizarTelaInicial(gw->telaInicial, delta);

        /* Qualquer hora que apertar ENTER, entra */
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        {
            gw->estadoTela = TELA_CARD_FASE;
            gw->cardContador = 0.0f;
            gw->cardSlide = -(float)GetScreenHeight();
            gw->fadeEntrada = true;
            gw->fadeContador = 0.0f;
            gw->fadeAlpha = 1.0f;
        }

        return;
    }

    if (gw->estadoTela == TELA_CARD_FASE)
    {
        updateTelaCard(gw, delta);
        return;
    }

    if (gw->estadoTela == TELA_VITORIA)
    {
        updateTelaVitoria(gw, delta);
        return;
    }

    if (gw->estadoTela == TELA_CONTINUE)
    {
        updateTelaContinue(gw, delta);
        return;
    }

    if (gw->estadoTela == TELA_GAME_OVER)
    {
        updateTelaGameOver(gw, delta);
        return;
    }

    /* ═══ TELA_JOGANDO ═════════════════════════════════════════════════ */

    /* ── música ────────────────────────────────────────────────────────── */
    Music *musicaAtual = (gw->faseAtual == 1) ? &rm.musicaFase01 : &rm.musicaFase02;

    if (!IsMusicStreamPlaying(*musicaAtual))
        PlayMusicStream(*musicaAtual);
    else
        UpdateMusicStream(*musicaAtual);

    /* ── fade de saída: espera terminar antes de trocar fase ────────── */
    if (gw->fadeSaida)
    {
        gw->fadeContador += delta;
        gw->fadeAlpha = gw->fadeContador / gw->fadeDuracao;
        if (gw->fadeAlpha >= 1.0f)
        {
            gw->fadeAlpha = 1.0f;
            gw->fadeSaida = false;

            /* Verifica se é Game Over e ativa Continue */
            if (jogadorSemVidas(gw))
            {
                gw->estadoTela = TELA_CONTINUE;
                gw->continueContador = 0.0f;
                gw->continueOpcao = 0;
                return;
            }

            /* Game over tem prioridade */
            if (gw->estadoTela == TELA_GAME_OVER)
            {
                StopMusicStream((gw->faseAtual == 1) ? rm.musicaFase01 : rm.musicaFase02);
                gw->gameOverContador = 0.0f;
                return;
            }

            /* Vitória na fase 2 */
            if (gw->faseAtual == 2 && gw->faseCompleta)
            {
                StopMusicStream(rm.musicaFase02);
                gw->estadoTela = TELA_VITORIA;
                return;
            }

            gw->trocandoFase = true;
        }
        return;
    }

    /* ── troca de fase efetiva ──────────────────────────────────────── */
    if (gw->trocandoFase)
    {
        gw->trocandoFase = false;
        StopMusicStream(*musicaAtual);

        int proximaFase = (gw->faseAtual == 1) ? 2 : 1;
        carregarFase(gw, proximaFase);

        /* mostra card da nova fase */
        gw->estadoTela = TELA_CARD_FASE;
        gw->cardContador = 0.0f;
        gw->cardSlide = -(float)GetScreenHeight(); /* entra vindo de cima */

        /* fade-in começa junto com o card */
        gw->fadeEntrada = true;
        gw->fadeContador = 0.0f;
        gw->fadeAlpha = 1.0f;
        return;
    }

    /* ── fade de entrada ────────────────────────────────────────────── */
    if (gw->fadeEntrada)
    {
        gw->fadeContador += delta;
        gw->fadeAlpha = 1.0f - (gw->fadeContador / gw->fadeDuracao);
        if (gw->fadeAlpha <= 0.0f)
        {
            gw->fadeAlpha = 0.0f;
            gw->fadeEntrada = false;
        }
    }

    /* ── lógica normal do jogo (congela tudo se fase já completa) ───── */
    if (!gw->faseCompleta)
    {
        atualizarMapa(gw->mapa, gw, delta);
        entradaJogador(gw->jogador, delta);
        atualizarJogador(gw->jogador, gw, delta);
        atualizarCamera(gw);

        gw->tempoJogo += delta;
        if (gw->tempoJogo > 599.0f)
            gw->tempoJogo = 599.0f;
    }

    /* ── detecta chegada ao fim da fase ─────────────────────────────── */
    if (!gw->faseCompleta && !gw->fadeSaida && jogadorChegouAoFim(gw))
    {
        gw->faseCompleta = true;

        if (gw->faseAtual == 2)
        {
            /* Fase final: vai para tela de vitória */
            calcularBonusVitoria(gw);
            gw->fadeSaida = true;
            gw->fadeContador = 0.0f;
            gw->fadeAlpha = 0.0f;
        }
        else
        {
            /* Vai para próxima fase */
            gw->fadeSaida = true;
            gw->fadeContador = 0.0f;
            gw->fadeAlpha = 0.0f;
        }
    }

    /* ── vitória por matar todos os inimigos ────────────────────────── */
    if (!gw->faseCompleta && !gw->fadeSaida && todosInimigosMortos(gw))
    {
        gw->faseCompleta = true;
        calcularBonusVitoria(gw);
        gw->fadeSaida = true;
        gw->fadeContador = 0.0f;
        gw->fadeAlpha = 0.0f;
    }

    /* ── game over: jogador sem vidas ───────────────────────────────── */
    if (!gw->fadeSaida && jogadorSemVidas(gw))
    {
        gw->fadeSaida = true;
        gw->fadeContador = 0.0f;
        gw->fadeAlpha = 0.0f;
        /* Inicializa contadores de continue */
        gw->continueContador = 0.0f;
        gw->continueOpcao = 0;
    }
}

/* ─────────────────────────────────────────────────────────────────────────── */
/*  DRAW                                                                       */
/* ─────────────────────────────────────────────────────────────────────────── */

void drawGameWorld(GameWorld *gw)
{
    if (gw->estadoTela == TELA_INICIAL)
    {
        BeginDrawing();
        ClearBackground(BLACK);

        desenharTelaInicial(gw->telaInicial);

        EndDrawing();
        return;
    }

    if (gw->estadoTela == TELA_CARD_FASE)
    {
        drawTelaCard(gw);
        return;
    }

    if (gw->estadoTela == TELA_VITORIA)
    {
        drawTelaVitoria(gw);
        return;
    }

    if (gw->estadoTela == TELA_CONTINUE)
    {
        drawTelaContinue(gw);
        return;
    }

    if (gw->estadoTela == TELA_GAME_OVER)
    {
        drawTelaGameOver(gw);
        return;
    }

    /* ═══ TELA_JOGANDO ═════════════════════════════════════════════════ */
    Color corFundo = (gw->faseAtual == 1) ? COR_FUNDO_FASE1 : COR_FUNDO_FASE2;

    BeginDrawing();
    ClearBackground(corFundo);

    BeginMode2D(gw->camera);
    desenharFundo(gw);
    desenharMapa(gw->mapa);
    desenharJogador(gw->jogador);
    EndMode2D();

    desenharHud(gw);

    drawFadeOverlay(gw->fadeAlpha);

    EndDrawing();
}

/* ═══════════════════════════════════════════════════════════════════════════ */
/*  Tela: Card de fase estilo Sonic                                            */
/* ═══════════════════════════════════════════════════════════════════════════ */

static void updateTelaCard(GameWorld *gw, float delta)
{
    gw->cardContador += delta;

    /* Slide para baixo (entra na tela) */
    float metade = CARD_DURACAO * 0.25f;
    if (gw->cardContador < metade)
    {
        float t = gw->cardContador / metade;
        /* ease-out: desacelera ao parar */
        float ease = 1.0f - (1.0f - t) * (1.0f - t);
        gw->cardSlide = -(float)GetScreenHeight() * (1.0f - ease);
    }
    else
    {
        gw->cardSlide = 0.0f;
    }

    /* Fade de entrada do jogo começa junto */
    if (gw->fadeEntrada)
    {
        gw->fadeContador += delta;
        gw->fadeAlpha = 1.0f - (gw->fadeContador / gw->fadeDuracao);
        if (gw->fadeAlpha <= 0.0f)
        {
            gw->fadeAlpha = 0.0f;
            gw->fadeEntrada = false;
        }
    }

    /* Terminou o card */
    if (gw->cardContador >= CARD_DURACAO)
    {
        gw->estadoTela = TELA_JOGANDO;
        gw->cardContador = 0.0f;
    }
}

static void drawTelaCard(GameWorld *gw)
{
    Color corFundo = (gw->faseAtual == 1) ? COR_FUNDO_FASE1 : COR_FUNDO_FASE2;

    /* Desenha o jogo atrás (mas sem atualizar) */
    BeginDrawing();
    ClearBackground(corFundo);

    BeginMode2D(gw->camera);
    desenharFundo(gw);
    desenharMapa(gw->mapa);
    desenharJogador(gw->jogador);
    EndMode2D();

    desenharHud(gw);

    /* Fade overlay */
    drawFadeOverlay(gw->fadeAlpha);

    /* ── Card central ────────────────────────────────────────────────── */
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    int cardW = sw * 2 / 3;
    int cardH = 80;
    int cardX = (sw - cardW) / 2;
    int cardY = (sh - cardH) / 2 + (int)gw->cardSlide;

    /* fundo do card */
    DrawRectangle(cardX - 4, cardY - 4, cardW + 8, cardH + 8, BLACK);
    DrawRectangle(cardX, cardY, cardW, cardH, (Color){30, 30, 80, 240});

    /* borda azul brilhante */
    DrawRectangleLinesEx((Rectangle){(float)cardX, (float)cardY, (float)cardW, (float)cardH}, 3, (Color){100, 160, 255, 255});

    /* Nome da zona */
    const char *nomeZona = (gw->faseAtual == 1) ? "GREEN HILL ZONE" : "MARBLE ZONE";
    int fontSize = 28;
    int textW = MeasureText(nomeZona, fontSize);
    DrawText(nomeZona, cardX + (cardW - textW) / 2, cardY + 12, fontSize, (Color){255, 230, 80, 255});

    /* Número do act */
    const char *nomeAct = "  ACT 1  ";
    int actW = MeasureText(nomeAct, 20);
    DrawText(nomeAct, cardX + (cardW - actW) / 2, cardY + 48, 20, WHITE);

    /* linha separadora */
    DrawRectangle(cardX + 20, cardY + 43, cardW - 40, 2, (Color){100, 160, 255, 180});

    EndDrawing();
}

/* ═══════════════════════════════════════════════════════════════════════════ */
/*  Tela: Vitória estilo Sonic (ACT CLEAR)                                    */
/* ═══════════════════════════════════════════════════════════════════════════ */

static void calcularBonusVitoria(GameWorld *gw)
{
    float tempoRestante = 599.0f - gw->tempoJogo;
    if (tempoRestante < 0.0f)
        tempoRestante = 0.0f;

    gw->bonusTempo = (int)tempoRestante * BONUS_TEMPO_POR_SEG;
    gw->bonusAnel = gw->jogador->quantidadeAneis * BONUS_ANEL;
    gw->pontuacaoFinal = gw->pontuacao + gw->bonusTempo + gw->bonusAnel;

    gw->bonusTempoExibido = 0;
    gw->bonusAnelExibido = 0;
    /* sempre conta (mesmo se zero, t chegará a 1.0 e bonusContando = false) */
    gw->bonusContando = true;
    gw->vitoriaAnimTimer = 0.0f;
    gw->vitoriaContador = 0.0f;
}

static void updateTelaVitoria(GameWorld *gw, float delta)
{
    gw->vitoriaContador += delta;
    gw->vitoriaAnimTimer += delta;

    if (gw->bonusContando)
    {
        /* progresso linear de 0.0 → 1.0 ao longo de BONUS_ANIM_DURACAO segundos */
        float t = gw->vitoriaAnimTimer / BONUS_ANIM_DURACAO;
        if (t > 1.0f)
            t = 1.0f;

        gw->bonusTempoExibido = (int)(t * (float)gw->bonusTempo);
        gw->bonusAnelExibido = (int)(t * (float)gw->bonusAnel);

        if (t >= 1.0f)
        {
            gw->bonusTempoExibido = gw->bonusTempo;
            gw->bonusAnelExibido = gw->bonusAnel;
            gw->bonusContando = false;
        }
    }

    /* reinicia só quando o jogador pressionar ENTER, após a contagem terminar */
    if (!gw->bonusContando && IsKeyPressed(KEY_ENTER))
        reiniciar(gw);
}

static void drawTelaVitoria(GameWorld *gw)
{
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    BeginDrawing();
    ClearBackground((Color){10, 10, 40, 255});

    /* ── Estrelas de fundo ──────────────────────────────────────────── */
    /* Pontinhos estáticos simulando estrelas */
    for (int i = 0; i < 60; i++)
    {
        int sx = (i * 137 + 11) % sw;
        int sy = (i * 89 + 7) % sh;
        DrawPixel(sx, sy, (Color){200 + (i % 55), 200 + (i % 55), 255, 255});
    }

    /* ── Título "YOU WIN!" ──────────────────────────────────────────── */
    const char *titulo = "YOU WIN!";
    int tSize = 52;
    int tW = MeasureText(titulo, tSize);

    /* Sombra */
    DrawText(titulo, (sw - tW) / 2 + 3, 43, tSize, (Color){30, 30, 100, 200});
    /* Texto animado pulsando */
    float pulso = 0.85f + 0.15f * sinf(gw->vitoriaContador * 4.0f);
    Color corTitulo = {
        (unsigned char)(255 * pulso),
        (unsigned char)(220 * pulso),
        (unsigned char)(50 * pulso),
        255};
    DrawText(titulo, (sw - tW) / 2, 40, tSize, corTitulo);

    /* ── Subtítulo ──────────────────────────────────────────────────── */
    const char *sub = "MARBLE ZONE  CLEARED!";
    int subW = MeasureText(sub, 20);
    DrawText(sub, (sw - subW) / 2, 105, 20, (Color){180, 220, 255, 255});

    /* ── Painel de pontuação ────────────────────────────────────────── */
    int panelW = 340;
    int panelH = 200;
    int panelX = (sw - panelW) / 2;
    int panelY = 140;

    DrawRectangle(panelX - 4, panelY - 4, panelW + 8, panelH + 8, BLACK);
    DrawRectangle(panelX, panelY, panelW, panelH, (Color){20, 20, 70, 230});
    DrawRectangleLinesEx((Rectangle){(float)panelX, (float)panelY, (float)panelW, (float)panelH}, 2, (Color){100, 160, 255, 200});

    int ly = panelY + 18;
    int lGap = 38;
    int xLabel = panelX + 18;
    int xVal = panelX + panelW - 18;
    int fS = 22;

    /* Pontuação base */
    DrawText("SCORE", xLabel, ly, fS, (Color){255, 255, 150, 255});
    /* pontuacaoFinal - bonusTempo - bonusAnel = pontuação base congelada */
    char buf[32];
    int scoreBase = gw->pontuacaoFinal - gw->bonusTempo - gw->bonusAnel;
    sprintf(buf, "%d", scoreBase);
    int bW = MeasureText(buf, fS);
    DrawText(buf, xVal - bW, ly, fS, WHITE);
    ly += lGap;

    /* Bônus de tempo */
    DrawText("TIME BONUS", xLabel, ly, fS, (Color){150, 255, 150, 255});
    sprintf(buf, "%d", gw->bonusTempoExibido);
    bW = MeasureText(buf, fS);
    DrawText(buf, xVal - bW, ly, fS,
             gw->bonusTempoExibido < gw->bonusTempo ? (Color){255, 200, 0, 255} : WHITE);
    ly += lGap;

    /* Bônus de anéis */
    DrawText("RING BONUS", xLabel, ly, fS, (Color){255, 200, 100, 255});
    sprintf(buf, "%d", gw->bonusAnelExibido);
    bW = MeasureText(buf, fS);
    DrawText(buf, xVal - bW, ly, fS,
             gw->bonusAnelExibido < gw->bonusAnel ? (Color){255, 200, 0, 255} : WHITE);
    ly += lGap;

    /* Divisória */
    DrawRectangle(panelX + 10, ly, panelW - 20, 2, (Color){100, 160, 255, 180});
    ly += 10;

    /* Total — usa os valores animados mas limitado ao final congelado */
    int totalExibido = (gw->pontuacaoFinal - gw->bonusTempo - gw->bonusAnel) + gw->bonusTempoExibido + gw->bonusAnelExibido;
    DrawText("TOTAL", xLabel, ly, fS + 2, (Color){255, 240, 60, 255});
    sprintf(buf, "%d", totalExibido);
    bW = MeasureText(buf, fS + 2);
    DrawText(buf, xVal - bW, ly, fS + 2, (Color){255, 240, 60, 255});

    /* ── Botão de reinício (só aparece após a contagem terminar) ───── */
    if (!gw->bonusContando)
    {
        int btnW = 260;
        int btnH = 36;
        int btnX = (sw - btnW) / 2;
        int btnY = panelY + panelH + 18;

        /* destaque pulsante na borda */
        float pulsoBtn = 0.5f + 0.5f * sinf(gw->vitoriaContador * 3.5f);
        Color corBorda = {
            (unsigned char)(80 + 175 * pulsoBtn),
            (unsigned char)(160 + 95 * pulsoBtn),
            255,
            255};

        DrawRectangle(btnX, btnY, btnW, btnH, (Color){20, 20, 80, 220});
        DrawRectangleLinesEx((Rectangle){(float)btnX, (float)btnY,
                                         (float)btnW, (float)btnH},
                             2, corBorda);

        const char *btnTxt = "[ ENTER ]  JOGAR NOVAMENTE";
        int tW2 = MeasureText(btnTxt, 16);
        DrawText(btnTxt, btnX + (btnW - tW2) / 2, btnY + 10, 16, corBorda);
    }
    else
    {
        /* enquanto conta: instrução para pular */
        const char *skip = "Aguarde...";
        int sW2 = MeasureText(skip, 14);
        DrawText(skip, (sw - sW2) / 2, panelY + panelH + 28, 14,
                 (Color){120, 120, 180, 180});
    }

    EndDrawing();
}

/* ═══════════════════════════════════════════════════════════════════════════ */
/*  Tela: Continue estilo Sonic                                                */
/* ═══════════════════════════════════════════════════════════════════════════ */

static void updateTelaContinue(GameWorld *gw, float delta)
{
    gw->continueContador += delta;

    /* Navegar com UP/DOWN */
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        gw->continueOpcao = 0;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        gw->continueOpcao = 1;
    }

    /* ENTER para confirmar */
    if (IsKeyPressed(KEY_ENTER))
    {
        if (gw->continueOpcao == 0)
        {
            /* Continue: reinicia a fase com uma vida de graça */
            gw->jogador->quantidadeVidas = 1;
            gw->jogador->quantidadeAneis = 0;
            reiniciar(gw);
        }
        else
        {
            /* Game Over: reinicia o jogo do zero */
            gw->estadoTela = TELA_GAME_OVER;
            gw->gameOverContador = 0.0f;
            gw->fadeEntrada = false;
            gw->fadeSaida = false;
        }
    }

    /* Timeout: após 30 segundos, vai para Game Over automaticamente */
    if (gw->continueContador >= 30.0f)
    {
        gw->estadoTela = TELA_GAME_OVER;
        gw->gameOverContador = 0.0f;
    }
}

static void drawTelaContinue(GameWorld *gw)
{
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    BeginDrawing();
    ClearBackground(BLACK);

    /* Estrelas de fundo */
    for (int i = 0; i < 60; i++)
    {
        int sx = (i * 137 + 11) % sw;
        int sy = (i * 89 + 7) % sh;
        DrawPixel(sx, sy, (Color){100, 150, 255, 255});
    }

    /* ── Título CONTINUE ──────────────────────────────────────────── */
    const char *titulo = "CONTINUE";
    int tSize = 52;
    int tW = MeasureText(titulo, tSize);

    float pulso = 0.85f + 0.15f * sinf(gw->continueContador * 2.0f);
    Color corTitulo = {
        (unsigned char)(100 * pulso),
        (unsigned char)(200 * pulso),
        255,
        255};

    DrawText(titulo, (sw - tW) / 2, 50, tSize, corTitulo);

    /* ── Contador regressivo (30 a 0) ────────────────────────────── */
    int tempoRestante = (int)(30.0f - gw->continueContador);
    if (tempoRestante < 0)
        tempoRestante = 0;

    char bufTempo[16];
    sprintf(bufTempo, "%02d", tempoRestante);
    int tW2 = MeasureText(bufTempo, 120);

    /* Pulsação do número */
    float pulsoDig = 0.7f + 0.3f * sinf(gw->continueContador * 3.0f);
    Color corDigito = {
        (unsigned char)(255 * pulsoDig),
        (unsigned char)(220 * pulsoDig),
        (unsigned char)(100 * pulsoDig),
        255};

    DrawText(bufTempo, (sw - tW2) / 2, 140, 120, corDigito);

    /* ── Opções (Continue / Game Over) ──────────────────────────── */
    int optY = 310;
    int optGap = 70;

    /* Opção 1: Continue */
    Color corOpt1 = gw->continueOpcao == 0 ? (Color){255, 255, 100, 255} : (Color){150, 150, 150, 255};
    const char *opt1 = "> CONTINUE <";
    int opt1W = MeasureText(opt1, 32);
    DrawText(opt1, (sw - opt1W) / 2, optY, 32, corOpt1);

    /* Opção 2: Game Over */
    Color corOpt2 = gw->continueOpcao == 1 ? (Color){255, 255, 100, 255} : (Color){150, 150, 150, 255};
    const char *opt2 = "> GAME OVER <";
    int opt2W = MeasureText(opt2, 32);
    DrawText(opt2, (sw - opt2W) / 2, optY + optGap, 32, corOpt2);

    /* ── Instrução ──────────────────────────────────────────────── */
    const char *hint = "USE ARROWS TO SELECT - PRESS ENTER";
    int hW = MeasureText(hint, 14);
    DrawText(hint, (sw - hW) / 2, sh - 35, 14, (Color){200, 200, 220, 220});

    EndDrawing();
}

/* ─────────────────────────────────────────────────────────────────────────── */

/* ═══════════════════════════════════════════════════════════════════════════ */
/*  Tela: Game Over estilo Sonic                                               */
/* ═══════════════════════════════════════════════════════════════════════════ */

static void updateTelaGameOver(GameWorld *gw, float delta)
{
    gw->gameOverContador += delta;

    /* reinicia só ao pressionar ENTER */
    if (IsKeyPressed(KEY_ENTER))
        reiniciar(gw);
}

static void drawTelaGameOver(GameWorld *gw)
{
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    BeginDrawing();
    ClearBackground(BLACK);

    /* Estrelas de fundo */
    for (int i = 0; i < 60; i++)
    {
        int sx = (i * 137 + 11) % sw;
        int sy = (i * 89 + 7) % sh;
        DrawPixel(sx, sy, (Color){180, 180, 255, 255});
    }

    /* "GAME OVER" pulsando em vermelho */
    const char *titulo = "GAME OVER";
    int tSize = 56;
    int tW = MeasureText(titulo, tSize);
    float pulso = 0.80f + 0.20f * sinf(gw->gameOverContador * 3.5f);
    Color corTitulo = {
        (unsigned char)(255 * pulso),
        (unsigned char)(30 * pulso),
        (unsigned char)(30 * pulso),
        255};
    /* Sombra */
    DrawText(titulo, (sw - tW) / 2 + 4, sh / 2 - 60 + 4, tSize, (Color){80, 0, 0, 200});
    DrawText(titulo, (sw - tW) / 2, sh / 2 - 60, tSize, corTitulo);

    /* Pontuação */
    char buf[64];
    sprintf(buf, "SCORE: %d", gw->pontuacao);
    int sW = MeasureText(buf, 24);
    DrawText(buf, (sw - sW) / 2, sh / 2 + 20, 24, (Color){220, 220, 255, 255});

    /* Instrução piscando */
    float alpha = 0.5f + 0.5f * sinf(gw->gameOverContador * 3.5f);
    Color corHint = {200, 200, 255, (unsigned char)(alpha * 255)};
    const char *hint = "PRESS ENTER TO TRY AGAIN";
    int hW = MeasureText(hint, 16);
    DrawText(hint, (sw - hW) / 2, sh / 2 + 70, 16, corHint);

    /* Fade overlay (para a entrada suave) */
    drawFadeOverlay(gw->fadeAlpha);

    EndDrawing();
}

/* ─────────────────────────────────────────────────────────────────────────── */

static bool todosInimigosMortos(GameWorld *gw)
{
    if (gw->mapa->quantidadeInimigos == 0)
        return false; /* sem inimigos no mapa não conta como vitória */

    ElementoMapa *el = gw->mapa->inimigos;
    while (el != NULL)
    {
        Inimigo *inimigo = (Inimigo *)el->objeto;

        bool ativo = false;
        if (inimigo->tipo == TIPO_INIMIGO_MOTOBUG)
            ativo = ((InimigoMotobug *)inimigo->objeto)->ativo;
        else if (inimigo->tipo == TIPO_INIMIGO_SPIKES)
            ativo = ((InimigoSpikes *)inimigo->objeto)->ativo;
        else if (inimigo->tipo == TIPO_INIMIGO_BALLHOG)
            ativo = ((InimigoBallHog *)inimigo->objeto)->ativo;
        else if (inimigo->tipo == TIPO_INIMIGO_BATBRAIN)
            ativo = ((InimigoBatbrain *)inimigo->objeto)->ativo;
        else if (inimigo->tipo == TIPO_INIMIGO_EGGMOBILE)
            ativo = ((InimigoEggMobile *)inimigo->objeto)->ativo;

        if (ativo)
            return false;
        el = el->proximo;
    }
    return true;
}

static bool jogadorSemVidas(GameWorld *gw)
{
    return gw->jogador->quantidadeVidas <= 0;
}

static void drawFadeOverlay(float alpha)
{
    if (alpha > 0.0f)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                      Fade(BLACK, alpha));
    }
}

/* ═══════════════════════════════════════════════════════════════════════════ */
/*  Funções estáticas de jogo                                                  */
/* ═══════════════════════════════════════════════════════════════════════════ */

static void desenharFundo(GameWorld *gw)
{
    Texture2D fundo = (gw->faseAtual == 1) ? rm.texturaFundo : rm.texturaFundo2;

    int larguraFundo = fundo.width;
    int larguraMapa = calcularLarguraMapa(gw->mapa);
    int alturaMapa = calcularAlturaMapa(gw->mapa);
    int repeticoes = larguraMapa / larguraFundo;

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
    Jogador *j = gw->jogador;
    Camera2D *c = &gw->camera;

    c->offset.x = GetScreenWidth() / 2;
    c->offset.y = GetScreenHeight() / 2;

    c->target.x = roundf(j->ret.x + j->ret.width / 2.0f);
    c->target.y = roundf(j->ret.y + j->ret.height / 2.0f);

    int minX = GetScreenWidth() / 2;
    int maxX = calcularLarguraMapa(gw->mapa) - GetScreenWidth() / 2;
    int maxY = calcularAlturaMapa(gw->mapa) - GetScreenHeight() / 2;

    if (c->target.x < minX)
        c->target.x = minX;
    else if (c->target.x > maxX)
        c->target.x = maxX;

    if (c->target.y > maxY)
        c->target.y = maxY;
}

static void carregarFase(GameWorld *gw, int fase)
{
    destruirFaseAtual(gw);

    gw->faseAtual = fase;
    gw->faseCompleta = false;
    gw->tempoJogo = 0.0f;

    const char *caminhoMapa = (fase == 1)
                                  ? "resources/mapas/mapa01.txt"
                                  : "resources/mapas/mapa02.txt";

    gw->mapa = carregarMapaFase(caminhoMapa, fase);

    gw->jogador = criarJogador(
        GetScreenWidth() / 2 + 144,
        calcularAlturaMapa(gw->mapa) - 196,
        96, 96);

    gw->camera = (Camera2D){
        .offset = {0},
        .target = {0},
        .rotation = 0.0f,
        .zoom = 1.0f};
}

static void destruirFaseAtual(GameWorld *gw)
{
    if (gw->mapa)
    {
        destruirMapa(gw->mapa);
        gw->mapa = NULL;
    }
    if (gw->jogador)
    {
        destruirJogador(gw->jogador);
        gw->jogador = NULL;
    }
}

static void inicializar(GameWorld *gw)
{
    gw->gravidade = 900;
    gw->pontuacao = 0;
    gw->estadoTela = TELA_INICIAL;

    gw->telaInicialContador = 0.0f;
    gw->iniciarJogo = false;

    /* card */
    gw->cardContador = 0.0f;
    gw->cardDuracao = CARD_DURACAO;
    gw->cardSlide = 0.0f;

    /* vitória */
    gw->vitoriaContador = 0.0f;
    gw->bonusTempo = 0;
    gw->bonusAnel = 0;
    gw->pontuacaoFinal = 0;
    gw->vitoriaAnimTimer = 0.0f;
    gw->bonusTempoExibido = 0;
    gw->bonusAnelExibido = 0;
    gw->bonusContando = false;

    /* continue */
    gw->continueContador = 0.0f;
    gw->continueDuracao = 30.0f;
    gw->continueOpcao = 0;

    /* game over */
    gw->gameOverContador = 0.0f;

    /* fade */
    gw->fadeDuracao = FADE_DURACAO;
    gw->fadeContador = 0.0f;
    gw->fadeAlpha = 1.0f;
    gw->fadeEntrada = true;
    gw->fadeSaida = false;
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
    gw->estadoTela = TELA_INICIAL;

    gw->telaInicialContador = 0.0f;
    gw->iniciarJogo = false;

    gw->fadeAlpha = 1.0f;
    gw->fadeEntrada = true;
    gw->fadeSaida = false;
    gw->trocandoFase = false;
    gw->fadeContador = 0.0f;
    gw->faseCompleta = false;

    gw->bonusContando = false;
    gw->vitoriaContador = 0.0f;
    gw->bonusTempoExibido = 0;
    gw->bonusAnelExibido = 0;
    gw->gameOverContador = 0.0f;
    gw->continueContador = 0.0f;

    gw->estadoTela = TELA_INICIAL;
    gw->telaInicialContador = 0.0f;
    gw->iniciarJogo = false;

    carregarFase(gw, 1);
}

static bool jogadorChegouAoFim(GameWorld *gw)
{
    int fimMapa = calcularLarguraMapa(gw->mapa);
    return (gw->jogador->ret.x + gw->jogador->ret.width) >= (fimMapa - 48);
}