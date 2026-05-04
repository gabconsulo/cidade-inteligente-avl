#ifndef CIDADE_H
#define CIDADE_H
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    ATIVO,
    RESOLVIDO
} Status;

typedef enum {
    BAIXO = 1,
    MEDIO = 2,
    ALTO = 3,
    MUITO_ALTO = 4,
    CRITICO = 5
} Severidade;

typedef struct {
    int dia;
    int mes;
    int ano;
    int hora;
    int minuto;
    int segundo;
} DataHora;

typedef struct Cidade
{
    int id;
    char tipoEvento[50];
    char regiaoCidade[50];
    Status status;
    Severidade severidade;
    DataHora timestamp;
    int alturaNo;
    struct Cidade *esquerda;
    struct Cidade *direita;
} Cidade;

#endif