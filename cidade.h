#ifndef CIDADE_H
#define CIDADE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    ATIVO,
    RESOLVIDO
} Status;

typedef enum {
    BAIXO = 1,
    MEDIO,
    ALTO,
    MUITO_ALTO,
    CRITICO
} Severidade;

typedef struct {
    int dia;
    int mes;
    int ano;
    int hora;
    int minuto;
    int segundo;
} DataHora;

typedef struct Cidade {
    int id;
    char tipoEvento[50];
    char regiaoCidade[50];
    Severidade severidade;
    Status status;
    DataHora timestamp;
    int alturaNo;
    struct Cidade *esquerda;
    struct Cidade *direita;
} Cidade;

int obterAltura(Cidade *no);
void atualizarAltura(Cidade *no);
int fatorBalanceamento(Cidade *no);
Cidade *rotacaoDireita(Cidade *y);
Cidade *rotacaoEsquerda(Cidade *x);
Cidade *balancear(Cidade *no);
Cidade *inserir(Cidade *raiz, int id, char tipo[], char regiao[], int severidade, DataHora timestamp);
Cidade *menorNo(Cidade *no);
Cidade *remover(Cidade *raiz, int id);
Cidade *buscarPorId(Cidade *raiz, int id);
void exibirEvento(Cidade *no);
void emOrdem(Cidade *raiz);
void listarPorSeveridade(Cidade *raiz, int min, int max);
void listarPorRegiao(Cidade *raiz, char regiao[]);
void listarPorIntervaloId(Cidade *raiz, int idMin, int idMax);
void alterarStatus(Cidade *raiz, int id);
void alterarSeveridade(Cidade *raiz, int id, int novaSeveridade);
int totalNos(Cidade *raiz);
int totalAtivos(Cidade *raiz);
int somaFatores(Cidade *raiz);
int contarNosParaMedia(Cidade *raiz);
void exibirMetricas(Cidade *raiz);
void liberarArvore(Cidade *raiz);

#endif
