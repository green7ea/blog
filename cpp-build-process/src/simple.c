#include <stdio.h>

#include "add.h"

int main(int argv, char **argc)
{
    printf("%i\n", sub(add(5, 6), 6));

    return 0;
}
