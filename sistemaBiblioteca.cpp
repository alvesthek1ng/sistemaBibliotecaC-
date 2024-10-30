#include <iostream>
#include <cstdio>
#include <limits>
#include <cstring>

using namespace std;

struct Emprestimo
{
    char data_emprestimo[10];
    char data_devolucao[10];
    char usuario[255];
};

struct Livro
{
    int codigo;
    int paginas;
    char area[30];
    char titulo[255];
    char autores[255];
    char editora[50];
    Emprestimo emprestimo;
};

// Declaração das funções
void cadastrarLivro();
void alterarLivro();
void excluirLivro();
void emprestarLivro();
void devolverLivro();
void consultarLivro();
void listarDisponiveis();
void listarTodosLivros();

int main()
{
    int opcao_principal;
    do
    {
        cout << "###### SISTEMA DE BIBLIOTECA ######" << endl;
        cout << "1 - Cadastro\n2 - Alteração\n3 - Exclusão\n4 - Empréstimo\n";
        cout << "5 - Devolução\n6 - Consulta de livro\n7 - Livros disponíveis\n";
        cout << "8 - Listagem geral de livros\n9 - Sair\n\n";
        cout << "Digite a opção desejada: ";
        cin >> opcao_principal;

        try
        {
            switch (opcao_principal)
            {
            case 1:
                cadastrarLivro();
                break;
            case 2:
                alterarLivro();
                break;
            case 3:
                excluirLivro();
                break;
            case 4:
                emprestarLivro();
                break;
            case 5:
                devolverLivro();
                break;
            case 6:
                consultarLivro();
                break;
            case 7:
                listarDisponiveis();
                break;
            case 8:
                listarTodosLivros();
                break;
            case 9:
                cout << "Obrigado por usar o sistema!" << endl;
                break;
            default:
                throw "Opção inválida!"; // Mensagem de erro para opção inválida
            }
        }
        catch (char const *mensagem)
        {
            cout << "Erro: " << mensagem << endl; // Exibe a mensagem de erro
        }

        cout << "\e[2J\e[0;0H"; // Limpeza da tela
    } while (opcao_principal != 9);
}

// Funções de manipulação de arquivos e operações com livros

void cadastrarLivro()
{
    try
    {
        FILE *arquivo = fopen("dados.dat", "ab");
        if (arquivo == NULL)
            throw "Erro ao abrir o arquivo para cadastro!";

        Livro livro;
        cout << "Código: ";
        cin >> livro.codigo;
        if (cin.fail())
        {
            throw "Código inválido!";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Área: ";
        cin.getline(livro.area, 30);
        cout << "Título: ";
        cin.getline(livro.titulo, 255);
        cout << "Autor(es): ";
        cin.getline(livro.autores, 255);
        cout << "Editora: ";
        cin.getline(livro.editora, 50);
        cout << "Nº páginas: ";
        cin >> livro.paginas;
        if (cin.fail())
        {
            throw "Número de páginas inválido!";
        }

        if (fwrite(&livro, sizeof(Livro), 1, arquivo) != 1)
        {
            fclose(arquivo);
            throw "Erro ao cadastrar o livro!";
        }

        cout << "Livro cadastrado com sucesso!" << endl;
        fclose(arquivo);
    }
    catch (char const *mensagem)
    {
        cout << "Erro: " << mensagem << endl; // Mensagem de erro
    }
}

void alterarLivro()
{
    try
    {
        FILE *arquivo = fopen("dados.dat", "rb+");
        if (arquivo == NULL)
            throw "Erro ao abrir o banco de dados!";

        int cod;
        cout << "Digite o código do livro que deseja alterar: ";
        cin >> cod;
        if (cin.fail())
        {
            throw "Código inválido!";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Livro livro;
        int pos = -1;
        bool encontrado = false;

        while (fread(&livro, sizeof(Livro), 1, arquivo))
        {
            pos++;
            if (cod == livro.codigo)
            {
                encontrado = true;
                cout << "Área: ";
                cin.getline(livro.area, 30);
                cout << "Título: ";
                cin.getline(livro.titulo, 255);
                cout << "Autor(es): ";
                cin.getline(livro.autores, 255);
                cout << "Editora: ";
                cin.getline(livro.editora, 50);
                cout << "Nº páginas: ";
                cin >> livro.paginas;
                if (cin.fail())
                {
                    throw "Número de páginas inválido!";
                }

                fseek(arquivo, sizeof(Livro) * pos, SEEK_SET);
                fwrite(&livro, sizeof(Livro), 1, arquivo);
                cout << "Livro alterado com sucesso!" << endl;
                break;
            }
        }
        fclose(arquivo);
        if (!encontrado)
            throw "Livro não encontrado!";
    }
    catch (char const *mensagem)
    {
        cout << "Erro: " << mensagem << endl; // Mensagem de erro
    }
}

void excluirLivro()
{
    try
    {
        int cod;
        cout << "Digite o código do livro que deseja excluir: ";
        cin >> cod;
        if (cin.fail())
        {
            throw "Código inválido!";
        }

        FILE *arquivo = fopen("dados.dat", "rb");
        FILE *arquivoAux = fopen("dados.aux", "wb");
        if (arquivo == NULL || arquivoAux == NULL)
            throw "Erro ao abrir o arquivo!";

        Livro livro;
        bool encontrado = false;

        while (fread(&livro, sizeof(Livro), 1, arquivo))
        {
            if (cod != livro.codigo)
            {
                fwrite(&livro, sizeof(Livro), 1, arquivoAux);
            }
            else
            {
                encontrado = true;
            }
        }

        fclose(arquivo);
        fclose(arquivoAux);
        remove("dados.dat");
        rename("dados.aux", "dados.dat");

        if (encontrado)
        {
            cout << "Livro excluído com sucesso!" << endl;
        }
        else
        {
            throw "Livro não encontrado!";
        }
    }
    catch (char const *mensagem)
    {
        cout << "Erro: " << mensagem << endl; // Mensagem de erro
    }
}

void consultarLivro()
{
    try
    {
        FILE *arquivo = fopen("dados.dat", "rb");
        if (arquivo == NULL)
            throw "Erro ao abrir o banco de dados!";

        int cod;
        cout << "Digite o código do livro que deseja consultar: ";
        cin >> cod;
        if (cin.fail())
        {
            throw "Código inválido!";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Livro livro;
        bool encontrado = false;

        while (fread(&livro, sizeof(Livro), 1, arquivo))
        {
            if (cod == livro.codigo)
            {
                encontrado = true;
                cout << "Código: " << livro.codigo << endl;
                cout << "Área: " << livro.area << endl;
                cout << "Título: " << livro.titulo << endl;
                cout << "Autor(es): " << livro.autores << endl;
                cout << "Editora: " << livro.editora << endl;
                cout << "Nº páginas: " << livro.paginas << endl;
                break;
            }
        }
        fclose(arquivo);
        if (!encontrado)
            throw "Livro não encontrado!";
    }
    catch (char const *mensagem)
    {
        cout << "Erro: " << mensagem << endl; // Mensagem de erro
    }
}
