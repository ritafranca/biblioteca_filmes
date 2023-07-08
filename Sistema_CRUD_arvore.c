#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct filme {
    int id;
    char titulo[50];
    char genero[50];
    int ano_lancamento;
} Filme;

typedef struct no {
    Filme filme;                // Estrutura Filme para armazenar os dados do filme
    struct no *esquerda;        // Ponteiro para o nó à esquerda
    struct no *direita;         // Ponteiro para o nó à direita
    struct no *pai;             // Ponteiro para o nó pai
} NO;

NO *raiz = NULL;                // Ponteiro para a raiz da árvore

NO *buscar_arvore(int x, NO *ptr) {
    // Função para buscar um elemento na árvore
    // Recebe um valor x e o nó raiz da subárvore atual (ou NULL se a subárvore é vazia)
    // Retorna o ponteiro para o nó que contém o valor x (se encontrado) ou NULL (se não encontrado)

    if (ptr == NULL || x == ptr->filme.id) {
        // Caso base: o nó atual é NULL (subárvore vazia) ou o valor do nó atual é igual a x
        // Retorna o ponteiro para o nó atual (pode ser NULL)
        return ptr;
    } else if (x < ptr->filme.id) {
        // O valor de x é menor que o valor do nó atual
        // Continua a busca na subárvore esquerda
        if (ptr->esquerda == NULL) {
            // A subárvore esquerda é vazia, então o valor x não está presente na árvore
            return NULL;
        } else {
            // Chama recursivamente a função para buscar na subárvore esquerda
            return buscar_arvore(x, ptr->esquerda);
        }
    } else {
        // O valor de x é maior que o valor do nó atual
        // Continua a busca na subárvore direita
        if (ptr->direita == NULL) {
            // A subárvore direita é vazia, então o valor x não está presente na árvore
            return NULL;
        } else {
            // Chama recursivamente a função para buscar na subárvore direita
            return buscar_arvore(x, ptr->direita);
        }
    }
}

void inserir_arvore(int id, const char *titulo, const char *genero, int ano_lancamento) {
    // Função para inserir um filme na árvore
    // Recebe os dados do filme: id, título, gênero e ano de lançamento

    Filme filme;
    filme.id = id;
    strncpy(filme.titulo, titulo, sizeof(filme.titulo) - 1);
    strncpy(filme.genero, genero, sizeof(filme.genero) - 1);
    filme.ano_lancamento = ano_lancamento;

    NO *ptr = buscar_arvore(id, raiz);
    if (ptr != NULL) {
        // O filme já existe na árvore (chave duplicada)
        if (ptr->filme.id == id) {
            printf("Chave duplicada!\n");
        } else {
            // O filme não existe na árvore
            NO *novoNo = malloc(sizeof(NO));
            novoNo->filme = filme;
            novoNo->esquerda = NULL;
            novoNo->direita = NULL;
            novoNo->pai = ptr;
            if (id < ptr->filme.id) {
                // Insere o novo nó como filho à esquerda do nó atual
                ptr->esquerda = novoNo;
            } else {
                // Insere o novo nó como filho à direita do nó atual
                ptr->direita = novoNo;
            }
        }
    } else {
        // A árvore está vazia, o novo filme será a raiz
        NO *novoNo = malloc(sizeof(NO));
        novoNo->filme = filme;
        novoNo->esquerda = NULL;
        novoNo->direita = NULL;
        novoNo->pai = NULL;
        raiz = novoNo;
    }
}

NO *encontrar_sucessor(NO *ptr) {
    // Função para encontrar o sucessor de um nó na árvore
    // Recebe o ponteiro para o nó atual
    // Retorna o ponteiro para o nó sucessor

    if (ptr->esquerda == NULL) {
        // Caso base: não há nós à esquerda, então o nó atual é o menor valor
        return ptr;
    }
    // Continua a busca pelo menor valor na subárvore esquerda
    return encontrar_sucessor(ptr->esquerda);
}

void atualizar_arvore(int id) {
    // Função para atualizar os dados de um filme na árvore
    // Recebe o id do filme a ser atualizado

    NO *filmeEncontrado = buscar_arvore(id, raiz);
    if (filmeEncontrado != NULL) {
        // O filme foi encontrado na árvore
        char novoTitulo[50];
        char novoGenero[50];
        int novoAnoLancamento;

        printf("Digite o novo titulo do filme: ");
        scanf("%s", novoTitulo);
        printf("Digite o novo genero do filme: ");
        scanf("%s", novoGenero);
        printf("Digite o novo ano de lancamento do filme: ");
        scanf("%d", &novoAnoLancamento);

        strncpy(filmeEncontrado->filme.titulo, novoTitulo, sizeof(filmeEncontrado->filme.titulo) - 1);
        strncpy(filmeEncontrado->filme.genero, novoGenero, sizeof(filmeEncontrado->filme.genero) - 1);
        filmeEncontrado->filme.ano_lancamento = novoAnoLancamento;

        printf("Filme atualizado com sucesso.\n");
    } else {
        // O filme não foi encontrado na árvore
        printf("Filme nao encontrado.\n");
    }
}

NO *encontrar_minimo(NO *ptr) {
    // Função para encontrar o nó com o menor valor (mínimo) na árvore
    // Recebe o ponteiro para o nó atual
    // Retorna o ponteiro para o nó com o menor valor

    if (ptr->esquerda == NULL) {
        // Caso base: não há nós à esquerda, então o nó atual é o menor valor
        return ptr;
    }
    // Continua a busca pelo menor valor na subárvore esquerda
    return encontrar_minimo(ptr->esquerda);
}

void remover_no(int x) {
    // Função para remover um nó da árvore
    // Recebe o valor x do nó a ser removido

    NO *ptr = buscar_arvore(x, raiz);

    if (ptr == NULL) {
        // O nó não foi encontrado na árvore
        printf("Filme nao encontrado.\n");
        return;
    }

    if (ptr->esquerda == NULL && ptr->direita == NULL) {
        // Caso 1: o nó é uma folha (não tem filhos)
        if (ptr->pai != NULL) {
            // O nó tem um pai
            if (ptr->filme.id < ptr->pai->filme.id) {
                // O nó é filho à esquerda do pai
                ptr->pai->esquerda = NULL;
            } else {
                // O nó é filho à direita do pai
                ptr->pai->direita = NULL;
            }
        } else {
            // O nó é a raiz
            raiz = NULL;
        }
        free(ptr);  // Libera a memória do nó removido
    } else if (ptr->esquerda != NULL && ptr->direita != NULL) {
        // Caso 2: o nó tem dois filhos
        NO *sucessor = encontrar_minimo(ptr->direita);
        ptr->filme = sucessor->filme;
        remover_no(sucessor->filme.id);
    } else {
        // Caso 3: o nó tem um filho
        NO *filho = (ptr->esquerda != NULL) ? ptr->esquerda : ptr->direita;
        if (ptr->pai != NULL) {
            // O nó tem um pai
            if (ptr->filme.id < ptr->pai->filme.id) {
                // O nó é filho à esquerda do pai
                ptr->pai->esquerda = filho;
            } else {
                // O nó é filho à direita do pai
                ptr->pai->direita = filho;
            }
            filho->pai = ptr->pai;
        } else {
            // O nó é a raiz
            raiz = filho;
            filho->pai = NULL;
        }
        free(ptr);  // Libera a memória do nó removido
    }
}

void pre_ordem(NO *ptr) {
    // Função para percorrer a árvore em pré-ordem (raiz, esquerda, direita)
    // Recebe o ponteiro para o nó atual

    printf("ID: %d | Titulo: %s | Genero: %s | Ano de Lancamento: %d\n",
        ptr->filme.id, ptr->filme.titulo, ptr->filme.genero, ptr->filme.ano_lancamento);

    if (ptr->esquerda != NULL) {
        // Chama recursivamente a função para percorrer a subárvore esquerda
        pre_ordem(ptr->esquerda);
    }

    if (ptr->direita != NULL) {
        // Chama recursivamente a função para percorrer a subárvore direita
        pre_ordem(ptr->direita);
    }
}

int main() {
    int opcao;

    do {
        printf("_____ MENU _____\n");
        printf("1 - Cadastrar filme\n");
        printf("2 - Pesquisar filme\n");
        printf("3 - Listar filmes\n");
        printf("4 - Atualizar filme\n");
        printf("5 - Deletar filme\n");
        printf("0 - Sair\n");
        printf("Escolha a opcao desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                // Opção 1: Cadastrar filme
                int id;
                char titulo[50];
                char genero[50];
                int ano_lancamento;
                printf("Digite o ID do filme: ");
                scanf("%d", &id);
                printf("Digite o titulo do filme: ");
                scanf("%s", titulo);
                printf("Digite o genero do filme: ");
                scanf("%s", genero);
                printf("Digite o ano de lancamento do filme: ");
                scanf("%d", &ano_lancamento);
                inserir_arvore(id, titulo, genero, ano_lancamento);
                break;
            }
            case 2: {
                // Opção 2: Pesquisar filme
                int id;
                printf("Digite o ID do filme a ser pesquisado: ");
                scanf("%d", &id);

                NO *filmeEncontrado = buscar_arvore(id, raiz);
                if (filmeEncontrado != NULL) {
                    printf("Filme encontrado:\n");
                    printf("ID: %d - Titulo: %s - Genero: %s - Ano de Lancamento: %d\n",
                        filmeEncontrado->filme.id, filmeEncontrado->filme.titulo,
                        filmeEncontrado->filme.genero, filmeEncontrado->filme.ano_lancamento);
                } else {
                    printf("Filme nao encontrado.\n");
                }
                break;
            }
            case 3:
                // Opção 3: Listar filmes
                printf("Listagem de filmes:\n");
                pre_ordem(raiz);
                break;
            case 4: {
                // Opção 4: Atualizar filme
                int id;
                printf("Digite o ID do filme para atualiza-lo: ");
                scanf("%d", &id);
                atualizar_arvore(id);
                break;
            }
            case 5: {
                // Opção 5: Deletar filme
                int id;
                printf("Digite o ID do filme para remove-lo: ");
                scanf("%d", &id);
                remover_no(id);
                break;
            }
            case 0:
                // Opção 0: Sair
                printf("Sessao encerrada.\n");
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}
