#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

typedef struct {
    int id;
    char nome[50];
    char genero[50];
    int ano;
    char diretor[50];
    float media_avaliacao;
    int favorito;
    char comentario[400];
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

    printf("Digite o ano de lancamento: ");
    scanf("%d", &novoFilme.ano);

    printf("Digite o nome do diretor: ");
    scanf(" %[^\n]", novoFilme.diretor);

    // Validar a media de avaliacao
    while (1) {
        printf("Quantas estrelas voce da (0.5 a 5.0): ");
        scanf("%f", &novoFilme.media_avaliacao);

        if (novoFilme.media_avaliacao >= 0.5 && novoFilme.media_avaliacao <= 5.0) {
            break;
        } else if (novoFilme.media_avaliacao > 5.0) {
            printf("So e permitido dar ate 5 estrelas!\n");
        } else if (novoFilme.media_avaliacao < 0.5) {
            printf("E preciso dar pelo menos 0.5!\n");
        }
    }

    printf("Digite um comentario sobre o filme (maximo 400 caracteres): ");
    scanf(" %[^\n]", novoFilme.comentario);

    fwrite(&novoFilme, sizeof(Filme), 1, arq);
    printf("Filme cadastrado com sucesso!\n");
}

void listarFilmes(FILE *arq) {
    rewind(arq);
    Filme filme;

    int encontrouFilme = 0; // Variavel para verificar se encontrou algum filme

    printf("Filmes cadastrados:\n");

    while (fread(&filme, sizeof(Filme), 1, arq) == 1) {
        encontrouFilme = 1; // Indica que foi encontrado pelo menos um filme
        printf("ID: %d\n", filme.id);
        printf("Nome: %s\n", filme.nome);
        printf("Ano de lancamento: %d\n", filme.ano);
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
            fseek(arq, -sizeof(Filme), SEEK_CUR); // Voltar para a posicao correta no arquivo
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

    int encontrouFavorito = 0; // Variavel para verificar se encontrou algum filme favorito

    printf("Filmes favoritos:\n");

    while (fread(&filme, sizeof(Filme), 1, arq) == 1) {
        if (filme.favorito == 1) {
            encontrouFavorito = 1;
            printf("ID: %d\n", filme.id);
            printf("Nome: %s\n", filme.nome);
            printf("Ano de lancamento: %d\n", filme.ano);
            printf("Diretor: %s\n", filme.diretor);
            printf("Nota que voce deu: %.2f\n", filme.media_avaliacao);
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
            fseek(arq, -sizeof(Filme), SEEK_CUR); // Voltar para a posicao correta no arquivo
            fwrite(&filme, sizeof(Filme), 1, arq); // Atualizar o registro do filme no arquivo
            printf("Filme removido dos favoritos com sucesso!\n");
            break;
        }
    }

    if (!encontrouFilme) {
        printf("Nao existe filme com esse ID na sua lista!\n");
    }
}

void verComentarios(FILE *arq) {
    rewind(arq);
    Filme filme;

    int encontrouComentario = 0; // Variavel para verificar se encontrou algum filme com comentario

    printf("Filmes com comentarios:\n");

    while (fread(&filme, sizeof(Filme), 1, arq) == 1) {
        if (strlen(filme.comentario) > 0) {
            encontrouComentario = 1;
            printf("ID: %d\n", filme.id);
            printf("Nome: %s\n", filme.nome);
            printf("Comentario: %s\n", filme.comentario);// Exibe as informações do filme que possui comentário
            printf("\n");
        }
    }

    if (!encontrouComentario) {
        printf("Nao ha filmes com comentarios!\n");
    }
}

void deletarFilme(FILE *arq) {
    int id;
    printf("Digite o ID do filme a ser removido: ");
    scanf("%d", &id);

    Filme filme;
    int encontrouFilme = 0;

    FILE *tempArq = fopen("temp.bin", "wb"); // Abrir um arquivo temporario para escrita

    rewind(arq);
    // Percorrer o arquivo para encontrar o filme pelo ID e copiar os outros filmes para o arquivo temporario
    while (fread(&filme, sizeof(Filme), 1, arq) == 1) {
        if (filme.id == id) {
            encontrouFilme = 1;
        } else {
            fwrite(&filme, sizeof(Filme), 1, tempArq); // Copiar o filme para o arquivo temporario
        }
    }

    fclose(arq);
    fclose(tempArq);

    if (encontrouFilme) {
        remove("filmes.bin"); // Remover o arquivo original
        rename("temp.bin", "filmes.bin"); // Renomear o arquivo temporario para substituir o arquivo original
        printf("Filme removido com sucesso!\n");
    } else {
        remove("temp.bin"); // Remover o arquivo temporario
        printf("Nao existe filme com esse ID na sua lista!\n");
    }

    arq = fopen("filmes.bin", "rb+"); // Abrir novamente o arquivo original para leitura e escrita
}

void editarFilme(FILE *arq) {
    int id;
    printf("Digite o ID do filme a ser editado: ");
    scanf("%d", &id);

    Filme filme;
    int encontrouFilme = 0;

    rewind(arq);

    // Percorrer o arquivo para encontrar o filme pelo ID
    while (fread(&filme, sizeof(Filme), 1, arq) == 1) {
        if (filme.id == id) {
            encontrouFilme = 1;

            int opcao;

            printf("Filme encontrado:\n");
            printf("ID: %d\n", filme.id);
            printf("Nome: %s\n", filme.nome);
            printf("Ano de lancamento: %d\n", filme.ano);
            printf("Diretor: %s\n", filme.diretor);
            printf("Nota que voce deu: %.2f\n", filme.media_avaliacao);
            printf("Comentario: %s\n", filme.comentario);
            printf("\n");

            printf("O que deseja editar?\n");
            printf("1. Nome\n");
            printf("2. Ano de lancamento\n");
            printf("3. Diretor\n");
            printf("4. Nota de avaliacao\n");
            printf("5. Comentario\n");
            printf("0. Sair\n");
            printf("Digite a opcao desejada: ");
            scanf("%d", &opcao);

            switch (opcao) {
                case 1:
                    printf("Digite o novo nome: ");
                    getchar(); // Consumir o caractere de nova linha pendente
                    fgets(filme.nome, sizeof(filme.nome), stdin);
                    filme.nome[strcspn(filme.nome, "\n")] = '\0'; // Remover o caractere de nova linha lido pelo fgets
                    break;
                case 2:
                    printf("Digite o novo ano de lancamento: ");
                    scanf("%d", &filme.ano);
                    break;
                case 3:
                    printf("Digite o novo diretor: ");
                    getchar(); // Consumir o caractere de nova linha pendente
                    fgets(filme.diretor, sizeof(filme.diretor), stdin);
                    filme.diretor[strcspn(filme.diretor, "\n")] = '\0'; // Remover o caractere de nova linha lido pelo fgets
                    break;
                case 4:
                    // Validar a nova media de avaliacao
                    while (1) {
                        printf("Digite a nova nota (0.5 a 5.0): ");
                        scanf("%f", &filme.media_avaliacao);

                        if (filme.media_avaliacao >= 0.5 && filme.media_avaliacao <= 5.0) {
                            break;
                        } else if (filme.media_avaliacao > 5.0) {
                            printf("So e permitido dar ate 5 estrelas!\n");
                        } else if (filme.media_avaliacao < 0.5) {
                            printf("E preciso dar pelo menos 0.5!\n");
                        }
                    }
                    break;
                case 5:
                    printf("Digite o novo comentario (maximo 400 caracteres): ");
                    getchar(); // Consumir o caractere de nova linha pendente
                    fgets(filme.comentario, sizeof(filme.comentario), stdin);
                    filme.comentario[strcspn(filme.comentario, "\n")] = '\0'; // Remover o caractere de nova linha lido pelo fgets
                    break;
                case 0:
                    return;
                default:
                    printf("Opcao invalida!\n");
                    break;
            }

            fseek(arq, -sizeof(Filme), SEEK_CUR); // Voltar para a posicao correta no arquivo
            fwrite(&filme, sizeof(Filme), 1, arq); // Atualizar o registro do filme no arquivo
            printf("Informacoes do filme atualizadas com sucesso!\n");
            break;
        }
    }

    if (!encontrouFilme) {
        printf("Nao existe filme com esse ID na sua lista!\n");
    }
}

int main() {
    setlocale(LC_ALL, ""); // Definir a localidade correta para exibir acentos corretamente
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
        printf("7. Ver comentarios\n");
        printf("8. Editar filme\n");
        printf("0. Sair\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarFilme(arq); // Chama a função para cadastrar um novo filme
                break;
            case 2:
                listarFilmes(arq); // Chama a função para listar os filmes cadastrados
                break;
            case 3:
                favoritarFilme(arq); // Chama a função para favoritar um filme
                break;
            case 4:
                listarFavoritos(arq); // Chama a função para listar os filmes favoritos
                break;
            case 5:
                removerFavorito(arq); // Chama a função para remover um filme dos favoritos
                break;
            case 6:
                deletarFilme(arq); // Chama a função para deletar um filme
                break;
            case 7:
                verComentarios(arq); // Chama a função para ver os filmes com comentários
                break;
            case 8:
                editarFilme(arq); // Chama a função para editar um filme
                break;
            case 0:
                fclose(arq);
                return 0; // Encerra o programa
            default:
                printf("Opcao invalida!\n");
                break;
        }
    }
}
