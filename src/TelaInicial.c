/**
 * @file TelaInicial.c
 * @brief Implementação da Tela Inicial.
 */

#include <stdlib.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "TelaInicial.h"
#include "ResourceManager.h"
#include "Tipos.h"

bool terminouAnimacao;

static void desenharQuadroAnimacaoTelaInicial(
    TelaInicial *tela,
    QuadroAnimacao *qa,
    Color tonalidade);

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria a tela inicial.
 */
TelaInicial *criarTelaInicial(void)
{

    TelaInicial *novaTela =
        (TelaInicial *)malloc(sizeof(TelaInicial));

    novaTela->ret = (Rectangle){
        0,
        0,
        GetScreenWidth(),
        GetScreenHeight()};

    novaTela->ativo = true;

    novaTela->animacaoSonic.quantidadeQuadros = 18;
    novaTela->animacaoSonic.quadroAtual = 0;
    novaTela->animacaoSonic.contadorTempoQuadro = 0;
    novaTela->animacaoSonic.pararNoUltimoQuadro = false;
    novaTela->animacaoSonic.executarUmaVez = false;
    novaTela->animacaoSonic.finalizada = false;

    criarQuadrosAnimacao(
        &novaTela->animacaoSonic,
        novaTela->animacaoSonic.quantidadeQuadros);

    inicializarQuadrosAnimacao(
        novaTela->animacaoSonic.quadros,
        novaTela->animacaoSonic.quantidadeQuadros,
        120,

        2,
        2,

        320,
        224,

        8,

        false,

        (Rectangle){0});

    novaTela->animacoes[0] = &novaTela->animacaoSonic;
    novaTela->quantidadeAnimacoes = 1;

    return novaTela;
}

/**
 * @brief Destroi a tela inicial.
 */
void destruirTelaInicial(TelaInicial *tela)
{

    if (tela != NULL)
    {

        for (int i = 0; i < tela->quantidadeAnimacoes; i++)
        {
            destruirQuadrosAnimacao(
                tela->animacoes[i]);
        }

        free(tela);
    }
}

/**
 * @brief Atualiza a tela inicial.
 */
void atualizarTelaInicial(
    TelaInicial *tela,
    float delta)
{

    if (tela->ativo)
    {
        atualizarAnimacao(&tela->animacaoSonic, delta);

        if (terminouAnimacao)
        {
            if (tela->animacaoSonic.quadroAtual == 0)
            {
                tela->animacaoSonic.quadroAtual = 16;
            }
        }
        else
        {
            if (tela->animacaoSonic.quadroAtual == 17)
            {
                terminouAnimacao = true;
            }
        }
    }
}

/**
 * @brief Desenha a tela inicial.
 */
void desenharTelaInicial(TelaInicial *tela)
{
    if (!tela->ativo)
    {
        return;
    }

    DrawTexturePro(
        rm.texturaFundo,
        (Rectangle){
            0,
            0,
            rm.texturaFundo.width,
            rm.texturaFundo.height},
        (Rectangle){
            0,
            0,
            GetScreenWidth(),
            GetScreenHeight()},
        (Vector2){0},
        0,
        WHITE);

    QuadroAnimacao *qa = getQuadroAnimacaoAtualTelaInicial(tela);

    desenharQuadroAnimacaoTelaInicial(
        tela,
        qa,
        WHITE);

    if ((GetTime() * 2) - (int)(GetTime() * 2) < 0.5)
    {
        DrawTexturePro(
            rm.texturaTelaInicial,
            (Rectangle){
                552,
                1992,
                144,
                8},
            (Rectangle){
                82 * 3,
                178 * 2,
                144 * 2,
                8 * 2},
            (Vector2){0},
            0,
            WHITE);
    }
}

/**
 * @brief Obtém o quadro atual.
 */
QuadroAnimacao *getQuadroAnimacaoAtualTelaInicial(
    TelaInicial *tela)
{
    return getQuadroAtualAnimacao(
        &tela->animacaoSonic);
}

static void desenharQuadroAnimacaoTelaInicial(
    TelaInicial *tela,
    QuadroAnimacao *qa,
    Color tonalidade)
{

    if (qa != NULL)
    {

        DrawTexturePro(
            rm.texturaAnimacaoMenu,
            qa->fonte,
            tela->ret,
            (Vector2){0},
            0.0f,
            tonalidade);

        if (MOSTRAR_RETANGULOS)
        {

            DrawRectangleLines(
                tela->ret.x,
                tela->ret.y,
                tela->ret.width,
                tela->ret.height,
                RED);
        }
    }
}