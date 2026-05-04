# Sistema de Gerenciamento de Eventos Criticos

## Sobre o trabalho

Esse trabalho foi feito em C para a disciplina de Estruturas de Dados Avancadas.
O objetivo e simular um sistema de eventos criticos de uma cidade inteligente
usando uma arvore AVL como estrutura principal.

Cada evento tem:

- ID
- tipo do evento
- severidade de 1 a 5
- data e hora
- regiao da cidade
- status do evento

## Guia rapido

Se quiser olhar o projeto de forma mais rapida, a ordem mais facil e esta:

1. Abrir `main.c`
   Aqui fica o menu do sistema e a parte que conversa com o usuario.

2. Abrir `cidade.h`
   Aqui estao a `struct` do evento, os `enum` e os prototipos das funcoes.

3. Abrir `cidade.c`
   Aqui esta a implementacao da arvore AVL e das operacoes do sistema.

4. Abrir `RELATORIO_TECNICO.md`
   Aqui esta o roteiro dos testes e o espaco para colocar as capturas.

## O que foi implementado

- insercao de eventos na AVL
- rotacoes simples e duplas
- remocao apenas de eventos resolvidos
- busca de evento por ID
- listagem de eventos ativos por faixa de severidade
- listagem de eventos ativos por regiao
- listagem de eventos por intervalo de ID
- alteracao de status
- alteracao de severidade
- metricas da arvore
- menu textual com submenus

## Onde esta cada parte

- `main.c`
  Menu principal, submenus e leitura dos dados digitados.

- `cidade.h`
  Tipos, structs e assinaturas das funcoes.

- `cidade.c`
  AVL, insercao, remocao, busca, listagens, atualizacoes e metricas.

## Como compilar

Usando GCC:

```bash
gcc -Wall -Wextra -pedantic -std=c11 main.c cidade.c -o cidade
```

No Windows:

```powershell
.\cidade.exe
```

## Como usar

Ao iniciar, o sistema mostra um menu principal com 4 partes:

- Cadastros
- Consultas
- Atualizacoes
- Relatorios

No cadastro, o usuario informa:

- ID do evento
- tipo do evento
- regiao
- severidade
- dia
- mes
- ano
- hora
- minuto
- segundo

## Regras importantes do sistema

- a arvore e organizada pelo ID do evento
- a severidade aceita somente valores de 1 a 5
- um evento so pode ser removido se estiver como `Resolvido`
- a alteracao de severidade so pode ser feita em evento ativo

## Observacao

As capturas de teste e os estao em
`RELATORIO_TECNICO.md`.
