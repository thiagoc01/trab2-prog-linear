#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

/*
The main method is in this program itself.
Instructions for compiling=>>
Run on any gcc compiler=>>
Special***** should compile in -std=c++11 or C++14 -std=gnu++11  *********  (mat be other versions syntacs can be different)
turorials point online compiler
==> go ti link   http://cpp.sh/ or  https://www.tutorialspoint.com/cplusplus/indice.htm and click try it(scorel below) and after go to c++ editor copy code and paste.
after that click button execute.
if you have -std=c++11 you can run in command line;
g++ -o output Simplex.cpp
./output
How to give inputs to the program =>>>
   Example:
	colSizeA = 6 // input colmn size
	tamanhoLinhaA = 3  // input row size
    float C[N]={-6,-5,-4,0,0,0};  //Initialize the C array  with the coefficients of the constraints of the objective function
    float B[M]={240,360,300};//Initialize the B array constants of the constraints respectively
   //initialize the A array by giving all the coefficients of all the variables
   float A[M][N] =  {
                 { 2,  1,  1,   1,  0, 0},
                { 1,  3,  2,   0,  1, 0 },
                {   2,    1,  2,   0,  0,  1}
                };
*/



class Simplex{

    private:
        int linhas, colunas;
        //stores coefficients of all the variables
        std::vector <std::vector<float> > A;
        //stores constants of constraints
        std::vector<float> B;
        //stores the coefficients of the objective function
        std::vector<float> C;

        float maximo;

        bool eIlimitado;

    public:
        Simplex(std::vector <std::vector<float> > matriz,std::vector<float> b ,std::vector<float> c){
            maximo = 0;
            eIlimitado = false;
            linhas = matriz.size();
            colunas = matriz[0].size();
            A.resize( linhas , vector<float>( colunas , 0 ) );
            B.resize(b.size());
            C.resize(c.size());




            for(int i= 0;i<linhas;i++){             //pass A[][] values to the metrix
                for(int j= 0; j< colunas;j++ ){
                    A[i][j] = matriz[i][j];

                }
            }



            for(int i=0; i< c.size() ;i++ ){      //pass c[] values to the B vector
                C[i] = c[i] ;
            }
            for(int i=0; i< b.size();i++ ){      //pass b[] values to the B vector
                B[i] = b[i];
            }




        }

        bool calculaIteracaoSimplex(){
            //check whether the table is optimal,if optimal no need to process further
            if(verificarSolucaoOtima()){
			    return true;
            }
            cout << "teste" << endl;
            //find the column which has the pivot.The least coefficient of the objective function(C array).
            int colunaNumPivo = achaColunaPivo();


            if(eIlimitado == true){
                cout<<"Error unbounded"<<endl;
			    return true;
            }
            
            //find the row with the pivot value.The least value item's row in the B array
            int linhaPivo = achaLinhaPivo(colunaNumPivo);
            //form the next table according to the pivot value
            realizaPivoteamento(linhaPivo,colunaNumPivo);
            return false;
        }

        bool verificarSolucaoOtima(){
             //if the table has further negative constraints,then it is not optimal
            bool eOtima = false;
            int contagemNumPositivos = 0;

            //check if the coefficients of the objective function are negative
            for(int i=0; i<C.size();i++){
                float value = C[i];
                if(value >= 0){
                    contagemNumPositivos++;
                }
            }
            //if all the constraints are positive now,the table is optimal
            if(contagemNumPositivos == C.size()){
                eOtima = true;
                printMatrizes();
            }
            return eOtima;
        }

        void realizaPivoteamento(int linhaPivo, int colunaNumPivo){

            float numPivo = A[linhaPivo][colunaNumPivo];//gets the pivot value

            float valoresLinhaPivo[colunas];//the column with the pivot

            float valoresColunaPivo[linhas];//the row with the pivot

            float linhaPivoAtualizada[colunas];//the row after processing the pivot value

            maximo = maximo - (C[colunaNumPivo]*(B[linhaPivo]/numPivo));  //set the maximum step by step
             //get the row that has the pivot value
             for(int i=0;i<colunas;i++){
                valoresLinhaPivo[i] = A[linhaPivo][i];
             }
             //get the column that has the pivot value
             for(int j=0;j<linhas;j++){
                valoresColunaPivo[j] = A[j][colunaNumPivo];
            }

            //set the row values that has the pivot value divided by the pivot value and put into new row
             for(int k=0;k<colunas;k++){
                linhaPivoAtualizada[k] = valoresLinhaPivo[k]/numPivo;
             }

            B[linhaPivo] = B[linhaPivo]/numPivo;


             //process the other coefficients in the A array by subtracting
             for(int m=0;m<linhas;m++){
                //ignore the pivot row as we already calculated that
                if(m !=linhaPivo){
                    for(int p=0;p<colunas;p++){
                        float multiplyValue = valoresColunaPivo[m];
                        A[m][p] = A[m][p] - (multiplyValue*linhaPivoAtualizada[p]);
                        //C[p] = C[p] - (multiplyValue*C[linhaPivo]);
                        //B[i] = B[i] - (multiplyValue*B[linhaPivo]);
                    }

                }
             }

            //process the values of the B array
            for(int i=0;i<B.size();i++){
                if(i != linhaPivo){

                        float multiplyValue = valoresColunaPivo[i];
                        B[i] = B[i] - (multiplyValue*B[linhaPivo]);

                }
            }
                //the least coefficient of the constraints of the objective function
                float multiplyValue = C[colunaNumPivo];
                //process the C array
                 for(int i=0;i<C.size();i++){
                    C[i] = C[i] - (multiplyValue*linhaPivoAtualizada[i]);

                }


             //replacing the pivot row in the new calculated A array
             for(int i=0;i<colunas;i++){
                A[linhaPivo][i] = linhaPivoAtualizada[i];
             }


        }

        //print the current A array
        void printMatrizes(){
            for(int i=0; i<linhas;i++){
                for(int j=0;j<colunas;j++){
                    cout<<A[i][j] <<" ";
                }
                cout<<""<<endl;
            }
            cout<<""<<endl;
        }

        //find the least coefficients of constraints in the objective function's position
        int achaColunaPivo(){

            int localizacao = 0;
            float minm = C[0];



            for(int i=1;i<C.size();i++){
                if(C[i]<minm){
                    minm = C[i];
                    localizacao = i;
                }
            }

            return localizacao;

        }

        //find the row with the pivot value.The least value item's row in the B array
        int achaLinhaPivo(int colunaNumPivo){
            float valoresPositivos[linhas];
            std::vector<float> resultado(linhas,0);
            //float resultado[linhas];
            int contagemNumNegativos = 0;

            for(int i=0;i<linhas;i++){
                if(A[i][colunaNumPivo]>0){
                    valoresPositivos[i] = A[i][colunaNumPivo];
                }
                else{
                    valoresPositivos[i]=0;
                    contagemNumNegativos+=1;
                }
            }
            //checking the unbound condition if all the values are negative ones
            if(contagemNumNegativos==linhas){
                eIlimitado = true;
            }
            else{
                for(int i=0;i<linhas;i++){
                    float value = valoresPositivos[i];
                    if(value>0){
                        resultado[i] = B[i]/value;

                    }
                    else{
                        resultado[i] = 0;
                    }
                }
            }
            //find the minimum's localizacao of the smallest item of the B array
            float minimo = 99999999;
            int localizacao = 0;
            for(int i=0;i< resultado.size() ;i++){
                if(resultado[i]>0){
                    if(resultado[i]<minimo){
                        minimo = resultado[i];

                        localizacao = i;
                    }
                }

            }

            return localizacao;

        }

        void aplicaSimplex(){
            bool fim= false;

            cout<<"initial array(Not optimal)"<<endl;
            printMatrizes();

            cout<<" "<<endl;
            cout<<"final array(Optimal solution)"<<endl;


            while(!fim){

                bool resultado = calculaIteracaoSimplex();

                if(resultado==true){

                    fim= true;


                    }
            }
            cout<<"Answers for the Constraints of variables"<<endl;

            for(int i=0;i< A.size(); i++){  //every basic column has the values, get it form B array
                int numZeros = 0;
                int indice = 0;
                for(int j=0; j< linhas; j++){
                    if(A[j][i]==0.0){
                        numZeros += 1;
                    }
                    else if(A[j][i]==1){
                        indice = j;
                    }


                }

                if(numZeros == linhas -1 ){

                    cout<<"variable"<<indice+1<<": "<<B[indice]<<endl;  //every basic column has the values, get it form B array
                }
                else{
                    cout<<"variable"<<indice+1<<": "<<0<<endl;

                }

            }


           cout<<""<<endl;
           cout<<"maximum value: "<<maximo<<endl;  //print the maximum values




        }

};

int main()
{

    int tamanhoColunaA=4;  //should initialise columns size in A
    int tamanhoLinhaA = 2;  //should initialise columns row in A[][] vector

    float C[]= {-70,-50,0,0};  //should initialis the c arry here
    float B[]={240, 100};  // should initialis the b array here



    float a[2][4] = {    //should intialis the A[][] array here
                   { 4,  3,  1, 0},
                   { 2,  1,  0, 1},
             };


        std::vector <std::vector<float> > vec2D(tamanhoLinhaA, std::vector<float>(tamanhoColunaA, 0));

        std::vector<float> b(tamanhoLinhaA,0);
        std::vector<float> c(tamanhoColunaA,0);




       for(int i=0;i<tamanhoLinhaA;i++){         //make a vector from given array
            for(int j=0; j<tamanhoColunaA;j++){
                vec2D[i][j] = a[i][j];
            }
       }





       for(int i=0;i<tamanhoLinhaA;i++){
            b[i] = B[i];
       }

        for(int i=0;i<tamanhoColunaA;i++){
            c[i] = C[i];
       }


      // hear the make the class parameters with A[m][n] vector b[] vector and c[] vector
      Simplex simplex(vec2D,b,c);
      simplex.aplicaSimplex();


    return 0;
}
