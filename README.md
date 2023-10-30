# 2324_ESE3745_GILLES_SERVAN_CHARBONNEAU

La fonction atoi permet de convertir la chaine de caractère en int.

La fonction HAL_TIM_PWM_Start permet d'activer les pwm sur les canaus normaux et HAL_TIMEx_PWMN_Start pour les canaux inversés.

Pour changer le rapport cyclique donc le pulse on utilise __HAL_TIM_SetCompare.

Pour avoir une commande en complémentaire décalée il faut mettre le timer en Center alligned mode et utiliser des valeurs complémentaires de rapport cyclique dans le code.

Calcul pour les dead times :
Dead Time 
Fréquence horloge =1/170mHz

                  =5.88*10^-9
                  
                  dead time = 100ns
                  
                  100ns/5.88*10^-9= 17 donc dead time =17 d'aprés la doc de ST d=X*T eavec T fréqeunce d'horloge d dead time désiré et X nombre de tick (entre 0 et 255)

Ajouter le calcul pour ARR et PSC

Faire la mise en page et comprendre pourquoi le doxygen veut pas tout commit

