#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidade.h"

int totalRotacoes = 0;

int obterAltura(Cidade *no) {
    if (no == NULL) return -1;
    return no->alturaNo;
}

void atualizarAltura(Cidade *no) {
    int alturaEsq = obterAltura(no->esquerda);
    int alturaDir = obterAltura(no->direita);

    if (alturaEsq > alturaDir){
        no->alturaNo = alturaEsq + 1;
    } else {
        no->alturaNo = alturaDir + 1;
    }
}

int fatorBalanceamento(Cidade *no){
    if (no == NULL) return 0;
    return obterAltura(no->esquerda) - obterAltura(no->direita);
}

Cidade *rotacaoDireita(Cidade *y) {
    totalRotacoes++;
    Cidade *x = y->esquerda;
    Cidade *T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    atualizarAltura(y);
    atualizarAltura(x);

    return x;  
}

Cidade *rotacaoEsquerda(Cidade *x) {
    totalRotacoes++;
    Cidade *y = x->direita;
    Cidade *T2 = y->esquerda;

    y->esquerda = x;
    x -> direita = T2;

    atualizarAltura(x);
    atualizarAltura(y);

    return y;
}

Cidade *balancear(Cidade *no) {
    atualizarAltura(no);
    int fb = fatorBalanceamento(no);

    // Caso 1: Esquerda-Esquerda
    if (fb > 1 && fatorBalanceamento(no->esquerda) >= 0)
        return rotacaoDireita(no);

    // Caso 2: Direita-Direita
    if (fb < -1 && fatorBalanceamento(no->direita) <= 0)
        return rotacaoEsquerda(no);

    // Caso 3: Esquerda-Direita
    if (fb > 1 && fatorBalanceamento(no->esquerda) < 0) {
        no->esquerda = rotacaoEsquerda(no->esquerda);
        return rotacaoDireita(no);
    }

    // Caso 4: Direita-Esquerda
    if (fb < -1 && fatorBalanceamento(no->direita) > 0) {
        no->direita = rotacaoDireita(no->direita);
        return rotacaoEsquerda(no);
    }

    return no;

}

    Cidade *inserir(Cidade *raiz, int id, char *tipo, char *regiao, int severidade, DataHora timestamp) {
    // 1. Inserção normal de BST
    if (raiz == NULL) {
        Cidade *novo = (Cidade *)malloc(sizeof(Cidade));
        novo->id = id;
        strcpy(novo->tipoEvento, tipo);
        strcpy(novo->regiaoCidade, regiao);
        novo->severidade = severidade;
        novo->timestamp = timestamp;
        novo->status = ATIVO;
        novo->alturaNo = 0;
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }


    if (id < raiz->id){
        raiz->esquerda = inserir(raiz->esquerda, id, tipo, regiao, severidade, timestamp);
    } else if (id > raiz->id) {
        raiz->direita = inserir(raiz->direita, id, tipo, regiao, severidade, timestamp);
    } else {
        printf("Evento com ID %d já existe!\n", id);
        return raiz;
    }

    return balancear(raiz);
}

Cidade *menorNo(Cidade *no) {
    while (no->esquerda != NULL)
        no = no->esquerda;
    return no;
}

Cidade *remover(Cidade *raiz, int id) {
    if (raiz == NULL) {
        printf("Evento não encontrado!\n");
        return NULL;
    }

    // 1. Desce até achar o nó
    if (id < raiz->id)
        raiz->esquerda = remover(raiz->esquerda, id);
    else if (id > raiz->id)
        raiz->direita = remover(raiz->direita, id);
    else {
        // Achou! Verifica se pode remover
        if (raiz->status != RESOLVIDO) {
            printf("Evento ainda ativo, não pode ser removido!\n");
            return raiz;
        }

        // Caso 1: sem filhos ou um filho
        if (raiz->esquerda == NULL || raiz->direita == NULL) {
            Cidade *temp = raiz->esquerda ? raiz->esquerda : raiz->direita;
            free(raiz);
            return temp;
        }

        // Caso 2: dois filhos — substitui pelo sucessor
        Cidade *sucessor = menorNo(raiz->direita);
        raiz->id = sucessor->id;
        strcpy(raiz->tipoEvento, sucessor->tipoEvento);
        strcpy(raiz->regiaoCidade, sucessor->regiaoCidade);
        raiz->severidade = sucessor->severidade;
        raiz->timestamp = sucessor->timestamp;
        raiz->status = sucessor->status;
        raiz->direita = remover(raiz->direita, sucessor->id);
    }

    return balancear(raiz);
}

Cidade *buscarPorId(Cidade *raiz, int id) {
    if (raiz == NULL) {
        printf("Evento não encontrado!\n");
        return NULL;
    }

    if (id == raiz->id)
        return raiz;
    else if (id < raiz->id)
        return buscarPorId(raiz->esquerda, id);
    else
        return buscarPorId(raiz->direita, id);
}

void exibirEvento(Cidade *no) {
    if (no == NULL) return;
    printf("ID: %d\n", no->id);
    printf("Tipo: %s\n", no->tipoEvento);
    printf("Regiao: %s\n", no->regiaoCidade);
    printf("Severidade: %d\n", no->severidade);
    printf("Status: %s\n", no->status == ATIVO ? "Ativo" : "Resolvido");
    printf("Data: %02d/%02d/%04d %02d:%02d:%02d\n",
        no->timestamp.dia, no->timestamp.mes, no->timestamp.ano,
        no->timestamp.hora, no->timestamp.minuto, no->timestamp.segundo);
}

void listarPorSeveridade(Cidade *raiz, int min, int max) {
    if (raiz == NULL) return;

    listarPorSeveridade(raiz->esquerda, min, max);

    if (raiz->status == ATIVO && raiz->severidade >= min && raiz->severidade <= max)
        exibirEvento(raiz);

    listarPorSeveridade(raiz->direita, min, max);
}

void listarPorRegiao(Cidade *raiz, char *regiao) {
    if (raiz == NULL) return;

    listarPorRegiao(raiz->esquerda, regiao);

    if (raiz->status == ATIVO && strcmp(raiz->regiaoCidade, regiao) == 0)
        exibirEvento(raiz);

    listarPorRegiao(raiz->direita, regiao);
}

void listarPorIntervaloId(Cidade *raiz, int idMin, int idMax) {
    if (raiz == NULL) return;

    if (raiz->id > idMin)
        listarPorIntervaloId(raiz->esquerda, idMin, idMax);

    if (raiz->id >= idMin && raiz->id <= idMax)
        exibirEvento(raiz);

    if (raiz->id < idMax)
        listarPorIntervaloId(raiz->direita, idMin, idMax);
}

void alterarStatus(Cidade *raiz, int id) {
    Cidade *no = buscarPorId(raiz, id);
    if (no == NULL) return;

    if (no->status == RESOLVIDO) {
        printf("Evento já está Resolvido!\n");
        return;
    }

    no->status = RESOLVIDO;
    printf("Status alterado para Resolvido!\n");
}

void alterarSeveridade(Cidade *raiz, int id, int novaSeveridade) {
    if (novaSeveridade < 1 || novaSeveridade > 5) {
        printf("Severidade inválida! Use valores entre 1 e 5.\n");
        return;
    }

    Cidade *no = buscarPorId(raiz, id);
    if (no == NULL) return;

    if (no->status == RESOLVIDO) {
        printf("Evento já resolvido, não pode ser alterado!\n");
        return;
    }

    no->severidade = novaSeveridade;
    printf("Severidade atualizada!\n");
}


int totalNos(Cidade *raiz) {
    if (raiz == NULL) return 0;
    return 1 + totalNos(raiz->esquerda) + totalNos(raiz->direita);
}

int totalAtivos(Cidade *raiz) {
    if (raiz == NULL) return 0;
    int ativo = (raiz->status == ATIVO) ? 1 : 0;
    return ativo + totalAtivos(raiz->esquerda) + totalAtivos(raiz->direita);
}

float fatorMedio(Cidade *raiz, int *count) {
    if (raiz == NULL) return 0;
    (*count)++;
    float soma = abs(fatorBalanceamento(raiz));
    soma += fatorMedio(raiz->esquerda, count);
    soma += fatorMedio(raiz->direita, count);
    return soma;
}

void exibirMetricas(Cidade *raiz) {
    int count = 0;
    float soma = fatorMedio(raiz, &count);
    printf("Altura da arvore: %d\n", obterAltura(raiz));
    printf("Total de nos: %d\n", totalNos(raiz));
    printf("Eventos ativos: %d\n", totalAtivos(raiz));
    printf("Fator medio: %.2f\n", count > 0 ? soma / count : 0);
    printf("Total de rotacoes: %d\n", totalRotacoes);
}