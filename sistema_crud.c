#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char nome[50];
    char genero[50];
    int ano;
    char diretor[50];
    float media_avaliacao;
    int favorito;
} Filme;

void cadastrarFilme(FILE *arq) {
    Filme novoFilme;
    printf("Digite o ID do filme: ");
    scanf("%d", &novoFilme.id);

    // Verificar se o ID já existe
    Filme filmeExistente;
    rewind(arq);
    while (fread(&filmeExistente, sizeof(Filme), 1, arq) == 1) {
        if (filmeExistente.id == novoFilme.id) {
            printf("Ja existe um filme cadastrado com esse ID. Digite um ID diferente.\n");
            return;
        }
    }

    printf("Digite o nome do filme: ");
    scanf(" %[^\n]", novoFilme.nome);

    printf("Digite o ano de lançamento: ");
    scanf("%d", &novoFilme.ano);

    printf("Digite o nome do diretor: ");
    scanf(" %[^\n]", novoFilme.diretor);

    // Validar a média de avaliação
    while (1) {
        printf("Quantas estrelas você dá (0.5 a 5.0): ");
        scanf("%f", &novoFilme.media_avaliacao);

        if (novoFilme.media_avaliacao >= 0.5 && novoFilme.media_avaliacao <= 5.0) {
            break;
        } else if (novoFilme.media_avaliacao > 5.0) {
            printf("Só é permitido dar até 5 estrelas!\n");
        } else if (novoFilme.media_avaliacao < 0.5) {
            printf("É preciso dar pelo menos 0.5!\n");
        }
    }

    fwrite(&novoFilme, sizeof(Filme), 1, arq);
    printf("Filme cadastrado com sucesso!\n");
}

void listarFilmes(FILE *arq) {
    rewind(arq);
    Filme filme;

    int encontrouFilme = 0; // Variável para verificar se encontrou algum filme

    printf("Filmes cadastrados:\n");

    while (fread(&filme, sizeof(Filme), 1, arq) == 1) {
        encontrouFilme = 1; // Indica que foi encontrado pelo menos um filme
        printf("ID: %d\n", filme.id);
        printf("Nome: %s\n", filme.nome);
        printf("Ano de lançamento: %d\n", filme.ano);
        printf("Diretor: %s\n", filme.diretor);
        printf("Nota que voce deu: %.2f\n", filme.media_avaliacao);
        printf("\n");
    }

    if (!encontrouFilme) {
        printf("Voce ainda nao cadastrou nenhum filme!\n");
    }
}

void favoritarFilme(FILE *arq) {
    int id;
    printf("Digite o ID do filme a ser favoritado: ");
    scanf("%d", &id);

    Filme filme;
    int encontrouFilme = 0;

    rewind(arq);

    // Percorrer o arquivo para encontrar o filme pelo ID
    while (fread(&filme, sizeof(Filme), 1, arq) == 1) {
        if (filme.id == id) {
            encontrouFilme = 1;
            filme.favorito = 1; // Marcar o filme como favorito
            fseek(arq, -sizeof(Filme), SEEK_CUR); // Voltar para a posição correta no arquivo
            fwrite(&filme, sizeof(Filme), 1, arq); // Atualizar o registro do filme no arquivo
            printf("Filme favoritado com sucesso!\n");
            break;
        }
    }

    if (!encontrouFilme) {
        printf("Nao existe filme com esse ID na sua lista!\n");
    }
}

void listarFavoritos(FILE *arq) {
    rewind(arq);
    Filme filme;

    int encontrouFavorito = 0; // Variável para verificar se encontrou algum filme favorito

    printf("Filmes favoritos:\n");

    while (fread(&filme, sizeof(Filme), 1, arq) == 1) {
        if (filme.favorito == 1) {
            encontrouFavorito = 1;
            printf("ID: %d\n", filme.id);
            printf("Nome: %s\n", filme.nome);
            printf("Ano de lançamento: %d\n", filme.ano);
            printf("Diretor: %s\n", filme.diretor);
            printf("Nota que você deu: %.2f\n", filme.media_avaliacao);
            printf("\n");
        }
    }

    if (!encontrouFavorito) {
        printf("Voce nao tem filmes favoritos!\n");
    }
}

void removerFavorito(FILE *arq) {
    int id;
    printf("Digite o ID do filme a ser removido dos favoritos: ");
    scanf("%d", &id);

    Filme filme;
    int encontrouFilme = 0;

    rewind(arq);

    // Percorrer o arquivo para encontrar o filme pelo ID
    while (fread(&filme, sizeof(Filme), 1, arq) == 1) {
        if (filme.id == id) {
            encontrouFilme = 1;
            filme.favorito = 0; // Remover o filme dos favoritos
            fseek(arq, -sizeof(Filme), SEEK_CUR); // Voltar para a posição correta no arquivo
            fwrite(&filme, sizeof(Filme), 1, arq); // Atualizar o registro do filme no arquivo
            printf("Filme removido dos favoritos com sucesso!\n");
            break;
        }
    }

    if (!encontrouFilme) {
        printf("Nao existe filme com esse ID na sua lista!\n");
    }
}

void deletarFilme(FILE *arq) {
    int id;
    printf("Digite o ID do filme a ser removido: ");
    scanf("%d", &id);

    Filme filme;
    int encontrouFilme = 0;

    FILE *tempArq = fopen("temp.bin", "wb"); // Abrir um arquivo temporário para escrita

    rewind(arq);
    // Percorrer o arquivo para encontrar o filme pelo ID e copiar os outros filmes para o arquivo temporário
    while (fread(&filme, sizeof(Filme), 1, arq) == 1) {
        if (filme.id == id) {
            encontrouFilme = 1;
        } else {
            fwrite(&filme, sizeof(Filme), 1, tempArq); // Copiar o```c
        fwrite(&filme, sizeof(Filme), 1, tempArq); // Copiar o filme para o arquivo temporário
    }
    
    }
    fclose(arq);
    fclose(tempArq);

    if (encontrouFilme) {
        remove("filmes.bin"); // Remover o arquivo original
        rename("temp.bin", "filmes.bin"); // Renomear o arquivo temporário para substituir o arquivo original
        printf("Filme removido com sucesso!\n");
    } else {
        remove("temp.bin"); // Remover o arquivo temporário
        printf("Nao existe filme com esse ID na sua lista!\n");
    }

    arq = fopen("filmes.bin", "rb+"); // Abrir novamente o arquivo original para leitura e escrita
}

int main() {
    FILE *arq = fopen("filmes.bin", "rb+");

    if (arq == NULL) {
        arq = fopen("filmes.bin", "wb+");
        if (arq == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            return 1;
        }
    }

    int opcao;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Cadastrar novo filme\n");
        printf("2. Listar filmes cadastrados\n");
        printf("3. Favoritar um filme\n");
        printf("4. Listar filmes favoritos\n");
        printf("5. Remover filme dos favoritos\n");
        printf("6. Deletar filme\n");
        printf("0. Sair\n");
        printf("Digite a opçao desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarFilme(arq);
                break;
            case 2:
                listarFilmes(arq);
                break;
            case 3:
                favoritarFilme(arq);
                break;
            case 4:
                listarFavoritos(arq);
                break;
            case 5:
                removerFavorito(arq);
                break;
            case 6:
                deletarFilme(arq);
                break;
            case 0:
                fclose(arq);
                return 0;
            default:
                printf("Opção inválida!\n");
                break;
        }
    }
}