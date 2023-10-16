/**
 *@file pwm.c
 *@brief Ce fichier contient les fonctionnement nécéssaires à l'asservissement du moteur.
 *@date Oct 16, 2023
 *@author Tom
 *@author Baptiste
 *@author Antoine
 */

#include "mylibs/codeur.h"
#include "mylibs/pwm.h"
#include "mylibs/shell.h"
#include "mylibs/PID.h"


int tab_speed[500];
int idx=2;
float erreur[3]={0};
float erreur_I[3]={0};
float pid_output[3]={0};
float Current_output[3]={0};
float New_consigne=0;
extern float vitesse[3];

/*static void ID_BO(int alpha)
{
	static int i = 0;
	int speed;
	pwm_start();
	for (;;)
	{
		set_pwm_alpha(100);
		speed = calc_speed();

		if (i < 500)
		{
			tab_speed[i]= speed;
			i++;
		}
		pwm_stop();
	}
}
 */

int Erreur(float consigne){
	erreur[0]=erreur[1];
	erreur[1]=erreur[2];
	erreur[idx]=consigne-vitesse[idx];
	return 0;  // Return an appropriate value.
}

int Erreur_I(float* consigne_current,float* adc_value){
	erreur_I[0]=erreur_I[1];
	erreur_I[1]=erreur_I[2];
	erreur_I[idx]=consigne_current[idx]-adc_value[idx];
	return 0;  // Return an appropriate value.
}

int PID(h_PID_t * h_PID,float *input,float* output){
	output[idx]=output[idx-2]+h_PID->b0*input[idx]+h_PID->b1*input[idx-1]+h_PID->b2-input[idx-2];
	if (output[idx] >= 3)
	{
		output[idx]=3;
	}
	return 0;  // Return an appropriate value.
}


int Current_PI(h_PID_t * h_PID ,float* input, float* output){
	output[idx]=output[idx-2]+h_PID->b0*input[idx]+h_PID->b1*input[idx-1]+h_PID->b2-input[idx-2]+0.5;
	return 0;  // Return an appropriate value.
}
float Out(float* input){
	New_consigne=(1024*input[idx]);
	return New_consigne;
}


