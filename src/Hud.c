/**
 * @file Hud.c
 * @brief Implementação do HUD estilo Sonic clássico usando sprite sheet.
 */

#include <stdio.h>
#include <string.h>

#include "Hud.h"
#include "Tipos.h"
#include "raylib/raylib.h"
#include "ResourceManager.h"

/* Coordenadas onde começam os números grandes na sprite sheet */
#define HUD_NUM_G_X 72
#define HUD_NUM_G_Y 432

/* Coordenadas onde começam os números pequenos na sprite sheet */
#define HUD_NUM_P_X 0
#define HUD_NUM_P_Y 0

/* Dimensões dos números grandes */
#define HUD_NUM_G_W 8
#define HUD_NUM_G_H 16

/* Dimensões dos números pequenos */
#define HUD_NUM_P_W 8
#define HUD_NUM_P_H 8

/* Espaçamento entre os números grandes */
#define HUD_NUM_G_SPACING 8

/* Espaçamento entre os números pequenos */
#define HUD_NUM_P_SPACING 1

/* Escala do HUD */
#define HUD_SCALE 2

/* Posição do ':' na sprite sheet */
#define HUD_COLON_X 88
#define HUD_COLON_Y 224

/* Mostrar overlay */
#define OVERLAY false

static void desenharNumeroGrande(int numero, int x, int y);
static void desenharNumeroPequeno(int numero, int x, int y);
static void desenharTempo(float tempo, int x, int y);
static void desenharTextoSprite(const char *texto, int x, int y);

/**
 * @brief Cria a HUD.
 */

void desenharHud(GameWorld *gw)
{
    if (OVERLAY)
    {
        DrawTexturePro(
            rm.texturaHud,
            (Rectangle){40, 208, 96, 48},
            (Rectangle){16, 16, 96 * HUD_SCALE, 48 * HUD_SCALE},
            (Vector2){0},
            0.0f,
            WHITE);
    }

    /* Pontuação */
    DrawTexturePro(
        rm.texturaHud,
        (Rectangle){24, 432, 40, 16},
        (Rectangle){16, 16, 40 * HUD_SCALE, 16 * HUD_SCALE},
        (Vector2){0},
        0.0f,
        WHITE);

    desenharNumeroGrande(
        gw->pontuacao,
        16 * 12,
        16);

    /* Tempo */
    DrawTexturePro(
        rm.texturaHud,
        (Rectangle){24, 456, 32, 16},
        (Rectangle){16, 16 * 3, 32 * HUD_SCALE, 16 * HUD_SCALE},
        (Vector2){0},
        0.0f,
        WHITE);

    desenharTempo(
        gw->tempoJogo,
        16 * 9,
        16 * 3);

    /* Aneis */
    DrawTexturePro(
        rm.texturaHud,
        (Rectangle){24, 480, 40, 16},
        (Rectangle){16, 16 * 5, 40 * HUD_SCALE, 16 * HUD_SCALE},
        (Vector2){0},
        0.0f,
        WHITE);

    desenharNumeroGrande(
        gw->jogador->quantidadeAneis,
        16 * 9,
        16 * 5);

    /* Vidas */
    DrawTexturePro(
        rm.texturaHud,
        (Rectangle){40, 400, 48, 16},
        (Rectangle){16, GetScreenHeight() - 48, 48 * HUD_SCALE, 16 * HUD_SCALE},
        (Vector2){0},
        0.0f,
        WHITE);

    desenharNumeroPequeno(
        gw->jogador->quantidadeVidas,
        96,
        GetScreenHeight() - 32);
}

static void desenharNumeroGrande(int numero, int x, int y)
{
    char texto[20];

    sprintf(texto, "%d", numero);

    desenharTextoSprite(texto, x, y);
}

static void desenharNumeroPequeno(int numero, int x, int y)
{
    char texto[20];

    sprintf(texto, "%d", numero);

    int tamanho = strlen(texto);

    for (int i = 0; texto[i] != '\0'; i++)
    {
        if (texto[i] < '0' || texto[i] > '9')
            continue;

        int digito = texto[i] - '0';

        Rectangle source = {
            HUD_NUM_P_X + (digito * (HUD_NUM_P_W + HUD_NUM_P_SPACING)),
            HUD_NUM_P_Y,
            HUD_NUM_P_W,
            HUD_NUM_P_H};

        Rectangle dest = {
            x - ((tamanho - 1 - i) * (HUD_NUM_P_W * HUD_SCALE)),
            y,
            HUD_NUM_P_W * HUD_SCALE,
            HUD_NUM_P_H * HUD_SCALE};

        DrawTexturePro(
            rm.texturaHudNumerosP,
            source,
            dest,
            (Vector2){0},
            0.0f,
            WHITE);
    }
}

static void desenharTempo(float tempo, int x, int y)
{
    int minutos = (int)tempo / 60;
    int segundos = (int)tempo % 60;

    char texto[20];

    sprintf(texto, "%01d:%02d", minutos, segundos);

    desenharTextoSprite(texto, x, y);
}

static void desenharTextoSprite(const char *texto, int x, int y)
{
    int tamanho = strlen(texto);

    for (int i = 0; texto[i] != '\0'; i++)
    {
        Rectangle source;

        if (texto[i] >= '0' && texto[i] <= '9')
        {
            int digito = texto[i] - '0';

            source = (Rectangle){
                HUD_NUM_G_X + (digito * (HUD_NUM_G_W + HUD_NUM_G_SPACING)),
                HUD_NUM_G_Y,
                HUD_NUM_G_W,
                HUD_NUM_G_H};
        }

        else if (texto[i] == ':')
        {
            source = (Rectangle){
                HUD_COLON_X,
                HUD_COLON_Y,
                HUD_NUM_G_W,
                HUD_NUM_G_H};
        }

        else
        {
            continue;
        }

        Rectangle dest = {
            x - ((tamanho - 1 - i) * (HUD_NUM_G_W * HUD_SCALE)),
            y,
            HUD_NUM_G_W * HUD_SCALE,
            HUD_NUM_G_H * HUD_SCALE};

        DrawTexturePro(
            rm.texturaHud,
            source,
            dest,
            (Vector2){0},
            0.0f,
            WHITE);
    }
}