#include "banco.h"
#define NAME_MAX_LEN 100  // tamanho max do nome
#define MAX_CLIENTS 100   // max de clintes da aplicacao
#define STATUS_MAX_LEN 20 // tamanho maximo do status
#define CPF_LEN 14        // tamanho do CPF

// struct de clientes
typedef struct
{
    // info do cliente
    char CPF[CPF_LEN + 1];
    char Name[MAX_CLIENTS + 1];
    int Age;
    // info da conta
    char Status[STATUS_MAX_LEN + 1];
    unsigned short AccountType; // 1 para corrente, 2 para poupanca
    int AccountNumber;
    double Balance;
} Clients;

Clients clients[MAX_CLIENTS]; // array de struct de usuarios
int num_of_clients;           // numero de usuarios atualmente no programa

int main(void)
{
    strcpy(clients[num_of_clients].Name, "Renato Oliveira");
    clients[num_of_clients].Age = 35;
    strcpy(clients[num_of_clients].CPF, "111.222.222-23");
    strcpy(clients[num_of_clients].Status, "ABERTA");
    clients[num_of_clients].AccountNumber = num_of_clients;
    clients[num_of_clients].AccountType = 1;
    clients[num_of_clients].Balance = 0.0;
    num_of_clients++; // registrar mais um cadastro de usuario
    main_menu();
}
// menu principal
void main_menu(void)
{
    bool continue_program = true;
    while (continue_program)
    {
        char option = '\0';
        printf("\n----Bem vindo ao Banco----\n");
        printf("Escolha uma das opcoes(com numero): \n");
        printf("Cadastrar cliente (1)\n");
        printf("Listar clientes (2)\n");
        printf("Realizar depósito (3)\n");
        printf("Realizar saque (4)\n");
        printf("Fechar Conta (5)\n");
        printf("Realizar Emprestimo\n");
        printf("Sair (0)\n");

        scanf(" %c", &option);
        switch (option)
        {
        case '0':
            continue_program = false;
            break;
        case '1':
            open_account();
            break;
        case '2':
            list_clients();
            break;
        case '3':
            deposit();
            break;
        case '4':
            withdraw();
            break;
        case '5':
            close_account();
            break;
        case '6':
            loan();
            break;
        default:
            printf("Opcao invalida.\n");
            break;
        }
    }
}
// funcao para abrir contas
void open_account(void)
{
    char name[NAME_MAX_LEN + 1];
    char cpf[CPF_LEN + 2]; // tamanho 16 pois: null char + caractere extra para validacao de input falhar caso maior que 14 = 16
    int age = 0;
    bool continue_registration = true; // flag de controle

    while (continue_registration)
    {
        printf("Digite seu nome completo: ");
        fgetc(stdin); // consome caracatere restante, fflush nao funcionou
        fgets(name, NAME_MAX_LEN, stdin);
        printf("Digite sua idade: ");
        scanf("%d", &age);
        if (age < 18)
        {
            printf("Cliente precisa ser maior de idade.\n");
            return; // retorna para o menu
        }

        while (true)
        {
            printf("Digite seu CPF(com pontos e hifen): ");
            scanf(" %16s", cpf);
            // validacao do formato do cpf(so compara se o tamanho = 14)
            if (is_cpf_format_valid(strlen(cpf)))
            {
                // se for valido, checar se ja esta cadastrado.
                if (does_client_already_exist(cpf))
                {
                    printf("Já existe um usuário cadastrado com este CPF.\n");
                    return;
                }
                else
                {
                    client_registration(name, age, cpf); // cadastrar cliente com dados ja pegos
                    account_registration();              // imediatamente apos, abrir conta
                    return;                              // voltar ao menu principal
                }
            }

            // se invalido, continue, aka: perguntar novamente
            else
            {
                printf("Formato de CPF inválido.\n");
                continue;
            }
        }
    }
}
// funcao para inserir usuario novo, parte do processo de abertura de conta
void client_registration(char *name, int age, char *cpf)
{
    strcpy(clients[num_of_clients].Name, name);
    clients[num_of_clients].Age = age;
    strcpy(clients[num_of_clients].CPF, cpf);
    printf("Cliente cadastrado com sucesso.\n");
}

void account_registration()
{
    bool continue_acc_regis_validation = true;
    while (continue_acc_regis_validation)
    {
        char option = '\0';
        printf("Deseja abrir qual tipo de conta (digite o numero equivalente)?\n");
        printf("Corrente (1)\n");
        printf("Poupanca (2)\n");

        scanf(" %c", &option);
        switch (option)
        {
        case '1':
            clients[num_of_clients].AccountType = 1; // corrrente
            continue_acc_regis_validation = false;
            break;
        case '2':
            clients[num_of_clients].AccountType = 2; // poupança
            continue_acc_regis_validation = false;
            break;
        default:
            printf("Tipo de conta invalida.");
            break;
        }
    }
    clients[num_of_clients].AccountNumber = num_of_clients;
    clients[num_of_clients].Balance = 0.0;
    strcpy(clients[num_of_clients].Status, "ABERTA");
    printf("Conta cadastrada com sucesso.\n");
    num_of_clients++; // registrar mais um cadastro de usuario
}

void list_clients(void)
{
    printf("----Listando Clientes----\n");
    if (num_of_clients == 0)
    {
        printf("Nao ha clientes.\n");
        return;
    }

    for (int i = 0; i < num_of_clients; i++)
    {
        printf("Informacoes do cliente %d:\n", i + 1);
        printf("Nome: %s\n", clients[i].Name);
        printf("Idade: %d\n", clients[i].Age);
        printf("CPF: %s\n", clients[i].CPF);
        printf("Numero da Conta %d\n", clients[i].AccountNumber);

        if (clients[i].AccountType == 1)
            printf("Tipo de Conta: Corrente.\n");
        else if (clients[i].AccountType == 2)
            printf("Tipo de Conta: Poupanca.\n");
        else
            printf("Ouve um erro: tipo de conta invalida.\n");

        printf("Status da conta: %s.\n", clients[i].Status);
        printf("Saldo da conta: %.2f.\n", clients[i].Balance);
    }
}

void withdraw(void)
{
    account_operation(1); // tipo 1 representa saque
}

void deposit(void)
{
    account_operation(2); // tipo 2 representa deposito
}
// estrutura de funcao comum entre saque e deposito
void account_operation(int operation_type)
{
    int account_number = 0;
    double value = 0.0;

    printf("Digite o numero da conta: ");
    scanf("%d", &account_number);
    for (int i = 0; i < num_of_clients; i++)
    {
        if (account_number == clients[i].AccountNumber)
        {
            // se tipo de operacao for saque
            if (operation_type == 1)
            {
                printf("Digite o valor a ser sacado: ");
                scanf("%lf", &value);
                if (value > clients[i].Balance || value <= 0)
                    printf("Valor de saque invalido.\n");
                else
                {
                    clients[i].Balance -= value;
                    printf("Saque realizado com sucesso!\n");
                    printf("Seu saldo agora e: %.2f", clients[i].Balance);
                }
            }
            // caso contrario deposito
            else
            {
                printf("Digite o valor a ser depositado: ");
                scanf("%lf", &value);
                if (value <= 0)
                {
                    printf("Valor de deposito invalido.\n");
                }
                else
                {
                    clients[i].Balance += value;
                    printf("Deposito realizado com sucesso!\n");
                    printf("Seu saldo agora e: %.2f", clients[i].Balance);
                }
            }
            return; // sai da funcao em caso de sucesso
        }
    }
    // caso contrario conta nao existe
    printf("Este numero de conta nao existe.");
}
// valida tamanho do cpf
bool is_cpf_format_valid(int strlen)
{
    return strlen == 14 ? true : false;
}
// funcao que verifica se usuario ja exist
int does_client_already_exist(char *cpf)
{
    // percorre usuarios cadastrados e compara cpfs com cpf do parametro
    for (int i = 0; i < num_of_clients; i++)
    {
        if (strcmp(cpf, clients[i].CPF) == 0)
            return true;
    }
    // caso nenhum strcmp retorne verdadeiro, cpf nao encontrado
    return false;
}
void loan(void)
{
}

void close_account(void)
{
    int account_number = 0;

    printf("Digite o numero da conta: ");
    scanf(" %d", &account_number);

    for (int i = 0; i < num_of_clients; i++)
    {
        if (clients[i].AccountNumber == account_number)
        {
            if (clients[i].Balance == 0.0)
            {
                strcpy(clients[i].Status, "FECHADA");
                printf("Conta fechada com sucesso.\n");
                return;
            }
            else
            {
                printf("E necessario esvaziar a conta antes de fecha-la");
                return;
            }
        }
    }
    // caso contrario conta nao existe
    printf("Este numero de conta nao existe.");
}