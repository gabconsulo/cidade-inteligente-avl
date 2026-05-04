#include "cidade.h"

int totalRotacoes = 0;

/* Retorna a altura de um no. Para no nulo, a altura sera -1. */
int obterAltura(Cidade *no) {
    if (no == NULL) {
        return -1;
    }

    return no->alturaNo;
}

/* Atualiza a altura de um no a partir da altura dos filhos. */
void atualizarAltura(Cidade *no) {
    int alturaEsquerda;
    int alturaDireita;

    if (no == NULL) {
        return;
    }

    alturaEsquerda = obterAltura(no->esquerda);
    alturaDireita = obterAltura(no->direita);

    if (alturaEsquerda > alturaDireita) {
        no->alturaNo = alturaEsquerda + 1;
    } else {
        no->alturaNo = alturaDireita + 1;
    }
}

/* Calcula o fator de balanceamento da AVL. */
int fatorBalanceamento(Cidade *no) {
    if (no == NULL) {
        return 0;
    }

    return obterAltura(no->esquerda) - obterAltura(no->direita);
}

/* Rotacao simples para a direita. */
Cidade *rotacaoDireita(Cidade *y) {
    Cidade *x = y->esquerda;
    Cidade *t2 = x->direita;

    totalRotacoes++;

    x->direita = y;
    y->esquerda = t2;

    atualizarAltura(y);
    atualizarAltura(x);

    return x;
}

/* Rotacao simples para a esquerda. */
Cidade *rotacaoEsquerda(Cidade *x) {
    Cidade *y = x->direita;
    Cidade *t2 = y->esquerda;

    totalRotacoes++;

    y->esquerda = x;
    x->direita = t2;

    atualizarAltura(x);
    atualizarAltura(y);

    return y;
}

/* Rebalanceia o no depois de insercao ou remocao. */
Cidade *balancear(Cidade *no) {
    int fb;

    if (no == NULL) {
        return NULL;
    }

    atualizarAltura(no);
    fb = fatorBalanceamento(no);

    if (fb > 1 && fatorBalanceamento(no->esquerda) >= 0) {
        return rotacaoDireita(no);
    }

    if (fb < -1 && fatorBalanceamento(no->direita) <= 0) {
        return rotacaoEsquerda(no);
    }

    if (fb > 1 && fatorBalanceamento(no->esquerda) < 0) {
        no->esquerda = rotacaoEsquerda(no->esquerda);
        return rotacaoDireita(no);
    }

    if (fb < -1 && fatorBalanceamento(no->direita) > 0) {
        no->direita = rotacaoDireita(no->direita);
        return rotacaoEsquerda(no);
    }

    return no;
}

/* Insere um novo evento na arvore respeitando a ordenacao por ID. */
Cidade *inserir(Cidade *raiz, int id, char tipo[], char regiao[], int severidade, DataHora timestamp) {
    if (severidade < 1 || severidade > 5) {
        printf("Severidade invalida. Use valores entre 1 e 5.\n");
        return raiz;
    }

    if (raiz == NULL) {
        Cidade *novo = (Cidade *)malloc(sizeof(Cidade));

        if (novo == NULL) {
            printf("Erro de alocacao de memoria.\n");
            return NULL;
        }

        novo->id = id;
        strcpy(novo->tipoEvento, tipo);
        strcpy(novo->regiaoCidade, regiao);
        novo->severidade = (Severidade)severidade;
        novo->status = ATIVO;
        novo->timestamp = timestamp;
        novo->alturaNo = 0;
        novo->esquerda = NULL;
        novo->direita = NULL;

        return novo;
    }

    if (id < raiz->id) {
        raiz->esquerda = inserir(raiz->esquerda, id, tipo, regiao, severidade, timestamp);
    } else if (id > raiz->id) {
        raiz->direita = inserir(raiz->direita, id, tipo, regiao, severidade, timestamp);
    } else {
        printf("Ja existe um evento com esse ID.\n");
        return raiz;
    }

    return balancear(raiz);
}

/* Encontra o menor elemento da subarvore, usado na remocao com dois filhos. */
Cidade *menorNo(Cidade *no) {
    Cidade *atual = no;

    while (atual != NULL && atual->esquerda != NULL) {
        atual = atual->esquerda;
    }

    return atual;
}

/*
 * Remove o sucessor sem validar status.
 * Essa funcao auxiliar so e usada internamente quando o no removido tem dois filhos.
 */
static Cidade *removerSucessor(Cidade *raiz, int id) {
    if (raiz == NULL) {
        return NULL;
    }

    if (id < raiz->id) {
        raiz->esquerda = removerSucessor(raiz->esquerda, id);
    } else if (id > raiz->id) {
        raiz->direita = removerSucessor(raiz->direita, id);
    } else {
        if (raiz->esquerda == NULL || raiz->direita == NULL) {
            Cidade *temp;

            if (raiz->esquerda != NULL) {
                temp = raiz->esquerda;
            } else {
                temp = raiz->direita;
            }

            free(raiz);
            return temp;
        } else {
            Cidade *sucessor = menorNo(raiz->direita);

            raiz->id = sucessor->id;
            strcpy(raiz->tipoEvento, sucessor->tipoEvento);
            strcpy(raiz->regiaoCidade, sucessor->regiaoCidade);
            raiz->severidade = sucessor->severidade;
            raiz->status = sucessor->status;
            raiz->timestamp = sucessor->timestamp;
            raiz->direita = removerSucessor(raiz->direita, sucessor->id);
        }
    }

    return balancear(raiz);
}

/* Remove um evento apenas se ele estiver com status Resolvido. */
Cidade *remover(Cidade *raiz, int id) {
    if (raiz == NULL) {
        printf("Evento nao encontrado.\n");
        return NULL;
    }

    if (id < raiz->id) {
        raiz->esquerda = remover(raiz->esquerda, id);
    } else if (id > raiz->id) {
        raiz->direita = remover(raiz->direita, id);
    } else {
        if (raiz->status != RESOLVIDO) {
            printf("Apenas eventos resolvidos podem ser removidos.\n");
            return raiz;
        }

        if (raiz->esquerda == NULL || raiz->direita == NULL) {
            Cidade *temp;

            if (raiz->esquerda != NULL) {
                temp = raiz->esquerda;
            } else {
                temp = raiz->direita;
            }

            free(raiz);
            return temp;
        } else {
            Cidade *sucessor = menorNo(raiz->direita);

            raiz->id = sucessor->id;
            strcpy(raiz->tipoEvento, sucessor->tipoEvento);
            strcpy(raiz->regiaoCidade, sucessor->regiaoCidade);
            raiz->severidade = sucessor->severidade;
            raiz->status = sucessor->status;
            raiz->timestamp = sucessor->timestamp;
            raiz->direita = removerSucessor(raiz->direita, sucessor->id);
        }
    }

    return balancear(raiz);
}

/* Busca um evento pelo ID. */
Cidade *buscarPorId(Cidade *raiz, int id) {
    if (raiz == NULL) {
        return NULL;
    }

    if (id == raiz->id) {
        return raiz;
    }

    if (id < raiz->id) {
        return buscarPorId(raiz->esquerda, id);
    }

    return buscarPorId(raiz->direita, id);
}

/* Exibe os dados completos de um evento. */
void exibirEvento(Cidade *no) {
    if (no == NULL) {
        return;
    }

    printf("ID: %d\n", no->id);
    printf("Tipo do evento: %s\n", no->tipoEvento);
    printf("Regiao: %s\n", no->regiaoCidade);
    printf("Severidade: %d\n", no->severidade);
    printf("Status: %s\n", no->status == ATIVO ? "Ativo" : "Resolvido");
    printf(
        "Data/Hora: %02d/%02d/%04d %02d:%02d:%02d\n",
        no->timestamp.dia,
        no->timestamp.mes,
        no->timestamp.ano,
        no->timestamp.hora,
        no->timestamp.minuto,
        no->timestamp.segundo
    );
    printf("----------------------------------------\n");
}

/* Percurso em ordem para listar eventos ordenados por ID. */
void emOrdem(Cidade *raiz) {
    if (raiz == NULL) {
        return;
    }

    emOrdem(raiz->esquerda);
    exibirEvento(raiz);
    emOrdem(raiz->direita);
}

/* Lista eventos ativos cuja severidade esteja dentro de um intervalo. */
void listarPorSeveridade(Cidade *raiz, int min, int max) {
    if (raiz == NULL) {
        return;
    }

    listarPorSeveridade(raiz->esquerda, min, max);

    if (raiz->status == ATIVO && (int)raiz->severidade >= min && (int)raiz->severidade <= max) {
        exibirEvento(raiz);
    }

    listarPorSeveridade(raiz->direita, min, max);
}

/* Lista os eventos ativos de uma regiao especifica. */
void listarPorRegiao(Cidade *raiz, char regiao[]) {
    if (raiz == NULL) {
        return;
    }

    listarPorRegiao(raiz->esquerda, regiao);

    if (raiz->status == ATIVO && strcmp(raiz->regiaoCidade, regiao) == 0) {
        exibirEvento(raiz);
    }

    listarPorRegiao(raiz->direita, regiao);
}

/* Lista os eventos cujo ID esteja entre dois valores. */
void listarPorIntervaloId(Cidade *raiz, int idMin, int idMax) {
    if (raiz == NULL) {
        return;
    }

    if (raiz->id > idMin) {
        listarPorIntervaloId(raiz->esquerda, idMin, idMax);
    }

    if (raiz->id >= idMin && raiz->id <= idMax) {
        exibirEvento(raiz);
    }

    if (raiz->id < idMax) {
        listarPorIntervaloId(raiz->direita, idMin, idMax);
    }
}

/* Altera o status do evento para Resolvido. */
void alterarStatus(Cidade *raiz, int id) {
    Cidade *no = buscarPorId(raiz, id);

    if (no == NULL) {
        printf("Evento nao encontrado.\n");
        return;
    }

    if (no->status == RESOLVIDO) {
        printf("Esse evento ja esta resolvido.\n");
        return;
    }

    no->status = RESOLVIDO;
    printf("Status alterado para Resolvido.\n");
}

/* Atualiza a severidade de um evento ativo. */
void alterarSeveridade(Cidade *raiz, int id, int novaSeveridade) {
    Cidade *no;

    if (novaSeveridade < 1 || novaSeveridade > 5) {
        printf("Severidade invalida. Use valores entre 1 e 5.\n");
        return;
    }

    no = buscarPorId(raiz, id);

    if (no == NULL) {
        printf("Evento nao encontrado.\n");
        return;
    }

    if (no->status == RESOLVIDO) {
        printf("Nao e permitido alterar a severidade de um evento resolvido.\n");
        return;
    }

    no->severidade = (Severidade)novaSeveridade;
    printf("Severidade atualizada com sucesso.\n");
}

/* Conta quantos nos existem na arvore. */
int totalNos(Cidade *raiz) {
    if (raiz == NULL) {
        return 0;
    }

    return 1 + totalNos(raiz->esquerda) + totalNos(raiz->direita);
}

/* Conta quantos eventos ainda estao ativos. */
int totalAtivos(Cidade *raiz) {
    int ativo;

    if (raiz == NULL) {
        return 0;
    }

    ativo = (raiz->status == ATIVO) ? 1 : 0;
    return ativo + totalAtivos(raiz->esquerda) + totalAtivos(raiz->direita);
}

/* Soma os fatores de balanceamento de todos os nos. */
int somaFatores(Cidade *raiz) {
    if (raiz == NULL) {
        return 0;
    }

    return fatorBalanceamento(raiz) +
           somaFatores(raiz->esquerda) +
           somaFatores(raiz->direita);
}

/* Conta quantos nos participam do calculo da media. */
int contarNosParaMedia(Cidade *raiz) {
    if (raiz == NULL) {
        return 0;
    }

    return 1 + contarNosParaMedia(raiz->esquerda) + contarNosParaMedia(raiz->direita);
}

/* Exibe as metricas exigidas no enunciado. */
void exibirMetricas(Cidade *raiz) {
    int quantidadeNos = contarNosParaMedia(raiz);
    float media = 0;
    int altura;

    if (quantidadeNos > 0) {
        media = (float)somaFatores(raiz) / quantidadeNos;
    }

    altura = obterAltura(raiz);
    if (altura < 0) {
        altura = 0;
    }

    printf("Altura total da arvore: %d\n", altura);
    printf("Numero total de nos: %d\n", totalNos(raiz));
    printf("Numero de eventos ativos: %d\n", totalAtivos(raiz));
    printf("Fator medio de balanceamento: %.2f\n", media);
    printf("Quantidade total de rotacoes: %d\n", totalRotacoes);
}

/* Libera toda a memoria alocada pela arvore. */
void liberarArvore(Cidade *raiz) {
    if (raiz == NULL) {
        return;
    }

    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}
