# Projeto Unirios - Sistema de cadastro de matriculas
## O projeto foi feito utilizando o dev-C++ 

- O codigo foi utilizando o sistema de listas para poder organizar as matriculas, aqui uma imagem mostrando o menu do codigo onde o usuario vai selecionar uma das opções para poder verificar as matriculas.

![image](https://github.com/user-attachments/assets/3eb66f3d-968c-43d1-9179-e876b52ddf44)

- É utilizado um repositorio para poder guardar as matriculas já feitas para que o usuario possa quando quiser verificar as matriculas.

28/11

O codigo está atualizado com o repositorio funcionando.

Exemplo do codigo atualizado, onde já mostra o carregamento funcionando.
![image](https://github.com/user-attachments/assets/6d7c4a59-c215-483b-9a40-d12bb4fad511)

-----------------
Estrutura completa do codigo:
----------------

### **1. Estruturas de Dados**

- **`Matricula`**: Estrutura que representa os dados de uma matrícula de um aluno.
  - **Campos**:
    - `id`: Identificador único da matrícula.
    - `habil`: Habilitação do aluno (por exemplo, primeira habilitação AB ou simplismente só colocar AB).
    - `cpf`: CPF do aluno.
    - `renach`: Codigo utilizado pelo DETRAN ao obter o laudo da primeira habilitação ou mudança de categoria.
    - `nome`: Nome completo do aluno.
    - `prox`: Ponteiro utilizado para definir a próxima matrícula.

- **`File`**: Estrutura que representa uma fila de matrículas.
  - **Campos**:
    - `inicio`: Ponteiro criado para informar o início da fila.
    - `fim`: Ponteiro criado para definir o final da fila.
    - `proxId`: Variável para rastrear o próximo ID disponível para uma nova matrícula.

### **2. Funções**

- **`criarMat`**: Cria uma nova matrícula, alocando memória para ela e preenchendo os dados.
  - **Parâmetros**: ID, habilitação, CPF, RENACH, nome.
  - **Retorno**: Ponteiro para a nova matrícula.

- **`startFile`**: Inicializa a fila (o inicio fica como `NULL`, o fim como `NULL` e o `proxId` é iniciado com 1).
  - Também carrega as matrículas de um arquivo, garantindo que o `proxId` seja correto.

- **`addFile`**: Adiciona uma nova matrícula à fila.
  - **Parâmetros**: Habilitação, CPF, RENACH, nome.
  - **Ações**: Cria a matrícula com os dados fornecidos e a insere no final da fila. O `proxId` é incrementado.

- **`listFile`**: Lista todas as matrículas presentes na fila.
  - **Ações**: Percorre a fila e exibe os dados de cada matrícula.

- **`editMatricula`**: Permite editar os dados de uma matrícula existente.
  - **Parâmetros**: ID da matrícula a ser editada.
  - **Ações**: Busca a matrícula pelo ID e permite ao usuário atualizar os dados (nome, CPF, habilitação, RENACH).

- **`removeMatricula`**: Remove uma matrícula da fila.
  - **Parâmetros**: ID da matrícula a ser removida.
  - **Ações**: Percorre a fila e remove o nó correspondente ao ID informado.

- **`saveToFile`**: Salva todas as matrículas presentes na fila em um arquivo de texto.
  - **Ações**: Abre o arquivo `matriculas.txt` para escrita e grava os dados das matrículas no formato: `ID CPF Habilitação RENACH Nome`.

- **`loadFromFile`**: Carrega as matrículas de um arquivo de texto e as insere na fila.
  - **Ações**: Abre o arquivo `matriculas.txt` para leitura, lê as matrículas e as insere na fila. Ajusta o `proxId` para o próximo ID disponível.

- **`lerOpcao`**: Lê a opção escolhida pelo usuário no menu, garantindo que a entrada seja válida.
  - **Retorno**: Número da opção escolhida.

### **3. Função Principal (`main`)**

A função principal executa o programa de acordo com as opções do menu:

- **Opções do menu**:
  1. **Adicionar Matrícula**: Solicita os dados do aluno e no final adiciona a matrícula à fila.
  2. **Listar Matrículas**: Exibe todas as matrículas presentes na fila.
  3. **Editar Matrícula**: Solicita o ID da matrícula e permite editar seus dados.
  4. **Remover Matrícula**: Solicita o ID da matrícula e a remove da fila.
  5. **Salvar e Sair**: Salva as matrículas no arquivo e finaliza o programa.

### **4. Informações Adicionais**

- O código utiliza **memória dinâmica** com a função `malloc` para alocar a estrutura de matrícula.
- A **fila é implementada como uma lista encadeada**, o que significa que as matrículas são armazenadas em nós (cada nó é um elemento da fila).
- O programa usa **funções auxiliares** como `fgets` para leitura de strings, garantindo que o programa não leia entradas inválidas ou com quebras de linha indesejadas.
- **`setlocale(LC_ALL, "Portuguese")`**: Essa linha define a localidade para o idioma português, garantindo que caracteres acentuados sejam tratados corretamente.

### **Fluxo de Operações**

1. O programa começa com o carregamento de matrículas de um arquivo, se disponível.
2. O menu principal é exibido continuamente até que o usuário opte por sair.
3. Dependendo da opção escolhida, o programa realiza as operações de adicionar, listar, editar, remover ou salvar as matrículas.
4. No final, ao escolher a opção 5, as matrículas são salvas no arquivo e o programa é encerrado.

### **Possíveis Melhorias e Observações**

- Melhorar o menu do projeto utilizando cores nos textos, utilizar formas de procurar as matrículas pelo nome, CPF ou RENACH, colocar uma opção que mostraria o resultado das provas (Que nem o do portal de serviços do DETRAN BA), arrumar o problema com a repetição do ID, etc. 
