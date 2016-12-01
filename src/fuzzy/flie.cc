// Projeto FLIE-Fuzzy Logic Inference Engine - João Alberto Fabro
// Adaptação/Utilização para o Trabalho de Conclusão de Curso - Algoritmos de Navegação Fuzzy: Uma Análise Qualitativa

//#define DOS
//#define DBG
#include <stdio.h>
#include <stdlib.h>
//#include <iostream.h>
#include <iostream>
using namespace std;

//#ifdef DOS
//#include <dir.h>
//#include <dos.h>
//#include <conio.h>
//#include <string.h>
//#endif

#include "flie.h"
#include "motor.h"
#include "FIE.h"

#include <unistd.h>
#include <stdio.h>
#include <errno.h>

flie::flie()
{
    flie_setup();
}

void flie::flie_setup()
{

    //Definição dos Trapézios para fuzzificação das leituras dos sensores
    float range_pertissimo [] = { 0, 0, 90, 200 };
    float range_perto[4] = { 0, 90, 300, 900 };
    float range_medio[4] = { 300, 900, 1000, 1200};
    float range_longe[4] = {1000, 1200, 7500, 7500};

    //Configuraçoes das funções de pertinência
    //Minimo e máximo para as entradas (sensores)
    rlow = 0;
    rhigh = range_longe [ 3 ];

    //Frente
    cat[0].setname("Perto");
    cat[0].setrange(rlow,rhigh);
    cat[0].setval(range_perto[0],range_perto[1],range_perto[2],range_perto[3]);

    cat[1].setname("Medio");
    cat[1].setrange(rlow,rhigh);
    cat[1].setval(range_medio[0],range_medio[1],range_medio[2],range_medio[3]);

    cat[2].setname("Longe");
    cat[2].setrange(rlow,rhigh);
    cat[2].setval(range_longe[0],range_longe[1],range_longe[2],range_longe[3]);

    //Lateral Esquerda (2 sensores)
    cat[3].setname("PertoEsquerda");
    cat[3].setrange(rlow,rhigh);
    cat[3].setval(range_perto[0],range_perto[1],range_perto[2],range_perto[3]);

    cat[4].setname("MedioEsquerda");
    cat[4].setrange(rlow,rhigh);
    cat[4].setval(range_medio[0],range_medio[1],range_medio[2],range_medio[3]);

    cat[5].setname("LongeEsquerda");
    cat[5].setrange(rlow,rhigh);
    cat[5].setval(range_longe[0],range_longe[1],range_longe[2],range_longe[3]);

    //Lateral Direita (2 sensores)
    cat[6].setname("PertoDireita");
    cat[6].setrange(rlow,rhigh);
    cat[6].setval(range_perto[0],range_perto[1],range_perto[2],range_perto[3]);
    cat[7].setname("MedioDireita");
    cat[7].setrange(rlow,rhigh);
    cat[7].setval(range_medio[0],range_medio[1],range_medio[2],range_medio[3]);
    cat[8].setname("LongeDireita");
    cat[8].setrange(rlow,rhigh);
    cat[8].setval(range_longe[0],range_longe[1],range_longe[2],range_longe[3]);

    float vel_lentissimo [] = { 0, 0, 0.4, 0.8 };
    float vel_lento [ 4 ] = { 0, 0, 0.5, 1 };
    float vel_medio [ 4 ] = { 0, 1, 3, 3.5 };
    float vel_rapido [ 4 ] = { 3, 3.5, 4, 4 };

    //Motor Velocidade

    cat[9].setname("Lento");
    cat[9].setrange( vel_lento [ 0 ], vel_rapido [ 3 ] );
    cat[9].setval( vel_lento[0], vel_lento [ 1 ], vel_lento [ 2 ], vel_lento [ 3 ] );

    cat[10].setname("Medio");
    cat[10].setrange( vel_lento [ 0 ], vel_rapido [ 3 ] );
    cat[10].setval( vel_medio [ 0 ], vel_medio [ 1 ], vel_medio [ 2 ], vel_medio [ 3 ] );

    cat[11].setname("Rapido");
    cat[11].setrange( vel_lento [ 0 ], vel_rapido [ 3 ] );
    cat[11].setval ( vel_rapido [ 0 ], vel_rapido [ 1 ], vel_rapido [ 2 ], vel_rapido [ 3 ] );

//    cat[12].setname("ViraDireita");
//    cat[12].setrange(0,180);
//    cat[12].setval(0,0,0,45);
//    cat[13].setname("ViraPoucoDireita");
//    cat[13].setrange(0, 180);
//    cat[13].setval(0, 45, 60, 90);
//    cat[14].setname("Reto");
//    cat[14].setrange(0,180);
//    cat[14].setval(60, 90,90,120);
//    cat[15].setname("ViraPoucoEsquerda");
//    cat[15].setrange(0, 180);
//    cat[15].setval( 90, 120, 135, 180);
//    cat[16].setname("ViraEsquerda");
//    cat[16].setrange(0,180);
//    cat[16].setval(135,180,180,180);


    cat[12].setname("ViraDireita");
    cat[12].setrange(0,180);
    cat[12].setval(0,0,15,30);
    cat[13].setname("ViraPoucoDireita");
    cat[13].setrange(0, 180);
    cat[13].setval(15, 30, 45, 60);
    cat[14].setname("Reto");
    cat[14].setrange(0,180);
    cat[14].setval(45, 60,90,105);
    cat[15].setname("ViraPoucoEsquerda");
    cat[15].setrange(0, 180);
    cat[15].setval( 90, 105, 120, 135);
    cat[16].setname("ViraEsquerda");
    cat[16].setrange(0,180);
    cat[16].setval(120,135,180,180);

    cat[17].setname("Lentissimo");
    cat[17].setrange(rlow,rhigh);
    cat[17].setval( vel_lentissimo[0], vel_lentissimo[ 1 ], vel_lentissimo[ 2 ], vel_lentissimo [ 3 ] );

//    cat[12].setname("ViraEsquerda");
//    cat[12].setrange(0,180);
//    cat[12].setval(0,0,0,45);
//    cat[13].setname("ViraPoucoEsquerda");
//    cat[13].setrange(0, 180);
//    cat[13].setval(0, 45, 45, 90);
//    cat[14].setname("Reto");
//    cat[14].setrange(0,180);
//    cat[14].setval(0,90,90,180);
//    cat[15].setname("ViraPoucoDireita");
//    cat[15].setrange(0, 180);
//    cat[15].setval(90, 135, 135, 180);
//    cat[16].setname("ViraDireita");
//    cat[16].setrange(0,180);
//    cat[16].setval(135,180,180,180);


    SensorFrente.setname("SensorFrente");
    SensorFrente.includecategory(&cat[0]);
    cat[0].define_lingvar(&SensorFrente);
    SensorFrente.includecategory(&cat[1]);
    cat[1].define_lingvar(&SensorFrente);
    SensorFrente.includecategory(&cat[2]);
    cat[2].define_lingvar(&SensorFrente);

    SensorEsquerda.setname("SensorEsquerda");

    SensorEsquerda.includecategory(&cat[3]);
    cat[3].define_lingvar(&SensorEsquerda);
    SensorEsquerda.includecategory(&cat[4]);
    cat[4].define_lingvar(&SensorEsquerda);
    SensorEsquerda.includecategory(&cat[5]);
    cat[5].define_lingvar(&SensorEsquerda);

    SensorDireita.setname("SensorDireita");
    SensorDireita.includecategory(&cat[6]);
    cat[6].define_lingvar(&SensorDireita);
    SensorDireita.includecategory(&cat[7]);
    cat[7].define_lingvar(&SensorDireita);
    SensorDireita.includecategory(&cat[8]);
    cat[8].define_lingvar(&SensorDireita);

    VelMotor.setname("Velocidade");
    VelMotor.includecategory(&cat[9]);
    cat[9].define_lingvar(&VelMotor);
    VelMotor.includecategory(&cat[10]);
    cat[10].define_lingvar(&VelMotor);
    VelMotor.includecategory(&cat[11]);
    cat[11].define_lingvar(&VelMotor);

//    AngMotor.setname("Angulo");
//    AngMotor.includecategory(&cat[16]);
//    cat[12].define_lingvar(&AngMotor);
//    AngMotor.includecategory(&cat[15]);
//    cat[13].define_lingvar(&AngMotor);
//    AngMotor.includecategory(&cat[14]);
//    cat[14].define_lingvar(&AngMotor);
//    AngMotor.includecategory(&cat[13]);
//    cat[15].define_lingvar(&AngMotor);
//    AngMotor.includecategory(&cat[12]);
//    cat[16].define_lingvar(&AngMotor);

    AngMotor.setname("Angulo");
    AngMotor.includecategory(&cat[12]);
    cat[12].define_lingvar(&AngMotor);
    AngMotor.includecategory(&cat[13]);
    cat[13].define_lingvar(&AngMotor);
    AngMotor.includecategory(&cat[14]);
    cat[14].define_lingvar(&AngMotor);
    AngMotor.includecategory(&cat[15]);
    cat[15].define_lingvar(&AngMotor);
    AngMotor.includecategory(&cat[16]);
    cat[16].define_lingvar(&AngMotor);


    fc.set_defuzz(CENTEROFAREA);
    fc.definevars(SensorEsquerda,SensorFrente,SensorDireita,VelMotor);

    fc2.set_defuzz(CENTEROFAREA);
    fc2.definevars(SensorEsquerda,SensorFrente,SensorDireita,AngMotor);

    char lingesquerda[64];
    char lingfrente[64];
    char lingdireita[64];
    char lingvel[64];
    char lingdirecao[64];
    FILE *regras = freopen("../ControlSystem/src/fuzzy/regras.txt","r",stdin);
    if ( regras == NULL )
    {
        regras = freopen("../src/fuzzy/regras.txt","r",stdin);
        if ( regras == NULL )
        {
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL)
                fprintf(stdout, "Current working dir: %s\n", cwd);
            printf ( "flie.cc - regras == NULL\n" );
            exit ( 0 );
        }

    }

    for(int i = 0; scanf("%s%s%s%s%s", lingesquerda, lingfrente, lingdireita, lingvel, lingdirecao) == 5; ++i){

        insertRule(i,lingesquerda,lingfrente,lingdireita,lingvel,lingdirecao);
#ifdef DBG
        printf("Regra %d [%s, %s, %s, %s, %s] inserida\n", i, lingesquerda, lingfrente, lingdireita,
                lingvel, lingdirecao);
#endif
    }

}

void flie::insertRule(int n,char *se, char *sf, char *sd, char *vm, char *d){
    fc.insert_rule(se,sf,sd,vm);
    infrule[n].inserePremissa(SensorEsquerda.getcatptr(se));
    infrule[n].inserePremissa(SensorFrente.getcatptr(sf));
    infrule[n].inserePremissa(SensorDireita.getcatptr(sd));
    infrule[n].insereConclusao(VelMotor.getcatptr(vm));
    fc2.insert_rule(se,sf,sd,d);
    infrule2[n].inserePremissa(SensorEsquerda.getcatptr(se));
    infrule2[n].inserePremissa(SensorFrente.getcatptr(sf));
    infrule2[n].inserePremissa(SensorDireita.getcatptr(sd));
    infrule2[n].insereConclusao(AngMotor.getcatptr(d));
}

void flie::inference(float se, float sf, float sd, float &vel, float &ang)
{
    //printf ( "se = %f\tsf = %f\tsd = %f\n", se, sf, sd );
    vel = fc.make_inference(se,sf,sd);
    ang = fc2.make_inference(se,sf,sd);
}

float flie::getRHigh()
{
    return rhigh;
}

float flie::getRLow()
{
    return rlow;
}


extern "C"
{
    flie *getFlie ()
    {
        return new flie;
    }

    void deleteFlie ( flie* _flie )
    {
        delete _flie;
    }
}
