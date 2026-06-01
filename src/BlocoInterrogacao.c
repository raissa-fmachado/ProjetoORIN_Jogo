/**
 * @file BlocoInterrogacao.c
 * @brief Bloco de interrogação estilo Sonic/Mario.
 *        Bata por baixo para ativar: o bloco salta para cima e solta anéis.
 */
#include <math.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "BlocoInterrogacao.h"
#include "Tipos.h"

#define SALT_ALTURA   14.0f   /* pixels que o bloco sobe no salto       */
#define SALT_DURACAO   0.18f  /* segundos de ida + volta                */
#define PULS_VEL       4.0f   /* velocidade da pulsação do ?            */

BlocoInterrogacao *criarBlocoInterrogacao( Rectangle ret, int aneisParaSoltar )
{
    BlocoInterrogacao *b = (BlocoInterrogacao *)malloc( sizeof(BlocoInterrogacao) );

    b->ret             = ret;
    b->estado          = ESTADO_BLOCO_INT_INTACTO;
    b->ativo           = true;
    b->animTimer       = 0.0f;
    b->pulsTimer       = 0.0f;
    b->saltY           = 0.0f;
    b->saltSubindo     = true;
    b->aneisParaSoltar = aneisParaSoltar;

    return b;
}

void destruirBlocoInterrogacao( BlocoInterrogacao *b )
{
    if ( b != NULL ) free( b );
}

void ativarBlocoInterrogacao( BlocoInterrogacao *b )
{
    if ( b->estado != ESTADO_BLOCO_INT_INTACTO ) return;

    b->estado      = ESTADO_BLOCO_INT_ATIVADO;
    b->animTimer   = 0.0f;
    b->saltY       = 0.0f;
    b->saltSubindo = true;
}

void atualizarBlocoInterrogacao( BlocoInterrogacao *b, float delta )
{
    if ( !b->ativo ) return;

    b->pulsTimer += delta;

    if ( b->estado == ESTADO_BLOCO_INT_ATIVADO )
    {
        b->animTimer += delta;
        float t = b->animTimer / SALT_DURACAO;
        if ( t > 1.0f ) t = 1.0f;

        /* arco suave: sobe até metade, desce na segunda metade */
        float arco = sinf( t * 3.14159f );
        b->saltY = -SALT_ALTURA * arco;

        if ( t >= 1.0f )
        {
            b->saltY  = 0.0f;
            b->estado = ESTADO_BLOCO_INT_ABERTO;
        }
    }
}

void desenharBlocoInterrogacao( BlocoInterrogacao *b )
{
    if ( !b->ativo ) return;

    float x = b->ret.x;
    float y = b->ret.y + b->saltY;
    float w = b->ret.width;
    float h = b->ret.height;

    if ( b->estado == ESTADO_BLOCO_INT_ABERTO )
    {
        /* bloco aberto: cinza escuro */
        DrawRectangle( (int)x, (int)y, (int)w, (int)h, (Color){80, 80, 80, 255} );
        DrawRectangleLinesEx( (Rectangle){x, y, w, h}, 3, (Color){50, 50, 50, 255} );

        /* Cruz interna indicando vazio */
        Color cv = (Color){60, 60, 60, 255};
        DrawLine( (int)(x + w*0.3f), (int)(y + h*0.3f),
                  (int)(x + w*0.7f), (int)(y + h*0.7f), cv );
        DrawLine( (int)(x + w*0.7f), (int)(y + h*0.3f),
                  (int)(x + w*0.3f), (int)(y + h*0.7f), cv );
    }
    else
    {
        /* bloco intacto: laranja/amarelo com ? pulsando */
        float pulso = 0.75f + 0.25f * sinf( b->pulsTimer * PULS_VEL );

        Color corBloco = {
            (unsigned char)(255 * pulso),
            (unsigned char)(165 * pulso),
            0,
            255
        };
        Color corBorda = {
            (unsigned char)(200 * pulso),
            (unsigned char)(100 * pulso),
            0,
            255
        };

        /* fundo */
        DrawRectangle( (int)x, (int)y, (int)w, (int)h, corBloco );

        /* borda */
        DrawRectangleLinesEx( (Rectangle){x, y, w, h}, 3, corBorda );

        /* brilho no canto superior esquerdo */
        DrawRectangle( (int)(x+4), (int)(y+4), (int)(w*0.2f), (int)(h*0.12f),
                       (Color){255, 255, 200, 180} );

        /* símbolo ? centralizado */
        int fs = (int)(h * 0.55f);
        if ( fs < 10 ) fs = 10;
        int tw = MeasureText( "?", fs );
        DrawText( "?",
                  (int)(x + (w - tw) / 2),
                  (int)(y + (h - fs) / 2 - 2),
                  fs,
                  WHITE );
    }
}