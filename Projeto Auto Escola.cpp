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
} File;

// Função para criar uma nova matricula
Matricula* criarMat(int id, const char* cpf, int renach, const char* nome, const char* habil) {
    Matricula* novo = (Matricula*)malloc(sizeof(Matricula));
    if (novo == NULL) {
        fprintf(stderr, "Erro ao alocar memoria. \n");
        exit(1);
    }
    novo->id = id;
    strcpy(novo->cpf, cpf); // strcpy ou string copy vai copiar o char CPF e deixar como string.
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
}

// Função para adicionar a matricula na fila
void addFile(File* file, int id, const char* cpf, int renach, const char* nome, const char* habil) {
    Matricula* novo = criarMat(id, cpf, renach, nome, habil);
    if (file->fim == NULL) {
        file->inicio = novo;
        file->fim = novo;
    } else {
        file->fim->prox = novo;
        file->fim = novo;
    }
    printf("Matricula de '%s' adicionada à fila. \n", nome);
}

// Função para listar todos as matriculas na fila
void listFile(File* file) {
    if (file->inicio == NULL) {
        printf("A lista de matricula está vazia.\n");
        return;
    }
    Matricula* temp = file->inicio;
    printf("As matriculas na fila são: \n");
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
            nome[strcspn(nome, "\n")] = 0; // utiliza o strcspn para poder remover a linha criada no char para que não ocorra repetições no codigo.

            printf("Digite o novo CPF do aluno: ");
            fgets(cpf, sizeof(cpf), stdin);
            cpf[strcspn(cpf, "\n")] = 0; // Remove a nova linha

            printf("Digite o novo Renach do aluno: BA");
            scanf("%d", &renach);
            getchar(); // Limpa o buffer do stdin

            printf("Digite a nova habilitação do aluno: ");
            fgets(habil, sizeof(habil), stdin);
            habil[strcspn(habil, "\n")] = 0; // Remove a nova linha
            
            // Atualiza os dados utilizando o string copy e o temp.
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
            if (anterior == NULL) { //Utilizando o ID das matriculas eu vou selecionar e apagar a matricula desejada
                file->inicio = temp->prox;
                if (file->inicio == NULL) {
                    file->fim = NULL; // Deixar a lista vazia 
                }
            } else {
                anterior->prox = temp->prox;
                if (temp->prox == NULL) {
                    file->fim = anterior; 
                }
            }
            free(temp);
            printf("Matricula com ID %d removida com sucesso.\n", id);
            return;
        }
        anterior = temp;
        temp = temp->prox;
    }
    printf("Matricula com ID %d não encontrada.\n", id); //Caso não tenha nada vai retornar essa mensagem
}

// Função Principal com o menu
int main() {
	setlocale(LC_ALL, "Portuguese"); //Fazer o codigo aceitar o PTBR
    File file;
    startFile(&file);
    char nome[50], habil[50], cpf[15];
    int option, id = 1, renach;

    while (1) {
        printf("\nBem-vindo ao sistema.\n");
        printf("Escolha uma das opções: \n");
        printf("1 - Adicionar Matricula \n");
        printf("2 - Mostrar Matriculas \n");
        printf("3 - Editar Matricula \n");
        printf("4 - Apagar Matricula \n");
        printf("5 - Sair \n");
        scanf("%d", &option);
        
        // Limpar o buffer do stdin, sem utilizar essa limpeza o codigo acabava causando a repetição do menu de opções depois da criação de uma matricula.
        while (getchar() != '\n');

        switch (option) {
            case 1:
                printf("Digite o nome do aluno: ");
                fgets(nome, sizeof(nome), stdin); //fgets é utilizado para poder criar um char onde pode ser utilizado o espaçamento, nesse caso utilizei para poder colocar nomes.
                nome[strcspn(nome, "\n")] = 0; // Remove a nova linha

                printf("Digite o CPF do aluno: ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = 0; // Remove a nova linha
				
				printf("\nExemplo de Renach: BA712000000 \n");
                printf("Digite o Renach do aluno: BA");
                scanf("%d", &renach);
                getchar(); // Limpa o buffer do stdin

                printf("\nObs: Primeira Categoria AB, A, B, Adição de Categoria A, Adição de Categoria B, Mudança de Categoria D.\n");
                printf("Digite a categoria do aluno: ");
                fgets(habil, sizeof(habil), stdin);
                habil[strcspn(habil, "\n")] = 0; // Remove a nova linha
                
                addFile(&file, id++, cpf, renach, nome, habil);
                break;
            case 2:
                listFile(&file);
                break;
            case 3:
                printf("Digite o ID da matricula que deseja editar: ");
                scanf("%d", &id);
                getchar(); // Limpa o buffer do stdin
                editMatricula(&file, id);
                break;
            case 4:
                printf("Digite o ID da matricula que deseja remover: ");
                scanf("%d", &id);
                removeMatricula(&file, id);
                break;
            case 5:
                printf("Programa finalizado com sucesso. \n");
                exit(0);
            default:
                printf("Opção inválida. \n");
        }
    }
    return 0;
}


