/**
 *@file codeur.c
 *@brief Ce fichier contient les fonction nécéssaires à la gestion du codeur
 *@date Oct 9, 2023
 *@author Tom
 *@author Baptiste
 *@author Antoine
 */
#include "mylibs/pwm.h"
#include "tim.h"


uint32_t counter = 0; /*!<Valeur absolue du compteur*/
int16_t pos = 0; //position du codeur (négative ou positive)
int16_t oldpos = 0; //<position du codeur à l'instant précédent
float vitesse = 0; //vitesse du modeur (le signe donne le sens)
int dt=500; //mesure de la vitesse toutes les 500ms
//int idx = 0; //nombre de ms
int reduRation = 20;
float hall_resolution = 20.4;
/**
 * @brief Démarre le compteur d'encodeur du Timer 3 en mode interruption.
 *
 * Cette fonction démarre le compteur d'encodeur sur le Timer 3 en mode interruption. L'encodeur est
 * configuré pour surveiller les deux canaux(TIM_CHANNEL_ALL).
 *
 * @note Vous devez appeler cette fonction pour activer l'acquisition des impulsions de l'encodeur.
 */
void codeur_start(void){
	HAL_TIM_Encoder_Start_IT(&htim3,TIM_CHANNEL_ALL);
}

/**
 * @brief Callback appelée lors de la capture d'un signal d'interruption du Timer.
 *
 * Cette fonction est appelée lors de la capture d'un signal d'interruption du Timer (TIM).
 * Elle récupère la valeur du compteur absolu à partir du Timer 3 et la stocke dans la variable 'counter'.
 * Ensuite, elle convertit cette valeur en un entier signé (int16_t) et la stocke dans la variable 'pos'.
 *
 * @param htim Un pointeur vers la structure TIM_HandleTypeDef associée à l'interruption.
 *
 * @note Cette fonction est destinée à être utilisée en tant que rappel (callback) pour gérer la capture d'interruption du Timer.
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	counter = __HAL_TIM_GET_COUNTER(&htim3); //valeur du compteur absolue
	pos = (int16_t)counter; //valeur du compteur avec les valeurs négatives
}

/**
 * @brief Calcule la vitesse en fonction de la position actuelle et précédente.
 *
 * Cette fonction calcule la vitesse en utilisant la différence entre la position actuelle (pos) et la position précédente (old_pos).
 * La vitesse est exprimée en fronts par seconde (front/s) et est stockée dans la variable 'vitesse'.
 * Après le calcul, la variable 'old_pos' est mise à jour avec la valeur actuelle de 'pos', et 'idx' est réinitialisé à 0.
 *
 * @note Cette fonction doit être appelée toutes les 500ms à l'aide de systick pour mettre à jour la vitesse en temps réel.
 */
void calc_speed(){
	vitesse = (((pos - oldpos)*10)/reduRation)/hall_resolution; //speed in incr/sec, 2=1/500 ms,
	oldpos = pos;
	//vitesse=(pos-old_pos)*1000/dt; //vitesse en front/s
}
