/**
 * @file ResourceManager.c
 * @author Prof. Dr. David Buzatto
 * @brief ResourceManager implementation.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "ResourceManager.h"
#include "Utils.h"

ResourceManager rm = {0};

void loadResourcesResourceManager(void)
{
    rm.texturaJogador = carregarTexturaAlterandoCores(
        "resources/imagens/sprites/sonic.png",
        (Color[]){
            {37, 102, 26, 255},
            {13, 72, 7, 255},
        },
        (Color[]){
            BLANK,
            BLANK,
        },
        2);

    rm.texturaBadniks = carregarTexturaAlterandoCores(
        "resources/imagens/sprites/badniks.png",
        (Color[]){
            {13, 72, 7, 255},
        },
        (Color[]){
            BLANK,
        },
        1);

    rm.texturaChefao = carregarTexturaAlterandoCores(
        "resources/imagens/sprites/chefao.png",
        (Color[]){
            {13, 72, 7, 255},
        },
        (Color[]){
            BLANK,
        },
        1);

    rm.texturaItens = carregarTexturaAlterandoCores(
        "resources/imagens/itens/itens.png",
        (Color[]){
            {16, 112, 132, 255},
        },
        (Color[]){
            BLANK,
        },
        1);

    rm.texturaHud = carregarTexturaAlterandoCores(
        "resources/imagens/hud/hud.png",
        (Color[]){
            {16, 112, 132, 255},
            {0, 183, 239, 255},
            {77, 109, 243, 255},
            {153, 217, 234, 255},
        },
        (Color[]){
            BLANK,
            BLANK,
            BLANK,
            BLANK,
        },
        4);

    rm.texturaHudNumerosP = carregarTexturaAlterandoCores(
        "resources/imagens/hud/numeros-pequenos.png",
        (Color[]){
            {16, 112, 132, 255},
        },
        (Color[]){
            BLANK,
        },
        1);

    rm.texturaTelaInicial = carregarTexturaAlterandoCores(
        "resources/tela-inicial/tela-inicial.png",
        (Color[]){
            {16, 112, 132, 255},
        },
        (Color[]){
            BLANK,
        },
        1);

    rm.texturaAnimacaoMenu = carregarTexturaAlterandoCores(
        "resources/tela-inicial/animacao-tela-inicial.png",
        (Color[]){
            {16, 112, 132, 255},
            {153, 217, 234, 255},
        },
        (Color[]){
            BLANK,
            BLANK,
        },
        2);

    rm.texturaDecoracoes = carregarTexturaAlterandoCores(
        "resources/imagens/sprites/decoracoes.png",
        (Color[]){
            {135, 16, 19, 255},
        },
        (Color[]){
            BLANK,
        },
        1);

    /* ── Tiles ─────────────────────────────────────────────────────────── */
    rm.texturaTerreno = LoadTexture("resources/imagens/tiles/terreno.png");
    rm.texturaTerreno2 = LoadTexture("resources/imagens/tiles/marble_tileset_final.png");

    rm.texturaParedeArena = LoadTexture("resources/imagens/tiles/parede_arena.png");

    /* ── Fundos ─────────────────────────────────────────────────────────── */
    rm.texturaFundo = LoadTexture("resources/imagens/fundo/fundo.png");
    rm.texturaFundo2 = LoadTexture("resources/imagens/fundo/fundo2.png");

    SetTextureFilter(rm.texturaJogador, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaBadniks, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaChefao, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaItens, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaTerreno, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaTerreno2, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaParedeArena, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaFundo, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaFundo2, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaHud, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaHudNumerosP, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaTelaInicial, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaAnimacaoMenu, TEXTURE_FILTER_POINT);
    SetTextureFilter(rm.texturaDecoracoes, TEXTURE_FILTER_POINT);

    rm.somAnel = LoadSound("resources/sons/efeitos/anel.wav");
    rm.somFrenagem = LoadSound("resources/sons/efeitos/frenagem.wav");
    rm.somHitComAnel = LoadSound("resources/sons/efeitos/hit-com-anel.wav");
    rm.somHitInimigo = LoadSound("resources/sons/efeitos/hit-inimigo.wav");
    rm.somMorte = LoadSound("resources/sons/efeitos/morte.wav");
    rm.somPulo = LoadSound("resources/sons/efeitos/pulo.wav");
    rm.somHitBoss = LoadSound("resources/sons/efeitos/boss-hit.mp3");
    rm.somExplosao = LoadSound("resources/sons/efeitos/yeehaw.mp3");
    rm.somAnelGColetado = LoadSound("resources/sons/efeitos/anel-g-coletado.mp3");
    rm.somAnelSurgindo = LoadSound("resources/sons/efeitos/anel-surgindo.mp3");
    rm.somTooEasy = LoadSound("resources/sons/efeitos/too-easy.mp3");
    rm.somEscudo = LoadSound("resources/sons/efeitos/shield.mp3");
    rm.somEscudoQuebrando = LoadSound("resources/sons/efeitos/shield-break.mp3");

    rm.musicaFase01 = LoadMusicStream("resources/sons/musicas/green-hill-zone.mp3");
    rm.musicaFase02 = LoadMusicStream("resources/sons/musicas/marble-zone.mp3");
    rm.musicaAbertura = LoadMusicStream("resources/sons/musicas/intro.mp3");
    rm.musicaBoss = LoadMusicStream("resources/sons/musicas/boss-theme.mp3");
}

void unloadResourcesResourceManager(void)
{
    UnloadTexture(rm.texturaJogador);
    UnloadTexture(rm.texturaBadniks);
    UnloadTexture(rm.texturaChefao);
    UnloadTexture(rm.texturaItens);
    UnloadTexture(rm.texturaTerreno);
    UnloadTexture(rm.texturaTerreno2);
    UnloadTexture(rm.texturaParedeArena);
    UnloadTexture(rm.texturaFundo);
    UnloadTexture(rm.texturaFundo2);
    UnloadTexture(rm.texturaHud);
    UnloadTexture(rm.texturaHudNumerosP);
    UnloadTexture(rm.texturaTelaInicial);
    UnloadTexture(rm.texturaAnimacaoMenu);
    UnloadTexture(rm.texturaDecoracoes);

    UnloadSound(rm.somAnel);
    UnloadSound(rm.somFrenagem);
    UnloadSound(rm.somHitComAnel);
    UnloadSound(rm.somHitInimigo);
    UnloadSound(rm.somMorte);
    UnloadSound(rm.somPulo);
    UnloadSound(rm.somHitBoss);
    UnloadSound(rm.somExplosao);
    UnloadSound(rm.somTooEasy);
    UnloadSound(rm.somAnelSurgindo);
    UnloadSound(rm.somAnelGColetado);
    UnloadSound(rm.somEscudo);
    UnloadSound(rm.somEscudoQuebrando);

    UnloadMusicStream(rm.musicaFase01);
    UnloadMusicStream(rm.musicaFase02);
    UnloadMusicStream(rm.musicaAbertura);
    UnloadMusicStream(rm.musicaBoss);
}