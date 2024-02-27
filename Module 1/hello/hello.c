#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string name = get_string("qual é o seu nome? \n");
    printf("eain,%s \n", name);
}