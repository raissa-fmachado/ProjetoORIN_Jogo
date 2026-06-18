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
    ESTADO_JOGADOR_SPIN,         /* Rolamento (movimento de spin) */
    ESTADO_JOGADOR_SPIN_PULANDO, /* Pulo duplo enquanto está em spin */
} EstadoJogador;

/**
 * @brief Representa o tipo de escudo que o jogador possui.
 */
typedef enum TipoEscudo
{
    TIPO_ESCUDO_NENHUM,
    TIPO_ESCUDO_AGUA, /* Escudo de água - pulo duplo com proteção contra fogo */
    TIPO_ESCUDO_FOGO, /* Escudo de fogo - pulo duplo com proteção contra gelo */
    TIPO_ESCUDO_RAIO, /* Escudo de raio - pulo duplo com aceleração */
} TipoEscudo;

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

typedef enum EstadoInimigoEggMobile
{
    ESTADO_INIMIGO_EGGMOBILE_VOANDO,
    ESTADO_INIMIGO_EGGMOBILE_DANO,
    ESTADO_INIMIGO_EGGMOBILE_DERROTADO
} EstadoInimigoEggMobile;

/**
 * @brief Representa o tipo de um inimigo.
 */
typedef enum TipoInimigo
{
    TIPO_INIMIGO_MOTOBUG,
    TIPO_INIMIGO_SPIKES,
    TIPO_INIMIGO_BALLHOG,
    TIPO_INIMIGO_BATBRAIN,
    TIPO_INIMIGO_EGGMOBILE,
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
    TIPO_ITEM_ESTRELINHA,  /* Invulnerabilidade */
    TIPO_ITEM_BOTINHA,     /* Velocidade */
    TIPO_ITEM_ESCUDO_AGUA, /* Escudo especial água */
    TIPO_ITEM_ESCUDO_FOGO, /* Escudo especial fogo */
    TIPO_ITEM_ESCUDO_RAIO, /* Escudo especial raio */
} TipoItem;

/**
 * @brief Representa o tipo de um elemento do mapa
 */
typedef enum TipoElementoMapa
{
    TIPO_ELEMENTO_MAPA_OBSTACULO,
    TIPO_ELEMENTO_MAPA_ITEM,
    TIPO_ELEMENTO_MAPA_INIMIGO,
    TIPO_ELEMENTO_MAPA_DECORACAO,
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

typedef struct TelaInicial
{
    Rectangle ret;
    bool ativo;

    Animacao *animacoes[1];
    int quantidadeAnimacoes;

    Animacao animacaoSonic;

} TelaInicial;

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
    TipoEscudo tipoEscudo; /* Tipo de escudo (água, fogo, raio) */

    /* Sistema de Spin */
    bool emSpin;             /* True quando o jogador está em movimento de spin */
    float contadorTempoSpin; /* Contador de tempo do spin */
    float tempoMaxSpin;      /* Tempo máximo de duração do spin */
    float velSpinHorizontal; /* Velocidade horizontal durante o spin */

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
    Animacao animacaoSpin; /* Animação de spin/rolamento */

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

typedef struct InimigoEggMobile
{
    Rectangle ret;
    Vector2 vel;

    Color cor;

    int vida;

    bool ativo;
    bool olhandoParaDireita;
    bool invulneravel;

    float contadorEstado;
    float contadorInvulnerabilidade;

    EstadoInimigoEggMobile estado;

    Animacao *animacoes[3];
    int quantidadeAnimacoes;

    Animacao animacaoVoando;
    Animacao animacaoDano;
    Animacao animacaoDerrotado;

    Rectangle retParteInferior;

    bool arenaAtivada;
    float limiteEsquerdoArena;
    float limiteDireitoArena;

    bool batalhaIniciada;

} InimigoEggMobile;

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
 * @brief Bloco de interrogação - bata por baixo para soltar itens.
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

    /* Sistema de geração de itens */
    bool itemGerado;    /* Flag para gerar item apenas uma vez */
    int tipoItemGerado; /* Tipo de item que foi gerado */

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

typedef enum TipoDecoracao
{
    TIPO_DECORACAO_FLOR,
    TIPO_DECORACAO_PALMEIRA,
    TIPO_DECORACAO_ARBUSTO,
} TipoDecoracao;

typedef struct Decoracao
{
    Rectangle ret;
    Rectangle fonte;

    Texture2D *textura;

    TipoDecoracao tipo;

    Animacao *animacoes[4];
    int quantidadeAnimacoes;

} Decoracao;

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

    ElementoMapa *decoracoes;
    int quantidadeDecoracoes;

    float dimensaoPadraoElementos;
    int linhas;
    int colunas;

} Mapa;

/**
 * @brief Estado global da "tela" do jogo.
 */
typedef enum EstadoTela
{
    TELA_INICIAL,
    TELA_JOGANDO,   /* jogo normal                               */
    TELA_CARD_FASE, /* card estilo Sonic "MARBLE ZONE / ACT 1"   */
    TELA_VITORIA,   /* tela de vitória após matar todos inimigos */
    TELA_CONTINUE,  /* tela de continue ao perder todas as vidas */
    TELA_GAME_OVER, /* tela de game over ao perder todas as vidas*/
} EstadoTela;

/**
 * @brief Representa o mundo do jogo e seus elementos.
 */
typedef struct GameWorld
{
    Mapa *mapa;
    Jogador *jogador;
    TelaInicial *telaInicial;

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

    /* ── Tela Inicial ── */
    float telaInicialContador;
    bool iniciarJogo;

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

    /* ── Tela de Continue ── */
    float continueContador; /* tempo na tela de continue                 */
    float continueDuracao;  /* duração do timeout da tela de continue    */
    int continueOpcao;      /* 0 = continue, 1 = sair                    */

    /* ── Transição (fade) ── */
    float fadeDuracao;  /* duração total de cada fade (seg)          */
    float fadeContador; /* contador de tempo do fade atual           */
    float fadeAlpha;    /* 0.0 = transparente, 1.0 = preto total     */
    bool fadeEntrada;   /* true = fade-in (preto→jogo)               */
    bool fadeSaida;     /* true = fade-out (jogo→preto)              */
    bool trocandoFase;  /* flag interna: fade-out concluído, trocar  */

    bool arenaBossAtivada;
    Obstaculo *paredeArena;
    float limiteEsquerdoArena;
    float limiteDireitoArena;

} GameWorld;