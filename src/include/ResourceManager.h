/**
 * @file ResourceManager.h
 * @author Prof. Dr. David Buzatto
 * @brief ResourceManager struct and function declarations.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "raylib/raylib.h"

typedef struct ResourceManager
{

    Texture2D texturaJogador;
    Texture2D texturaBadniks;
    Texture2D texturaChefao;
    Texture2D texturaItens;
    Texture2D texturaHud;
    Texture2D texturaHudNumerosP;
    Texture2D texturaDecoracoes;

    Texture2D texturaTelaInicial;
    Texture2D texturaAnimacaoMenu;

    Texture2D texturaTerreno;  /* tiles da fase 1 (Green Hill)  */
    Texture2D texturaTerreno2; /* tiles da fase 2 (Marble Zone) */
    Texture2D texturaFundo;    /* fundo da fase 1               */
    Texture2D texturaFundo2;   /* fundo da fase 2               */
    Texture2D texturaParedeArena;

    Sound somAnel;
    Sound somFrenagem;
    Sound somHitComAnel;
    Sound somHitInimigo;
    Sound somMorte;
    Sound somPulo;
    Sound somHitBoss;
    Sound somExplosao;

    Music musicaAbertura;
    Music musicaFase01;
    Music musicaFase02;
    Music musicaBoss;

} ResourceManager;

/**
 * @brief Global ResourceManager instance.
 */
extern ResourceManager rm;

/**
 * @brief Load global game resources.
 */
void loadResourcesResourceManager(void);

/**
 * @brief Unload global game resources.
 */
void unloadResourcesResourceManager(void);