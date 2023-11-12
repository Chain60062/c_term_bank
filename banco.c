#include <stdio.h>
#include "banco.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define NAME_MAX_LEN 100 // tamanho max do nome
#define MAX_CLIENTS 100  // max de clintes da aplicacao
#define CPF_LEN 14

// struct de clientes
typedef struct
{
    // info do cliente
    char *CPF;
    char *Name;
    int Age;
    // info da conta
    char *Status;
    unsigned short AccountType; // 1 para corrente, 2 para poupanca
    long AccountNumber;
    double Balance;
} Clients;

Clients clients[MAX_CLIENTS]; // array de struct de usuarios
int num_of_clients;           // numero de usuarios atualmente no programa

int main(void)
{
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
                // se for valido, checar se ja esta cadastrado.
                if (does_client_already_exist(cpf))
                {
                    printf("Já existe um usuário cadastrado com este CPF.\n");
                    return;
                }
                else
                {
                    client_registration(name, age, cpf);//cadastrar cliente com dados ja pegos
                    account_registration();//imediatamente apos, abrir conta
                    break;
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
    clients[num_of_clients].Name = name;
    clients[num_of_clients].Age = age;
    clients[num_of_clients].CPF = cpf;
    printf("Cliente cadastrado com sucesso.");
}

void account_registration()
{
    while (true)
    {
        char option = '\0';
        printf("Deseja abrir qual tipo de conta (digite o numero equivalente)?");
        printf("Corrente (1)");
        printf("Poupanca (2)");

        switch (option)
        {
        case '1':
            clients[num_of_clients].AccountType = 1; // corrrente
            break;
        case '2':
            clients[num_of_clients].AccountType = 2; // poupança
            break;
        default:
            printf("Tipo de conta invalida.");
            break;
        }
    }
    clients[num_of_clients].AccountNumber = num_of_clients;
    clients[num_of_clients].Balance = 0.0;
    clients[num_of_clients].Status = "ABERTA";
    printf("Conta cadastrada com sucesso.");
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
        printf("Informacoes do cliente %d.\n", i);
        printf("Nome: %s.\n", clients[i].Name);
        printf("Numero da Conta %ld.\n", clients[i].AccountNumber);

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
    long account_number = 0;
    double value = 0.0;

    printf("Digite o número da conta");
    scanf("%ld", &account_number);
    for (int i = 0; i < num_of_clients; i++)
    {
        if (account_number == clients[i].AccountNumber)
        {
            // se tipo de operacao for saque
            if (operation_type == 1)
            {
                if (value > clients[i].Balance || value <= 0)
                    printf("Valor de saque invalido.");
                else
                {
                    clients[i].Balance -= value;
                    printf("Saque realizado com sucesso!");
                }
            }
            // caso contrario deposito
            else
            {
                if (value <= 0)
                    printf("Valor de deposito invalido.");
                else
                {
                    clients[i].Balance += value;
                    printf("Deposito realizado com sucesso!");
                }
            }
            printf("Seu saldo agora e: %.2f", clients[i].Balance);
            return; // sai da funcao em caso de sucesso
        }
    }
    // caso contrario conta nao existe
    printf("Este numero de conta nao existe.");
}

int is_cpf_format_valid(int strlen)
{
    printf("TAMANHO: %d\n", strlen);
    return strlen != 14 ? false : true;
}
// funcao que verifica se usuario ja exist
int does_client_already_exist(char *cpf)
{
    // percorre usuarios cadastrados e compara cpfs com cpf do parametro
    for (int i = 0; i < MAX_CLIENTS; i++)
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
    long account_number = 0;

    printf("Digite o numero da conta");

    for (int i = 0; i < num_of_clients; i++)
    {
        if (clients[i].AccountNumber == account_number)
        {
            if (clients[i].Balance == 0.0)
            {
                clients[i].Status = "FECHADA";
            }
            else
            {
                printf("e necessario esvaziar a conta antes de fecha-la");
                return;
            }
        }
    }
    // caso contrario conta nao existe
    printf("Este numero de conta nao existe.");
}