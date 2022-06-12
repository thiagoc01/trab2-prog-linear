#include <iostream>
#include <cmath>
#include <vector>
#include <iterator>
#include <limits>

using namespace std;

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
        Simplex (std::vector <std::vector<float>> coeficientes, std::vector<float> b, std::vector<float> c)
        {
            maximo = 0;
            eIlimitado = false;
            linhas = coeficientes.size();
            colunas = coeficientes[0].size();
            
            B.assign(b.begin(), b.end());
            C.assign(c.begin(), c.end()); 
            
            for (int i= 0;i<linhas;i++)             //pass A[][] values to the metrix
                A.push_back(coeficientes[i]);

            int j = 0;

            for (int i = 0 ; i < C.size() ; i++)
            {
                if (!C[i])
                {
                    base.push_back( {i, B[j]} );
                    j++;
                }
            }
        }

        bool calculaIteracaoSimplex()
        {
            //check whether the table is optimal,if optimal no need to process further
            if (verificarSolucaoOtima())
			    return true;
             
            //find the column which has the pivot.The least coefficient of the objective function(C array).
            int colunaNumPivo = achaColunaPivo();           
            
            //find the row with the pivot value.The least value item's row in the B array
            int linhaPivo = achaLinhaPivo(colunaNumPivo);

            if (eIlimitado)
            {
                cout <<"Solução ilimitada."<< endl;
                printMatrizes();
			    return true;
            }

            //form the next table according to the pivot value
            realizaPivoteamento(linhaPivo,colunaNumPivo);
            
            return false;
        }

        bool verificarSolucaoOtima()
		{
            //if the table has further negative constraints,then it is not optimal
            bool eOtima = false;
            int contagemNumPositivos = 0;

            //check if the coefficients of the objective function are negative
            for (int i = 0; i < C.size() ; i++)
		    {
                float value = C[i];
                if (value >= 0)
                    contagemNumPositivos++;
            }
            //if all the constraints are positive now,the table is optimal
            if (contagemNumPositivos == C.size())
            {
                eOtima = true;
                printMatrizes();
            }
            return eOtima;
        }

        void realizaPivoteamento(int linhaPivo, int colunaNumPivo)
        {
            float numPivo = A[linhaPivo][colunaNumPivo];//gets the pivot value

            maximo = maximo - (C[colunaNumPivo]*(B[linhaPivo]/numPivo));  //set the maximum step by step          

            //set the row values that has the pivot value divided by the pivot value and put into new row
            for (int k = 0 ; k < colunas ; k++)
                A[linhaPivo][k] = A[linhaPivo][k] / numPivo;

            B[linhaPivo] = B[linhaPivo] / numPivo;

            base[linhaPivo] = {colunaNumPivo, B[linhaPivo]};  

            //process the values of the B array
            for (int i = 0 ; i < B.size() ; i++)
            {
                if (i != linhaPivo)
                {
                    float multiplicadorLinha =  A[i][colunaNumPivo];
                    B[i] = B[i] - (multiplicadorLinha * B[linhaPivo]);
                    base[i] = {base[i].first, B[i]};
                }
            }          

             //process the other coefficients in the A array by subtracting
            for (int m = 0 ; m < linhas ; m++)
            {
                float multiplicadorLinha = A[m][colunaNumPivo];
                //ignore the pivot row as we already calculated that
                if (m != linhaPivo)
                {
                    for (int p = 0 ; p < colunas ; p++)                        
                        A[m][p] = A[m][p] - (multiplicadorLinha * A[linhaPivo][p]);                    
                }
            }            

            //the least coefficient of the constraints of the objective function
            float multiplicadorLinha = C[colunaNumPivo];
            //process the C array
            for (int i = 0 ; i < C.size() ; i++)
                C[i] = C[i] - (multiplicadorLinha * A[linhaPivo][i]);
        }

        //print the current A array
        void printMatrizes()
        {
            for (int i = 0; i<linhas;i++)
            {
                for (int j = 0 ; j < colunas ; j++)
                    cout << A[i][j] << " | ";
                cout << endl;
            }

            for (int i = 0; i<B.size();i++)
                    cout << B[i] << " | ";

            cout << endl;

            for (int i = 0 ; i < C.size() ; i++)
                    cout << C[i] << " | ";
            cout << endl;
        }

        //find the least coefficients of constraints in the objective function's position
        int achaColunaPivo()
        {
            int localizacao = 0;
            float menor = C[0];

            for (int i = 1 ; i < C.size() ; i++)
            {
                if (C[i] < menor)
                {
                    menor = C[i];
                    localizacao = i;
                }
            }

            return localizacao;
        }

        //find the row with the pivot value.The least value item's row in the B array
        int achaLinhaPivo(int colunaNumPivo)
        {
            int contagemNumNegativos = 0;
            float minimo = std::numeric_limits<float>::max();
            int localizacao = 0;

            for (int i = 0 ; i < linhas ; i++)
            {
                if (A[i][colunaNumPivo] <= 0)
                    contagemNumNegativos++;
            }

            //checking the unbound condition if all the values are negative ones
            if (contagemNumNegativos == linhas)
            {
                eIlimitado = true;
                return -1;
            }

            for (int i = 0 ; i < linhas ; i++)
            {
                if (A[i][colunaNumPivo] > 0)
                {
                    if (B[i] / A[i][colunaNumPivo] < minimo)
                    {              
                        minimo =  B[i] / A[i][colunaNumPivo];
                        localizacao = i;
                    }
                }
            }

            return localizacao;
        }

        void aplicaSimplex()
        {
            bool fim = false;

            cout <<"Matriz de coeficientes inicial " << endl;
            printMatrizes();

            cout << endl;
            cout << "Matriz de coeficientes final" << endl;

            while (!fim)
            {
                bool resultado = calculaIteracaoSimplex();                

                if (resultado)
                    fim = true;
            }

            cout << endl << "Variáveis básicas na última iteração: " << endl;

            auto it = base.begin();

            while (it != base.end())
            {
                cout << "x" << it->first + 1 << " " << it->second << " " << endl;
                it++;
            }

           cout << endl;

           cout << "Solução ótima: " << maximo << endl;
        }

};

int main()
{
    int tamanhoColunaA; 
    int tamanhoLinhaA; 

    cout << "Digite o número de coeficientes na função objetivo:" << endl;
    cin >> tamanhoColunaA;

    cout << "Digite o número de restrições do problema:" << endl;
    cin >> tamanhoLinhaA;

    std::vector <std::vector<float>> a(tamanhoLinhaA, std::vector<float>(tamanhoColunaA, 0));
    std::vector<float> b(tamanhoLinhaA,0);
    std::vector<float> c(tamanhoColunaA,0);

    for (int i = 0 ; i < tamanhoLinhaA ; i++)
    {    
        cout << "Digite os coeficientes da restrição " + to_string(i + 1) << endl;

        for (int j = 0 ; j < tamanhoColunaA ; j++)
            cin >> a[i][j];
    }

    cout << "Digite os valores do vetor B:\n";
    for (int i = 0 ; i < tamanhoLinhaA ; i++)
            cin >> b[i];

    cout << "Digite os coeficientes da função objetivo:\n";
    for (int i = 0 ; i < tamanhoColunaA ; i++)
        cin >> c[i];

      
    // hear the make the class parameters with A[m][n] vector b[] vector and c[] vector
    Simplex simplex(a,b,c);
    simplex.aplicaSimplex();

    return 0;
}
