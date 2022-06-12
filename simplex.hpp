#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <vector>

class Simplex
{
    private:
        int linhas, colunas;
        //stores coefficients of all the variables
        std::vector <std::vector<float> > A;
        //stores constants of constraints
        std::vector<float> B;
        //stores the coefficients of the objective function
        std::vector<float> C;
        std::vector< std::pair<int, float> > base;
        float maximo;
        bool eIlimitado;

    public:
        Simplex (std::vector <std::vector<float>> coeficientes, std::vector<float> b, std::vector<float> c);
        bool calculaIteracaoSimplex();
        bool verificarSolucaoOtima();
        void realizaPivoteamento(int linhaPivo, int colunaNumPivo);
        void printMatrizes();
        int achaColunaPivo();
        int achaLinhaPivo(int colunaNumPivo);
        void aplicaSimplex();
};

#endif