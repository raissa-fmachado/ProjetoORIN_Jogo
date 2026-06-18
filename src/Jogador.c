/**
 * @file Jogador.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Jogador.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "Inimigo.h"
#include "InimigoMotobug.h"
#include "InimigoSpikes.h"
#include "InimigoBallHog.h"
#include "InimigoBatbrain.h"
#include "InimigoEggMobile.h"
#include "Item.h"
#include "ItemAnel.h"
#include "ItemEscudo.h"
#include "BlocoInterrogacao.h"
#include "Macros.h"
#include "Jogador.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void desenharQuadroAnimacaoJogador(Jogador *j, QuadroAnimacao *qa, Color tonalidade);
static QuadroAnimacao *getQuadroAnimacaoAtualJogador(Jogador *j);
static Animacao *getAnimacaoAtualJogador(Jogador *j);

static void resolverColisaoJogadorObstaculosMapaX(Jogador *j, Mapa *mapa);
static void resolverColisaoJogadorObstaculosMapaY(Jogador *j, Mapa *mapa);

static void resolverColisaoJogadorItensMapa(Jogador *j, Mapa *mapa, GameWorld *gw);
static void resolverColisaoJogadorInimigosMapa(Jogador *j, Mapa *mapa, GameWorld *gw);

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria uma instância alocada dinamicamente da struct Jogador.
 */
Jogador *criarJogador(float x, float y, float w, float h)
{

    Jogador *novoJogador = (Jogador *)malloc(sizeof(Jogador));

    novoJogador->ret.x = x;
    novoJogador->ret.y = y;
    novoJogador->ret.width = w;
    novoJogador->ret.height = h;
    novoJogador->vel = (Vector2){0};

    novoJogador->cor = BLUE;

    novoJogador->velAndando = 200;
    novoJogador->velAndandoRapido = 400;
    novoJogador->velCorrendo = 800;
    novoJogador->velPulo = -550;
    novoJogador->velMaxQueda = 600;

    novoJogador->aceleracao = 200;
    novoJogador->desaceleracao = 400;
    novoJogador->frenagem = 1800;

    novoJogador->quantidadePulos = 0;
    novoJogador->quantidadeMaxPulos = 1;

    novoJogador->quantidadeAneis = 0;
    novoJogador->quantidadeVidas = 3;

    novoJogador->invulneravel = false;
    novoJogador->tempoInvulnerabilidade = 3.0f;
    novoJogador->contadorTempoInvulnerabilidade = 0.0f;

    novoJogador->olhandoParaDireita = true;

    novoJogador->piscaPisca = false;
    novoJogador->tempoPiscaPisca = 0.05f;
    novoJogador->contadorTempoPiscaPisca = 0.0f;

    novoJogador->possuiEscudo = false;
    novoJogador->tipoEscudo = TIPO_ESCUDO_NENHUM;

    /* Inicializar sistema de spin */
    novoJogador->emSpin = false;
    novoJogador->contadorTempoSpin = 0.0f;
    novoJogador->tempoMaxSpin = 999.0f; /* spin dura enquanto DOWN estiver pressionado */
    novoJogador->velSpinHorizontal = 0.0f;

    novoJogador->freando = false;

    int quantidadeAnimacoes = 0;

    novoJogador->animacaoParado.quantidadeQuadros = 1;
    novoJogador->animacaoParado.quadroAtual = 0;
    novoJogador->animacaoParado.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoParado.pararNoUltimoQuadro = false;
    novoJogador->animacaoParado.executarUmaVez = false;
    novoJogador->animacaoParado.finalizada = false;
    criarQuadrosAnimacao(&novoJogador->animacaoParado, novoJogador->animacaoParado.quantidadeQuadros);
    inicializarQuadrosAnimacao(
        novoJogador->animacaoParado.quadros,
        novoJogador->animacaoParado.quantidadeQuadros,
        1000,       // duração padrão para todos os quadros
        24, 251,    // início
        48, 48,     // dimensões
        4,          // separação
        false,      // de trás para frente
        (Rectangle){// retângulo de colisão padrão para cada quadro
                    32, 20, 42, 76});

    novoJogador->animacaoAndando.quantidadeQuadros = 8;
    novoJogador->animacaoAndando.quadroAtual = 0;
    novoJogador->animacaoAndando.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoAndando.pararNoUltimoQuadro = false;
    novoJogador->animacaoAndando.executarUmaVez = false;
    novoJogador->animacaoAndando.finalizada = false;
    criarQuadrosAnimacao(&novoJogador->animacaoAndando, novoJogador->animacaoAndando.quantidadeQuadros);
    inicializarQuadrosAnimacao(
        novoJogador->animacaoAndando.quadros,
        novoJogador->animacaoAndando.quantidadeQuadros,
        80,         // duração padrão para cada quadro
        664, 324,   // início
        48, 48,     // dimensões
        4,          // separação
        false,      // de trás para frente
        (Rectangle){// retângulo de colisão padrão para cada quadro
                    32, 20, 42, 76});

    novoJogador->animacaoAndandoRapido.quantidadeQuadros = 8;
    novoJogador->animacaoAndandoRapido.quadroAtual = 0;
    novoJogador->animacaoAndandoRapido.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoAndandoRapido.pararNoUltimoQuadro = false;
    novoJogador->animacaoAndandoRapido.executarUmaVez = false;
    novoJogador->animacaoAndandoRapido.finalizada = false;
    criarQuadrosAnimacao(&novoJogador->animacaoAndandoRapido, novoJogador->animacaoAndandoRapido.quantidadeQuadros);
    inicializarQuadrosAnimacao(
        novoJogador->animacaoAndandoRapido.quadros,
        novoJogador->animacaoAndandoRapido.quantidadeQuadros,
        40,         // duração padrão para cada quadro
        664, 324,   // início
        48, 48,     // dimensões
        4,          // separação
        false,      // de trás para frente
        (Rectangle){// retângulo de colisão padrão para cada quadro
                    32, 20, 42, 76});

    novoJogador->animacaoCorrendo.quantidadeQuadros = 4;
    novoJogador->animacaoCorrendo.quadroAtual = 0;
    novoJogador->animacaoCorrendo.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoCorrendo.pararNoUltimoQuadro = false;
    novoJogador->animacaoCorrendo.executarUmaVez = false;
    novoJogador->animacaoCorrendo.finalizada = false;
    criarQuadrosAnimacao(&novoJogador->animacaoCorrendo, novoJogador->animacaoCorrendo.quantidadeQuadros);
    inicializarQuadrosAnimacao(
        novoJogador->animacaoCorrendo.quadros,
        novoJogador->animacaoCorrendo.quantidadeQuadros,
        20,         // duração padrão para cada quadro
        24, 397,    // início
        48, 48,     // dimensões
        4,          // separação
        false,      // de trás para frente
        (Rectangle){// retângulo de colisão padrão para cada quadro
                    32, 20, 42, 76});

    novoJogador->animacaoPulando.quantidadeQuadros = 4;
    novoJogador->animacaoPulando.quadroAtual = 0;
    novoJogador->animacaoPulando.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoPulando.pararNoUltimoQuadro = false;
    novoJogador->animacaoPulando.executarUmaVez = false;
    novoJogador->animacaoPulando.finalizada = false;
    criarQuadrosAnimacao(&novoJogador->animacaoPulando, novoJogador->animacaoPulando.quantidadeQuadros);
    inicializarQuadrosAnimacao(
        novoJogador->animacaoPulando.quadros,
        novoJogador->animacaoPulando.quantidadeQuadros,
        40,         // duração padrão para cada quadro
        248, 397,   // início
        48, 48,     // dimensões
        4,          // separação
        false,      // de trás para frente
        (Rectangle){// retângulo de colisão padrão para cada quadro
                    32, 36, 42, 60});

    novoJogador->animacaoPulandoRapido.quantidadeQuadros = 4;
    novoJogador->animacaoPulandoRapido.quadroAtual = 0;
    novoJogador->animacaoPulandoRapido.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoPulandoRapido.pararNoUltimoQuadro = false;
    novoJogador->animacaoPulandoRapido.executarUmaVez = false;
    novoJogador->animacaoPulandoRapido.finalizada = false;
    criarQuadrosAnimacao(&novoJogador->animacaoPulandoRapido, novoJogador->animacaoPulandoRapido.quantidadeQuadros);
    inicializarQuadrosAnimacao(
        novoJogador->animacaoPulandoRapido.quadros,
        novoJogador->animacaoPulandoRapido.quantidadeQuadros,
        25,         // duração padrão para cada quadro
        248, 397,   // início
        48, 48,     // dimensões
        4,          // separação
        false,      // de trás para frente
        (Rectangle){// retângulo de colisão padrão para cada quadro
                    32, 36, 42, 60});

    novoJogador->animacaoPulandoCorrendo.quantidadeQuadros = 4;
    novoJogador->animacaoPulandoCorrendo.quadroAtual = 0;
    novoJogador->animacaoPulandoCorrendo.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoPulandoCorrendo.pararNoUltimoQuadro = false;
    novoJogador->animacaoPulandoCorrendo.executarUmaVez = false;
    novoJogador->animacaoPulandoCorrendo.finalizada = false;
    criarQuadrosAnimacao(&novoJogador->animacaoPulandoCorrendo, novoJogador->animacaoPulandoCorrendo.quantidadeQuadros);
    inicializarQuadrosAnimacao(
        novoJogador->animacaoPulandoCorrendo.quadros,
        novoJogador->animacaoPulandoCorrendo.quantidadeQuadros,
        15,         // duração padrão para cada quadro
        248, 397,   // início
        48, 48,     // dimensões
        4,          // separação
        false,      // de trás para frente
        (Rectangle){// retângulo de colisão padrão para cada quadro
                    32, 36, 42, 60});

    novoJogador->animacaoEscudo.quantidadeQuadros = 2;
    novoJogador->animacaoEscudo.quadroAtual = 0;
    novoJogador->animacaoEscudo.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoEscudo.pararNoUltimoQuadro = false;
    novoJogador->animacaoEscudo.executarUmaVez = false;
    novoJogador->animacaoEscudo.finalizada = false;

    criarQuadrosAnimacao(&novoJogador->animacaoEscudo, 4);

    inicializarQuadrosAnimacao(
        novoJogador->animacaoEscudo.quadros,
        novoJogador->animacaoEscudo.quantidadeQuadros,
        80,
        1, 35, // origem do escudo no spritesheet (ajuste se necessário)
        48, 48,
        1,
        false,
        (Rectangle){0});

    /* Inicializar animação de SPIN (usa a animação de corrida) */
    novoJogador->animacaoSpin.quantidadeQuadros = 4;
    novoJogador->animacaoSpin.quadroAtual = 0;
    novoJogador->animacaoSpin.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoSpin.pararNoUltimoQuadro = false;
    novoJogador->animacaoSpin.executarUmaVez = false;
    novoJogador->animacaoSpin.finalizada = false;
    criarQuadrosAnimacao(&novoJogador->animacaoSpin, novoJogador->animacaoSpin.quantidadeQuadros);
    inicializarQuadrosAnimacao(
        novoJogador->animacaoSpin.quadros,
        novoJogador->animacaoSpin.quantidadeQuadros,
        15,         // duração rápida para efeito de rotação */
        24, 397,    // início (mesma da corrida)
        48, 48,     // dimensões
        4,          // separação
        false,      // de trás para frente
        (Rectangle){// retângulo de colisão durante spin
                    32, 20, 42, 76});

    novoJogador->animacoes[ESTADO_JOGADOR_PARADO] = &novoJogador->animacaoParado;
    quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_ANDANDO] = &novoJogador->animacaoAndando;
    quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_ANDANDO_RAPIDO] = &novoJogador->animacaoAndandoRapido;
    quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_CORRENDO] = &novoJogador->animacaoCorrendo;
    quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_PULANDO] = &novoJogador->animacaoPulando;
    quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_PULANDO_RAPIDO] = &novoJogador->animacaoPulandoRapido;
    quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_PULANDO_CORRENDO] = &novoJogador->animacaoPulandoCorrendo;
    quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_SPIN] = &novoJogador->animacaoSpin;
    quantidadeAnimacoes++;
    novoJogador->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoJogador;
}

/**
 * @brief Destrói um objeto Jogador e libera seus recursos.
 */
void destruirJogador(Jogador *j)
{
    if (j != NULL)
    {
        for (int i = 0; i < j->quantidadeAnimacoes; i++)
        {
            destruirQuadrosAnimacao(j->animacoes[i]);
        }
        free(j);
    }
}

/**
 * @brief Lê a entrada do usuário e atualiza as velocidades do jogador.
 */
void entradaJogador(Jogador *j, float delta)
{

    EstadoJogador estadoAnterior = j->estado;

    bool direitaDown = IsKeyDown(KEY_RIGHT) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT));
    bool esquerdaDown = IsKeyDown(KEY_LEFT) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT));
    bool puloPressed = IsKeyPressed(KEY_SPACE) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN));
    bool baixoDown = IsKeyDown(KEY_DOWN) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN));

    /* ─────────────────── Sistema de SPIN ──────────────────────── */
    if (j->emSpin)
    {
        /* Mantém a velocidade do spin (com direção) */
        if (j->olhandoParaDireita)
        {
            j->vel.x = j->velSpinHorizontal;
        }
        else
        {
            j->vel.x = -j->velSpinHorizontal;
        }

        /* Pulo durante spin: verificado ANTES de qualquer saída do spin */
        if (puloPressed && j->quantidadePulos == 0)
        {
            /* pula independente de DOWN estar pressionado ou não */
            j->vel.y = j->velPulo;
            j->quantidadePulos++;
            j->contadorTempoSpin = 0.0f;
            PlaySound(rm.somPulo);
            /* NÃO sai do spin: bolinha continua no ar */
        }
        else
        {
            j->contadorTempoSpin += delta;
            if (j->contadorTempoSpin >= j->tempoMaxSpin)
            {
                j->emSpin = false;
                j->contadorTempoSpin = 0.0f;
            }
            /* só sai do spin se soltar DOWN E estiver no chão E não pulou */
            else if (!baixoDown && j->quantidadePulos == 0)
            {
                j->emSpin = false;
                j->contadorTempoSpin = 0.0f;
            }
        }
    }
    else
    {
        /* Iniciando SPIN """""""""""""""""""""""""""""""""""""""
           - Se estiver andando e apertar DOWN
           - Se estiver parado, apertar DOWN + PULO
        */
        if (baixoDown)
        {
            /* Caso 1: Andando e apertar DOWN - entra em spin */
            float absVelX = fabsf(j->vel.x);
            if (absVelX >= j->velAndando && j->quantidadePulos == 0)
            {
                j->emSpin = true;
                j->contadorTempoSpin = 0.0f;
                j->velSpinHorizontal = 400.0f; /* Velocidade do spin */
                PlaySound(rm.somPulo);         /* Som do spin */
            }
            /* Caso 2: Parado + Pulo - entra em spin e pula */
            else if (puloPressed && j->quantidadePulos == 0)
            {
                j->emSpin = true;
                j->contadorTempoSpin = 0.0f;
                j->velSpinHorizontal = 250.0f; /* Velocidade mais baixa para spin parado */
                j->vel.y = j->velPulo;
                j->quantidadePulos++;
                PlaySound(rm.somPulo);
            }
        }
    }

    /* Se está em spin, não processa input normal de movimento */
    if (!j->emSpin)
    {
        if (direitaDown)
        {
            if (j->vel.x < 0)
            {
                j->vel.x += j->frenagem * delta;
                if (!j->freando && j->estado == ESTADO_JOGADOR_CORRENDO)
                {
                    PlaySound(rm.somFrenagem);
                    j->freando = true;
                }
                if (j->vel.x > 0)
                {
                    j->vel.x = 0;
                    j->freando = false;
                }
            }
            else
            {
                j->vel.x += j->aceleracao * delta;
                if (j->vel.x > j->velCorrendo)
                {
                    j->vel.x = j->velCorrendo;
                }
            }
            j->olhandoParaDireita = true;
        }
        else if (esquerdaDown)
        {
            if (j->vel.x > 0)
            {
                j->vel.x -= j->frenagem * delta;
                if (!j->freando && j->estado == ESTADO_JOGADOR_CORRENDO)
                {
                    PlaySound(rm.somFrenagem);
                    j->freando = true;
                }
                if (j->vel.x < 0)
                {
                    j->vel.x = 0;
                    j->freando = false;
                }
            }
            else
            {
                j->vel.x -= j->aceleracao * delta;
                if (j->vel.x < -j->velCorrendo)
                {
                    j->vel.x = -j->velCorrendo;
                }
            }
            j->olhandoParaDireita = false;
        }
        else
        {
            if (j->vel.x > 0)
            {
                j->vel.x -= j->desaceleracao * delta;
                if (j->vel.x < 0)
                {
                    j->vel.x = 0;
                }
            }
            else if (j->vel.x < 0)
            {
                j->vel.x += j->desaceleracao * delta;
                if (j->vel.x > 0)
                {
                    j->vel.x = 0;
                }
            }
        }
    }

    float absVelX = fabsf(j->vel.x);

    /* ─────────────────── Estados do jogador ──────────────────────── */
    if (j->emSpin)
    {
        j->estado = ESTADO_JOGADOR_SPIN;
    }
    else if (j->quantidadePulos > 0)
    {
        if (absVelX <= j->velAndando)
        {
            j->estado = ESTADO_JOGADOR_PULANDO;
        }
        else if (absVelX <= j->velAndandoRapido)
        {
            j->estado = ESTADO_JOGADOR_PULANDO_RAPIDO;
        }
        else
        {
            j->estado = ESTADO_JOGADOR_PULANDO_CORRENDO;
        }
    }
    else if (absVelX < 1.0f)
    {
        j->estado = ESTADO_JOGADOR_PARADO;
    }
    else if (absVelX <= j->velAndando)
    {
        j->estado = ESTADO_JOGADOR_ANDANDO;
    }
    else if (absVelX <= j->velAndandoRapido)
    {
        j->estado = ESTADO_JOGADOR_ANDANDO_RAPIDO;
    }
    else
    {
        j->estado = ESTADO_JOGADOR_CORRENDO;
    }

    /* ─────────────────── Pulo (chão ou duplo com escudo) ──────────────────────── */
    if (puloPressed && j->quantidadePulos == 0)
    {
        /* Pulo do chão: funciona com ou sem escudo */
        j->vel.y = j->velPulo;
        j->quantidadePulos++;
        PlaySound(rm.somPulo);
    }
    else if (puloPressed && j->quantidadePulos > 0 && j->quantidadePulos < j->quantidadeMaxPulos && j->possuiEscudo && !j->emSpin)
    {
        /* Pulo duplo no ar — só com escudo */
        j->vel.y = j->velPulo;
        j->quantidadePulos++;

        switch (j->tipoEscudo)
        {
        case TIPO_ESCUDO_AGUA:
            j->vel.y *= 1.15f;
            break;
        case TIPO_ESCUDO_FOGO:
            if (j->olhandoParaDireita)
                j->vel.x = j->velCorrendo;
            else
                j->vel.x = -j->velCorrendo;
            break;
        case TIPO_ESCUDO_RAIO:
            j->vel.y *= 1.25f;
            j->quantidadeMaxPulos = 2;
            break;
        default:
            break;
        }

        PlaySound(rm.somPulo);
    }

    // sincronização de animações andando e andando rápido
    if (estadoAnterior == ESTADO_JOGADOR_ANDANDO && j->estado == ESTADO_JOGADOR_ANDANDO_RAPIDO)
    {
        sincronizarAnimacao(&j->animacaoAndandoRapido, &j->animacaoAndando);
    }
    else if (estadoAnterior == ESTADO_JOGADOR_ANDANDO_RAPIDO && j->estado == ESTADO_JOGADOR_ANDANDO)
    {
        sincronizarAnimacao(&j->animacaoAndando, &j->animacaoAndandoRapido);
    }
}

/**
 * @brief Aplica física e resolve colisões do jogador com o mundo.
 */
void atualizarJogador(Jogador *j, GameWorld *gw, float delta)
{

    if (j->invulneravel)
    {

        j->contadorTempoPiscaPisca += delta;
        if (j->contadorTempoPiscaPisca >= j->tempoPiscaPisca)
        {
            j->contadorTempoPiscaPisca = 0.0f;
            j->piscaPisca = !j->piscaPisca;
        }

        j->contadorTempoInvulnerabilidade += delta;
        if (j->contadorTempoInvulnerabilidade >= j->tempoInvulnerabilidade)
        {
            j->contadorTempoInvulnerabilidade = 0.0f;
            j->invulneravel = false;
            j->contadorTempoPiscaPisca = 0.0f;
            j->piscaPisca = false;
        }
    }

    if (j->possuiEscudo)
    {
        atualizarAnimacao(&j->animacaoEscudo, delta);
    }

    Animacao *animacaoAtual = getAnimacaoAtualJogador(j);
    atualizarAnimacao(animacaoAtual, delta);

    // fase X: move horizontalmente e resolve colisões laterais
    j->ret.x += j->vel.x * delta;
    resolverColisaoJogadorObstaculosMapaX(j, gw->mapa);

    // fase Y: aplica gravidade, move verticalmente e resolve colisões verticais
    j->vel.y += gw->gravidade * delta;
    if (j->vel.y > j->velMaxQueda)
    {
        j->vel.y = j->velMaxQueda;
    }
    j->ret.y += j->vel.y * delta;
    resolverColisaoJogadorObstaculosMapaY(j, gw->mapa);

    resolverColisaoJogadorItensMapa(j, gw->mapa, gw);
    resolverColisaoJogadorInimigosMapa(j, gw->mapa, gw);
}

/**
 * @brief Desenha o jogador.
 */
void desenharJogador(Jogador *j)
{
    if (!j->piscaPisca)
    {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador(j);
        desenharQuadroAnimacaoJogador(j, qa, WHITE);
    }

    if (j->possuiEscudo)
    {
        QuadroAnimacao *qaEscudo =
            getQuadroAtualAnimacao(&j->animacaoEscudo);

        DrawTexturePro(
            rm.texturaItens,
            qaEscudo->fonte,
            j->ret,
            (Vector2){0},
            0.0f,
            WHITE);
    }

    if (MOSTRAR_RETANGULOS)
    {
        DrawRectangleRec(j->ret, Fade(j->cor, 0.5f));
        DrawRectangleLines(j->ret.x, j->ret.y, j->ret.width, j->ret.height, BLACK);
    }
}

static void desenharQuadroAnimacaoJogador(Jogador *j, QuadroAnimacao *qa, Color tonalidade)
{

    if (qa != NULL)
    {

        DrawTexturePro(
            rm.texturaJogador,
            (Rectangle){
                qa->fonte.x,
                qa->fonte.y,
                j->olhandoParaDireita ? qa->fonte.width : -qa->fonte.width,
                qa->fonte.height},
            j->ret,
            (Vector2){0},
            0.0f,
            tonalidade);

        if (MOSTRAR_RETANGULOS)
        {
            float xDesenho = j->olhandoParaDireita
                                 ? j->ret.x + qa->retColisao.x
                                 : j->ret.x + j->ret.width - qa->retColisao.x - qa->retColisao.width;
            float yDesenho = j->ret.y + qa->retColisao.y;
            DrawRectangle(xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade(GREEN, 0.5f));
        }
    }
}

static QuadroAnimacao *getQuadroAnimacaoAtualJogador(Jogador *j)
{
    return getQuadroAtualAnimacao(getAnimacaoAtualJogador(j));
}

static Animacao *getAnimacaoAtualJogador(Jogador *j)
{
    return j->animacoes[j->estado];
}

/**
 * @brief Resolve colisões do jogador com o mapa no eixo X.
 */
static void resolverColisaoJogadorObstaculosMapaX(Jogador *j, Mapa *mapa)
{

    ElementoMapa *el = mapa->obstaculos;

    while (el != NULL)
    {

        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador(j);

        float deslocamentoX = j->olhandoParaDireita
                                  ? qa->retColisao.x
                                  : j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            j->ret.x + deslocamentoX,
            j->ret.y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height};

        Obstaculo *o = (Obstaculo *)el->objeto;

        if (CheckCollisionRecs(retColCalculado, o->ret))
        {
            if (retColCalculado.x + retColCalculado.width / 2 < o->ret.x + o->ret.width / 2)
            {
                j->ret.x = o->ret.x - qa->retColisao.width - deslocamentoX;
            }
            else
            {
                j->ret.x = o->ret.x + o->ret.width - deslocamentoX;
            }
            j->vel.x = 0;
        }

        el = el->proximo;
    }
}

/**
 * @brief Resolve colisões do jogador com o mapa no eixo Y.
 */
static void resolverColisaoJogadorObstaculosMapaY(Jogador *j, Mapa *mapa)
{

    /* ── obstáculos normais ─────────────────────────────────────────── */
    ElementoMapa *el = mapa->obstaculos;

    while (el != NULL)
    {

        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador(j);

        float deslocamentoX = j->olhandoParaDireita
                                  ? qa->retColisao.x
                                  : j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            j->ret.x + deslocamentoX,
            j->ret.y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height};

        Obstaculo *o = (Obstaculo *)el->objeto;

        if (CheckCollisionRecs(retColCalculado, o->ret))
        {
            if (retColCalculado.y + retColCalculado.height / 2 < o->ret.y + o->ret.height / 2)
            {
                j->ret.y = o->ret.y - qa->retColisao.height - deslocamentoY;
                j->quantidadePulos = 0;
            }
            else
            {
                j->ret.y = o->ret.y + o->ret.height - deslocamentoY;
            }
            j->vel.y = 0;
        }

        el = el->proximo;
    }

    /* ── blocos de interrogação (sólidos pelo topo) ─────────────────── */
    el = mapa->itens;
    while (el != NULL)
    {

        Item *item = (Item *)el->objeto;

        if (item->tipo == TIPO_ITEM_BLOCO_INTERROGACAO)
        {

            BlocoInterrogacao *bloco = (BlocoInterrogacao *)item->objeto;

            Rectangle retBloco = {
                bloco->ret.x,
                bloco->ret.y + bloco->saltY,
                bloco->ret.width,
                bloco->ret.height};

            QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador(j);
            float deslocamentoX = j->olhandoParaDireita
                                      ? qa->retColisao.x
                                      : j->ret.width - qa->retColisao.x - qa->retColisao.width;
            float deslocamentoY = qa->retColisao.y;

            Rectangle retColCalculado = {
                j->ret.x + deslocamentoX,
                j->ret.y + deslocamentoY,
                qa->retColisao.width,
                qa->retColisao.height};

            if (CheckCollisionRecs(retColCalculado, retBloco))
            {
                float centroJogadorY = retColCalculado.y + retColCalculado.height / 2.0f;
                float centroBlocoY = retBloco.y + retBloco.height / 2.0f;

                if (centroJogadorY < centroBlocoY && j->vel.y >= 0)
                {
                    /* jogador em cima: pousa sobre o bloco */
                    j->ret.y = retBloco.y - qa->retColisao.height - deslocamentoY;
                    j->quantidadePulos = 0;
                    j->vel.y = 0;
                }
                /* colisão por baixo é tratada em resolverColisaoJogadorItensMapa */
            }
        }

        el = el->proximo;
    }
}

static void resolverColisaoJogadorItensMapa(Jogador *j, Mapa *mapa, GameWorld *gw)
{

    ElementoMapa *el = mapa->itens;

    while (el != NULL)
    {

        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador(j);

        float deslocamentoX = j->olhandoParaDireita
                                  ? qa->retColisao.x
                                  : j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            j->ret.x + deslocamentoX,
            j->ret.y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height};

        Item *item = (Item *)el->objeto;

        if (item->tipo == TIPO_ITEM_ANEL)
        {

            ItemAnel *itemAnel = (ItemAnel *)item->objeto;

            if (!itemAnel->ativo || itemAnel->estado == ESTADO_ITEM_ANEL_COLETADO)
            {
                el = el->proximo;
                continue;
            }

            QuadroAnimacao *qaItem = getQuadroAnimacaoAtualItemAnel(itemAnel);

            Rectangle retColItemCalculado = {
                itemAnel->ret.x + qaItem->retColisao.x,
                itemAnel->ret.y + qaItem->retColisao.y,
                qaItem->retColisao.width,
                qaItem->retColisao.height};

            if (CheckCollisionRecs(retColCalculado, retColItemCalculado))
            {
                itemAnel->estado = ESTADO_ITEM_ANEL_COLETADO;
                j->quantidadeAneis++;
                gw->pontuacao += 100; /* +100 pontos por anel coletado */
                PlaySound(rm.somAnel);
            }
        }

        if (item->tipo == TIPO_ITEM_ESCUDO)
        {
            ItemEscudo *itemEscudo = (ItemEscudo *)item->objeto;

            if (!itemEscudo->ativo ||
                itemEscudo->estado == ESTADO_ITEM_ESCUDO_COLETADO)
            {
                el = el->proximo;
                continue;
            }

            QuadroAnimacao *qaItem =
                getQuadroAnimacaoAtualItemEscudo(itemEscudo);

            Rectangle retColItemCalculado = {
                itemEscudo->ret.x + qaItem->retColisao.x,
                itemEscudo->ret.y + qaItem->retColisao.y,
                qaItem->retColisao.width,
                qaItem->retColisao.height};

            if (CheckCollisionRecs(retColCalculado, retColItemCalculado))
            {
                itemEscudo->estado = ESTADO_ITEM_ESCUDO_COLETADO;

                j->possuiEscudo = true;

                gw->pontuacao += 500;

                // PlaySound(rm.somEscudo);

                // PlaySound(rm.somAnel);
            }
        }

        if (item->tipo == TIPO_ITEM_ANEL_GIGANTE)
        {
            ItemAnelGigante *anelG = (ItemAnelGigante *)item->objeto;

            if (!anelG->ativo || anelG->estado == ESTADO_ITEM_ANEL_GIGANTE_COLETADO)
            {
                el = el->proximo;
                continue;
            }

            Rectangle retItem = anelG->ret;

            if (CheckCollisionRecs(retColCalculado, retItem))
            {
                anelG->estado = ESTADO_ITEM_ANEL_GIGANTE_COLETADO;

                j->quantidadeAneis += 10;
                gw->pontuacao += 1000;

                PlaySound(rm.somAnel);
            }
        }

        if (item->tipo == TIPO_ITEM_BLOCO_INTERROGACAO)
        {

            BlocoInterrogacao *bloco = (BlocoInterrogacao *)item->objeto;

            if (bloco->estado != ESTADO_BLOCO_INT_INTACTO)
            {
                el = el->proximo;
                continue;
            }

            /* retângulo de colisão do bloco (com saltY aplicado) */
            Rectangle retBloco = {
                bloco->ret.x,
                bloco->ret.y + bloco->saltY,
                bloco->ret.width,
                bloco->ret.height};

            if (CheckCollisionRecs(retColCalculado, retBloco))
            {
                /* só ativa se o jogador está subindo e bate na parte de baixo */
                float centroJogadorY = retColCalculado.y + retColCalculado.height / 2.0f;
                float centroBlocoY = retBloco.y + retBloco.height / 2.0f;

                if (j->vel.y < 0 && centroJogadorY > centroBlocoY)
                {
                    ativarBlocoInterrogacao(bloco);

                    /* solta os anéis: cria novos itens no mundo */
                    j->quantidadeAneis += bloco->aneisParaSoltar;
                    gw->pontuacao += bloco->aneisParaSoltar * 100;
                    PlaySound(rm.somAnel);

                    /* empurra o jogador para baixo */
                    j->vel.y = 100.0f;
                    j->ret.y = retBloco.y + retBloco.height - deslocamentoY;
                }
            }
        }

        el = el->proximo;
    }
}

static void resolverColisaoJogadorInimigosMapa(Jogador *j, Mapa *mapa, GameWorld *gw)
{

    ElementoMapa *el = mapa->inimigos;

    while (el != NULL)
    {

        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador(j);

        float deslocamentoX = j->olhandoParaDireita
                                  ? qa->retColisao.x
                                  : j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            j->ret.x + deslocamentoX,
            j->ret.y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height};

        Inimigo *inimigo = (Inimigo *)el->objeto;

        QuadroAnimacao *qaInimigo = NULL;
        bool *olhandoParaDireita = NULL;
        Rectangle *ret = NULL;

        if (inimigo->tipo == TIPO_INIMIGO_MOTOBUG)
        {

            InimigoMotobug *motobug = (InimigoMotobug *)inimigo->objeto;

            if (!motobug->ativo || motobug->estado == ESTADO_INIMIGO_MOTOBUG_MORRENDO)
            {
                el = el->proximo;
                continue;
            }

            qaInimigo = getQuadroAnimacaoAtualInimigoMotobug(motobug);
            olhandoParaDireita = &motobug->olhandoParaDireita;
            ret = &motobug->ret;

            float deslocXIni = *olhandoParaDireita
                                   ? ret->width - qaInimigo->retColisao.x - qaInimigo->retColisao.width
                                   : qaInimigo->retColisao.x;
            float deslocYIni = qaInimigo->retColisao.y;

            Rectangle retColInimigoCalculado = {
                ret->x + deslocXIni,
                ret->y + deslocYIni,
                qaInimigo->retColisao.width,
                qaInimigo->retColisao.height};

            if (CheckCollisionRecs(retColCalculado, retColInimigoCalculado))
            {
                if (j->estado >= ESTADO_JOGADOR_PULANDO && j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO)
                {
                    j->vel.y = j->velPulo;
                    motobug->estado = ESTADO_INIMIGO_MOTOBUG_MORRENDO;
                    gw->pontuacao += 100;
                    PlaySound(rm.somHitInimigo);
                }
                else if (!j->invulneravel)
                {
                    if (j->possuiEscudo)
                    {

                        j->possuiEscudo = false;

                        j->animacaoEscudo.quadroAtual = 0;

                        PlaySound(rm.somHitInimigo);
                    }
                    else
                    {
                        if (j->quantidadeAneis > 0)
                        {
                            j->quantidadeAneis = 0;
                            PlaySound(rm.somHitComAnel);
                        }
                        else
                        {
                            j->quantidadeVidas--;
                            PlaySound(rm.somMorte);
                        }
                    }

                    j->invulneravel = true;
                }
                return;
            }
        }
        else if (inimigo->tipo == TIPO_INIMIGO_SPIKES)
        {

            InimigoSpikes *spikes = (InimigoSpikes *)inimigo->objeto;

            if (!spikes->ativo || spikes->estado == ESTADO_INIMIGO_SPIKES_MORRENDO)
            {
                el = el->proximo;
                continue;
            }

            qaInimigo = getQuadroAnimacaoAtualInimigoSpikes(spikes);
            olhandoParaDireita = &spikes->olhandoParaDireita;
            ret = &spikes->ret;

            float deslocXIni = *olhandoParaDireita
                                   ? ret->width - qaInimigo->retColisao.x - qaInimigo->retColisao.width
                                   : qaInimigo->retColisao.x;
            float deslocYIni = qaInimigo->retColisao.y;

            Rectangle retColInimigoCalculado = {
                ret->x + deslocXIni,
                ret->y + deslocYIni,
                qaInimigo->retColisao.width,
                qaInimigo->retColisao.height};

            if (CheckCollisionRecs(retColCalculado, retColInimigoCalculado))
            {
                if (j->estado >= ESTADO_JOGADOR_PULANDO && j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO)
                {
                    j->vel.y = j->velPulo;
                    spikes->estado = ESTADO_INIMIGO_SPIKES_MORRENDO;
                    gw->pontuacao += 100;
                    PlaySound(rm.somHitInimigo);
                }
                else if (!j->invulneravel)
                {
                    if (j->possuiEscudo)
                    {

                        j->possuiEscudo = false;

                        j->animacaoEscudo.quadroAtual = 0;

                        PlaySound(rm.somHitInimigo);
                    }
                    else
                    {
                        if (j->quantidadeAneis > 0)
                        {
                            j->quantidadeAneis = 0;
                            PlaySound(rm.somHitComAnel);
                        }
                        else
                        {
                            j->quantidadeVidas--;
                            PlaySound(rm.somMorte);
                        }
                    }

                    j->invulneravel = true;
                }
                return;
            }
        }
        else if (inimigo->tipo == TIPO_INIMIGO_BALLHOG)
        {

            InimigoBallHog *ballhog = (InimigoBallHog *)inimigo->objeto;

            if (!ballhog->ativo || ballhog->estado == ESTADO_INIMIGO_BALLHOG_MORRENDO)
            {
                el = el->proximo;
                continue;
            }

            qaInimigo = getQuadroAnimacaoAtualInimigoBallHog(ballhog);
            olhandoParaDireita = &ballhog->olhandoParaDireita;
            ret = &ballhog->ret;

            float deslocXIni = *olhandoParaDireita
                                   ? ret->width - qaInimigo->retColisao.x - qaInimigo->retColisao.width
                                   : qaInimigo->retColisao.x;
            float deslocYIni = qaInimigo->retColisao.y;

            Rectangle retColInimigoCalculado = {
                ret->x + deslocXIni,
                ret->y + deslocYIni,
                qaInimigo->retColisao.width,
                qaInimigo->retColisao.height};

            if (CheckCollisionRecs(retColCalculado, retColInimigoCalculado))
            {
                if (j->estado >= ESTADO_JOGADOR_PULANDO && j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO)
                {
                    j->vel.y = j->velPulo;
                    ballhog->estado = ESTADO_INIMIGO_BALLHOG_MORRENDO;
                    gw->pontuacao += 100;
                    PlaySound(rm.somHitInimigo);
                }
                else if (!j->invulneravel)
                {
                    if (j->possuiEscudo)
                    {

                        j->possuiEscudo = false;

                        j->animacaoEscudo.quadroAtual = 0;

                        PlaySound(rm.somHitInimigo);
                    }
                    else
                    {
                        if (j->quantidadeAneis > 0)
                        {
                            j->quantidadeAneis = 0;
                            PlaySound(rm.somHitComAnel);
                        }
                        else
                        {
                            j->quantidadeVidas--;
                            PlaySound(rm.somMorte);
                        }
                    }

                    j->invulneravel = true;
                }
                return;
            }
        }
        else if (inimigo->tipo == TIPO_INIMIGO_BATBRAIN)
        {

            InimigoBatbrain *batbrain = (InimigoBatbrain *)inimigo->objeto;

            if (!batbrain->ativo || batbrain->estado == ESTADO_INIMIGO_BATBRAIN_MORRENDO)
            {
                el = el->proximo;
                continue;
            }

            qaInimigo = getQuadroAnimacaoAtualInimigoBatbrain(batbrain);
            olhandoParaDireita = &batbrain->olhandoParaDireita;
            ret = &batbrain->ret;

            float deslocXIni = *olhandoParaDireita
                                   ? ret->width - qaInimigo->retColisao.x - qaInimigo->retColisao.width
                                   : qaInimigo->retColisao.x;
            float deslocYIni = qaInimigo->retColisao.y;

            Rectangle retColInimigoCalculado = {
                ret->x + deslocXIni,
                ret->y + deslocYIni,
                qaInimigo->retColisao.width,
                qaInimigo->retColisao.height};

            if (CheckCollisionRecs(retColCalculado, retColInimigoCalculado))
            {

                if (j->estado >= ESTADO_JOGADOR_PULANDO &&
                    j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO)
                {

                    j->vel.y = j->velPulo;
                    batbrain->estado = ESTADO_INIMIGO_BATBRAIN_MORRENDO;
                    gw->pontuacao += 100;
                    PlaySound(rm.somHitInimigo);
                }
                else if (!j->invulneravel)
                {
                    if (j->possuiEscudo)
                    {

                        j->possuiEscudo = false;

                        j->animacaoEscudo.quadroAtual = 0;

                        PlaySound(rm.somHitInimigo);
                    }
                    else
                    {
                        if (j->quantidadeAneis > 0)
                        {
                            j->quantidadeAneis = 0;
                            PlaySound(rm.somHitComAnel);
                        }
                        else
                        {
                            j->quantidadeVidas--;
                            PlaySound(rm.somMorte);
                        }
                    }

                    j->invulneravel = true;
                }

                return;
            }
        }
        else if (inimigo->tipo == TIPO_INIMIGO_EGGMOBILE)
        {
            InimigoEggMobile *eggmobile =
                (InimigoEggMobile *)inimigo->objeto;

            if (!eggmobile->ativo ||
                eggmobile->estado == ESTADO_INIMIGO_EGGMOBILE_DERROTADO)
            {
                el = el->proximo;
                continue;
            }

            qaInimigo =
                getQuadroAnimacaoAtualInimigoEggMobile(eggmobile);

            olhandoParaDireita =
                &eggmobile->olhandoParaDireita;

            ret = &eggmobile->ret;

            float deslocXIni = *olhandoParaDireita
                                   ? ret->width -
                                         qaInimigo->retColisao.x -
                                         qaInimigo->retColisao.width
                                   : qaInimigo->retColisao.x;

            float deslocYIni = qaInimigo->retColisao.y;

            Rectangle retColInimigoCalculado = {
                ret->x + deslocXIni,
                ret->y + deslocYIni,
                qaInimigo->retColisao.width,
                qaInimigo->retColisao.height};

            if (CheckCollisionRecs(
                    retColCalculado,
                    retColInimigoCalculado))
            {

                bool pulando =
                    (j->estado >= ESTADO_JOGADOR_PULANDO &&
                     j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO);

                bool acertouPorBaixo =
                    pulando &&
                    j->vel.y < 0 &&
                    CheckCollisionRecs(
                        retColCalculado,
                        eggmobile->retParteInferior);

                if (pulando && !acertouPorBaixo)
                {
                    j->vel.y = -250;

                    if (j->ret.x < eggmobile->ret.x)
                    {
                        j->vel.x = -200;
                    }
                    else
                    {
                        j->vel.x = 200;
                    }

                    if (!eggmobile->invulneravel)
                    {
                        eggmobile->vida--;

                        eggmobile->invulneravel = true;
                        eggmobile->contadorInvulnerabilidade = 0;

                        eggmobile->estado =
                            ESTADO_INIMIGO_EGGMOBILE_DANO;

                        PlaySound(rm.somHitInimigo);

                        if (eggmobile->vida <= 0)
                        {
                            eggmobile->estado =
                                ESTADO_INIMIGO_EGGMOBILE_DERROTADO;

                            gw->pontuacao += 5000;
                        }
                    }
                }
                else if (!j->invulneravel)
                {
                    if (acertouPorBaixo)
                    {
                        j->vel.y = 200;
                    }

                    if (j->possuiEscudo)
                    {
                        j->possuiEscudo = false;
                        j->animacaoEscudo.quadroAtual = 0;

                        PlaySound(rm.somHitInimigo);
                    }
                    else
                    {
                        if (j->quantidadeAneis > 0)
                        {
                            j->quantidadeAneis = 0;
                            PlaySound(rm.somHitComAnel);
                        }
                        else
                        {
                            j->quantidadeVidas--;
                            PlaySound(rm.somMorte);
                        }
                    }

                    j->invulneravel = true;
                }

                return;
            }
        }

        el = el->proximo;
    }
}