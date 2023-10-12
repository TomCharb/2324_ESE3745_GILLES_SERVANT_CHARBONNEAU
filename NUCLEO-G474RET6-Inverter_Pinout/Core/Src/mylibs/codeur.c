/*
 * codeur.c
 *
 *  Created on: Oct 9, 2023
 *      Author: Tomtogaz
 */
#include "mylibs/pwm.h"
#include "tim.h"

uint32_t counter = 0; //valeur absolue du compteur
int16_t pos = 0; //position du codeur (négative ou positive)
int16_t old_pos = 0; //position du codeur à l'instant précédent
int vitesse = 0; //vitesse du modeur (le signe donne le sens)
int dt=500; //mesure de la vitesse toutes les 500ms
int idx = 0; //nombre de ms

void codeur_start(void){
	HAL_TIM_Encoder_Start_IT(&htim3,TIM_CHANNEL_ALL);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	counter = __HAL_TIM_GET_COUNTER(&htim3); //valeur du compteur absolue
	pos = (int16_t)counter; //valeur du compteur avec les valeurs négatives
}

void calc_speed(){
	vitesse=(pos-old_pos)*1000/dt; //vitesse en front/s
	old_pos=pos;
	idx =0;
}
