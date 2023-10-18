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
#include "mylibs/codeur.h"

/**
 * @brief Compteur de position du moteur.
 *
 * Cette variable stocke la valeur absolue du compteur utilisé pour suivre la position du moteur.
 */
uint32_t counter = 0;    //Valeur absolue du compteur
/**
 * @brief Position actuelle du moteur.
 *
 * Cette variable stocke la position actuelle du moteur, pouvant être négative ou positive.
 */
int16_t pos = 0;         //position du codeur (négative ou positive)
/**
 * @brief Position précédente du moteur.
 *
 * Cette variable stocke la position du moteur à l'instant précédent.
 */
int16_t oldpos = 0;      //position du codeur à l'instant précédent
/**
 * @brief Tableau pour stocker la vitesse du moteur.
 *
 * Ce tableau contient trois éléments pour stocker les valeurs de vitesse du moteur à difféents instants
 */
float vitesse[3] ={0};   //vitesse du modeur (le signe donne le sens)
/**
 * @brief Intervalle de mesure de la vitesse.
 *
 * Cette variable spécifie l'intervalle de temps en millisecondes entre chaque mesure de vitesse.
 */
int dt=100; 			 //mesure de la vitesse toutes les 100ms
/**
 * @brief Résolution du compteur pour un tour complet du moteur.
 *
 * Cette variable indique le nombre d'incréments du compteur nécessaires pour effectuer un tour complet du moteur.
 */
float resolution = 4096; //nombre d'incrément du compteur pour un tour du moteur

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
	pos = (int16_t)counter; 				 //valeur du compteur avec les valeurs négatives
}

/**
 * @brief Calcule la vitesse en fonction de la position actuelle et précédente.
 *
 * Cette fonction calcule la vitesse en utilisant la différence entre la position actuelle (pos) et la position précédente (old_pos).
 * La vitesse est exprimée en fronts par seconde (front/s) et est stockée dans la variable 'vitesse'.
 * Après le calcul, la variable 'old_pos' est mise à jour avec la valeur actuelle de 'pos', et 'idx' est réinitialisé à 0.
 *
 * @note Cette fonction doit être appelée toutes les 100ms dans le Callback du TIM 16
 */
void calc_speed(){
	vitesse[0]=vitesse[1];
	vitesse[1]=vitesse[2];
	vitesse[2] = (((pos - oldpos)/dt)/resolution)*ms_To_s*10;
	oldpos = pos;
}


