#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <random>
#include <time.h>
#include <ctime>
#include <chrono>

using namespace std;

/** Struct DadosCidade - armazenamento dos dados referentes as cidades. **/
struct DadosCidade
{
    string data;
    string estado;
    string nome;
    int codigo;
    int casos;
    int mortes;
};

/** Struct Desempenho - armazenamento referente as metricas de desempenho. **/
struct Desempenho
{
    string nome_algoritmo;                       // Nome do algoritmo (Nome)
    vector<int> numRegistros;                    // Numero de registros (N)
    vector<unsigned int> tempo;                  // Vetor double que guarda o tempo de cada execucao individual da combinacao (Nome, N)
    vector<long double> comparacoes;                     // Vetor int que guarda a qtd de comparacoes de cada execucao individual da combinacao (Nome, N)
    vector<long double> trocas;                          // Vetor int que guarda a qtd de trocas de cada execucao individual da combinacao (Nome, N)
    long double soma_tempo[5] = {0, 0, 0, 0, 0}; //Guarda a soma de tempo dos M
    long double soma_comparacoes[5] = {0, 0, 0, 0, 0};
    long double soma_troca[5] = {0, 0, 0, 0, 0};
};

/** Declaracao das funcoes principais**/
string preProcessamentoDados(string nomeArqEntrada);
void modTestes(string &arqv);
void quickSort(string &arqv, int tipoCaso);
void bucketSort(string &arqv, int tipoCaso);
void countingSort(string &arqv, int tipoCaso);

/** Declaracao das funcoes de exibir/imprimir dados**/
void imprimirCidade(vector<DadosCidade> &v, int caso);
void imprimirDesempenho(Desempenho d, int qtdValoresN, int caso);
void auxImpressao(vector<DadosCidade> cidades, vector<int> &nCasos, int caso);

/** Declaracao das funcoes auxiliares**/
int consoleOuArquivo();
vector<int> casoN(int caso);
vector<int> numCasos(vector<DadosCidade> &vet);
vector<DadosCidade> leAleatorio(string &origem, int qntd);
vector<DadosCidade> lerPreProcessamento(string &origem, int qntd);
int dividir(vector<int> &nCasos, int inicio, int fim, double &comparacoes, double &trocas);
void quickSortAux(vector<int> &nCasos, int inicio, int fim, double &comparacoes, double &trocas);

/**
Funcao main
Funcao com o menu principal. Permite a entrada do nome de arquivo, em *argv[], via linha de comando.
**/
int main(int argc, char *argv[])
{

    if (argc > 2) //Caso mais de um arquivo tenha sido passado na linha de comando.
    {
        cout << "Insira apenas um arquivo .csv na linha de comando." << endl;
        cout << "Modelo: ./main nome_do_arquivo.csv" << endl;
        return 0;
    }
    else if (argc == 1) //Caso nenhum arquivo tenha sido passado na linha de comando.
    {
        cout << "Insira um arquivo .csv na linha de comando." << endl;
        cout << "Modelo: ./main nome_do_arquivo.csv" << endl;
        return 0;
    }


    // String com o nome do arquivo
    string arquivo(argv[1]);

    cout << "Fazendo pre-processamento de dados..." << endl;
    arquivo = preProcessamentoDados(arquivo);

    if (arquivo == "0")
    {
        cout << "Nao foi possivel a conclusao do pre-processamento." << endl;
        return 0; // Encerra o programa.
    }

    cout << "Concluido o pre-processamento de dados..." << endl;
    cout << endl;

    srand(time(0)); // Usado para gerar os numeros aleatorios posteriormente

    //Menu de opcoes para o usuario
    cout << "MENU DE OPCOES" << endl;
    cout << "\n1 -> para a analise de desempenho do QuickSort" << endl;
    cout << "2 -> para a analise de desempenho do CountingSort" << endl;
    cout << "3 -> para a analise de desempenho do BucketSort" << endl;
    cout << "4 -> para abrir o Modulo de Testes" << endl;
    cout << "\nDigite o numero da sua escolha: ";

    int opcao;
    cin >> opcao;

    switch (opcao)
    {
    case 1:
        quickSort(arquivo, 0);
        cout << "\nO seu resultado esta no arquivo saida.txt" << endl;
        break;

    case 2:
        countingSort(arquivo, 0);
        cout << "\nO seu resultado esta no arquivo saida.txt" << endl;
        break;

    case 3:
        bucketSort(arquivo, 0);
        cout << "\nO seu resultado esta no arquivo saida.txt" << endl;
        break;

    case 4:
        modTestes(arquivo);
        break;

    default:
        cout << "\nOpcao invalida. Por favor, escolha um numero entre 1 e 4" << endl;
        return 0;
    }

    cout << "\nFIM!!!" << endl; //Eh pra comemorar!!!

    return 0;
}

/**
Funcao lerPreProcessamento
Argumentos: string com o nome do arquivo de entrada (string &origem) e a quantidade de dados necessarios (int qntd).
Retorno: um vetor de 'DadosCidade'.
Funcao para a leitura de uma quantidade para verificar os dados pre-processados a partir
do modulo de testes, selecionando aleatoriamente uma linha para ler uma quantidade definida de dados.
**/
vector<DadosCidade> lerPreProcessamento(string &origem, int qntd)
{
    //Ler as linhas do arquivo pre-processado e colocar em um vetor onde cada indice guarda uma linha da base de dados
    vector<string> arquivo_csv;
    arquivo_csv.clear();

    fstream arquivo;               // Arquivo de entrada com os dados
    arquivo.open(origem, ios::in); // Abrindo o arquivo com o parametro da funcao (origem).

    string linha; // Auxiliares na leitura dos dados

    getline(arquivo, linha); // Desconsidera a primeira linha de cabecalho

    while (!arquivo.fail())
    {
        getline(arquivo, linha);
        arquivo_csv.push_back(linha);
    }

    //Ler a quantidade N pedida e armazenar em um vetor do tipo DadosCidade
    vector<DadosCidade> vetCidades;

    vector<string> temp; //Vetor temporario
    string item;

    // Selecionar um numero inteiro aleatorio para ler a partir da linha entre 0 e 1000000
    int indice = rand() % 1000000;
    for (int i = 0; i < qntd; i++)
    {
        stringstream aux(arquivo_csv[indice++]); // Fucao para possibilitar a separacao da linha e ler cada coluna

        temp.clear(); //Limpar o vetor temporario

        while (getline(aux, item, ',')) //Le cada coluna da linha
        {
            temp.push_back(item);
        }

        DadosCidade dados;
        dados.data = temp[0];
        dados.estado = temp[1];
        dados.nome = temp[2];
        dados.codigo = stoi(temp[3]);
        dados.casos = stoi(temp[4]);
        dados.mortes = stoi(temp[5]);

        vetCidades.push_back(dados); //Armazenando as informacoes no vetor de retorno do tipo DadosCidade
    }

    arquivo.close(); //Fechar o arquivo

    return vetCidades;
}

/**
Funcao leAleatorio
Argumentos: string com o nome do arquivo de entrada (string &origem) e a quantidade de dados necessarios (int qntd).
Retorno: um vetor de 'DadosCidade'.
Funcao para capturar dados aleatorios do arquivo referente ao numero de casos e de
obitos de COVID-19 no Brasil e retornar um vetor de DadosCidades para as analises de desempenho.
**/
vector<DadosCidade> leAleatorio(string &origem, int qntd)
{
    //1a parte: ler as linhas do arquivo .csv e colocar em um vetor onde cada indice guarda uma linha da base de dados
    vector<string> arquivo_csv;
    arquivo_csv.clear();

    fstream arquivo;               // Arquivo de entrada com os dados
    arquivo.open(origem, ios::in); // Abrindo o arquivo com o parametro da funcao (origem).

    string linha; // Auxiliares na leitura dos dados

    getline(arquivo, linha); // Desconsidera a primeira linha de cabecalho

    while (!arquivo.fail())
    {
        getline(arquivo, linha);
        arquivo_csv.push_back(linha);
    }

    //2a parte: embaralhar o vetor, ou seja, embaralhar as cidades
    random_shuffle(arquivo_csv.begin(), arquivo_csv.end());

    //3a parte: ler a quantidade N pedida e armazenar em um vetor do tipo DadosCidade
    vector<DadosCidade> vetCidades;

    vector<string> temp; //Vetor temporario
    string item;

    int i = 0; //Inteiro auxiliar

    while (i < qntd)
    {
        stringstream aux(arquivo_csv[i]); // Fucao para possibilitar a separacao da linha e ler cada coluna

        temp.clear(); //Limpar o vetor temporario

        while (getline(aux, item, ',')) //Le cada coluna da linha
        {
            temp.push_back(item);
        }

        DadosCidade dados;
        dados.data = temp[0];
        dados.estado = temp[1];
        dados.nome = temp[2];
        dados.codigo = stoi(temp[3]);
        dados.casos = stoi(temp[4]);
        dados.mortes = stoi(temp[5]);

        vetCidades.push_back(dados); //Armazenando as informacoes no vetor de retorno do tipo DadosCidade
        i++;
    }

    arquivo.close(); //Fechar o arquivo

    return vetCidades;
}

/**
Funcao consoleOuArquivo
Funcao para auxiliar na funcao modTestes (Modulo de Testes), perguntando ao usuario se
prefere a saida dos resultados no console ou num arquivo, de acordo com os parametros do trabalho.
**/
int consoleOuArquivo()
{
    int opcao; //numero inteiro para armazenar a opcao
    do
    {
        cout << "\nVoce deseja que o resultado seja exibido" << endl;
        cout << "\n1 -> No console (com N = 10)" << endl;
        cout << "OU" << endl;
        cout << "2 -> No arquivo (com N = 100)" << endl;

        cout << "\nDigite o numero da sua escolha: ";
        cin >> opcao;

        switch (opcao)
        {
        case 1:
            return 1;
        case 2:
            return 2;
        default:
            cout << "\nOpcao invalida. Por favor, escolha entre 1 e 2." << endl;
            break;
        }
    } while (opcao < 1 || opcao > 2);

    cout << "\nFalha na funcao da escolha de exibir ou no console ou no arquivo" << endl;
    return -1;
}

/**
Funcao casoN
Argumento: numero inteiro, indicando o caso requisitado (execucao normal ou no modulo de testes - console ou arquivo).
Retorno: um vetor de inteiros com o conjunto de N registros.
Funcao para indicar os valores de N para o calculo e impressao das metricas de desempenho
do algoritmo de ordenacao, a partir do caso (ou o padrao com todos os valores de N ou
um dos dois casos do modulo de testes).
**/
vector<int> casoN(int caso)
{
    if (caso == 0) // Caso padrao, quando necessita executar o algoritmo com todos os valores de N
    {
        vector<int> vetorN{10000, 50000, 100000, 500000, 1000000}; // Todos os valores de N registros
        return vetorN;
    }
    else if (caso == 1) // Caso do Modulo de Teste com saida na linha de comando.
    {
        vector<int> vetorN{10}; // N definido quando a saida eh na linha de comando.
        return vetorN;
    }
    else if (caso == 2) // Caso do Modulo de Teste com saida no arquivo.
    {
        vector<int> vetorN{100}; // N definido quando a saida eh no arquivo.
        return vetorN;
    }

    cout << "\nFalha na funcao casoN." << endl;
    return {0};
}

/**
Funcao modTestes
Funcao com o sub-menu, referente ao modulo de testes. Possui rotinas em pequena quantidade
para verificacao do correto funcionamento.
**/
void modTestes(string &arqv)
{
    int opcao;

    do
    {
        cout << "\nMODULO DE TESTES" << endl;
        cout << "\n1 -> para exibir o pre-processamento dos dados" << endl;
        cout << "2 -> para exibir a importacao de N registros aleatorios" << endl;
        cout << "3 -> para exibir o resultado de cada algoritmo de ordenacao" << endl;
        cout << "0 -> para SAIR" << endl;

        cout << "\nDigite o numero da sua escolha: ";

        cin >> opcao;

        switch (opcao)
        {
        case 1: //Para exibir o pre-processamento dos dados
            if (consoleOuArquivo() == 1)
            {
                int n = 10;
                vector<DadosCidade> cidades = lerPreProcessamento(arqv, n);
                imprimirCidade(cidades, 1);
            }
            else
            {
                int n = 100;
                vector<DadosCidade> cidades = lerPreProcessamento(arqv, n);
                imprimirCidade(cidades, 2);
                cout << "\nO seu resultado esta no arquivo teste_registros.txt" << endl;
            }
            return;

        case 2: //Para exibir a importacao de N registros aleatorios
            if (consoleOuArquivo() == 1)
            {
                int n = 10;
                vector<DadosCidade> cidades = leAleatorio(arqv, n);
                imprimirCidade(cidades, 1);
            }
            else
            {
                int n = 100;
                vector<DadosCidade> cidades = leAleatorio(arqv, n);
                imprimirCidade(cidades, 2);
                cout << "\nO seu resultado esta no arquivo teste_registros.txt" << endl;
            }
            return;

        case 3: //Para exibir o resultado de cada algoritmo de ordenacao
            if (consoleOuArquivo() == 1)
            {
                countingSort(arqv, 1);
                quickSort(arqv, 1);
                bucketSort(arqv, 1);
            }
            else
            {
                countingSort(arqv, 2);
                quickSort(arqv, 2);
                bucketSort(arqv, 2);
                cout << "\nO seu resultado esta no arquivo teste_algoritmos.txt" << endl;
            }
            return;

        case 0: //Para sair do modulo de testes
            cout << "Modulo de testes encerrado." << endl;
            return;

        default:
            cout << "\nOpcao invalida. Por favor, escolha um numero entre 0 e 3" << endl;
            break;
        }

    } while (opcao < 0 || opcao > 3);
}

/**
Funcao auxImpressao
Funcao que verifica qual cidade, armazenada no vector<DadosCidade> cidades,  possui o numero de casos em vector<int> nCasos.
**/
void auxImpressao(vector<DadosCidade> cidades, vector<int> &nCasos, int caso)
{
    vector<DadosCidade> aux;
    aux.clear();

    for (int i = 0; i < nCasos.size(); i++) //Percorre o vetor nCasos, que está ordenado, e funciona como um gabarito
    {
        auto iterador = cidades.begin(); //Iterador apontando para o começo do vetor cidades
        int j = 0;

        //Percorre o vetor cidades atrás da primeira ocorrência do elemento i do vetor nCasos
        while (j < cidades.size())
        {
            if (cidades[j].casos == nCasos[i])
            {
                aux.push_back(cidades[j]); //Quando encontra, adiciona a cidade ao vetor aux, que estará ordenado
                cidades.erase(iterador);   //Após colocar em aux, apaga a cidade do vetor cidades para não lê-la novamente
            }
            j++;
            iterador++;
        }
    }

    if (caso == 1) // Caso seja exibido no console (modulo de testes)
    { 
        imprimirCidade(aux, 1); 
    }

    else // Caso seja exibido no arquivo .txt
    {
        imprimirCidade(aux, 3);
    }
    aux.clear();
}

/**
Funcao imprimirDesempenho
Argumentos: dados de desempenho(Desempenho d), numero de valores de N utilizados (int qtdValoresN) e o tipo de caso (int caso).
Funcao para exibir/imprimir os resultados das metricas de desempenho, com todos os valores de N
requisitados, de acordo com o tipo de caso.
**/
void imprimirDesempenho(Desempenho d, int qtdValoresN, int caso)
{
    if (caso == 0) //Saida padrao. Produz um arquivo saida.txt com o resultado do desempenho do algoritmo de ordenacao
    {
        fstream saida("saida.txt", ios::out); //Cria um arquivo novo. Nao acrescenta apendice

        saida << d.nome_algoritmo << endl;

        for (int i = 0; i < (qtdValoresN * 5); i += 5)
        {
            saida << "\nN = " << d.numRegistros[i / 5] << endl;
            saida << "\nM | Tempo (ns) | Comparacoes | Trocas" << endl;

            for (int j = 0; j < 5; j++)
            {
                saida << j + 1 << " | " << d.tempo[j + i] << " | " << d.comparacoes[j + i] << " | " << d.trocas[j + i] << endl;
            }
            saida << "Medias | " << d.soma_tempo[i / 5] / 5.0 << " | " << d.soma_comparacoes[i / 5] / 5.0 << " | " << d.soma_troca[i / 5] / 5.0 << endl;
        }
    }
}

/**
Funcao imprimirCidade
Argumentos: dados da cidade (vector<DadosCidade> &v) e o tipo de caso (int caso).
Funcao para exibir/imprimir os dados referentes as cidades no modulo de testes, de acordo com o tipo de caso.
**/
void imprimirCidade(vector<DadosCidade> &v, int caso)
{
    if (caso == 1) // Exibir no console
    {
        cout << "\n";

        for (int i = 0; i < v.size(); i++)
        {
            cout << v[i].data << " | ";
            cout << v[i].estado << " | ";
            cout << v[i].nome << " | ";
            cout << v[i].codigo << " | ";
            cout << v[i].casos << " | ";
            cout << v[i].mortes << endl;
        }

        cout << endl;
    }

    else if (caso == 2) // Imprimir no arquivo
    {
        fstream saida("teste_registros.txt", ios::out); //Cria um arquivo novo

        for (int i = 0; i < v.size(); i++)
        {
            saida << v[i].data << " | ";
            saida << v[i].estado << " | ";
            saida << v[i].nome << " | ";
            saida << v[i].codigo << " | ";
            saida << v[i].casos << " | ";
            saida << v[i].mortes << endl;
        }
    }

    else if (caso == 3) // Imprimir no arquivo
    {
        fstream saida("teste_algoritmos.txt", ios::app); //Cria um arquivo novo
        saida << "\n";

        for (int i = 0; i < v.size(); i++)
        {
            saida << v[i].data << " | ";
            saida << v[i].estado << " | ";
            saida << v[i].nome << " | ";
            saida << v[i].codigo << " | ";
            saida << v[i].casos << " | ";
            saida << v[i].mortes << endl;
        }
    }
}

/**
Funcao preProcessamento (principal da etapa)
Argumentos: string com o nome do arquivo de entrada (string &arqv).
Retorno: string com o nome do arquivo com os dados pré-processados.
Funcao para realizar o pre-processamento de dados, com Bucket Sort como ordenacao inicial.
**/
string preProcessamentoDados(string nomeArqEntrada)
{
    int tam = 1431490;                         // Quantidade de dados esperados no arquivo
    int numCidades = 5570;                     // Quantidade de cidades, necessario para a ordenacao inicial
    DadosCidade *dados = new DadosCidade[tam]; // Vetor de estrutura referente aos dados
    vector<string> aux;
    DadosCidade temp;         // Auxiliar na captura das informacoes.
    string buffer;            // Auxiliar na leitura dos dados
    ifstream arqEntrada;      // arquivo de entrada
    fstream arqSaidaPreDados; // arquivo de saida

    arqEntrada.open(nomeArqEntrada, ios::in); // Abrindo o arquivo com o parametro da funcao (origem).

    if (!arqEntrada.is_open())
    {
        cout << "Erro de leitura do arquivo informado." << endl;
        return "0";
    }

    getline(arqEntrada, buffer, '\n'); // Desconsidera a primeira linha de cabecalho
    buffer.clear();                    // Limpa o buffer

    int i = 0; // indice auxiliar para preenchimento

    // Faz a leitura dos dados
    while (!arqEntrada.eof() && i < tam)
    {
        getline(arqEntrada, buffer, '\n');

        stringstream fSeparar(buffer); // Fucao para possibilitar a separacao da linha e ler cada coluna

        aux.clear(); //Limpar o vetor temporario

        while (getline(fSeparar, buffer, ',')) //Le cada coluna da linha
        {
            aux.push_back(buffer);
            buffer.clear();
        }

        temp.data = aux[0];
        temp.estado = aux[1];
        temp.nome = aux[2];
        temp.codigo = stoi(aux[3]);
        temp.casos = stoi(aux[4]);
        temp.mortes = stoi(aux[5]);
        dados[i] = temp;
        i++;
    }

    // Ordenacao utilizando baldes por meio da biblioteca vector de duas dimensoes
    vector<DadosCidade> listaBaldes[numCidades]; // vetor para ser utilizado nos baldes.
    int indiceBalde;                             // indice auxiliar para indicar o balde

    //Ordenar os valores pela combinacao de UF e Cidade, e a data em seguida pelo Bucket Sort (Listas)
    for (int i = 0; i < tam; i++)
    {
        indiceBalde = i % numCidades; //o indice do dado de cada cidade esta relacionado ao resto da divisão da qtd total de cidades do Brasil
        listaBaldes[indiceBalde].push_back(dados[i]);
    }

    //Voltar ao vetor principal
    int auxTam = 0;       // indice para auxilio na insercao do vetor principal
    int auxTamBaldes = 0; // indice para percorrer o vetor de baldes

    while (auxTamBaldes < numCidades)
    {
        while (!listaBaldes[auxTamBaldes].empty())
        {
            dados[auxTam++] = *(listaBaldes[auxTamBaldes].begin());
            listaBaldes[auxTamBaldes].erase(listaBaldes[auxTamBaldes].begin());
        }
        auxTamBaldes++; //selecionar a próximo balde do vetor de baldes
    }

    //Transformar os casos acumulados em casos diarios, com a lista ja ordenada
    for (int i = tam - 1; i > 0; i--)
    {
        if (dados[i].codigo == dados[i - 1].codigo)
        {
            dados[i].casos = dados[i].casos - dados[i - 1].casos;
            dados[i].mortes = dados[i].mortes - dados[i - 1].mortes;
            // Pode acontecer casos em que os dados de casos e mortes não estejam acumulados e derem
            // negativo na subtracao. O grupo decidiu substiuir zero nesses casos.
            if (dados[i].casos < 0)
            {
                dados[i].casos = 0;
            }
            if (dados[i].mortes < 0)
            {
                dados[i].mortes = 0;
            }
        }
    }

    string nomeArqSaidaPreDados = "brazil_covid19_cities_processado.txt"; // nomeia o arquivo de acordo com a definicao do trabalho
    arqSaidaPreDados.open(nomeArqSaidaPreDados, ios::out | ios::trunc);   // apaga qualquer dado já existente

    if (!arqSaidaPreDados)
    {
        cout << "Nao foi possivel abrir o arquivo de pre-processamento de dados!!\n";
        return "0";
    }

    arqSaidaPreDados << "date,state,name,code,cases,deaths" << endl;
    for (int i = 0; i < tam; i++)
    {
        arqSaidaPreDados << dados[i].data << "," << dados[i].estado << "," << dados[i].nome << ",";
        arqSaidaPreDados << dados[i].codigo << "," << dados[i].casos << "," << dados[i].mortes << endl;
    }

    delete[] dados;
    arqEntrada.close();
    arqSaidaPreDados.close();
    return nomeArqSaidaPreDados;
}

/**
Funcao numCasos
Argumento: ponteiro do vetor de DadosCidade (&vet).
Retorno: um vetor de inteiros indicando somente os numeros de casos obtidos do &vet.
Funcao para colocar os valores aleatorios de casos do vetor de DadosCidade em
um vetor int separado para ordenar.
**/
vector<int> numCasos(vector<DadosCidade> &vet)
{
    vector<int> num;
    num.clear();

    for (int i = 0; i < vet.size(); i++)
    {
        num.push_back(vet[i].casos); //Convertendo string em int e colocando o valor no vetor num
    }
    return num;
}

/**
Funcao countingSort
Argumentos: string com o nome do arquivo de entrada (string &arqv) e o tipo do caso pedido, podendo ser
o padrao - com todos os valores de N - ou um dos dois casos do modulo de testes (int tipoCaso).
Algoritmo de ordenacao Counting Sort, com a exibicao dos resultados das metricas de desempenho.
**/
void countingSort(string &arqv, int tipoCaso)
{
    vector<int> N;
    N.clear();
    N = casoN(tipoCaso);

    vector<DadosCidade> cidades;
    vector<int> nCasos;

    Desempenho desempenho;
    desempenho.nome_algoritmo = "COUNTING SORT";
    desempenho.numRegistros = N;

    for (int i = 0; i < N.size(); i++)
    {

        for (int M = 1; M <= 5; M++) //Numero de repeticoes para se calcular a media
        {
			if(tipoCaso == 0){ // Caso for fazer a analise, mostrar a execucao.
				cout << "Execucao " << M << " de 5 para N=" << N[i] << endl;
			}
			
            cidades = leAleatorio(arqv, N[i]); //Preenche o vetor cidades com os N registros aleatorios
            nCasos = numCasos(cidades);

            auto ti = chrono::high_resolution_clock::now(); //Comeca a contar o tempo

            int max = *max_element(nCasos.begin(), nCasos.end());
            int min = *min_element(nCasos.begin(), nCasos.end());
            int amplitude = max - min + 1; //Quantidade de possiveis numeros diferentes no vetor nCasos

            vector<int> contagem(amplitude + 1);

            for (int i = 0; i < nCasos.size(); i++)
            {
                contagem[nCasos[i]]++;
            }

            for (int i = 0, j = 0; i < amplitude + 1; i++)
            {
                while (contagem[i]-- > 0)
                {
                    nCasos[j++] = i;
                }
            }

            auto tf = chrono::high_resolution_clock::now(); //Termina de contar o tempo
            auto duracao = chrono::duration_cast<chrono::nanoseconds>(tf - ti).count();

            if (tipoCaso == 1)
            {
                cout << "\nCOUNTING SORT" << endl;
                auxImpressao(cidades, nCasos, 1);
				contagem.clear(); //Limpa o vetor contagem para a proxima repeticao
				cidades.clear(); //Limpa o vetor cidades para a proxima repeticao
				nCasos.clear();  //Limpa o vetor nCasos para a proxima repeticao
                return;
            }
            else if (tipoCaso == 2)
            {
                fstream saida("teste_algoritmos.txt", ios::app); //Cria um arquivo novo
                saida << "COUNTING SORT\n";

                auxImpressao(cidades, nCasos, 3);
				contagem.clear(); //Limpa o vetor contagem para a proxima repeticao
				cidades.clear(); //Limpa o vetor cidades para a proxima repeticao
				nCasos.clear();  //Limpa o vetor nCasos para a proxima repeticao
                return;
            }

            //Atualizar o desempenho
            desempenho.tempo.push_back(duracao); //Guardando os tempos de cada M
            desempenho.soma_tempo[i] += duracao; //Guardando o total de tempo das 5 execucoes para depois fazer a media
            desempenho.comparacoes.push_back(0);
            desempenho.soma_comparacoes[i] += 0;
            desempenho.trocas.push_back(0);
            desempenho.soma_troca[i] += 0;
			
			contagem.clear(); //Limpa o vetor contagem para a proxima repeticao
            cidades.clear(); //Limpa o vetor cidades para a proxima repeticao
            nCasos.clear();  //Limpa o vetor nCasos para a proxima repeticao
        }
        if (tipoCaso == 0)
        {
            imprimirDesempenho(desempenho, N.size(), tipoCaso); //Escreve as metricas de desempenho no arquivo saida.txt
        }
    }
}

/**
Funcao quickSortAux
Argumentos: vetor de inteiros com o numero de casos (vector<int> &nCasos), indice de inicio (int inicio) e de
final (int final) para a divisão do vetor para ordenacao, quantidade de comparacoes e trocas para a computacao
das metricas de desempenho.
Funcao recursiva auxiliar do Quick Sort para fazer a estrategia do dividir para conquistar.
**/
void quickSortAux(vector<int> &nCasos, int inicio, int fim, double &comparacoes, double &trocas)
{
    int pivo;

    if (fim > inicio)
    {
        pivo = dividir(nCasos, inicio, fim, comparacoes, trocas);
        quickSortAux(nCasos, inicio, pivo - 1, comparacoes, trocas); // pivo - 1 indica ordenar ate o pivo
        quickSortAux(nCasos, pivo + 1, fim, comparacoes, trocas);
    }
}

/**
Funcao dividir
Argumentos: vetor de inteiros com o numero de casos (vector<int> &nCasos), indice de inicio (int inicio) e de
final (int final) para a divisão do vetor para a ordenacao, quantidade de comparacoes e trocas para a
computacao das metricas de desempenho.
Retorno: indice do novo pivo
Funcao auxiliar do Quick Sort para fazer a troca do pivo e a atualizacao do indice do novo pivo.
**/
int dividir(vector<int> &nCasos, int inicio, int fim, double &comparacoes, double &trocas)
{
    int esq, dir, pivo, aux;
    esq = inicio;
    dir = fim;
    pivo = nCasos[inicio];
    while (esq < dir)
    {
        while (esq <= fim && nCasos[esq] <= pivo)
        {
            comparacoes++;
            esq++;
        }
        comparacoes++;
        while (dir >= 0 && nCasos[dir] > pivo)
        {
            comparacoes++;
            dir--;
        }
        comparacoes++;
        if (esq < dir)
        {
            aux = nCasos[esq];
            nCasos[esq] = nCasos[dir];
            nCasos[dir] = aux;
            trocas++;
        }
    }
    nCasos[inicio] = nCasos[dir];
    nCasos[dir] = pivo;
    trocas++;

    return dir;
}

/**
Funcao quickSort
Argumentos: string com o nome do arquivo de entrada (string &arqv) e o tipo do caso pedido, podendo ser
o padrao - com todos os valores de N - ou um dos dois casos do modulo de testes (int tipoCaso).
Algoritmo de ordenacao Quick Sort, com a exibicao dos resultados das metricas de desempenho.
**/
void quickSort(string &arqv, int tipoCaso)
{
    vector<int> N;
    N.clear();
    N = casoN(tipoCaso);
	
    vector<DadosCidade> cidades;
    vector<int> nCasos;

    Desempenho desempenho;
    desempenho.nome_algoritmo = "QUICK SORT";
    desempenho.numRegistros = N;

    for (int i = 0; i < N.size(); i++)
    {

        for (int M = 1; M <= 5; M++) //Número de repetições para se calcular a média
        {
			if(tipoCaso == 0){ // Caso for fazer a analise, mostrar a execucao.
				cout << "Execucao " << M << " de 5 para N=" << N[i] << endl;
			}
			
            cidades = leAleatorio(arqv, N[i]); //Preenche o vetor cidades com os N registros aleatórios
            nCasos = numCasos(cidades);
            double comparacoes = 0, trocas = 0;

            auto ti = chrono::high_resolution_clock::now(); //Começa a contar o tempo
			
            quickSortAux(nCasos, 0, (cidades.size() - 1), comparacoes, trocas);

            auto tf = chrono::high_resolution_clock::now(); //Termina de contar o tempo
            auto duracao = chrono::duration_cast<chrono::nanoseconds>(tf - ti).count();

            if (tipoCaso == 1) //Para imprimir os registros ordenados no console
            {
                cout << "QUICK SORT" << endl;
                auxImpressao(cidades, nCasos, 1);
				cidades.clear();
				nCasos.clear();
                return;
            }
            else if (tipoCaso == 2) //Para imprimir os registros ordenados em um arquivo teste_algoritmos.txt
            {
                fstream saida("teste_algoritmos.txt", ios::app); //Cria um arquivo novo
                saida << "QUICK SORT\n";
                auxImpressao(cidades, nCasos, 3);
				cidades.clear();
				nCasos.clear();
                return;
            }

            //Atualizar o desempenho
            desempenho.tempo.push_back(duracao); //Guardando os tempos de cada M
            desempenho.soma_tempo[i] += duracao; //Guardando o total de tempo das 5 execuções para depois fazer a média
            desempenho.comparacoes.push_back(comparacoes);
            desempenho.soma_comparacoes[i] += comparacoes;
            desempenho.trocas.push_back(trocas);
            desempenho.soma_troca[i] += trocas;

            cidades.clear(); //Limpa o vetor cidades para a próxima repetição
            nCasos.clear();  //Limpa o vetor nCasos para a próxima repetição
        }
    }
    if (tipoCaso == 0)
    {
        imprimirDesempenho(desempenho, N.size(), tipoCaso); //Escreve as metricas de desempenho no arquivo saida.txt
    }
}

/**
Funcao bucketSort
Argumentos: ponteiro de um vetor, o qual é um balde, e um inteiro de contagem de comparacoes, para a analise.
Algoritmo de ordenacao auxiliar do Bucket Sort, ordenando cada balde.
A metodologia aplicaca nesta funcao e a InsertionSort, devido aos dados bastante repetidos.
**/
void auxOrdenacaoBucketSort(vector<int> &v, long double &comparacoes, long double &trocas)
{
	int pivo, k, qtdTrocas;
	for(int i = 1; i < v.size(); i++) {
		pivo = v[i];
		k = i - 1;

		while ((k >= 0) && (v[k] > pivo)) {
			v[k + 1] = v[k];
			qtdTrocas++;
			k--;
		}
		v[k + 1] = pivo;
	}

	comparacoes += qtdTrocas + (v.size() - 1);
	trocas += qtdTrocas;
}

/**
Funcao bucketSort
Argumentos: string com o nome do arquivo de entrada (string &arqv) e o tipo do caso pedido, podendo ser
o padrao - com todos os valores de N - ou um dos dois casos do modulo de testes (int tipoCaso).
Algoritmo de ordenacao Bucket Sort, com a exibicao dos resultados das metricas de desempenho.
**/
void bucketSort(string &arqv, int tipoCaso)
{
    vector<int> N;
    N.clear();
    N = casoN(tipoCaso);

    vector<DadosCidade> cidades;
    vector<int> nCasos;
    int numBaldes = 10; // Quantidade de baldes utilizados
    vector<vector<int>> listaBaldes; // vetor para ser utilizado nos baldes.

    Desempenho desempenho;
    desempenho.nome_algoritmo = "BUCKET SORT";
    desempenho.numRegistros = N;
    long double comparacoes;
    long double trocas;

    for (int i = 0; i < N.size(); i++)
    {

        for (int M = 1; M <= 5; M++) //Numero de repeticoes para se calcular a media
        {
            if(tipoCaso == 0){ // Caso for fazer a analise, mostrar a execucao.
				cout << "Execucao " << M << " de 5 para N=" << N[i] << endl;
			}
            
			cidades = leAleatorio(arqv, N[i]); //Preenche o vetor cidades com os N registros aleatorios
            nCasos = numCasos(cidades);
            listaBaldes.resize(numBaldes);
            comparacoes = 0;
            trocas = 0;

            auto ti = chrono::high_resolution_clock::now(); //Comeca a contar o tempo

            // Ordenacao utilizando baldes por meio da biblioteca vector de duas dimensoes
            int tam = nCasos.size();
            int indiceBalde; // indice auxiliar para indicar o balde
            //Ordenar os valores pelo numero de casos, e o indice sera calculado por intervalos, verificando a abrangencia da qtd de casos
            // Colocar os valores nos baldes
			for(int k=0; k<tam; k++){

                // Relaciono o indice do balde referente ao intervalo do numero de casos
                if(nCasos[k] == 0){
                    indiceBalde = 0; // Todos os dados com zero casos, no balde 0
                } else if(nCasos[k] == 1){
                    indiceBalde = 1;  // Todos os dados com 1 caso, no balde 1
                } else if(nCasos[k] == 2){
                    indiceBalde = 2; // Todos os dados com 2 casos, no balde 2
                } else if(nCasos[k] >= 3 && nCasos[k] <= 5){
                    indiceBalde = 3; // Todos os dados com 3 a 5 casos, no balde 3
                } else if(nCasos[k] > 5 && nCasos[k] <= 10){
                    indiceBalde = 4;  // Todos os dados com 6 a 10 casos, no balde 4
                } else if(nCasos[k] > 10 && nCasos[k] <= 20){
                    indiceBalde = 5; // Todos os dados com 11 a 20 casos, no balde 5
                } else if(nCasos[k] > 20 && nCasos[k] <= 40){
                    indiceBalde = 6; // Todos os dados com 21 a 40 casos, no balde 6
                } else if(nCasos[k] > 40 && nCasos[k] <= 100){
                    indiceBalde = 7;  // Todos os dados com 41 a 100 casos, no balde 7
                } else if(nCasos[k] > 100 && nCasos[k] <= 500){
                    indiceBalde = 8; // Todos os dados com 101 a 500 casos, no balde 8
                } else {
                    indiceBalde = 9; // Todos os dados restantes, no balde 9
                }

                listaBaldes[indiceBalde].push_back(nCasos[k]); // Se estiver vazia, acrescenta um elemento.
            }

			// Ordenar os baldes internamente, utilizando InsertionSort.
			// Excetua os baldes de 0 a 2, pois só contém um valor e aqueles que estao vazios.
			for(int k = 3; k<numBaldes;k++){
				if(!listaBaldes[k].empty())
					auxOrdenacaoBucketSort(listaBaldes[k], comparacoes, trocas);
            }

            //Voltar ao vetor principal
            int auxTam = 0; // indice para auxilio na insercao do vetor principal
            while(auxTam < tam){
                for(int k=0; k<numBaldes; k++){
                    if(!listaBaldes[k].empty()){
                        for (auto it = listaBaldes[k].begin(); it != listaBaldes[k].end(); ++it){
                            nCasos[auxTam] = *it;
                            auxTam++;
                        }
                    }
                }
            }

            auto tf = chrono::high_resolution_clock::now(); //Termina de contar o tempo
            auto duracao = chrono::duration_cast<chrono::nanoseconds>(tf - ti).count();

            for(int k=0; k<numBaldes; k++){
                if(!listaBaldes.empty()){
                    listaBaldes[k].clear(); //Limpa o vetor de cada balde
                }
            }
            
            if (tipoCaso == 1) //Para imprimir os registros ordenados no console
            {
                cout << "BUCKET SORT" << endl;
                auxImpressao(cidades, nCasos, 1);
				cidades.clear();
				nCasos.clear();
                return;
            }
            else if (tipoCaso == 2) //Para imprimir os registros ordenados em um arquivo teste_algoritmos.txt
            {
                fstream saida("teste_algoritmos.txt", ios::app); //Cria um arquivo novo
                saida << "BUCKET SORT\n";
                auxImpressao(cidades, nCasos, 3);
				cidades.clear();
				nCasos.clear();
                return;
            }

            //Atualizar o desempenho
            desempenho.tempo.push_back(duracao); //Guardando os tempos de cada M
            desempenho.soma_tempo[i] += duracao; //Guardando o total de tempo das 5 execuções para depois fazer a média
            desempenho.comparacoes.push_back(comparacoes);
            desempenho.soma_comparacoes[i] += comparacoes;
            desempenho.trocas.push_back(trocas);
            desempenho.soma_troca[i] += trocas;
            
            cidades.clear(); //Limpa o vetor cidades para a proxima repeticao
            nCasos.clear();  //Limpa o vetor nCasos para a proxima repeticao


    }
    if (tipoCaso == 0)
    {
        imprimirDesempenho(desempenho, N.size(), tipoCaso); //Escreve as metricas de desempenho no arquivo saida.txt
    }
}
}
