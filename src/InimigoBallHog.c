/**
 * @file InimigoBallHog.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Inimigo (BallHog).
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "Inimigo.h"
#include "InimigoBallHog.h"
#include "Macros.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void desenharQuadroAnimacaoInimigoBallHog( InimigoBallHog *inimigo, QuadroAnimacao *qa, Color tonalidade );
static void desenharQuadroAnimacaoInimigoBallHogMorrendo( InimigoBallHog *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade );
static void desenharBolaBallHog( InimigoBallHog *inimigo );
static Animacao *getAnimacaoAtualInimigoBallHog( InimigoBallHog *inimigo );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria um novo Inimigo (ballhog).
 */
InimigoBallHog *criarInimigoBallHog( Rectangle ret, Color cor ) {

    InimigoBallHog *novoInimigo = (InimigoBallHog*) malloc( sizeof( InimigoBallHog ) );

    novoInimigo->ret = ret;
    novoInimigo->vel = (Vector2) { 0 };
    novoInimigo->cor = cor;

    novoInimigo->velAndando = 80;
    novoInimigo->velMaxQueda = 600;

    novoInimigo->estado = ESTADO_INIMIGO_BALLHOG_ANDANDO;
    novoInimigo->ativo = true;
    novoInimigo->olhandoParaDireita = false;

    // bola preta: começa inativa
    novoInimigo->bolaAtiva = false;
    novoInimigo->bolaVel = (Vector2) { 0 };
    novoInimigo->bolaRet = (Rectangle) { 0 };
    novoInimigo->contadorTempoBola = 0.0f;
    novoInimigo->tempoBola = 0.3f;  // animação da bola pisca a cada 0.3s

    int quantidadeAnimacoes = 0;

    // Animação andando: 4 frames, x=1, y=72, w=24, h=40, sep=1
    novoInimigo->animacaoAndando.quantidadeQuadros = 4;
    novoInimigo->animacaoAndando.quadroAtual = 0;
    novoInimigo->animacaoAndando.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoAndando.pararNoUltimoQuadro = false;
    novoInimigo->animacaoAndando.executarUmaVez = false;
    novoInimigo->animacaoAndando.finalizada = false;
    criarQuadrosAnimacao( &novoInimigo->animacaoAndando, novoInimigo->animacaoAndando.quantidadeQuadros );
    inicializarQuadrosAnimacao(
        novoInimigo->animacaoAndando.quadros,
        novoInimigo->animacaoAndando.quantidadeQuadros,
        200,             // duração padrão para todos os quadros
        1, 72,           // início: x=1, y=72
        24, 40,          // dimensões reais no spritesheet
        1,               // separação entre frames
        false,           // de trás para frente
        (Rectangle) {    // retColisao relativo ao ret (48x80)
            4, 4, 40, 72
        }
    );

    // Animação morrendo: animal saindo — mesma do Motobug/Spikes
    // x=169, y=1, w=32, h=32, 4 frames, sep=1
    novoInimigo->animacaoMorrendo.quantidadeQuadros = 4;
    novoInimigo->animacaoMorrendo.quadroAtual = 0;
    novoInimigo->animacaoMorrendo.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoMorrendo.pararNoUltimoQuadro = false;
    novoInimigo->animacaoMorrendo.executarUmaVez = true;
    novoInimigo->animacaoMorrendo.finalizada = false;
    criarQuadrosAnimacao( &novoInimigo->animacaoMorrendo, novoInimigo->animacaoMorrendo.quantidadeQuadros );
    inicializarQuadrosAnimacao(
        novoInimigo->animacaoMorrendo.quadros,
        novoInimigo->animacaoMorrendo.quantidadeQuadros,
        100,
        169, 1,           // início: x=169, y=1
        32, 32,           // dimensões
        1,
        false,
        (Rectangle) { 0 }
    );

    novoInimigo->animacoes[ESTADO_INIMIGO_BALLHOG_ANDANDO]  = &novoInimigo->animacaoAndando;  quantidadeAnimacoes++;
    novoInimigo->animacoes[ESTADO_INIMIGO_BALLHOG_MORRENDO] = &novoInimigo->animacaoMorrendo; quantidadeAnimacoes++;
    novoInimigo->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoInimigo;

}

/**
 * @brief Destroi um inimigo (ballhog).
 */
void destruirInimigoBallHog( InimigoBallHog *inimigo ) {
    if ( inimigo != NULL ) {
        for ( int i = 0; i < inimigo->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( inimigo->animacoes[i] );
        }
        free( inimigo );
    }
}

/**
 * @brief Atualiza um inimigo (ballhog).
 */
void atualizarInimigoBallHog( InimigoBallHog *inimigo, GameWorld *gw, float delta ) {

    if ( inimigo->ativo ) {

        if ( inimigo->estado == ESTADO_INIMIGO_BALLHOG_ANDANDO ) {

            Animacao *animacaoAtual = getAnimacaoAtualInimigoBallHog( inimigo );
            atualizarAnimacao( animacaoAtual, delta );

            Inimigo ini = {
                .objeto = inimigo,
                .tipo = TIPO_INIMIGO_BALLHOG
            };

            if ( inimigo->olhandoParaDireita ) {
                inimigo->vel.x = inimigo->velAndando;
            } else {
                inimigo->vel.x = -inimigo->velAndando;
            }

            // fase X
            inimigo->ret.x += inimigo->vel.x * delta;
            resolverColisaoInimigoObstaculosMapaX( &ini, gw->mapa );

            inimigo->vel.y += gw->gravidade * delta;
            if ( inimigo->vel.y > inimigo->velMaxQueda ) {
                inimigo->vel.y = inimigo->velMaxQueda;
            }

            // fase Y
            inimigo->ret.y += inimigo->vel.y * delta;
            resolverColisaoInimigoObstaculosMapaY( &ini, gw->mapa );

            // atualiza bola preta se estiver ativa
            if ( inimigo->bolaAtiva ) {
                inimigo->bolaRet.x += inimigo->bolaVel.x * delta;
                inimigo->bolaRet.y += inimigo->bolaVel.y * delta;
                inimigo->bolaVel.y += gw->gravidade * delta;

                inimigo->contadorTempoBola += delta;
                // desativa a bola após 1.5 segundos
                if ( inimigo->contadorTempoBola > 1.5f ) {
                    inimigo->bolaAtiva = false;
                }
            }

        } else if ( inimigo->estado == ESTADO_INIMIGO_BALLHOG_MORRENDO ) {

            atualizarAnimacao( &inimigo->animacaoMorrendo, delta );

            // lança a bola preta ao começar a morrer (apenas uma vez)
            if ( !inimigo->bolaAtiva && inimigo->animacaoMorrendo.quadroAtual == 0
                 && inimigo->animacaoMorrendo.contadorTempoQuadro < 50 ) {
                inimigo->bolaAtiva = true;
                inimigo->contadorTempoBola = 0.0f;
                // bola começa no centro do BallHog
                inimigo->bolaRet = (Rectangle) {
                    inimigo->ret.x + inimigo->ret.width / 2.0f - 16,
                    inimigo->ret.y + inimigo->ret.height / 2.0f - 16,
                    32, 32   // 16x16 pixels escalados a 2x
                };
                // dispara na direção que o inimigo estava olhando
                float dir = inimigo->olhandoParaDireita ? 1.0f : -1.0f;
                inimigo->bolaVel = (Vector2) { dir * 200.0f, -300.0f };
            }

            // atualiza bola em queda livre
            if ( inimigo->bolaAtiva ) {
                inimigo->bolaRet.x += inimigo->bolaVel.x * delta;
                inimigo->bolaRet.y += inimigo->bolaVel.y * delta;
                inimigo->bolaVel.y += gw->gravidade * delta;
                inimigo->contadorTempoBola += delta;
                if ( inimigo->contadorTempoBola > 1.5f ) {
                    inimigo->bolaAtiva = false;
                }
            }

            if ( inimigo->animacaoMorrendo.finalizada ) {
                inimigo->ativo = false;
            }

        }

    }

}

/**
 * @brief Desenha um inimigo (ballhog).
 */
void desenharInimigoBallHog( InimigoBallHog *inimigo ) {

    if ( inimigo->ativo ) {

        if ( inimigo->estado == ESTADO_INIMIGO_BALLHOG_ANDANDO ) {
            QuadroAnimacao *qa = getQuadroAnimacaoAtualInimigoBallHog( inimigo );
            desenharQuadroAnimacaoInimigoBallHog( inimigo, qa, WHITE );
        } else if ( inimigo->estado == ESTADO_INIMIGO_BALLHOG_MORRENDO ) {
            desenharQuadroAnimacaoInimigoBallHogMorrendo( inimigo, getQuadroAtualAnimacao( &inimigo->animacaoMorrendo ), 2.0f, WHITE );
        }

        // desenha a bola preta se estiver ativa
        if ( inimigo->bolaAtiva ) {
            desenharBolaBallHog( inimigo );
        }

        if ( MOSTRAR_RETANGULOS ) {
            DrawRectangleRec( inimigo->ret, Fade( inimigo->cor, 0.5f ) );
            DrawRectangleLines( inimigo->ret.x, inimigo->ret.y, inimigo->ret.width, inimigo->ret.height, BLACK );
        }

    }

}

/**
 * @brief Obtém o quadro de animação atual de um inimigo (ballhog).
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoBallHog( InimigoBallHog *inimigo ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualInimigoBallHog( inimigo ) );
}

static void desenharQuadroAnimacaoInimigoBallHog( InimigoBallHog *inimigo, QuadroAnimacao *qa, Color tonalidade ) {

    if ( qa != NULL ) {

        DrawTexturePro(
            rm.texturaBadniks,
            (Rectangle) {
                qa->fonte.x,
                qa->fonte.y,
                inimigo->olhandoParaDireita ? -qa->fonte.width : qa->fonte.width,
                qa->fonte.height
            },
            inimigo->ret,
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULOS ) {
            float xDesenho = inimigo->olhandoParaDireita
                ? inimigo->ret.x + inimigo->ret.width - qa->retColisao.x - qa->retColisao.width
                : inimigo->ret.x + qa->retColisao.x;
            float yDesenho = inimigo->ret.y + qa->retColisao.y;
            DrawRectangle( xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade( GREEN, 0.5f ) );
        }

    }

}

static void desenharQuadroAnimacaoInimigoBallHogMorrendo( InimigoBallHog *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade ) {

    if ( qa != NULL ) {

        DrawTexturePro(
            rm.texturaBadniks,
            qa->fonte,
            (Rectangle) {
                inimigo->ret.x,
                inimigo->ret.y,
                qa->fonte.width * escala,
                qa->fonte.height * escala
            },
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

    }

}

static void desenharBolaBallHog( InimigoBallHog *inimigo ) {

    // Bola preta: x=101, y=72, w=16, h=16 no spritesheet
    // Alterna entre frame 0 (x=101) e frame 1 (x=118) para dar efeito de rotação
    float frameX = ( (int)( inimigo->contadorTempoBola / 0.1f ) % 2 == 0 ) ? 101.0f : 118.0f;

    DrawTexturePro(
        rm.texturaBadniks,
        (Rectangle) { frameX, 72.0f, 16.0f, 16.0f },
        inimigo->bolaRet,
        (Vector2) { 0 },
        0.0f,
        WHITE
    );

}

static Animacao *getAnimacaoAtualInimigoBallHog( InimigoBallHog *inimigo ) {
    return inimigo->animacoes[inimigo->estado];
}