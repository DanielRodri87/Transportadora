#ifndef TRANSPORTADORA_H
#define TRANSPORTADORA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cliente {
    char nome[50];
    char cpf[15];
    char estado[3];
    char cidade[50];
    char rua[50];
    int numero;
    char telefone[15];
    char email[50];
    struct Produto *produtos;
    struct Cliente* prox;
} Cliente;

typedef struct Produto {
    int id;
    char nome[30];
    struct Produto *prox; 
} Produto;

typedef struct Rota {
    Cliente *tentativa1;
    Cliente *tentativa2;
    int tentativas;
    int pessoas;
    int entregas_relizadas;
} Rota;

typedef struct Transportadora {
    int score;
    int entregas_realizadas;
    Rota *rota_on;
    Cliente *lista_clientes;
    Produto *fila_devolucao;
} Transportadora;

typedef struct NaoEntregue {
    Cliente *cliente;
    struct NaoEntregue *prox;
} NaoEntregue;

typedef struct Pilha
{
    NaoEntregue *topo;
} Pilha;


typedef struct ListaDevolucao {
    Cliente *cliente;
    struct ListaDevolucao* prox;
} ListaDevolucao;

typedef struct Devolucao {
    ListaDevolucao* ini;
    ListaDevolucao* fim;
} Devolucao;

typedef struct TransportadoraFila {
    Rota *rota_on;
    Devolucao *fila_devolucao;
} TransportadoraFila;


extern Cliente* lista_clientes; 

void cadastrar_cliente(const char *nome, const char *cpf, const char *estado, const char *cidade, const char *rua, int numero, const char *telefone, const char *email);
void exibir_clientes();
Cliente *buscarClientePorCPF(const char *cpf);

void inicializar_transportadora(Transportadora *t);
Rota *gerar_rota();
void ativar_rota(Transportadora *t);
Transportadora* concluir_rota(Transportadora *t);
void cadastrar_cliente_rota(Transportadora *t, Cliente *buscado);
void cadastrar_produto_cliente(Transportadora *t, Cliente *buscado, const int id, const char *nome);
void mostrar_fila_entregas(Transportadora *t);

#endif
