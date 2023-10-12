# 2324_ESE3745_GILLES_SERVAN_CHARBONNEAU

on utilise la fonction atoi pour convertir la chaine de caractère en int

on utilise HAL_TIM_PWM_Start pour activer les pwm sur les canaus normaux et HAL_TIMEx_PWMN_Start pour les canaux inversés

pour changer le rapport cyclique donc le pulse on utilise __HAL_TIM_SetCompare

Pour avoir une commande en complémentaire décalée il faut mettre le timer en Center alligned mode et utiliser des valeurs complémentaires de rapport cyclique dans le code*

Ajouter le calcul pour les dead times

Ajouter le calcul pour ARR et PSC

Faire la mise en page et comprendre pourquoi le doxygen veut pas tout commit
