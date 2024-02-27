#include <stdio.h>
#include <cs50.h>

int high(void);

int main(void)
{
    int h = high();
    for (int v=0 ; v < h; v++)
    {
        for(int z = h-1; z > v; z--)
        {
            printf(" ");
        }
        for(int j = -1; j < v; j++)
        {
            printf("#");
        }
        //a partir daqui começa a linha da piramide dupla
        printf("  ");
        for(int k = -1; k < v; k++)
        {
            printf("#");
        }
        //a linha termina aqui
        printf("\n");
    }
}

// pra pegar a altura
int high(void)
{
    int h;
    do
    {
        h = get_int("Qual a altura?\n ");
    }
    while (h > 8 || h < 1);
    return h;
}
