/**
 *@file pwm.h
 *@brief Ce fichier contient les fonctions nécéssaires à la génération des signaux PWM pour controler le moteur.
 *@date Oct 2, 2023
 *@author Tom
 *@author Baptiste
 *@author Antoine
 */

#ifndef INC_MYLIBS_PID_H_
#define INC_MYLIBS_PID_H_

typedef struct h_PID_struct
{
	float ki,kd,Te;
	float b0,b1,b2;
	int ordre;
}h_PID_t;

void ID_BO(int alpha);
int Erreur(float consigne);
int Erreur_I(float* consigne_current,float* adc_value);
int PID(h_PID_t * h_PID,float *input,float* output);
int Current_PI(h_PID_t * h_PID ,float* input, float* output);
float Out(float* input);

#endif /* INC_MYLIBS_PID_H_ */
