#include "cidade.h"

void limparBuffer(void);
void lerTexto(char texto[], int tamanho);
void menuCadastros(Cidade **raiz);
void menuConsultas(Cidade *raiz);
void menuAtualizacoes(Cidade *raiz);
void menuRelatorios(Cidade *raiz);

int main(void) {
    Cidade *raiz = NULL;
    int opcao;

    do {
        printf("\n========================================\n");
        printf("Sistema de Gerenciamento de Eventos\n");
        printf("========================================\n");
        printf("1. Cadastros\n");
        printf("2. Consultas\n");
        printf("3. Atualizacoes\n");
        printf("4. Relatorios\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                menuCadastros(&raiz);
                break;
            case 2:
                menuConsultas(raiz);
                break;
            case 3:
                menuAtualizacoes(raiz);
                break;
            case 4:
                menuRelatorios(raiz);
                break;
            case 0:
                printf("Encerrando o sistema...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    liberarArvore(raiz);
    return 0;
}

/* Limpa o buffer do teclado depois do scanf. */
void limparBuffer(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/* Le uma linha de texto, removendo o enter do final. */
void lerTexto(char texto[], int tamanho) {
    fgets(texto, tamanho, stdin);
    texto[strcspn(texto, "\n")] = '\0';
}

/* Submenu de cadastro e remocao. */
void menuCadastros(Cidade **raiz) {
    int opcao;

    do {
        printf("\n----------- Cadastros -----------\n");
        printf("1. Cadastrar evento\n");
        printf("2. Remover evento resolvido\n");
        printf("0. Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            int id;
            int severidade;
            char tipo[50];
            char regiao[50];
            DataHora agora;

            printf("ID do evento: ");
            scanf("%d", &id);
            limparBuffer();

            printf("Tipo do evento: ");
            lerTexto(tipo, 50);

            printf("Regiao da cidade: ");
            lerTexto(regiao, 50);

            printf("Severidade (1 a 5): ");
            scanf("%d", &severidade);
            limparBuffer();

            printf("Dia: ");
            scanf("%d", &agora.dia);
            limparBuffer();

            printf("Mes: ");
            scanf("%d", &agora.mes);
            limparBuffer();

            printf("Ano: ");
            scanf("%d", &agora.ano);
            limparBuffer();

            printf("Hora: ");
            scanf("%d", &agora.hora);
            limparBuffer();

            printf("Minuto: ");
            scanf("%d", &agora.minuto);
            limparBuffer();

            printf("Segundo: ");
            scanf("%d", &agora.segundo);
            limparBuffer();

            *raiz = inserir(*raiz, id, tipo, regiao, severidade, agora);
        } else if (opcao == 2) {
            int id;

            printf("ID do evento a remover: ");
            scanf("%d", &id);
            limparBuffer();

            *raiz = remover(*raiz, id);
        } else if (opcao != 0) {
            printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

/* Submenu com as consultas pedidas no enunciado. */
void menuConsultas(Cidade *raiz) {
    int opcao;

    do {
        printf("\n----------- Consultas -----------\n");
        printf("1. Buscar por ID\n");
        printf("2. Listar ativos por severidade\n");
        printf("3. Listar ativos por regiao\n");
        printf("4. Listar por intervalo de ID\n");
        printf("0. Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            int id;
            Cidade *evento;

            printf("Informe o ID: ");
            scanf("%d", &id);
            limparBuffer();

            evento = buscarPorId(raiz, id);

            if (evento == NULL) {
                printf("Evento nao encontrado.\n");
            } else {
                exibirEvento(evento);
            }
        } else if (opcao == 2) {
            int min;
            int max;

            printf("Severidade minima: ");
            scanf("%d", &min);
            limparBuffer();

            printf("Severidade maxima: ");
            scanf("%d", &max);
            limparBuffer();

            if (min > max) {
                int aux = min;
                min = max;
                max = aux;
            }

            listarPorSeveridade(raiz, min, max);
        } else if (opcao == 3) {
            char regiao[50];

            printf("Informe a regiao: ");
            lerTexto(regiao, 50);
            listarPorRegiao(raiz, regiao);
        } else if (opcao == 4) {
            int idMin;
            int idMax;

            printf("ID minimo: ");
            scanf("%d", &idMin);
            limparBuffer();

            printf("ID maximo: ");
            scanf("%d", &idMax);
            limparBuffer();

            if (idMin > idMax) {
                int aux = idMin;
                idMin = idMax;
                idMax = aux;
            }

            listarPorIntervaloId(raiz, idMin, idMax);
        } else if (opcao != 0) {
            printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

/* Submenu para alterar status e severidade. */
void menuAtualizacoes(Cidade *raiz) {
    int opcao;

    do {
        printf("\n---------- Atualizacoes ----------\n");
        printf("1. Alterar status para Resolvido\n");
        printf("2. Alterar severidade\n");
        printf("0. Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            int id;

            printf("Informe o ID do evento: ");
            scanf("%d", &id);
            limparBuffer();

            alterarStatus(raiz, id);
        } else if (opcao == 2) {
            int id;
            int novaSeveridade;

            printf("Informe o ID do evento: ");
            scanf("%d", &id);
            limparBuffer();

            printf("Nova severidade (1 a 5): ");
            scanf("%d", &novaSeveridade);
            limparBuffer();

            alterarSeveridade(raiz, id, novaSeveridade);
        } else if (opcao != 0) {
            printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

/* Submenu dos relatorios e metricas. */
void menuRelatorios(Cidade *raiz) {
    int opcao;

    do {
        printf("\n----------- Relatorios -----------\n");
        printf("1. Exibir metricas da arvore\n");
        printf("2. Listar todos os eventos\n");
        printf("0. Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            exibirMetricas(raiz);
        } else if (opcao == 2) {
            if (raiz == NULL) {
                printf("Nao ha eventos cadastrados.\n");
            } else {
                emOrdem(raiz);
            }
        } else if (opcao != 0) {
            printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}
