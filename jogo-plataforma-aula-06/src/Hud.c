/**
 * @file Hud.c
 * @brief Implementação do HUD no estilo Sonic the Hedgehog 1 (Mega Drive).
 *
 * Layout original do Sonic 1:
 *   Canto superior esquerdo da tela (posição fixa, fora da câmera):
 *
 *   SCORE          XXXXXXX   (pontuação sem zero à esquerda, máx 9.999.990)
 *   TIME             XXX     (contador de segundos, formato 000, máx 999)
 *   RINGS            XXX     (quantidade de anéis coletados)
 *
 *   Canto inferior esquerdo (ou superior esquerdo abaixo dos outros):
 *   [face Sonic] x  X        (quantidade de vidas)
 */
#include <stdio.h>

#include "Hud.h"
#include "Tipos.h"
#include "raylib/raylib.h"

/* ── Constantes de layout ──────────────────────────────────────────── */

/* Posição X do bloco HUD na tela */
#define HUD_X           16

/* Posição Y inicial do bloco HUD */
#define HUD_Y           16

/* Espaçamento vertical entre linhas do HUD */
#define HUD_LINHA_H     28

/* Tamanho da fonte dos rótulos (SCORE / TIME / RINGS) */
#define HUD_FONTE_LABEL 20

/* Tamanho da fonte dos valores */
#define HUD_FONTE_VALOR 20

/* Deslocamento X do valor em relação ao rótulo */
#define HUD_VALOR_OFFSET_X  120

/* Cores no estilo Sonic 1 */
#define HUD_COR_LABEL   WHITE
#define HUD_COR_VALOR   WHITE
#define HUD_COR_SOMBRA  ((Color){0, 0, 0, 180})

/* ── Função auxiliar: desenha texto com sombra ─────────────────────── */
static void desenharTextoHud( const char *texto, int x, int y, int fontSize, Color cor ) {
    /* Sombra deslocada 2px para tornar o texto legível sobre qualquer fundo */
    DrawText( texto, x + 2, y + 2, fontSize, HUD_COR_SOMBRA );
    DrawText( texto, x,     y,     fontSize, cor );
}

/* ── Função principal ──────────────────────────────────────────────── */
void desenharHud( GameWorld *gw ) {

    Jogador *j = gw->jogador;

    int x      = HUD_X;
    int y      = HUD_Y;
    int vOff   = HUD_VALOR_OFFSET_X;
    int lh     = HUD_LINHA_H;
    int fLabel = HUD_FONTE_LABEL;
    int fValor = HUD_FONTE_VALOR;

    /* ── SCORE ── */
    desenharTextoHud( "SCORE", x, y, fLabel, HUD_COR_LABEL );
    desenharTextoHud(
        TextFormat( "%d", gw->pontuacao ),
        x + vOff, y, fValor, HUD_COR_VALOR
    );

    /* ── TIME ── */
    y += lh;
    desenharTextoHud( "TIME", x, y, fLabel, HUD_COR_LABEL );

    /* Tempo em segundos inteiros, exibido com 3 dígitos (000–999) */
    int segundos = (int) gw->tempoJogo;
    if ( segundos > 999 ) segundos = 999;   /* trava no máximo como no original */
    desenharTextoHud(
        TextFormat( "%03d", segundos ),
        x + vOff, y, fValor, HUD_COR_VALOR
    );

    /* ── RINGS ── */
    y += lh;
    desenharTextoHud( "RINGS", x, y, fLabel, HUD_COR_LABEL );
    desenharTextoHud(
        TextFormat( "%d", j->quantidadeAneis ),
        x + vOff, y, fValor, HUD_COR_VALOR
    );

    /* ── LIVES (vidas) ── */
    /*
     * No Sonic 1 original as vidas ficam no canto inferior esquerdo com
     * a face do Sonic. Aqui desenhamos logo abaixo das outras informações
     * como placeholder visual fiel à estrutura do jogo.
     * Se quiser mover para o canto inferior, basta trocar y pelo valor:
     *     y = GetScreenHeight() - 48;
     */
    y += lh + 8;   /* pequena separação extra */
    desenharTextoHud( "SONIC", x, y, fLabel, HUD_COR_LABEL );

    /* "x" minúsculo entre o ícone e o número — estilo original */
    desenharTextoHud( "x", x + vOff - 22, y + 2, fLabel - 4, HUD_COR_VALOR );
    desenharTextoHud(
        TextFormat( "%d", j->quantidadeVidas ),
        x + vOff, y, fValor, HUD_COR_VALOR
    );

}