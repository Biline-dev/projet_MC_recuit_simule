#ifndef PARAM_H
#define PARAM_H

#define  NBVILLES    100
#define COTECARTE    50

/*********************************************************************/
/************ Verification contraintes sur les defines ***************/
/*********************************************************************/


#if NBVILLES>COTECARTE*COTECARTE
#error "Attention: NBVILLES>COTECARTE*COTECARTE"
#endif

#endif /* PARAM_H */
