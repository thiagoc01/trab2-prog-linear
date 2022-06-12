#include <vector>
#include <iostream>

#include "simplex.hpp"

int main()
{
    int tamanhoColunaA; 
    int tamanhoLinhaA; 

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
            std::cin >> a[i][j];
    }

    std::cout << "Digite os valores do vetor B:\n";
    for (int i = 0 ; i < tamanhoLinhaA ; i++)
            std::cin >> b[i];

    std::cout << "Digite os coeficientes da função objetivo:\n";
    for (int i = 0 ; i < tamanhoColunaA ; i++)
        std::cin >> c[i];

      
    // hear the make the class parameters with A[m][n] vector b[] vector and c[] vector
    Simplex simplex(a,b,c);
    simplex.aplicaSimplex();

    return 0;
}