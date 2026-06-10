#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_DADOS "clientes.dat"
#define TAM_NOME 50

// Estrutura de registro de tamanho fixo
typedef struct {
    int numero_conta;    // Funciona como o ID/Posição (0, 1, 2...)
    char nome[TAM_NOME];
    double saldo;
    int ativo;           // Flag booleana: 1 para ativo, 0 para removido/vazio
} Cliente;

// Protótipos das funções
void menu();
void cadastrar_cliente(FILE *arq);
void consultar_cliente(FILE *arq);
void atualizar_saldo(FILE *arq);
void remover_cliente(FILE *arq);
void listar_clientes(FILE *arq);
void restaurar_e_listar(FILE *arq);
void limpar_buffer();

int main() {
    // Abre o arquivo para leitura e escrita em modo binário (r+b)
    // Se não existir, cria um novo arquivo (w+b)
    FILE *arq = fopen(ARQUIVO_DADOS, "r+b");
    if (arq == NULL) {
        arq = fopen(ARQUIVO_DADOS, "w+b");
        if (arq == NULL) {
            perror("Erro ao abrir/criar o arquivo de dados");
            return EXIT_FAILURE;
        }
    }

    int opcao = 0;
    do {
        menu();
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida!\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        switch (opcao) {
            case 1: cadastrar_cliente(arq); break;
            case 2: consultar_cliente(arq); break;
            case 3: atualizar_saldo(arq); break;
            case 4: remover_cliente(arq); break;
            case 5: listar_clientes(arq); break;
            case 6: restaurar_e_listar(arq); break;
            case 7: printf("\nEncerrando o sistema. Até logo!\n"); break;
            default: printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 7);

    fclose(arq);
    return EXIT_SUCCESS;
}

void menu() {
    printf("\n================ SISTEMA DE CONTAS ================\n");
    printf("1. Cadastrar um novo cliente (posição específica)\n");
    printf("2. Consultar um cliente pelo número da conta\n");
    printf("3. Atualizar o saldo de um cliente\n");
    printf("4. Encerrar conta (remover cliente)\n");
    printf("5. Listar todos os clientes\n");
    printf("6. Restaurar leitura (rewind) e repetir listagem\n");
    printf("7. Encerrar\n");
    printf("====================================================\n");
}

void cadastrar_cliente(FILE *arq) {
    int posicao;
    Cliente novo;

    printf("\n--- Cadastrar Cliente ---\n");
    printf("Digite a posição/número da conta desejada (>= 0): ");
    if (scanf("%d", &posicao) != 1 || posicao < 0) {
        printf("Posição inválida!\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    // Move o ponteiro para a posição calculada com fseek
    fseek(arq, posicao * sizeof(Cliente), SEEK_SET);
    
    // Verifica se já existe um cliente ativo nesta posição
    Cliente checagem;
    if (fread(&checagem, sizeof(Cliente), 1, arq) == 1 && checagem.ativo == 1) {
        printf("Aviso: Já existe uma conta ativa nesta posição (%s). Sobrescrever? (S/N): ", checagem.nome);
        char conf;
        scanf("%c", &conf);
        limpar_buffer();
        if (conf != 'S' && conf != 's') {
            printf("Operação cancelada.\n");
            return;
        }
    }

    // Coleta dos novos dados
    novo.numero_conta = posicao;
    printf("Digite o nome do cliente: ");
    fgets(novo.nome, TAM_NOME, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0'; // Remove o \n do final

    printf("Digite o saldo inicial: ");
    scanf("%lf", &novo.saldo);
    limpar_buffer();
    novo.ativo = 1; // Define o registro como ativo

    // Reposiciona para garantir a escrita no local correto após a leitura de checagem
    fseek(arq, posicao * sizeof(Cliente), SEEK_SET);
    if (fwrite(&novo, sizeof(Cliente), 1, arq) == 1) {
        printf("Cliente cadastrado com sucesso na posição %d!\n", posicao);
    } else {
        printf("Erro ao gravar os dados do cliente.\n");
    }
}

void consultar_cliente(FILE *arq) {
    int conta;
    Cliente c;

    printf("\n--- Consultar Cliente ---\n");
    printf("Digite o número da conta: ");
    if (scanf("%d", &conta) != 1 || conta < 0) {
        printf("Número inválido!\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    fseek(arq, conta * sizeof(Cliente), SEEK_SET);

    if (fread(&c, sizeof(Cliente), 1, arq) == 1 && c.ativo == 1) {
        printf("\nConta: %d\nNome: %s\nSaldo: R$ %.2f\n", c.numero_conta, c.nome, c.saldo);
    } else {
        printf("Conta número %d não encontrada ou inativa.\n", conta);
    }
}

void atualizar_saldo(FILE *arq) {
    int conta;
    Cliente c;

    printf("\n--- Atualizar Saldo ---\n");
    printf("Digite o número da conta: ");
    if (scanf("%d", &conta) != 1 || conta < 0) {
        printf("Número inválido!\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    fseek(arq, conta * sizeof(Cliente), SEEK_SET);

    if (fread(&c, sizeof(Cliente), 1, arq) == 1 && c.ativo == 1) {
        printf("Saldo atual de %s: R$ %.2f\n", c.nome, c.saldo);
        printf("Digite o novo saldo: ");
        scanf("%lf", &c.saldo);
        limpar_buffer();

        // Volta o ponteiro para o início do registro para atualizar
        fseek(arq, conta * sizeof(Cliente), SEEK_SET);
        fwrite(&c, sizeof(Cliente), 1, arq);
        printf("Saldo atualizado com sucesso!\n");
    } else {
        printf("Conta não encontrada para atualização.\n");
    }
}

void remover_cliente(FILE *arq) {
    int conta;
    Cliente c;

    printf("\n--- Encerrar Conta ---\n");
    printf("Digite o número da conta que deseja encerrar: ");
    if (scanf("%d", &conta) != 1 || conta < 0) {
        printf("Número inválido!\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    fseek(arq, conta * sizeof(Cliente), SEEK_SET);

    if (fread(&c, sizeof(Cliente), 1, arq) == 1 && c.ativo == 1) {
        c.ativo = 0; // Exclusão lógica (soft delete)

        fseek(arq, conta * sizeof(Cliente), SEEK_SET);
        fwrite(&c, sizeof(Cliente), 1, arq);
        printf("Conta número %d encerrada com sucesso!\n", conta);
    } else {
        printf("Conta não encontrada ou já inativa.\n");
    }
}

void listar_clientes(FILE *arq) {
    Cliente c;
    int encontrou = 0;

    printf("\n--- Lista de Clientes Ativos ---\n");
    printf("%-7s | %-30s | %-12s\n", "Conta", "Nome", "Saldo");
    printf("-----------------------------------------------------\n");

    // Lê sequencialmente do ponto atual do arquivo até o fim (EOF)
    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo == 1) {
            printf("%-7d | %-30s | R$ %-10.2f\n", c.numero_conta, c.nome, c.saldo);
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("Nenhum cliente ativo encontrado nesta região do arquivo.\n");
    }
}

void restaurar_e_listar(FILE *arq) {
    printf("\n[Sistema] Executando rewind()...\n");
    rewind(arq); // Reseta o indicador de posição para o início do arquivo
    listar_clientes(arq);
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}