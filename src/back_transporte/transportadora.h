#ifndef TRANSPORTADORA_H
#define TRANSPORTADORA_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Cliente {
    char nome[50];
    char cpf[14];
    struct Cliente* prox;
} Cliente;

typedef struct ListaEntrega {
    int id_entrega;
    struct ListaEntrega* prox;
} ListaEntrega;

typedef struct Fila {
    ListaEntrega* ini;
    ListaEntrega* fim;
} Fila;

typedef struct PilhaNode {
    int id_entrega;
    int tentativas;
    struct PilhaNode* prox;
} PilhaNode;

typedef struct Pilha {
    PilhaNode* topo;
} Pilha;

typedef struct ListaDevolucao {
    int id_entrega;
    struct ListaDevolucao* prox;
} ListaDevolucao;

typedef struct Devolucao {
    ListaDevolucao* ini;
    ListaDevolucao* fim;
} Devolucao;

typedef struct Transportadora {
    Cliente* lista_clientes;
    Fila fila_entregas;
    Pilha pilha_nao_entregues;
    Devolucao fila_devolucao;
    int pontuacao_total;
    int rotas_feitas;
} Transportadora;


Cliente* criar_cliente(const char* nome, const char* cpf);
void adicionar_cliente(Transportadora* transportadora, Cliente* cliente);
Cliente* buscar_cliente(Transportadora* transportadora, const char* cpf);
void imprimir_clientes(const Transportadora* transportadora);

void inicializar_fila(Fila* fila);
void adicionar_entrega(Fila* fila, int id_entrega);
ListaEntrega* remover_entrega(Fila* fila);
int fila_vazia(const Fila* fila);
void imprimir_fila(const Fila* fila);

void inicializar_pilha(Pilha* pilha);
void adicionar_na_pilha(Pilha* pilha, int id_entrega, int tentativas);
PilhaNode* remover_da_pilha(Pilha* pilha);
int pilha_vazia(const Pilha* pilha);
void imprimir_pilha(const Pilha* pilha);

void inicializar_devolucao(Devolucao* devolucao);
void adicionar_na_devolucao(Devolucao* devolucao, int id_entrega);
ListaDevolucao* remover_da_devolucao(Devolucao* devolucao);
int devolucao_vazia(const Devolucao* devolucao);
void imprimir_devolucao(const Devolucao* devolucao);

void inicializar_transportadora(Transportadora* transportadora);
void processar_entregas(Transportadora* transportadora);
void calcular_escore(Transportadora* transportadora);
void imprimir_transportadora(const Transportadora* transportadora);

#endif 
