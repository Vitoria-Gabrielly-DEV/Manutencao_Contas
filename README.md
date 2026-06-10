# Sistema de Manutenção de Contas Bancárias em C

Este projeto consiste em um sistema de manutenção de contas de tamanho fixo utilizando persistência de dados em arquivos binários (`.dat`). O software permite operações de CRUD direto em disco, utilizando funções de posicionamento arbitrário.

## Funcionalidades Implementadas

1. **Cadastro em posição específica:** Usa `fseek()` para gravar o registro do cliente diretamente no *offset* correspondente ao número da conta.
2. **Consulta por conta:** Busca instantânea pelo cálculo da posição do registro, evitando varreduras lineares desnecessárias.
3. **Atualização de saldo:** Localiza o registro e sobrescreve apenas os dados necessários em disco.
4. **Encerramento de conta:** Aplica a técnica de *soft delete* (exclusão lógica), alterando a flag `ativo` do registro para `0`.
5. **Listagem de clientes:** Exibe todos os registros ativos a partir do ponteiro atual do arquivo.
6. **Uso de `rewind()`:** Move o indicador de posição de volta ao início do arquivo para permitir uma releitura completa dos dados.

## Como Executar o Projeto

1. **Clonar o repositório:**
   ```bash
   git clone [https://github.com/SEU_USUARIO/NOME_DO_REPOSITORIO.git](https://github.com/SEU_USUARIO/NOME_DO_REPOSITORIO.git)
   cd NOME_DO_REPOSITORIO
