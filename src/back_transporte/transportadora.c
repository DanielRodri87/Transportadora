#include "transportadora.h"

// Definição da lista de clientes global
Cliente *lista_clientes = NULL;
Produto *lista_produtos = NULL;
// ########################### CLIENTES ###############################

void cadastrar_cliente(const char *nome, const char *cpf, const char *estado, const char *cidade, const char *rua, int numero, const char *telefone, const char *email)
{
    printf("Entrou na função.\n");

    Cliente *novo_cliente = (Cliente *)malloc(sizeof(Cliente));
    if (novo_cliente == NULL)
    {
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

    strncpy(novo_cliente->rua, rua, sizeof(novo_cliente->rua) - 1);
    novo_cliente->rua[sizeof(novo_cliente->rua) - 1] = '\0';

    novo_cliente->numero = numero;

    strncpy(novo_cliente->telefone, telefone, sizeof(novo_cliente->telefone) - 1);
    novo_cliente->telefone[sizeof(novo_cliente->telefone) - 1] = '\0';

    strncpy(novo_cliente->email, email, sizeof(novo_cliente->email) - 1);
    novo_cliente->email[sizeof(novo_cliente->email) - 1] = '\0';

    // Configurar o ponteiro 'prox' do novo cliente
    novo_cliente->prox = NULL;

    novo_cliente->produtos = NULL;

    // Inserir o novo cliente na lista de clientes
    if (lista_clientes == NULL)
    {
        lista_clientes = novo_cliente;
    }
    else
    {
        Cliente *atual = lista_clientes;
        while (atual->prox != NULL)
        {
            atual = atual->prox;
        }
        atual->prox = novo_cliente;
    }

    printf("Saiu da função.\n");
}

void exibir_clientes()
{
    Cliente *atual = lista_clientes;
    while (atual != NULL)
    {
        printf("Nome: %s\nCPF: %s\nEstado: %s\nCidade: %s\nRua: %s\nNúmero: %d\nTelefone: %s\nEmail: %s\n",
               atual->nome, atual->cpf, atual->estado, atual->cidade, atual->rua, atual->numero,
               atual->telefone, atual->email);
        atual = atual->prox;
    }
}

Cliente *buscarClientePorCPF(const char *cpf) {
    Cliente *atual = lista_clientes;
    while (atual != NULL) {
        if (strcmp(atual->cpf, cpf) == 0) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL; // Retorna NULL se o cliente não for encontrado
}

// ################################ ROTAS ################################

Rota* gerar_rota() {
    Rota *nova_rota = (Rota *)malloc(sizeof(Rota));
    if (nova_rota == NULL) {
        printf("Erro ao alocar memória para a nova rota.\n");
        return NULL;
    }
    nova_rota->tentativa1 = NULL;
    nova_rota->tentativa2 = NULL;
    nova_rota->tentativas = 0;
    nova_rota->pessoas = 0;
    nova_rota->entregas_relizadas = 0;
    return nova_rota;
}

void inicializar_transportadora(Transportadora *t)
{
    t->score = 0;
    t->entregas_realizadas = 0;
    t->rota_on = NULL;
    t->lista_clientes = NULL;
    t->fila_devolucao = NULL;
}



void ativar_rota(Transportadora *t)
{
    if (t == NULL) {
        printf("Erro: Transportadora inválida.\n");
        return;
    }
    if (t->rota_on == NULL)
    {
        t->rota_on = gerar_rota();
        if (t->rota_on != NULL) {
            printf("Rota ativada.\n");
        } else {
            printf("Falha ao ativar rota.\n");
        }
    }
    else
    {
        printf("Rota já estava ativada.\n");
    }
}

Transportadora* concluir_rota(Transportadora *t)
{
    if (t == NULL) {
        printf("Erro: Transportadora inválida.\n");
        return NULL;
    }
    if (t->rota_on != NULL)
    {
        t->entregas_realizadas += t->rota_on->entregas_relizadas;
        free(t->rota_on);
        t->rota_on = NULL;
        printf("Rota concluída.\n");
    }
    else
    {
        printf("Não há rota para concluir.\n");
    }
    return t;
}

void cadastrar_cliente_rota(Transportadora *t, Cliente *buscado)
{
    Cliente *cliente = (Cliente *)malloc(sizeof(Cliente));
    Cliente *fila;

    if (buscado != NULL)
    {
        strcpy(cliente->nome, buscado->nome);
        strcpy(cliente->cpf, buscado->cpf);
        strcpy(cliente->estado, buscado->estado);
        strcpy(cliente->cidade, buscado->cidade);
        strcpy(cliente->rua, buscado->rua);
        cliente->numero = buscado->numero;
        strcpy(cliente->telefone, buscado->telefone);
        strcpy(cliente->email, buscado->email);
        
        if (t->rota_on->tentativa1 == NULL)
        {
            cliente->prox = NULL;
            t->rota_on->tentativa1 = cliente;
            t->rota_on->pessoas++;
            printf("Cliente adicionado à rota.\n");
        } else
        {
            fila = t->rota_on->tentativa1;
            while (fila->prox != NULL)
            {
                fila = fila->prox;
            }
            cliente->prox = NULL;
            fila->prox = cliente;
            t->rota_on->pessoas++;
            printf("Cliente adicionado à rota.\n");
        }
    } else
        printf("Cliente não encontrado.\n");
}


void cadastrar_produto_cliente(Transportadora *t, Cliente *buscado, const int id, const char *nome)
{
    Produto *p = (Produto *)malloc(sizeof(Produto));
    if (p == NULL) {
        printf("Erro ao alocar memória para o produto.\n");
        return;
    }

    p->id = id;
    strcpy(p->nome, nome);
    p->prox = NULL;

    if (buscado != NULL) {
        if (buscado->produtos == NULL) {
            // Caso o cliente não tenha produtos, adiciona o primeiro produto
            buscado->produtos = p;
        } else {
            // Adiciona o novo produto ao final da lista de produtos do cliente
            Produto *aux = buscado->produtos;
            while (aux->prox != NULL) {
                aux = aux->prox;
            }
            aux->prox = p;
        }
        printf("Produto adicionado ao cliente.\n");
    }
}


void exibir_produtos_cliente(Cliente *c)
{
    Produto *p = c->produtos;
    while (p != NULL) {
        printf("ID: %d\nNome: %s\n", p->id, p->nome);
        p = p->prox;
    }
}


void mostrar_fila_entregas(Transportadora *t)
{
    // Implementação básica
    printf("Fila de entregas exibida.\n");
}
