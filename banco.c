#include "banco.h"
#define NAME_MAX_LEN 100  // tamanho max do nome
#define MAX_CLIENTS 100   // max de clintes da aplicacao
#define MAX_LOANS 200     // max de emprestimos da aplicacao, 2 por cliente
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
typedef struct
{
    int AccountNumber;
    double Value;
} Loan;

Clients clients[MAX_CLIENTS]; // array de struct de usuarios
Loan loans[MAX_LOANS];        // array de struct de emprestimos
int num_of_clients = 0;       // numero de usuarios atualmente no programa
int num_of_loans = 0;         // numero de emprestimos

int main()
{
    main_menu();
}
// menu principal
void main_menu()
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
        printf("Realizar Emprestimo (6)\n");
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
void open_account()
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
        scanf("%i", &age);
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

void list_clients()
{
    printf("----Listando Clientes----\n");
    if (num_of_clients == 0)
    {
        printf("Nao ha clientes.\n");
        return;
    }

    for (int i = 0; i < num_of_clients; i++)
    {
        printf("Informacoes do cliente %i:\n", i + 1);
        printf("Nome: %s\n", clients[i].Name);
        printf("Idade: %i\n", clients[i].Age);
        printf("CPF: %s\n", clients[i].CPF);
        printf("Numero da Conta %i\n", clients[i].AccountNumber);

        if (clients[i].AccountType == 1)
            printf("Tipo de Conta: Corrente.\n");
        else if (clients[i].AccountType == 2)
            printf("Tipo de Conta: Poupanca.\n");
        else
            printf("Ouve um erro: tipo de conta invalida.\n");

        printf("Status da conta: %s.\n", clients[i].Status);
        printf("Saldo da conta: %.2f.\n", clients[i].Balance);

        printf("\nPressione ENTER para voltar ao menu principal\n");
        getchar();//consome char a mais
        while (getchar() != '\n');//espera enter
    }
}

void withdraw()
{
    printf("----Saque----\n");
    account_operation(1); // tipo 1 representa saque
}

void deposit()
{
    printf("----Deposito----\n");
    account_operation(2); // tipo 2 representa deposito
}
// estrutura de funcao comum entre saque e deposito
void account_operation(int operation_type)
{
    int account_number = 0;
    double value = 0.0;

    printf("Digite o numero da conta: ");
    scanf("%i", &account_number);
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
                    printf("Deposito realizado com sucesso!\n\n");
                    printf("Seu saldo agora e: %.2f\n", clients[i].Balance);
                }
            }
            return; // sai da funcao em caso de sucesso
        }
    }
    // caso contrario conta nao existe
    printf("Este numero de conta nao existe.");
}

void close_account()
{
    printf("----Fechamento de Conta----\n");
    int account_number = 0;

    printf("Digite o numero da conta: ");
    scanf("%i", &account_number);

    for (int i = 0; i < num_of_clients; i++)
    {
        if (clients[i].AccountNumber == account_number)
        {
            if (clients[i].Balance == 0.0)
            {
                strcpy(clients[i].Status, "FECHADA");
                printf("Conta fechada com sucesso.\n");
                printf("Lamentamos sua decisao.\n");
                return;
            }
            else
            {
                printf("E necessario esvaziar a conta antes de fecha-la.\n");
                return;
            }
        }
    }
    // caso contrario conta nao existe
    printf("Este numero de conta nao existe.");
}

void loan()
{

    double loan_value = 0.0;
    int account_number = 0;

    printf("----Emprestimo----\n");
    printf("Digite o numero da conta: ");
    scanf("%i", &account_number);

    for (int i = 0; i < num_of_clients; i++)
    {
        if (clients[i].AccountNumber == account_number)
        {
            while (true)
            {
                printf("Digite o valor do emprestimo: \n");
                scanf("%lf", &loan_value);

                if (clients[i].Balance == loan_value * 2)
                {
                    printf("Valor maior que o seu limite");
                    continue;
                }

                double balance_sum = 0.0;
                for (int i = 0; i < num_of_clients; i++)
                {
                    balance_sum += clients[i].Balance;
                }
                if (loan_value > balance_sum * 0.2)
                {
                    printf("Valor maior que o crédito disponível nesta agência.");
                    return;
                }

                clients[i].Balance += loan_value;
                loans[i].AccountNumber = clients[i].AccountNumber;
                loans[i].Value = loan_value;
                num_of_loans++;
                printf("Emprestimo efetuado com sucesso");
                printf("Seu saldo agora e: %.2f", clients[i].Balance);
                return;
            }
        }
    }
    printf("Este numero de conta nao existe.");
}

