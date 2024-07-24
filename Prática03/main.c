/*
Gabriel Lucchetta Garcia Sanchez - 828513
*/
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *arquivo;
    char buffer[100];
    int quantidade = 0;
    float preco = 0.0;
    float total = 0.0;
    int tamanho = 0;

    int i = 0;

    arquivo = fopen("compras.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }
    while (1)
    {
        tamanho = fgetc(arquivo); 
        
        if (tamanho == EOF)
        {
            break;
        };

        fread(buffer, sizeof(char), tamanho, arquivo);
        buffer[tamanho] = '\0';
        for (i = 0; i < tamanho; i++)
        {
            if (buffer[i] == '|')
            {
                quantidade = atoi(&buffer[i + 1]);
                preco = atof(&buffer[i + 3]);
                break;
            };
        };
        total += quantidade * preco;
    }
    fclose(arquivo);
    printf("Total da compra: R$%.2f\n", total);

    return 0;
}
