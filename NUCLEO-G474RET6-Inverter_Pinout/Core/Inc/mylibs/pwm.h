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
#define alpha_MAX 1024
#define PWM_MAX 100

/**
 * @brief initialise les commandes PWM pour 2 canaux et leur complémentaires
 */
void pwm_start(void);
void pwm_stop(void);
void set_pwm_alpha(int alpha);



#endif /* INC_MYLIBS_PWM_H_ */
