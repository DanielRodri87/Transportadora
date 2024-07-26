#include <stdio.h>
#include <stdlib.h>
#include "transportadora.h"
#include <string.h>

// Função para cadastrar um novo cliente
void cadastrarCliente(Transportadora* transp, const char* nome, const char* cpf, const char* estado, const char* cidade, const char* bairro, const char* rua, int numero, const char* telefone, const char* email) {
    printf("Entrou na função.\n");

    Cliente* novo_cliente = (Cliente*)malloc(sizeof(Cliente));
    if (novo_cliente == NULL) {
        fprintf(stderr, "Erro de alocação de memória para novo cliente.\n");
        return;
    }

    // Preencher os dados do cliente
    strncpy(novo_cliente->nome, nome, sizeof(novo_cliente->nome) - 1);
    novo_cliente->nome[sizeof(novo_cliente->nome) - 1] = '\0';

    strncpy(novo_cliente->cpf, cpf, sizeof(novo_cliente->cpf) - 1);
    novo_cliente->cpf[sizeof(novo_cliente->cpf) - 1] = '\0';

    strncpy(novo_cliente->estado, estado, sizeof(novo_cliente->estado) - 1);
    novo_cliente->estado[sizeof(novo_cliente->estado) - 1] = '\0';

    strncpy(novo_cliente->cidade, cidade, sizeof(novo_cliente->cidade) - 1);
    novo_cliente->cidade[sizeof(novo_cliente->cidade) - 1] = '\0';

    strncpy(novo_cliente->bairro, bairro, sizeof(novo_cliente->bairro) - 1);
    novo_cliente->bairro[sizeof(novo_cliente->bairro) - 1] = '\0';

    strncpy(novo_cliente->rua, rua, sizeof(novo_cliente->rua) - 1);
    novo_cliente->rua[sizeof(novo_cliente->rua) - 1] = '\0';

    novo_cliente->numero = numero;

    strncpy(novo_cliente->telefone, telefone, sizeof(novo_cliente->telefone) - 1);
    novo_cliente->telefone[sizeof(novo_cliente->telefone) - 1] = '\0';

    strncpy(novo_cliente->email, email, sizeof(novo_cliente->email) - 1);
    novo_cliente->email[sizeof(novo_cliente->email) - 1] = '\0';

    // Configurar o ponteiro 'prox' do novo cliente
    novo_cliente->prox = NULL;

    // Inserir o novo cliente na lista de clientes
    if (transp->lista_clientes == NULL) {
        // Se a lista está vazia, o novo cliente é o primeiro cliente
        transp->lista_clientes = novo_cliente;
    } else {
        // Caso contrário, percorrer a lista até o último cliente e adicionar o novo cliente lá
        Cliente* atual = transp->lista_clientes;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo_cliente;
    }

    printf("Saiu da função.\n");
}
