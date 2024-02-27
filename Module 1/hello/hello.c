#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string name = get_string("Qual é o seu nome? \n");
    printf("Hello,%s \n", name);
}
