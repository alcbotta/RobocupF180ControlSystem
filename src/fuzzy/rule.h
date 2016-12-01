#ifndef RULE
#define RULE
#define MAXRULES 150  // Maximum number of rules per rulebase for fuzzy_control

class rule : public TRegra
{

private:


public:
	trapezoid_category *inputcat1, *inputcat2, *inputcat3;

	trapezoid_category *outputcat;

        int num_inputs;

        float at1, at2, at3;

        rule();

	void definerule(trapezoid_category *input1,trapezoid_category *output);

	void definerule(trapezoid_category *input1,trapezoid_category *input2,trapezoid_category *output);

	void definerule(trapezoid_category *input1,trapezoid_category *input2,trapezoid_category *input3,

                        trapezoid_category *output);

	float evaluaterule(float &inputval1);
        float evaluaterule_memberships(float memb_input1);
        float evaluaterule(float &inputval1,float &inputval2);
        float evaluaterule_memberships(float memb_input1, float memb_input2);
        float evaluaterule(float &inputval1,float &inputval2,float &inputval3);
        float evaluaterule_memberships(float memb_input1, float memb_input2, float memb_input3);

	trapezoid_category *getoutputcategory();

        void clearinputcategoryactivations();

        void clearoutputcategoryactivations();

        void save_m(FILE *file);
        void save(FILE *file);

        void load(FILE *file);

        ~rule();
        void inserePremissa(trapezoid_category *tc);
        void insereConclusao(trapezoid_category *tc);
        void Notifica(trapezoid_category *at);
        void Notifica_Conclusao();

};
#endif

