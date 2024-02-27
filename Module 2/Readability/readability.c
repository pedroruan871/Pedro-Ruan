#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int measurement(string blas);

int main(void)
{
    string txt = get_string("Text: \n");
    int Grade = measurement(txt);
    if (Grade <= 1)
        printf("Before Grade 1\n");
    else if (Grade >= 16)
        printf("Grade 16+\n");
    else
        printf("Grade %d\n", Grade);
}

int measurement(char zz[])
{
    int letter = 0;
    int word = 0;
    int phrase = 0;

    // Faz a contagem de palavras, frases e letras.

    for (int i = 0, n = strlen(zz); i < n; i++)
    {
        if ((zz[i] >= 'a' && zz[i] <= 'z') || (zz[i] >= 'A' && zz[i] <= 'Z'))
            letter = letter + 1;
        if (zz[i] == ' ')
            word = word + 1;
        if (zz[i] == '.' || zz[i] == '!' || zz[i] == '?')
        {
            phrase = phrase + 1;
            if (zz[i + 1] != ' ')
                word = word + 1;
        }
    }

    // Faz a média de quantas letras tem por 100 palavras.

    float L = (100 * letter) / (float)word;

    // Faz a média de quantas frases tem por 100 palavras.

    float S = (100 * phrase) / (float)word;

    // Aplica a fórmula e diz o nivel de leitura

    float readability = (0.0588 * L) - (0.296 * S) - 15.8;

    return round(readability);
}