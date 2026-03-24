#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
Nesta parte do código precisamos inserir um limpeza de buffer
pois com alguns testes preticos obvservamos que as entradas do teclado
estavam atrapalhando a inserção de novos nós.
*/
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 Aqui ele faz a validação para ver se o usuário inseriu um 
 valor inteiro e usa a função comentada anteriormente auxiliar no processo
*/
int lerInt(const char *msg) {
    int valor;
    while (1) {
        printf("%s", msg);
        if (scanf("%d", &valor) == 1) { limparBuffer(); return valor; }
        limparBuffer();
        printf("Entrada invalida. Digite um numero inteiro.\n");
    }
}
// mesma coisa do metodo antiror porém para Float
float lerFloat(const char *msg) {
    float valor;
    while (1) {
        printf("%s", msg);
        if (scanf("%f", &valor) == 1) { limparBuffer(); return valor; }
        limparBuffer();
        printf("Entrada invalida. Digite um numero.\n");
    }
}

 
/*
Essa é a estrutura principal do projeto, 
ela é um struct que contém, os dados do funcionário
e dois ponteiros para outras duas structs, que representam 
o nó a esqueda e o nó a direita (os filhos desse nó) com o
 intuito principal de ordenar os valores automaticamente no momento da incersão 
*/
typedef struct No {
    int matricula;
    char nome[100];
    char cargo[100];
    float salario;
    struct No *left;
    struct No *right;
} No;

/*
Isso vai cirar um nó: Inicialmente reserva um espaço na memória com
a função malloc e preenche os valores do novo nó com as informações 
do funcionário passados por parametro. Inicialmente os nos a esquerda 
e a direita são nullos pois ainda não sabemos se esse tera um nó filho.
*/
No *criarNo(int matricula, char *nome, char *cargo, float salario) {
    No *novo = (No *)malloc(sizeof(No));
    novo->matricula = matricula;
    strcpy(novo->nome, nome);
    strcpy(novo->cargo, cargo);
    novo->salario = salario;
    novo->left = NULL;
    novo->right = NULL;
    return novo;
}

/*
Aqui ele vai ter duas funções, quando for chamado a primeira vez o nó
 sempre vai ser null, então ele vai criar usando a função anterior e retornar 1
 e quando não for null, ou seja, nossa estrutura já foi iniciada, ele chama a mesma função 
porém passando o nó a esquerda ou a direita dependendo se o valor da matricula é menor ou maior
e é dessa forma que ele faz a ordenação: pela matricula
*/
int inserir(No **raiz, int matricula, char *nome, char *cargo, float salario) {
    if (*raiz == NULL) {
        *raiz = criarNo(matricula, nome, cargo, salario);
        return 1;
    }
    if (matricula < (*raiz)->matricula)
        return inserir(&(*raiz)->left, matricula, nome, cargo, salario);
    else if (matricula > (*raiz)->matricula)
        return inserir(&(*raiz)->right, matricula, nome, cargo, salario);
    else
        return 0;
}

/*
Aqui é uma implementação de busca binária, se for o valor ele 
vai retornar um ponteiro para o nó, caso contrário ele vai verificar 
se é menor ou maior, decidindo dessa forma se o proximo que ele vai 
verificar, recursivamente, é o da esquerda ou o da direita  
*/
No *buscar(No *raiz, int matricula) {
    if (raiz == NULL || raiz->matricula == matricula)
        return raiz;
    if (matricula < raiz->matricula)
        return buscar(raiz->left, matricula);
    return buscar(raiz->right, matricula);
}

/*
Atualiza o cadastro de um funcionário ja existente, para isso 
ele primeiro vai utilizar a função de busca que comentamos anteriormente, para ver
ser o nó realmente existe e se estamos alterando o funcionário certo. Depois de
encontrado apenas alteramos os dados fazendo um overwrite.
*/ 
void atualizar(No *raiz, int matricula) {
    No *no = buscar(raiz, matricula);
    if (no == NULL) { printf("Funcionario nao encontrado.\n"); return; }

    printf("Novo nome: ");
    scanf(" %[^\n]", no->nome);
    printf("Novo cargo: ");
    scanf(" %[^\n]", no->cargo);
    no->salario = lerFloat("Novo salario: ");
    printf("Dados atualizados com sucesso.\n");
}

/*
Listamos todos os dados ja cadastrados utilizando o método in-order, ou seja,
primeiro apresentando os nós a esquerda, depois o nó pai e por fim os nós maiores
que o pai (a direita) de forma recursiva.
*/
void listar(No *raiz) {
    if (raiz == NULL) return;
    listar(raiz->left);
    printf("Matricula: %d | Nome: %s | Cargo: %s | Salario: %.2f\n",
           raiz->matricula, raiz->nome, raiz->cargo, raiz->salario);
    listar(raiz->right);
}
/*Esse não tem muito o que falar, ele tá usando todos os metodos que 
comentamos usando uma lógica bem basica atraves de um switch case para escolher a operação
 */
int main() {
    No *raiz = NULL;
    int opcao, matricula;
    char nome[100], cargo[100];
    float salario;

    do {
        opcao = lerInt("\n1. Inserir\n2. Buscar\n3. Atualizar\n4. Listar\n0. Sair\nOpcao: ");

        switch (opcao) {
        case 1:
            matricula = lerInt("Matricula: ");
            printf("Nome: ");
            scanf(" %[^\n]", nome);
            printf("Cargo: ");
            scanf(" %[^\n]", cargo);
            salario = lerFloat("Salario: ");
            if (inserir(&raiz, matricula, nome, cargo, salario))
                printf("Funcionario inserido.\n");
            else
                printf("Matricula %d ja cadastrada.\n", matricula);
            break;

        case 2:;
            int mat2 = lerInt("Matricula: ");
            No *resultado = buscar(raiz, mat2);
            if (resultado)
                printf("Nome: %s | Cargo: %s | Salario: %.2f\n",
                       resultado->nome, resultado->cargo, resultado->salario);
            else
                printf("Funcionario nao encontrado.\n");
            break;

        case 3:
            atualizar(raiz, lerInt("Matricula: "));
            break;

        case 4:
            listar(raiz);
            break;
        }
    } while (opcao != 0);

    return 0;
}