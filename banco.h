#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

int is_cpf_valid();
void validate_cpf(char *cpf);
void list_clients();
void withdraw();
void deposit();
void account_operation(int operation_type);
bool is_cpf_format_valid(int strlen);
int does_client_already_exist(char *cpf);
void open_account();
void loan();
void close_account();
void main_menu();
void client_registration();
void account_registration();