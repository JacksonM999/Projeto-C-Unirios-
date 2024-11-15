#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Definir a estrutura da ficha de matricula
typedef struct Matricula {
    int id, renach;
    char nome[50], habil[50], cpf[15];
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

// Função para criar uma nova matricula
Matricula* criarMat(int id, const char* cpf, int renach, const char* nome, const char* habil) {
    Matricula* novo = (Matricula*)malloc(sizeof(Matricula));
    if (novo == NULL) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        exit(1);
    }
    novo->id = id;
    strcpy(novo->cpf, cpf);
    novo->renach = renach;
    strcpy(novo->nome, nome);
    strcpy(novo->habil, habil);
    novo->prox = NULL;
    return novo;
}

// Função para iniciar a fila
void startFile(File* file) {
    file->inicio = NULL;
    file->fim = NULL;
    file->proxId = 1; // Inicia com 1, mas pode ser alterado se houver matrículas carregadas

    // Carregar as matrículas do arquivo, para garantir que o proxId será correto
    loadFromFile(file);
}

// Função para adicionar a matricula na fila
void addFile(File* file, const char* cpf, int renach, const char* nome, const char* habil) {
    Matricula* novo = criarMat(file->proxId, cpf, renach, nome, habil);
    if (file->fim == NULL) {
        file->inicio = novo;
        file->fim = novo;
    } else {
        file->fim->prox = novo;
        file->fim = novo;
    }
    printf("Matricula de '%s' adicionada à fila.\n", nome);
    file->proxId++; // Incrementa o proxId para o próximo
    // Verifique os valores atribuídos:
    printf("Nome: %s, Habilitação: %s\n", novo->nome, novo->habil);
}

// Função para listar todas as matriculas na fila
void listFile(File* file) {
    if (file->inicio == NULL) {
        printf("A lista de matrículas está vazia.\n");
        return;
    }
    Matricula* temp = file->inicio;
    printf("As matrículas na fila são:\n");
    while (temp != NULL) {
        printf("ID: %d, CPF: %s, Renach: BA%d, Nome: %s, Habilitação: %s\n", temp->id, temp->cpf, temp->renach, temp->nome, temp->habil);
        temp = temp->prox;
    }
}

// Função para editar uma matricula
void editMatricula(File* file, int id) {
    Matricula* temp = file->inicio;
    while (temp != NULL) {
        if (temp->id == id) {
            char nome[50], habil[50], cpf[15];
            int renach;
            printf("Digite o novo nome do aluno: ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = 0;  // Remove a nova linha, se houver

            printf("Digite o novo CPF do aluno: ");
            fgets(cpf, sizeof(cpf), stdin);
            cpf[strcspn(cpf, "\n")] = 0;  // Remove a nova linha, se houver

            printf("Digite o novo Renach do aluno: BA");
            scanf("%d", &renach);
            getchar();  // Limpar o '\n' do buffer

            printf("Digite a nova habilitação do aluno: ");
            fgets(habil, sizeof(habil), stdin);
            habil[strcspn(habil, "\n")] = 0;  // Remove a nova linha, se houver

            // Verifique as atribuições
            printf("Editando matrícula: Nome: %s, Habilitação: %s\n", nome, habil);

            strcpy(temp->nome, nome);
            strcpy(temp->cpf, cpf);
            temp->renach = renach;
            strcpy(temp->habil, habil);
            printf("Matricula com ID %d editada com sucesso.\n", id);
            return;
        }
        temp = temp->prox;
    }
    printf("Matricula com ID %d não encontrada.\n", id);
}

// Função para apagar uma matricula
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

// Função para salvar as matriculas em um arquivo
void saveToFile(File* file) {
    FILE* fp = fopen("matriculas.txt", "w");
    if (fp == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo para salvar.\n");
        return;
    }
    Matricula* temp = file->inicio;
    while (temp != NULL) {
        // Verifique os valores antes de gravar no arquivo
        printf("Salvando: ID=%d, CPF=%s, Renach=BA%d, Nome=%s, Habilitação=%s\n", 
               temp->id, temp->cpf, temp->renach, temp->nome, temp->habil);
        
        // Grava no arquivo
        fprintf(fp, "%d %s %d %s %s\n", temp->id, temp->cpf, temp->renach, temp->nome, temp->habil);
        temp = temp->prox;
    }
    fclose(fp);
    printf("Matrículas salvas com sucesso.\n");
}

// Função para carregar as matriculas de um arquivo
void loadFromFile(File* file) {
    FILE* fp = fopen("matriculas.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo para carregar.\n");
        return;
    }

    char linha[1000]; // Buffer para ler as linhas
    while (fgets(linha, sizeof(linha), fp) != NULL) {
        int id, renach;
        char cpf[15], nome[50], habil[50];

        // Usando sscanf para ler id, cpf e renach (os três primeiros campos)
        if (sscanf(linha, "%d %s %d", &id, cpf, &renach) == 3) {
            // Agora, encontrar a parte do nome (que pode ter espaços)
            char* nomeStart = strchr(linha, ' ') + 1;  // Encontra o primeiro espaço após o ID
            nomeStart = strchr(nomeStart, ' ') + 1;     // Encontra o espaço após o CPF

            // Encontrar onde começa a habilitação (após o nome)
            char* habilStart = strrchr(linha, ' ') + 1; // Encontrar o último espaço na linha (antes da habilitação)

            // Agora, copiamos o nome e a habilitação
            int nomeLen = habilStart - nomeStart - 1;  // Calcula o comprimento do nome
            strncpy(nome, nomeStart, nomeLen);         // Copia o nome para a variável nome
            nome[nomeLen] = '\0';                      // Garante que o nome está corretamente finalizado

            // Copiar a habilitação (o restante da linha após o nome)
            strcpy(habil, habilStart);

            // Adiciona a matrícula à fila
            addFile(file, cpf, renach, nome, habil);

            // Ajusta o proxId para o próximo número após o maior id encontrado
            if (id >= file->proxId) {
                file->proxId = id + 1;
            }
        }
    }
    fclose(fp);
    printf("Matrículas carregadas com sucesso.\n");
}


// Função Principal com o menu
int main() {
    setlocale(LC_ALL, "Portuguese");
    File file;
    startFile(&file); // Carregar matrículas ao iniciar e definir proxId

    char nome[50], habil[50], cpf[15];
    int option, renach;

    while (1) {
        printf("\nBem-vindo ao sistema.\n");
        printf("Escolha uma das opções:\n");
        printf("1 - Adicionar Matrícula\n");
        printf("2 - Mostrar Matrículas\n");
        printf("3 - Editar Matrícula\n");
        printf("4 - Apagar Matrícula\n");
        printf("5 - Salvar Matrículas\n");
        printf("6 - Sair\n");
        scanf("%d", &option);
        
        while (getchar() != '\n');  // Limpa o buffer de entrada

        switch (option) {
            case 1:
                printf("Digite o nome do aluno: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;

                printf("Digite o CPF do aluno: ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = 0;
                
                printf("\nExemplo de Renach: BA712000000\n");
                printf("Digite o Renach do aluno: BA");
                scanf("%d", &renach);
                getchar();  // Limpa o \n deixado pelo scanf

                printf("\nObs: Primeira Categoria AB, A, B, Adição de Categoria A, Adição de Categoria B, Mudança de Categoria D.\n");
                printf("Digite a categoria do aluno: ");
                fgets(habil, sizeof(habil), stdin);
                habil[strcspn(habil, "\n")] = 0;
                
                addFile(&file, cpf, renach, nome, habil);
                break;
            case 2:
                listFile(&file);
                break;
            case 3:
                printf("Digite o ID da matrícula que deseja editar: ");
                scanf("%d", &option);
                getchar(); // Limpa o \n
                editMatricula(&file, option);
                break;
            case 4:
                printf("Digite o ID da matrícula que deseja remover: ");
                scanf("%d", &option);
                removeMatricula(&file, option);
                break;
            case 5:
                saveToFile(&file);
                break;
            case 6:
                printf("Programa finalizado com sucesso.\n");
                exit(0);
            default:
                printf("Opção inválida.\n");
        }
    }
    return 0;
}

