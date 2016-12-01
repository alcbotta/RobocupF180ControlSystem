#ifndef MOTOR
#define MOTOR
#define TAM 10
enum Qual {Maior,Menor,Igual};

//typedef bool boolean ; /*enum boolean {false,true};*/

typedef int TVetor[TAM];


class TTipoPremissa
{
    public:
		 char *atributo;
         float valor;
         int tamvp;
         TVetor vPremissas;

             TTipoPremissa();
         TTipoPremissa(char *newAtributo, float newV);
         void setValor(float newValor);
         TVetor *getVPremissas();
         int getValorPremissa(int premissa);
         int getTamvp();
         void inserePremissa (int premissa);

         void Notificacao();
};

class TCondicao
{
    public:
        float valor;
        float valorRecebido;

        TCondicao(float newV, float newVR);
        void setValorRecebido(float newVR);
        bool Maior(); //tipo (1)
        bool Menor(); //tipo (2)
        bool Igual(); //tipo (3)
        ~TCondicao();
};
class TPremissa
{
    public:
		 char *atributo;
         TCondicao condicao;
         bool status;
         Qual tipo;
         TVetor vRegras;
         int tamvr;

         TPremissa();
         TPremissa (Qual newTipo, char *newAtributo, float newV, float newVR, bool newStatus);
         void setStatus (float valorRecebido);
         bool getStatus();
         Qual getTipo();
         TVetor *getVRegras();
         void InsereRegras(int regra);
         int getValorRegra(int regra);
         int getTamvr();
         void Notifica();
};


class TConclusao
{
    public:
		char *mensagem;
        TVetor vRegras;
        bool status;

        TConclusao(char *newMensagem, bool newStatus);
        char * getMensagem();
        TVetor *getVRegras();
        void setStatus (bool newStatus);
        bool getStatus();
};

class TRegra
{
    public:
        char *nome;
        TVetor vPremissas;
        TVetor conclusao;
        int tamvp, tamc;

        TRegra();
        TRegra (char * newNome);
        char * getNome();
        TVetor *getPremissas();
        TVetor *getConclusao();
        int getTamvp();
        int getTamc();
        void InserePremissa(int Premissa);
        void InsereConclusao(int newConclusao);
        int getValorPremissa (int premissa);
        int getValorConclusao();
        void Notifica();
};

typedef TPremissa *TVP[10];
typedef TRegra *TR[10];
typedef TConclusao *TC[10];

#endif
