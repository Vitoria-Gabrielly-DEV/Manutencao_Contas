# Sistema de Manutenção de Contas Bancárias em C

Este projeto consiste em um sistema de manutenção de contas de tamanho fixo utilizando persistência de dados em arquivos binários (`.dat`). O software permite operações de CRUD direto em disco, utilizando funções de posicionamento arbitrário.

## Funcionalidades Implementadas

1. **Cadastro em posição específica:** Usa `fseek()` para gravar o registro do cliente diretamente no *offset* correspondente ao número da conta.
2. **Consulta por conta:** Busca instantânea pelo cálculo da posição do registro, evitando varreduras lineares desnecessárias.
3. **Atualização de saldo:** Localiza o registro e sobrescreve apenas os dados necessários em disco.
4. **Encerramento de conta:** Aplica a técnica de *soft delete* (exclusão lógica), alterando a flag `ativo` do registro para `0`.
5. **Listagem de clientes:** Exibe todos os registros ativos a partir do ponteiro atual do arquivo.
6. **Uso de `rewind()`:** Move o indicador de posição de volta ao início do arquivo para permitir uma releitura completa dos dados.

---

## Como Executar o Projeto

Esta seção detalha as instruções para clonar, compilar e executar o sistema em qualquer sistema operacional.

### Pré-requisitos

Antes de começar, certifique-se de ter um compilador C instalado no seu sistema. O mais recomendado é o **GCC** (*GNU Compiler Collection*).

* **Linux:** O GCC costuma vir instalado por padrão. Caso não tenha, instale utilizando o gerenciador de pacotes da sua distribuição (ex: `sudo apt install build-essential` para distribuições baseadas em Debian/Ubuntu).
* **macOS:** Instale as ferramentas de linha de comando digitando `xcode-select --install` no terminal.
* **Windows:** Recomenda-se utilizar o **MinGW**, configurar o ambiente do **VS Code** para C, ou utilizar o **WSL** (*Windows Subsystem for Linux*).

### Passo a Passo

#### 1. Clonar o Repositório
Abra o seu terminal (ou Prompt de Comando/PowerShell no Windows) e execute os comandos abaixo:

```bash
git clone https://github.com/Vitoria-Gabrielly-DEV/Manutencao_Contas.git
cd Manutencao_Contas

```

#### 2. Compilação

O programa utiliza apenas as bibliotecas padrão da linguagem C (`stdio.h`, `stdlib.h`, `string.h`), portanto, a compilação é direta e sem dependências externas.

* **No Linux e macOS:**
```bash
gcc sistema_contas.c -o sistema_contas

```


* **No Windows (Via Prompt/PowerShell com MinGW):**
```powershell
gcc sistema_contas.c -o sistema_contas.exe

```



>  **Nota do Desenvolvedor:** O argumento `-o` define o nome do arquivo executável de saída. Se omitido, o compilador gerará um arquivo padrão chamado `a.out` (Linux/macOS) ou `a.exe` (Windows).

#### 3. Execução

Após compilar com sucesso, execute o binário gerado de acordo com o seu sistema operacional:

* **No Linux e macOS:**
```bash
./sistema_contas

```


* **No Windows:**
```powershell
.\sistema_contas.exe

```
