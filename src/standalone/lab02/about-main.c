#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    int total_size = 0;
    for (int i = 0; i < argc; ++i)
    {
        total_size += strlen(argv[i]);
        printf("%-20s%d\n", argv[i], strlen(argv[i]));
    }
    double average = total_size / argc;
    printf("%-20s%d\n", "Total length", total_size);
    printf("%-20s%.2f\n", "Average length", average);
    return total_size;
}


