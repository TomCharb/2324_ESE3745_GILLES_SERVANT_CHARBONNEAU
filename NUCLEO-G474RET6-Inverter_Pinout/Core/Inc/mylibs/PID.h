/**
 *@file PID.h
 *@brief Ce fichier contient les fonctions nécéssaires à l'asservissement
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

//void ID_BO(int alpha);

int Erreur(float consigne, float * erreur, float * vitesse);
int Erreur_I(float * Iconsigne, float * Imes);
int Speed_PI(h_PID_t * h_PID, float * input,float * output);
int Current_PI(h_PID_t * h_PID, float * input, float * output);
int NewPWM(float * input);
void start_asserv(void);

#endif /* INC_MYLIBS_PID_H_ */
