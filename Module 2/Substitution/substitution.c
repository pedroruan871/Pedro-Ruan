#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Função para checar o argv[1]
int leng(string argv1);
//Função para criptografar texto solicitado
string susbs(string txto, string txtc);
//Função para deixar a chave maiuscula
string maius(string k, string M);
//Função para deixar a chave minuscula
string minus(string k, string m);

int main(int argc, string argv[])
{
    // Vê se tem dois argumentos para não dar "Segmentation error"
    if(argc == 2)
    {
        //Essa linha vê se o segundo argumento(chave) ta so com letras e tem 26 de tamanho
        int argvisok = leng(argv[1]);
        if (argvisok == 26)
        {
            string texttcrip = get_string("plaintext:\n");
            string criptxt = susbs(texttcrip, argv[1]);
            printf("ciphertext: %s\n", criptxt);
        }
        else
        {
            printf("Usage: ./substitution key\n");
            return 1;
        }
    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

}

int leng(string argv1)
{
    int numb;
    int ret = 0;
    string y = (argv1);
    string z = (argv1);
    for (int i = 0; i < 26; i++)
    {
        if (isupper(y[i]) || islower(y[i]))
        {
            int vall = 0;
            for (int x = 0; x < 26; x++)
            {
                if (z[x] != y[i])
                    vall = vall + 1;
            }
            if (vall == 25)
            {
                ret = ret + 1;
            }
            else
                ret = 0;
        }
        else
        {
            ret = 0;
        }
    }
    return ret;
}

string susbs(string txto, string txtc) //txto = frase; txtc/key = chave
{
    string key = txtc;
    string txc = txto;
    for (int i = 0, n = strlen(txto); i < n; i++)
    {
        if islower(txto[i])
        {
            string k = minus(txtc, key);
            int numer = txto[i] - 97;
            txc[i] = txtc[numer];
        }
        if isupper(txto[i])
        {
            string k = maius(txtc, key);
            int numer = txto[i] - 65;
            txc[i] = txtc[numer];
        }
    }
    return txc;
}

string maius(string k, string M) //k = original key & M = copy key
{
    for (int i = 0; i < 26; i++ )
    {
        M[i] = toupper(k[i]);
    }
    return M;
}

string minus(string k, string m)//k = original key & m = copy key
{
    for (int i = 0; i < 26; i++ )
    {
        m[i] = tolower(k[i]);
    }
    return m;
}