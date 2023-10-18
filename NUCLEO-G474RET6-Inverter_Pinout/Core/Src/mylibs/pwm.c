/**
 *@file pwm.c
 *@brief Ce fichier contient les fonctions nécéssaires à la génération des signaux PWM pour controler le moteur.
 *@date Oct 2, 2023
 *@author Tom
 *@author Baptiste
 *@author Antoine
 */

#include "mylibs/pwm.h"
#include "tim.h"

int aTemp;

/**
 * @def alpha_MAX
 * Résolution maximal du rapport cyclique
 */

/**
 * @brief Démarre le PWM.
 *
 * Cette fonction démarre le PWM sur les canaux 1 et 2 du Timer 1.
 * Elle active la sortie PWM et la sortie complémentaire PWM (PWMN) sur ces canaux.
 *
 * @note Cette fonction doit être appelée après avoir configuré les paramètres PWM appropriés.
 */
void pwm_start(void){
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);

	aTemp=50;
}

/**
 * @brief Stop le PWM.
 *
 * Cette fonction arrete le PWM sur les canaux 1 et 2 du Timer 1.
 * Elle désactive la sortie PWM et la sortie complémentaire PWM (PWMN) sur ces canaux.
 *
 */

void pwm_stop(void){
	set_pwm_alpha(50);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);

	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
}

/**
 * @brief Configure le signal PWM en fonction de la valeur alpha.
 *
 * Cette fonction ajuste le signal PWM pour correspondre à la valeur alpha spécifiée avec une rampe pour ne pas cramer le moteur.
 *
 * @param alpha La valeur du rapport cyclique qui détermine la configuration du signal PWM.
 */
void set_pwm_alpha(int alpha){
	int a1;
	int a2;
	int aTransi=aTemp;

	if (alpha > aTransi){
		while(aTransi != alpha){

			aTransi++;
			a1=alpha_MAX*aTransi/PWM_MAX;
			a2=alpha_MAX*(PWM_MAX-aTransi)/PWM_MAX;
			__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,a1);
			__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,a2);
			HAL_Delay(50);
		}
	}

	if (alpha < aTransi){
		while(aTransi != alpha){

			aTransi--;
			a1=alpha_MAX*aTransi/PWM_MAX;
			a2=alpha_MAX*(PWM_MAX-aTransi)/PWM_MAX;
			__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,a1);
			__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,a2);
			HAL_Delay(50);
		}
	}
	aTemp = alpha;

}

/**
 * @brief Configure le signal PWM en fonction de la valeur alpha.
 *
 * Cette fonction configure le signal PWM en ajustant sans rampe.
 *
 * @param alpha La valeur alpha qui détermine la configuration du signal PWM.
 */
void set_pwm(float alpha){
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,alpha);
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,1024-alpha);
}

