#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Definir a estrutura da ficha de matrícula
typedef struct Matricula {
    int id;
    char habil[50], cpf[20], renach[20], nome[50];  // Nome agora é o último
    struct Matricula* prox;
} Matricula;

// Definir a estrutura da fila
typedef struct File {
    Matricula* inicio;
    Matricula* fim;
    int proxId; // Variável para armazenar o próximo id
} File;

// Declaração da função loadFromFile (importante para que ela seja reconhecida no código)
void loadFromFile(File* file);

// Função para criar uma nova matrícula
Matricula* criarMat(int id, const char* habil, const char* cpf, const char* renach, const char* nome) {
    Matricula* novo = (Matricula*)malloc(sizeof(Matricula));
    if (novo == NULL) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        exit(1);
    }
    novo->id = id;
    strcpy(novo->habil, habil);
    strcpy(novo->cpf, cpf);
    strcpy(novo->renach, renach);
    strcpy(novo->nome, nome);  // Nome agora é o último
    novo->prox = NULL;
    return novo;
}

// Função para iniciar a fila
void startFile(File* file) {
    file->inicio = NULL;
    file->fim = NULL;
    file->proxId = 1; // O ID inicia com 1, mas pode ser alterado se houver outras matrículas carregadas

    // Carregar as matrículas do arquivo, para garantir que o proxId será correto
    loadFromFile(file);
}

// Função para adicionar a matrícula na fila
void addFile(File* file, const char* habil, const char* cpf, const char* renach, const char* nome) {
    Matricula* novo = criarMat(file->proxId, habil, cpf, renach, nome);
    if (file->fim == NULL) {
        file->inicio = novo;
        file->fim = novo;
    } else {
        file->fim->prox = novo;
        file->fim = novo;
    }
    printf("Matrícula de '%s' adicionada à fila.\n", nome);
    file->proxId++; // Incrementa o proxId para o próximo
    // Verifique os valores atribuídos:
    printf("Nome: %s, Habilitação: %s, RENACH: %s\n", novo->nome, novo->habil, novo->renach);
}

// Função para listar todas as matrículas na fila
void listFile(File* file) {
    if (file->inicio == NULL) {
        printf("A lista de matrículas está vazia.\n");
        return;
    }
    Matricula* temp = file->inicio;
    printf("As matrículas na fila são:\n");
    while (temp != NULL) {
        printf("ID: %d, CPF: %s, Nome: %s, Habilitação: %s, RENACH: %s\n", temp->id, temp->cpf, temp->nome, temp->habil, temp->renach);
        temp = temp->prox;
    }
}

// Função para editar uma matrícula
void editMatricula(File* file, int id) {
    Matricula* temp = file->inicio;
    while (temp != NULL) {
        if (temp->id == id) {
            char habil[50], cpf[20], renach[20], nome[50];
            printf("Digite o novo nome do aluno: ");
            fgets(nome, sizeof(nome), stdin); 
            nome[strcspn(nome, "\n")] = 0;  // Remove a nova linha, se houver

            printf("Digite o novo CPF do aluno: ");
            fgets(cpf, sizeof(cpf), stdin);
            cpf[strcspn(cpf, "\n")] = 0;  

            printf("Digite a nova habilitação do aluno: ");
            fgets(habil, sizeof(habil), stdin);
            habil[strcspn(habil, "\n")] = 0;  

            printf("Digite o novo RENACH: ");
            fgets(renach, sizeof(renach), stdin);
            renach[strcspn(renach, "\n")] = 0;  

            // Verifique as atribuições
            printf("Editando matrícula: Nome: %s, Habilitação: %s, RENACH: %s\n", nome, habil, renach);

            strcpy(temp->nome, nome);
            strcpy(temp->cpf, cpf);
            strcpy(temp->habil, habil);
            strcpy(temp->renach, renach); 
            printf("Matrícula com ID %d editada com sucesso.\n", id);
            return;
        }
        temp = temp->prox;
    }
    printf("Matrícula com ID %d não encontrada.\n", id);
}

// Função para apagar uma matrícula
void removeMatricula(File* file, int id) {
    Matricula* temp = file->inicio;
    Matricula* anterior = NULL;

    while (temp != NULL) {
        if (temp->id == id) {
            if (anterior == NULL) {
                file->inicio = temp->prox;
                if (file->inicio == NULL) {
                    file->fim = NULL; 
                }
            } else {
                anterior->prox = temp->prox;
                if (temp->prox == NULL) {
                    file->fim = anterior; 
                }
            }
            free(temp);
            printf("Matrícula com ID %d removida com sucesso.\n", id);
            return;
        }
        anterior = temp;
        temp = temp->prox;
    }
    printf("Matrícula com ID %d não encontrada.\n", id);
}

// Função para salvar as matrículas em um arquivo
void saveToFile(File* file) {
    FILE* fp = fopen("matriculas.txt", "w");
    if (fp == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo para salvar.\n");
        return;
    }
    Matricula* temp = file->inicio;
    while (temp != NULL) {
        // Verifique os valores antes de gravar no arquivo
        printf("Salvando: ID=%d, CPF=%s, Nome=%s, Habilitação=%s, RENACH=%s\n", 
               temp->id, temp->cpf, temp->nome, temp->habil, temp->renach);
        
        // Grava no arquivo
        fprintf(fp, "%d %s %s %s %s\n", temp->id, temp->cpf, temp->habil, temp->renach, temp->nome);
        temp = temp->prox;
    }
    fclose(fp);
    printf("Matrículas salvas com sucesso.\n");
}

// Função para carregar as matrículas de um arquivo
void loadFromFile(File* file) {
    FILE* fp = fopen("matriculas.txt", "r");  // Abrir o arquivo para leitura
    if (fp == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo para carregar.\n");
        return;
    }

    char linha[1000];  // Buffer para armazenar cada linha lida do arquivo
    int idMax = file->proxId;  // Inicia o idMax com o valor de proxId

    while (fgets(linha, sizeof(linha), fp) != NULL) {
        int id;
        char cpf[20], habil[100], renach[20], nome[100];

        // Usar sscanf para ler os primeiros campos (id, cpf, habilitação, renach)
        if (sscanf(linha, "%d %s %s %s", &id, cpf, habil, renach) == 4) {
        	// Foi necessario fazer o nome ser pulado varias vezes para não dar problema na hora de colocar um nome longo, procurar outro metodo de resolver isso no futuro.
            char* nomeInicio = strchr(linha, ' ');  // Pula o ID
            nomeInicio = strchr(nomeInicio + 1, ' ');  // Pula o CPF
            nomeInicio = strchr(nomeInicio + 1, ' ');  // Pula a Habilitação
            nomeInicio = strchr(nomeInicio + 1, ' ');  // Pula o RENACH

            if (nomeInicio != NULL) {
                // O nome começa após o espaço encontrado
                strcpy(nome, nomeInicio + 1);  // Copiar o nome completo para que deixe de ter problema de separação no nome ou que ele apareça em outras variaveis.
                nome[strcspn(nome, "\n")] = 0;  // Remover a quebra de linha
            }

            // Adiciona a matrícula à fila
            addFile(file, habil, cpf, renach, nome);

            // Atualiza o proxId para garantir que seja maior que o ID atual
            if (id >= idMax) {
                idMax = id + 1;  // Ajusta proxId para o próximo ID disponível
            }
        }
    }

    file->proxId = idMax;  // Ajusta o proxId após carregar as matrículas
    fclose(fp);
    printf("Matrículas carregadas com sucesso.\n");
}

// Função para ler uma opção do menu e verificar se é válida
int lerOpcao() {
    int opcao;
    while (1) {
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            // Se a leitura falhar, limpa o buffer e pede para tentar novamente
            while (getchar() != '\n');  // Limpa o buffer de entrada
            printf("Opção inválida! Por favor, insira um número entre 1 e 5.\n");
        } else {
            // Limpar o buffer de entrada após ler a opção
            while (getchar() != '\n');  // Limpar o '\n' deixado no buffer
            break; // Se a opção foi lida com sucesso, sai do loop
        }
    }
    return opcao;
}

// Função Principal com o menu
int main() {
    setlocale(LC_ALL, "Portuguese");
    File file;
    startFile(&file);

    char nome[50], habil[50], cpf[20], renach[20];

    int opcao;
    do {
        printf("\nMenu de opções:\n");
        printf("1. Adicionar Matrícula\n");
        printf("2. Listar Matrículas\n");
        printf("3. Editar Matrícula\n");
        printf("4. Remover Matrícula\n");
        printf("5. Salvar e Sair\n");

        opcao = lerOpcao();  // Chama a função que garante a leitura correta da opção

        switch (opcao) {
            case 1:
                // A seguir, use getchar() para limpar antes de capturar strings
                printf("Digite a habilitação: ");
                fgets(habil, sizeof(habil), stdin);
                habil[strcspn(habil, "\n")] = 0;  // Remove a nova linha

                printf("Digite o CPF: ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = 0;  // Remove a nova linha

                printf("Digite o nome do aluno: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;

                printf("Digite o RENACH: ");
                fgets(renach, sizeof(renach), stdin);
                renach[strcspn(renach, "\n")] = 0;

                addFile(&file, habil, cpf, renach, nome);
                break;

            case 2:
                listFile(&file);
                break;

            case 3:
                printf("Digite o ID da matrícula para editar: ");
                int id;
                scanf("%d", &id);
                getchar();  // Limpa o \n deixado pelo scanf
                editMatricula(&file, id);
                break;

            case 4:
                printf("Digite o ID da matrícula para remover: ");
                scanf("%d", &id);
                getchar();  // Limpa o \n deixado pelo scanf
                removeMatricula(&file, id);
                break;

            case 5:
                saveToFile(&file);
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 5);

    return 0;
}
