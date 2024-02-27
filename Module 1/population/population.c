#include <stdio.h>
#include <cs50.h>
//make population

int years(void);

int main(void)
{
    int n;
    int m;

     // TODO: Solicite o valor inicial ao usuário
    do
    {
        n = get_int("Quanto eh a populacao inicial?\n");
    }
    while (n < 9);

     // TODO: Solicite o valor final ao usuário
    do
    {
        m = get_int("Quanto eh a populacao final?\n");
    }
    while (m < n);

    int j;
    int y=0;

     // TODO: Calcule o número de anos até o limite
    do
    {
        int q = ( n/3 );
        int k = ( n/4 );
        n = ( q-k+n );
        y = ( y+1);
        printf("%d",y);
    }
    while (n < m);

      // TODO: Imprima o número de anos
        printf("levarao %i\n", y);
}