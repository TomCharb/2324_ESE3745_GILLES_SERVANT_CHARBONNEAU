# 2324_ESE3745_GILLES_SERVAN_CHARBONNEAU

## TP 1

La fonction atoi permet de convertir la chaine de caractère en int.

La fonction HAL_TIM_PWM_Start permet d'activer les pwm sur les canaus normaux et HAL_TIMEx_PWMN_Start pour les canaux inversés.

Pour changer le rapport cyclique donc le pulse on utilise __HAL_TIM_SetCompare.

Pour avoir une commande en complémentaire décalée il faut mettre le timer en Center alligned mode et utiliser des valeurs complémentaires de rapport cyclique dans le code.

Calcul pour ARR et PSC :

Fréquence du processeur 170 MHz et on veut en sortie environ 20kHz avec un ARR = 1023, alors on choisit PSC = 8 ce qui nous donne une fréquence à 18,4kHz.

Calcul pour les dead times :
Dead Time 
Fréquence horloge =1/170mHz

                  =5.88*10^-9
                  
                  dead time = 100ns
                  
                  100ns/5.88*10^-9= 17 donc dead time =17 d'aprés la doc de ST d=X*T eavec T fréqeunce d'horloge d dead time désiré et X nombre de tick (entre 0 et 255)

Lorsque nous réalisons un passage du rapport cyclique de 50% à 70%, nous observons un appel de courant quand le rapport cyclique est directement mis à 70%.

## TP 2 et 3

Pour l'asservissement, nous avons utilisé les valeurs données en TP. Nous avons créé 2 structures PID pour réaliser les calcules.
Les calculs sont effectués à chaque appel du callback de l'ADC pour l'asservissement en courant et à chaque appel du callback du tim16 (que nous avons mis à une fréquence de 100 Hz) pour l'asservissement en vitesse.



