#ifndef flieheader
#define flieheader
#include <iostream>
#include "motor.h"
#include "fuzzfier.h"
#include "lingvar.h"
#include "rule.h"
#include "fuzzy_control.h"
#include "stdlib.h"
#include "string.h"
#include "FIE.h"

#define tnorm(x,y) ((x)<(y)?(x):(y)) //min : macros that define the T-norm
#define snorm(x,y) ((x)<(y)?(y):(x)) //max : and the S-norm used in the fuzzy inference

class flie
{
	public:

    flie ();
	subat cat[21];

	linguisticvariable SensorEsquerda,SensorFrente,SensorDireita, VelMotor, AngMotor;

	rule infrule[54];
	rule infrule2[54];

	fuzzy_control fc,fc2;

	float PercPwm, Ang;

	void flie_setup();
	void insertRule(int, char *, char *, char *, char*, char*);
    virtual void inference(float , float , float , float &, float &);
    virtual float getRHigh ();
    virtual float getRLow ();

private:
    float rlow;
    float rhigh;

};

extern "C"
{
    typedef flie *flie_load_t ();
    typedef void flie_unload_t ( flie * );
    flie *getFlie ();
    void deleteFlie ( flie * _flie);
}

#endif
