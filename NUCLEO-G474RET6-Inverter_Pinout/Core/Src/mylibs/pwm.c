/**
 *@file pwm.c
 *@brief Ce fichier contient les fonctionnement nécéssaires à la génération des signaux PWM pour controler le moteur.
 *@date Oct 2, 2023
 *@author Tom
 *@author Baptiste
 *@author Antoine
*/

#include "mylibs/pwm.h"
#include "tim.h"

/**
 * @def alpha_MAX
 * Résolution maximal du rapport cyclique
 */
#define alpha_MAX 1024

void pwm_start(void){
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
}

void set_pwm_alpha(int alpha){
	int a1=alpha_MAX*alpha/100;
	int a2=alpha_MAX*(100-alpha)/100;
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,a1);
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,a2);
}
