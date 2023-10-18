/**
 *@file PID.c
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
#include "main.h"

extern uint32_t buffer[ADC_BUF_SIZE]; //buffer de l'ADC
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim16;
//int tab_speed[500];
/**
 * @brief Tableau des erreurs de vitesse.
 *
 * Ce tableau contient les erreurs de vitesse aux instants t, t-1 et t-2, stockées dans un tableau de trois éléments.
 */
float erreur[3]={0};	//tableau contenant les erreurs de vitesse aux instants t, t-1 et t-2
/**
 * @brief Tableau des erreurs de courant.
 *
 * Ce tableau contient les erreurs de courant aux instants t, t-1 et t-2, stockées dans un tableau de trois éléments.
 */
float erreur_I[3]={0};	//tableau contenant les erreurs de courant aux instants t, t-1 et t-2
/**
 * @brief Tableau des consignes de courant.
 *
 * Ce tableau contient les consignes de courant aux instants t, t-1 et t-2, stockées dans un tableau de trois éléments.
 */
float Iconsigne[3]={0}; //Tableau contenant les consignes de dourant aux instants t, t-1 et t-2
/**
 * @brief Tableau des valeurs de alpha.
 *
 * Ce tableau contient les valeurs de alpha aux instants t, t-1 et t-2, stockées dans un tableau de trois éléments.
 */
float alpha[3]={0.5};	//Tableau contenant les valeurs de alpha aux instants t, t-1 et t-2
/**
 * @brief Tableau des valeurs de vitesse.
 *
 * Ce tableau stocke les valeurs de vitesse aux instants t, t-1 et t-2.
 */
extern float vitesse[3];
/**
 * @brief Nouvelle valeur de alpha.
 *
 * Cette variable stocke la nouvelle valeur de alpha.
 */
float New_alpha=0.5; //Nouvelle valeur de alpha

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

/**
 * @brief Calcule l'erreur en vitesse.
 *
 * Cette fonction calcule l'erreur en vitesse en comparant la vitesse actuelle à la vitesse de consigne.
 *
 * @param consigne La valeur de consigne à comparer.
 * @param erreur Un tableau de trois éléments contenant les erreurs aux instants t, t-1 et t-2.
 * @param vitesse Un tableau de trois éléments contenant les valeurs de vitesse aux instants t, t-1 et t-2.
 *
 * @return 0 si la fonction s'est exécutée avec succès.
 */
int Erreur(float consigne, float * erreur, float * vitesse){
	erreur[0]=erreur[1];
	erreur[1]=erreur[2];
	erreur[2]=consigne-vitesse[2];
	return 0;
}
/**
 * @brief Calcule l'erreur en courant.
 *
 * Cette fonction calcule l'erreur en courant en comparant la consigne actuelle de courant à la mesure de courant.
 *
 * @param Iconsigne Un tableau de trois éléments contenant les consignes de courant aux instants t, t-1 et t-2.
 * @param Imes Un tableau de trois éléments contenant les mesures de courant aux instants t, t-1 et t-2.
 *
 * @return 0 si la fonction s'est exécutée avec succès.
 */
int Erreur_I(float * Iconsigne,float * Imes){
	erreur_I[0]=erreur_I[1];
	erreur_I[1]=erreur_I[2];
	erreur_I[2]=Iconsigne[2]-Imes[2];
	return 0;
}
/**
 * @brief Calcul du contrôleur PI de vitesse.
 *
 * Cette fonction calcule la sortie d'un contrôleur PI de vitesse en utilisant les coefficients du contrôleur et les entrées actuelles.
 *
 * @param h_PID Un pointeur vers une structure contenant les coefficients du contrôleur PI.
 * @param input Un tableau de trois éléments contenant les entrées aux instants t, t-1 et t-2.
 * @param output Un tableau de trois éléments contenant les sorties aux instants t, t-1 et t-2.
 *
 * @return 0 si la fonction s'est exécutée avec succès.
 */
int Speed_PI(h_PID_t * h_PID,float * input,float * output){
	output[2]=output[0]+h_PID->b0*input[2]+h_PID->b1*input[1]+h_PID->b2-input[0];
	return 0;
}
/**
 * @brief Calcul du contrôleur PI de courant.
 *
 * Cette fonction calcule la sortie d'un contrôleur PI de courant en utilisant les coefficients du contrôleur et les entrées actuelles. Elle effectue également une saturation de la sortie.
 *
 * @param h_PID Un pointeur vers une structure contenant les coefficients du contrôleur PI.
 * @param input Un tableau de trois éléments contenant les entrées aux instants t, t-1 et t-2.
 * @param output Un tableau de trois éléments contenant les sorties aux instants t, t-1 et t-2.
 *
 * @return 0 si la fonction s'est exécutée avec succès.
 */
int Current_PI(h_PID_t * h_PID ,float * input, float * output){
	output[2]=output[0]+h_PID->b0*input[2]+h_PID->b1*input[1]+h_PID->b2-input[0]+0.5;
	if (output[2] >= 3)
	{
		output[2]=3; //Saturation en courant
	}
	return 0;
}
/**
 * @brief Met à jour le signal PWM en fonction de l'entrée.
 *
 * Cette fonction calcule la nouvelle valeur de `New_alpha` en multipliant l'entrée par la valeur maximale d'alpha (alpha_MAX), puis ajuste le signal PWM en conséquence.
 *
 * @param input Un tableau de trois éléments contenant l'entrée aux instants t, t-1 et t-2.
 *
 * @return 0 si la fonction s'est exécutée avec succès.
 */
int NewPWM(float* input){
	New_alpha=(alpha_MAX*input[2]);
	set_pwm(New_alpha);
	return 0;
}

/**
 * @brief Démarre le système d'asservissement.
 *
 * Cette fonction démarre le système d'asservissement en activant la conversion ADC DMA, en lançant un timer périodique, et en initialisant le codeur.
 */
void start_asserv(void){
	if(HAL_OK != HAL_ADC_Start_DMA(&hadc1, buffer, ADC_BUF_SIZE)){
		Error_Handler();
	}
	HAL_TIM_Base_Start_IT(&htim16);
	codeur_start();
}



