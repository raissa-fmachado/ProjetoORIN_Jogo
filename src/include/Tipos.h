/**
 * @file Tipos.h
 * @author Prof. Dr. David Buzatto
 * @brief Definição dos tipos utilizados no jogo.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include <stdbool.h>

#include "raylib/raylib.h"

/**
 * @brief Representa o estado do jogador.
 */
typedef enum EstadoJogador
{
    ESTADO_JOGADOR_PARADO,
    ESTADO_JOGADOR_ANDANDO,
    ESTADO_JOGADOR_ANDANDO_RAPIDO,
    ESTADO_JOGADOR_CORRENDO,
    ESTADO_JOGADOR_PULANDO,
    ESTADO_JOGADOR_PULANDO_RAPIDO,
    ESTADO_JOGADOR_PULANDO_CORRENDO,
} EstadoJogador;

/**
 * @brief Representa o estado do inimigo do tipo Motobug.
 */
typedef enum EstadoInimigoMotobug
{
    ESTADO_INIMIGO_MOTOBUG_ANDANDO,
    ESTADO_INIMIGO_MOTOBUG_MORRENDO,
} EstadoInimigoMotobug;

/**
 * @brief Representa o estado do inimigo do tipo Spikes.
 */
typedef enum EstadoInimigoSpikes
{
    ESTADO_INIMIGO_SPIKES_ANDANDO,
    ESTADO_INIMIGO_SPIKES_MORRENDO,
} EstadoInimigoSpikes;

/**
 * @brief Representa o estado do inimigo do tipo BallHog.
 */
typedef enum EstadoInimigoBallHog
{
    ESTADO_INIMIGO_BALLHOG_ANDANDO,
    ESTADO_INIMIGO_BALLHOG_MORRENDO,
} EstadoInimigoBallHog;

/**
 * @brief Representa o estado do inimigo do tipo Batbrain.
 */
typedef enum EstadoInimigoBatbrain
{
    ESTADO_INIMIGO_BATBRAIN_PARADO,
    ESTADO_INIMIGO_BATBRAIN_MERGULHANDO,
    ESTADO_INIMIGO_BATBRAIN_SUBINDO,
    ESTADO_INIMIGO_BATBRAIN_MORRENDO,
} EstadoInimigoBatbrain;

/**
 * @brief Representa o tipo de um inimigo.
 */
typedef enum TipoInimigo
{
    TIPO_INIMIGO_MOTOBUG,
    TIPO_INIMIGO_SPIKES,
    TIPO_INIMIGO_BALLHOG,
    TIPO_INIMIGO_BATBRAIN,
} TipoInimigo;

/**
 * @brief Representa o estado do item do tipo anel.
 */
typedef enum EstadoItemAnel
{
    ESTADO_ITEM_ANEL_PARADO,
    ESTADO_ITEM_ANEL_COLETADO,
} EstadoItemAnel;

/**
 * @brief Representa o estado do item do tipo Escudo
 * .
 */
typedef enum EstadoItemEscudo

{
    ESTADO_ITEM_ESCUDO_PARADO,
    ESTADO_ITEM_ESCUDO_COLETADO,
} EstadoItemEscudo;

typedef enum EstadoItemAnelGigante
{
    ESTADO_ITEM_ANEL_GIGANTE_PARADO,
    ESTADO_ITEM_ANEL_GIGANTE_COLETADO,
} EstadoItemAnelGigante;

/**
 * @brief Estado do bloco de interrogação.
 */
typedef enum EstadoBlocoInterrogacao
{
    ESTADO_BLOCO_INT_INTACTO,
    ESTADO_BLOCO_INT_ATIVADO,
    ESTADO_BLOCO_INT_ABERTO,
} EstadoBlocoInterrogacao;

/**
 * @brief Representa o tipo de um item.
 */
typedef enum TipoItem
{
    TIPO_ITEM_ANEL,
    TIPO_ITEM_BLOCO_INTERROGACAO,
    TIPO_ITEM_ESCUDO,
    TIPO_ITEM_ANEL_GIGANTE,
} TipoItem;

/**
 * @brief Representa o tipo de um elemento do mapa
 */
typedef enum TipoElementoMapa
{
    TIPO_ELEMENTO_MAPA_OBSTACULO,
    TIPO_ELEMENTO_MAPA_ITEM,
    TIPO_ELEMENTO_MAPA_INIMIGO,
} TipoElementoMapa;

/**
 * @brief Representa um quadro de animação com imagem.
 */
typedef struct QuadroAnimacao
{
    Rectangle fonte;
    int duracao;          // milisegundos
    Rectangle retColisao; // deve ser interpretado como posicionamento relativo
} QuadroAnimacao;

/**
 * @brief Representa uma animação.
 */
typedef struct Animacao
{
    QuadroAnimacao *quadros;
    int quantidadeQuadros;
    int quadroAtual;
    int contadorTempoQuadro;
    bool pararNoUltimoQuadro;
    bool executarUmaVez;
    bool finalizada;
} Animacao;

/**
 * @brief Representa o jogador controlado pelo usuário.
 */
typedef struct Jogador
{

    Rectangle ret;
    Vector2 vel;
    Color cor;

    float velAndando;
    float velAndandoRapido;
    float velCorrendo;
    float velPulo;
    float velMaxQueda;

    float aceleracao;
    float desaceleracao;
    float frenagem;

    int quantidadePulos;
    int quantidadeMaxPulos;

    int quantidadeAneis;
    int quantidadeVidas;

    bool invulneravel;
    float tempoInvulnerabilidade;
    float contadorTempoInvulnerabilidade;

    bool piscaPisca;
    float tempoPiscaPisca;
    float contadorTempoPiscaPisca;

    bool possuiEscudo;

    bool freando;

    EstadoJogador estado;
    bool olhandoParaDireita;

    Animacao *animacoes[21];
    int quantidadeAnimacoes;

    Animacao animacaoParado;
    Animacao animacaoAndando;
    Animacao animacaoAndandoRapido;
    Animacao animacaoCorrendo;
    Animacao animacaoPulando;
    Animacao animacaoPulandoRapido;
    Animacao animacaoPulandoCorrendo;
    Animacao animacaoEscudo;

} Jogador;

/**
 * @brief Representa um inimigo do tipo Motobug.
 */
typedef struct InimigoMotobug
{

    Rectangle ret;
    Vector2 vel;
    Color cor;

    float velAndando;
    float velMaxQueda;

    EstadoInimigoMotobug estado;
    bool ativo;
    bool olhandoParaDireita;
    Animacao *animacoes[2];
    int quantidadeAnimacoes;

    Animacao animacaoAndando;
    Animacao animacaoMorrendo;

} InimigoMotobug;

/**
 * @brief Representa um inimigo do tipo Spikes.
 */
typedef struct InimigoSpikes
{

    Rectangle ret;
    Vector2 vel;
    Color cor;

    float velAndando;
    float velMaxQueda;

    EstadoInimigoSpikes estado;
    bool ativo;
    bool olhandoParaDireita;
    Animacao *animacoes[2];
    int quantidadeAnimacoes;

    Animacao animacaoAndando;
    Animacao animacaoMorrendo;

} InimigoSpikes;

/**
 * @brief Representa um inimigo do tipo BallHog.
 */
typedef struct InimigoBallHog
{

    Rectangle ret;
    Vector2 vel;
    Color cor;

    float velAndando;
    float velMaxQueda;

    EstadoInimigoBallHog estado;
    bool ativo;
    bool olhandoParaDireita;
    Animacao *animacoes[2];
    int quantidadeAnimacoes;

    Animacao animacaoAndando;
    Animacao animacaoMorrendo;

    // bola preta lançada ao morrer
    bool bolaAtiva;
    Rectangle bolaRet;
    Vector2 bolaVel;
    float contadorTempoBola;
    float tempoBola;

} InimigoBallHog;

/**
 * @brief Representa um inimigo do tipo Batbrain.
 */
typedef struct InimigoBatbrain
{
    Rectangle ret;
    Vector2 vel;

    Vector2 posInicial;

    Color cor;

    float velAndando;
    float velMaxQueda;

    bool mergulhando;
    float velocidadeMergulho;
    float distanciaAtivacao;
    float alturaRetorno;

    bool ativo;
    bool olhandoParaDireita;

    EstadoInimigoBatbrain estado;

    Animacao *animacoes[4];
    int quantidadeAnimacoes;

    Animacao animacaoParado;
    Animacao animacaoAndando;
    Animacao animacaoMorrendo;

} InimigoBatbrain;

/**
 * @brief Representa um inimigo.
 * O inimigo de fato é endereçado via membro "objeto".
 */
typedef struct Inimigo
{
    void *objeto;
    TipoInimigo tipo;
} Inimigo;

/**
 * @brief Representa um item do tipo anel.
 */
typedef struct ItemAnel
{

    Rectangle ret;
    Color cor;

    EstadoItemAnel estado;
    bool ativo;

    Animacao *animacoes[2];
    int quantidadeAnimacoes;

    Animacao animacaoParado;
    Animacao animacaoColetando;

} ItemAnel;

typedef struct ItemAnelGigante
{
    Rectangle ret;
    Color cor;

    EstadoItemAnelGigante estado;
    bool ativo;

    Animacao *animacoes[1];
    int quantidadeAnimacoes;

    Animacao animacaoParado;
    Animacao animacaoColetando;
} ItemAnelGigante;

/**
 * @brief Representa um item do tipo escudo.
 */
typedef struct ItemEscudo

{
    Rectangle ret;
    Color cor;

    EstadoItemEscudo
        estado;
    bool ativo;

    Animacao *animacoes[2];
    int quantidadeAnimacoes;

    Animacao animacaoParado;
    Animacao animacaoColetando;

} ItemEscudo;

/**
 * @brief Bloco de interrogação - bata por baixo para soltar anéis.
 */
typedef struct BlocoInterrogacao
{
    Rectangle ret;
    EstadoBlocoInterrogacao estado;
    bool ativo;

    float animTimer;
    float pulsTimer;
    float saltY;
    bool saltSubindo;
    int aneisParaSoltar;

} BlocoInterrogacao;

/**
 * @brief Representa um item estático do mapa.
 * O item de fato é endereçado via membro "objeto".
 */
typedef struct Item
{
    void *objeto;
    TipoItem tipo;
} Item;

/**
 * @brief Representa um obstáculo estático do mapa.
 */
typedef struct Obstaculo
{
    Rectangle ret;
    Color cor;
    Rectangle fonte;
    Texture2D *textura;
} Obstaculo;

/**
 * @brief Representa um elemento do mapa.
 * O elemento de fato é endereçado via membro "objeto".
 */
typedef struct ElementoMapa ElementoMapa;
struct ElementoMapa
{
    void *objeto;
    TipoElementoMapa tipo;
    ElementoMapa *proximo;
};

/**
 * @brief Representa um mapa de fase do jogo.
 */
typedef struct Mapa
{
    // listas ligadas de elementos do mapa
    ElementoMapa *obstaculos;
    int quantidadeObstaculos;

    ElementoMapa *itens;
    int quantidadeItens;

    ElementoMapa *inimigos;
    int quantidadeInimigos;

    float dimensaoPadraoElementos;
    int linhas;
    int colunas;

} Mapa;

/**
 * @brief Estado global da "tela" do jogo.
 */
typedef enum EstadoTela
{
    TELA_JOGANDO,   /* jogo normal                               */
    TELA_CARD_FASE, /* card estilo Sonic "MARBLE ZONE / ACT 1"   */
    TELA_VITORIA,   /* tela de vitória após matar todos inimigos */
    TELA_GAME_OVER, /* tela de game over ao perder todas as vidas*/
} EstadoTela;

/**
 * @brief Representa o mundo do jogo e seus elementos.
 */
typedef struct GameWorld
{
    Mapa *mapa;
    Jogador *jogador;

    Camera2D camera;

    float gravidade;

    /* ── HUD ── */
    int pontuacao;
    float tempoJogo;

    /* ── Fases ── */
    int faseAtual;     /* 1 ou 2                                    */
    bool faseCompleta; /* true quando o jogador chega ao fim        */

    /* ── Estado de tela ── */
    EstadoTela estadoTela;

    /* ── Card de fase ── */
    float cardContador; /* tempo exibindo o card (seg)               */
    float cardDuracao;  /* duração total do card                     */
    float cardSlide;    /* posição Y do slide (0=dentro, <0=saindo)  */

    /* ── Tela de vitória ── */
    float vitoriaContador;  /* tempo na tela de vitória                  */
    int bonusTempo;         /* bônus calculado pelo tempo restante       */
    int bonusAnel;          /* bônus calculado pelos anéis coletados     */
    int pontuacaoFinal;     /* pontuação total + bônus                   */
    float vitoriaAnimTimer; /* timer para animar a contagem de bônus     */
    int bonusTempoExibido;  /* valor animado do bônus de tempo           */
    int bonusAnelExibido;   /* valor animado do bônus de anel            */
    bool bonusContando;     /* true enquanto a contagem está em curso    */

    /* ── Tela de Game Over ── */
    float gameOverContador; /* tempo na tela de game over                */

    /* ── Transição (fade) ── */
    float fadeDuracao;  /* duração total de cada fade (seg)          */
    float fadeContador; /* contador de tempo do fade atual           */
    float fadeAlpha;    /* 0.0 = transparente, 1.0 = preto total     */
    bool fadeEntrada;   /* true = fade-in (preto→jogo)               */
    bool fadeSaida;     /* true = fade-out (jogo→preto)              */
    bool trocandoFase;  /* flag interna: fade-out concluído, trocar  */

} GameWorld;