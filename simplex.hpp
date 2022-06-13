#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <vector>

/**
 * @brief Implementa somente a segunda fase do método Simplex.
        É necessário que o problema tenha solução viável inicial e não tenha restrições do tipo >=.
 * 
 */

class Simplex
{
    private:
        int linhas, colunas; // Linhas = número de restrições ; Colunas = número de variáveis
        std::vector <std::vector<float> > A; // Matriz dos coeficientes das restrições
        std::vector<float> B; // Vetor de soluções das restrições
        std::vector<float> C; // Vetor de coeficientes da função objetivo.
        std::vector< std::pair<int, float> > base; // Vetor de pares para mapearmos as bases e os B_i's respectivos
        float solucaoOtima; // Solução ótima do problema
        bool eIlimitado; // Caso que o problema é ilimitado
        bool eMaximizacao; // Utilizada para verificar se o problema é de maximização ou não.
        bool semSolucao; // Caso que o problema não possui solução.

    public:
        /**
         * @brief Construtor da classe
         * 
         * @param coeficientes Coeficientes da matriz A
         * @param b Vetor de soluções de cada restrição
         * @param c Vetor de coeficientes da função objetivo
         * @param tipoProblema true se é de maximização, false se é de minimização.
         */
        Simplex (std::vector <std::vector<float>> coeficientes, std::vector<float> b, std::vector<float> c, bool tipoProblema);

        /**
         * @brief Realiza o cálculo de uma iteração da segunda fase do Simplex.
         * 
         * @return true - Se a iteração for final por algum motivo do problema.
         * @return false - Se ainda há possibilidade de maximizar/minimizar.
         */

        bool calculaIteracaoSimplex();

        /**
         * @brief Verifica se todos os coeficientes são positivos ou nulos.
         * 
         * @return true - Se não há valores negativos na função objetivo.
         * @return false - Se ainda há valor negativo na função objetivo.
         */
        bool verificarSolucaoOtima();

        /**
         * @brief Aplica o pivoteamento em cada linha que não seja a pivô.
         * 
         * @param linhaPivo O índice da linha do número pivô
         * @param colunaNumPivo O índice da coluna do número pivô
         */
        void realizaPivoteamento(int linhaPivo, int colunaNumPivo);

        /**
         * @brief Imprime a matriz A e os vetores B e C.
         * 
         */
        void printMatrizes();

        /**
         * @brief Procura pelo coeficiente mais negativo da função objetivo
         * 
         * @return int - O índice da coluna que contém o número mais negativo.
         */
        int achaColunaPivo();

        /**
         * @brief Procura pela linha i tal que B_i / A[i][colunaNumPivo] é o menor dos valores.
         * 
         * @param colunaNumPivo O índice da coluna da variável a entrar na base. 
         * @return int O índice da linha do número pivô.
         */
        int achaLinhaPivo(int colunaNumPivo);

        /**
         * @brief Método que inicia o método Simplex.
         * 
         */
        void aplicaSimplex();
};

#endif