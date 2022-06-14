#include <iostream>
#include <cmath>
#include <vector>
#include <iterator>
#include <limits>
#include <algorithm>

#include "simplex.hpp"

using namespace std;

Simplex::Simplex (std::vector <std::vector<float>> coeficientes, std::vector<float> b, std::vector<float> c, bool tipoProblema, bool eDuasFases, int numVarArtificiais, std::vector<int> ondeAdicionar)
{
    solucaoOtima = solucaoOtimaPrimeiraFase = 0;
    eIlimitado = false;
    semSolucao = false;
    onde = ondeAdicionar;
    this->eDuasFases = eDuasFases;
    this->numVarArtificiais = numVarArtificiais;
    linhas = coeficientes.size();
    colunas = coeficientes[0].size(); // O tamanho de uma linha indica o número de variáveis no problema.
    
    B.assign(b.begin(), b.end()); // Inicializa o vetor B.

    if (tipoProblema) // Se o problema for de maximização, basta copiar o vetor C e setar a booleana.
    {
        C.assign(c.begin(), c.end());
        eMaximizacao = true;
    }

    else // Caso contrário, é necessário multiplicar a linha inteira por -1 para obtemos o problema equivalente e mantermos o mesmo código.
    {
        for (int i = 0 ; i < c.size() ; i++)
        {
            if (c[i] != 0)
                C.push_back(c[i] * -1);
            else
                C.push_back(0);
        }
        
        eMaximizacao = false;
    }
    
    for (int i = 0 ; i < linhas ; i++)
        A.push_back(coeficientes[i]); // Inicializa a matriz A.

    
    if (!eDuasFases)
    {
        int j = 0;

        for (int i = C.size() - 1 ; i >= 0 ; i--)
        {
            if (!C[i]) // As variáveis básicas têm coeficiente 0 no ínicio do problema.
            {
                base.push_back( {i, B[j]} );
                j++;
            }
        }
    }
}

bool Simplex::calculaIteracaoSimplex()
{
    static int iteracao = 1;

    if (verificarSolucaoOtima())
        return true;

    int colunaNumPivo = achaColunaPivo();

    int linhaPivo = achaLinhaPivo(colunaNumPivo);

    if (eIlimitado)
    {
        cout << "Solução ilimitada.\n" << endl;
        return true;
    }

    realizaPivoteamento(linhaPivo, colunaNumPivo);

    if (semSolucao)
    {
        cout << "O problema não possui solucão.\n";
        return true;
    }

    cout << "Matriz de coeficientes e vetores B e C na iteração " + to_string(iteracao) << endl;

    printMatrizes();

    cout << endl;

    cout << "Variáveis básicas na iteração " + to_string(iteracao) << endl;

    auto it = base.begin();

    while (it != base.end())
    {
        cout << "x" << it->first + 1 << " " << it->second << " " << endl;
        it++;
    }

    iteracao++;
    
    return false;
}

bool Simplex::verificarSolucaoOtima()
{
    bool eOtima = false;
    int contagemNumPositivos = 0;

    if (eDuasFases)
    {
        for (int i = 0 ; i < C_artificial.size() ; i++)
        {
            if (C_artificial[i] >= 0)
                contagemNumPositivos++;
        }

        if (contagemNumPositivos == C_artificial.size())
            eOtima = true;
    }

    else
    {
        for (int i = 0 ; i < C.size() ; i++)
        {
            if (C[i] >= 0)
                contagemNumPositivos++;
        }

        if (contagemNumPositivos == C.size())
            eOtima = true;
    }   

    return eOtima;
}

void Simplex::realizaPivoteamento(int linhaPivo, int colunaNumPivo)
{
    float numPivo = A[linhaPivo][colunaNumPivo];

    if (eDuasFases)
        solucaoOtimaPrimeiraFase = solucaoOtimaPrimeiraFase - (C_artificial[colunaNumPivo]*(B[linhaPivo]/numPivo));

    solucaoOtima = solucaoOtima - (C[colunaNumPivo]*(B[linhaPivo]/numPivo));           

    for (int k = 0 ; k < colunas ; k++)
        A[linhaPivo][k] = A[linhaPivo][k] / numPivo; // Divide a linha pivô pelo número pivô.

    B[linhaPivo] = B[linhaPivo] / numPivo; // O vetor B está separado, realiza a mesma ação anterior.

    base[linhaPivo] = {colunaNumPivo, B[linhaPivo]};  // Altera a base correspondente a essa linha.

    for (int i = 0 ; i < B.size() ; i++)
    {
        if (i != linhaPivo)
        {
            float multiplicadorLinha =  A[i][colunaNumPivo]; // Capturamos o elemento que faz zerar o elemento B dessa linha.

            B[i] = B[i] - (multiplicadorLinha * B[linhaPivo]); // Atualiza o B_i

            if (B[i] < 0) // Se algum B[i] é menor que 0, o problema é inviável.
                semSolucao = true;

            base[i] = {base[i].first, B[i]}; // Atualiza no vetor de bases o valor de B_i.
        }
    } 

    for (int m = 0 ; m < linhas ; m++)
    {
        float multiplicadorLinha = A[m][colunaNumPivo]; // Capturamos o elemento que faz zerar o elemento da coluna pivô dessa linha.

        if (m != linhaPivo)
        {
            for (int p = 0 ; p < colunas ; p++)                        
                A[m][p] = A[m][p] - (multiplicadorLinha * A[linhaPivo][p]);  // Atualiza cada elemento da linha, realizando o pivoteamento.                 
        }
    }

    /* Processo análogo para o vetor de coeficientes da função objetivo. */    
    float multiplicadorLinha = C[colunaNumPivo];

    for (int i = 0 ; i < colunas ; i++)
        C[i] = C[i] - (multiplicadorLinha * A[linhaPivo][i]);

    if (eDuasFases)
    {
        multiplicadorLinha = C_artificial[colunaNumPivo];

        for (int i = 0 ; i < C_artificial.size() ; i++)
            C_artificial[i] = C_artificial[i] - (multiplicadorLinha * A[linhaPivo][i]);
    }
}

void Simplex::printMatrizes()
{
    cout << "Matriz A: \n";

    for (int i = 0 ; i < linhas ; i++)
    {
        for (int j = 0 ; j < colunas ; j++)
            cout << A[i][j] << " | ";
        cout << endl;
    }

    cout << "Vetor B: \n";
    for (int i = 0 ; i < B.size() ; i++)
        cout << B[i] << " | ";

    cout << endl;

    cout << "Vetor C: \n";
    for (int i = 0 ; i < C.size() ; i++)
        cout << C[i] << " | ";
    cout << endl;

    if (eDuasFases)
    {
        cout << "Vetor C artificial: \n";
        for (int i = 0 ; i < C_artificial.size() ; i++)
            cout << C_artificial[i] << " | ";
    }
    cout << endl;
}

int Simplex::achaColunaPivo()
{
    /* Inicializamos tomando a primeira posição como o menor elemento */
    int localizacao = 0;
    float menor;

    if (eDuasFases)
    {
        menor = C_artificial[0];

        for (int i = 1 ; i < C_artificial.size() ; i++) // Realiza um procedimento comum para encontrar o menor
        {
            if (C_artificial[i] < menor)
            {
                menor = C_artificial[i];
                localizacao = i;
            }
        }
    }

    else
    {
        menor = C[0];

        for (int i = 1 ; i < C.size() ; i++) // Realiza um procedimento comum para encontrar o menor
        {
            if (C[i] < menor)
            {
                menor = C[i];
                localizacao = i;
            }
        }
    }
    

    return localizacao;
}

int Simplex::achaLinhaPivo(int colunaNumPivo)
{
    int contagemNumNegativos = 0;
    float minimo = std::numeric_limits<float>::max();
    int localizacao = 0;

    for (int i = 0 ; i < linhas ; i++)
    {
        if (A[i][colunaNumPivo] <= 0)
            contagemNumNegativos++;
    }

    if (contagemNumNegativos == linhas) // Critério de parada do Simplex. Se todos os coeficientes são 0 na função objetivo.
    {
        eIlimitado = true;
        return -1;
    }

    for (int i = 0 ; i < linhas ; i++)
    {
        if (A[i][colunaNumPivo] > 0) // Iremos testar apenas linhas que possuem coeficientes positivos na coluna pivô.
        {
            if (B[i] / A[i][colunaNumPivo] < minimo) // Devemos encontrar o mínimo.
            {              
                minimo =  B[i] / A[i][colunaNumPivo];
                localizacao = i;
            }
        }
    }

    return localizacao;
}

bool Simplex::iniciaPrimeiraFase()
{
    vector<int> artificiais;
    C_artificial.resize(colunas-numVarArtificiais, 0);

    for (int i = 1 ; i <= numVarArtificiais ; i++)
        C_artificial.push_back(1);

    cout << "Coeficientes da função objetivo artificial: \n";

    for (int i = 0 ; i < colunas ; i++)
        cout << C_artificial[i] << " ";

    cout << endl;

    int k = 0;

    for (int i = 0 ; i < linhas ; i++)
    {
        for (int j = colunas - linhas - numVarArtificiais ; j < colunas ; j++)
        {
            if (A[i][j] == 1)
            {
                base.push_back( {j, B[k]} );
                k++;
                break;
            }
        }
        if (std::find(onde.begin(), onde.end(), i) != onde.end())
            artificiais.push_back(i);
    }
    
    for (int i = 0 ; i < base.size() ; i++)
        cout << base[i].first << " " << base[i].second << endl;    

    for (int i = 0 ; i < artificiais.size() ; i++)
    {
        for (int j = 0 ; j < colunas ; j++)
            C_artificial[j] = C_artificial[j] - A[artificiais[i]][j];
        
        solucaoOtimaPrimeiraFase -= B[artificiais[i]];
    }

    printMatrizes();
    cout << "\n\n\n\n\n";
    realizaPrimeiraFase();
    
    return true;

}

bool Simplex::realizaPrimeiraFase()
{
    bool fim = false;

    while ( !fim )
    {
        bool resultado = calculaIteracaoSimplex();            

        if (resultado)
            fim = true;
    }
    cout << "Fim da primeira fase.\n\n";
    

    if (solucaoOtimaPrimeiraFase == 0)
    {
        cout << "O problema possui solução.\n";
        cout << "Iniciando a segunda fase...\n";
        C_artificial.clear();

        for (int i = 0 ; i < numVarArtificiais ; i++)
            C.pop_back();

        for (int i = 0 ; i < linhas ; i++)
        {
            for (int j = 0 ; j < numVarArtificiais ; j++)
                A[i].pop_back();
        }

        colunas = C.size();
        
        eDuasFases = false;
        return true;
    }

    else
    {
        cout << "O problema não possui solução.\n";
        return false;
    }
}

void Simplex::aplicaSimplex()
{
    if (eDuasFases)
    {
        cout << "O método de duas fases deve ser aplicado. Iniciando primeira fase... \n";
        bool temSegundaFase = iniciaPrimeiraFase();

        if (!temSegundaFase)
            return;
    }

    bool fim = false;

    cout << "Matriz de coeficientes e vetores B e C iniciais: " << endl;
    printMatrizes();

    cout << endl;

    while (!fim)
    {
        bool resultado = calculaIteracaoSimplex();            

        if (resultado)
            fim = true;
    }

    if (!semSolucao && !eIlimitado)
    {
        cout << "Matriz de coeficientes e vetores B e C finais: " << endl;
        printMatrizes();

        cout << endl;

        cout << "Variáveis básicas na última iteração: " << endl;

        auto it = base.begin();

        while (it != base.end())
        {
            cout << "x" << it->first + 1 << " " << it->second << " " << endl;
            it++;
        }

        cout << endl;

        if (!eMaximizacao && solucaoOtima != 0)
            solucaoOtima *= -1; // A implementação é baseada em maximização. Para obter a solução de uma minimização, basta multiplicar por -1.

        cout << "Solução ótima: " << solucaoOtima << endl;
    }    
}
