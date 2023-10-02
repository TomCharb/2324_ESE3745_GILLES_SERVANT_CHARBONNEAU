/**
 *@file pwm.h
 *@brief Ce fichier contient les fonctionnement nécéssaires à la génération des signaux PWM pour controler le moteur.
 *@date Oct 2, 2023
 *@author Tom
 *@author Baptiste
 *@author Antoine
*/

#ifndef INC_MYLIBS_PWM_H_
#define INC_MYLIBS_PWM_H_


/**
 * @brief initialise les commandes PWM pour 2 canaux et leur complémentaires
 */
void pwm_start(void);
/**
 * @brief permet de choisir le rapport cyclique de notre PWM
 * @param alpha Valeur de rapport cyclique comprise entre 0 et 100
 */
void set_pwm_alpha(int alpha);



#endif /* INC_MYLIBS_PWM_H_ */
