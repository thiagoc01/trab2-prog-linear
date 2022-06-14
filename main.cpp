#include <vector>
#include <iostream>

#include "simplex.hpp"

int main()
{
    int tamanhoColunaA; 
    int tamanhoLinhaA;
    bool tipoProblema;
    bool eDuasFases = false;
    int numDesigualdades = 0;
    int numVarArtificiais = 0;
    bool eDesigualdade = false;
    std::vector<int> ondeAdicionar;

    std::cout << "Se o problema for de maximização, digite 1. Caso contrário, digite 0." << std::endl;

    std::cin >> tipoProblema;

    std::cout << "Digite o número de coeficientes na função objetivo:" << std::endl;
    std::cin >> tamanhoColunaA;

    std::cout << "Digite o número de restrições do problema:" << std::endl;
    std::cin >> tamanhoLinhaA;

    std::vector <std::vector<float>> a(tamanhoLinhaA, std::vector<float>(tamanhoColunaA, 0));
    std::vector<float> b(tamanhoLinhaA,0);
    std::vector<float> c(tamanhoColunaA,0);

    for (int i = 0 ; i < tamanhoLinhaA ; i++)
    {    
        std::cout << "Digite os coeficientes da restrição " + std::to_string(i + 1) << std::endl;

        for (int j = 0 ; j < tamanhoColunaA ; j++)
        {
            std::cin >> a[i][j];

            if (a[i][j] < 0 && j >= tamanhoColunaA - tamanhoLinhaA + numDesigualdades) // Se houver algum coeficiente negativo nas variáveis de folga, temos uma restrição >=
            {
                eDuasFases = true;
                ondeAdicionar.push_back(i);
                eDesigualdade = true;
                numDesigualdades++;
                numVarArtificiais++;              
            }
            else if (a[i][j] == 1 && j >= tamanhoColunaA - tamanhoLinhaA + numDesigualdades)
            {
                eDesigualdade = true;
                numDesigualdades++;
            }
        }

        if (!eDesigualdade) // Se todas as variáveis de folga são 0, então temos uma restrição =.
        {
            eDuasFases = true;
            ondeAdicionar.push_back(i);
            numVarArtificiais++;
        }

        eDesigualdade = false;

    }

    std::cout << "Digite os valores do vetor B:\n";
    for (int i = 0 ; i < tamanhoLinhaA ; i++)
        std::cin >> b[i];

    std::cout << "Digite os coeficientes da função objetivo:\n";
    for (int i = 0 ; i < tamanhoColunaA ; i++)
        std::cin >> c[i];


    for (int i = 0 ; i < ondeAdicionar.size() ; i++)
    {
        std::cout << ondeAdicionar[i] << std::endl;
    }

    for (int k = 0 ; k < ondeAdicionar.size() ; k++)
    {
        for (int i = 0 ; i < tamanhoLinhaA ; i++)
        {
            if (i != ondeAdicionar[k])
                a[i].push_back(0);
            else
                a[i].push_back(1);
        }
        tamanhoColunaA++;
        c.push_back(0);
    }

      
    // hear the make the class parameters with A[m][n] vector b[] vector and c[] vector
    Simplex simplex(a,b,c, tipoProblema, eDuasFases, numVarArtificiais, ondeAdicionar);
    simplex.printMatrizes();
    simplex.aplicaSimplex();

    return 0;
}