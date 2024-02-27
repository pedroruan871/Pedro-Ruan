#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int its_positive(string str);
int make_k(string ko);
string cipherization(string ptext, int k);

int main(int argc, string argv[])
{
    if (argc == 2)
    {
        int argvlen = (strlen(argv[1]));
        int argvpon = its_positive(argv[1]);
        if (argvlen == argvpon)
        {
            int key = make_k(argv[1]);
            string ptext = get_string("plaintext: ");
            string cipheredtxt = cipherization(ptext, key);
            printf("ciphertext: %s\n", ptext);
        }
        else
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}

int its_positive(string str)
{
    int retvalue = 0;
    for (int i = 0, n = strlen(str); i < n; i++)
    {
        if ((str[i] >= '0' && str[i] <= '9') || (str[i] == '\0'))
            retvalue = retvalue + 1;
    }
    return retvalue;
}

int make_k(string ko)
{
    int nbr = atoi(ko);
    return nbr;
}

string cipherization(string ptext, int k)
{
    string ciphed = ptext;
    for (int i = 0, n = strlen(ptext); i < n; i++)
    {
        int c = ptext[i];
        int x;
        char crip;
        if (c >= 'a' && c <= 'z')
        {
            x = c - 96;
            crip = ((x + k) % 26) + 96;
        }
        else if (c >= 'A' && c <= 'Z')
        {
            x = c - 64;
            crip = ((x + k) % 26) + 64;
        }
        else
            crip = c;
        ptext[i] = crip;
    }
    return ptext;
}