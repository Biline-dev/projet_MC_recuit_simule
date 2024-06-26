
#define VISUMEILLEUR 1  /* Si 1, visu de la meilleure solution connue    */
                        /* Si 0, visu en continu de la solution courante */
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "random.h"
#include "gnuplot.h"
#include "geo.h"

#include "params.h"  /* Pour NBVILLES et COTECARTE */

#if NBVILLES==8

 Carte carte={{{5,2},{7,3},{8,5},{7,7},{5,8},{3,7},{2,5},{3,3}}, 8};

#elif NBVILLES==16

 Carte carte={{{5,2},{6,2},{7,3},{8,4},
               {8,5},{8,6},{7,7},{6,8},
               {5,8},{4,8},{3,7},{2,6},
               {2,5},{2,4},{3,3},{4,2}}, 16};

#elif NBVILLES==30

 Carte carte={{{0,0},{3,3},{4,1},{1,9},{7,6},
               {2,1},{9,8},{3,5},{4,6},{5,9},
               {3,9},{0,4},{8,5},{2,6},{6,1},
               {7,8},{5,2},{3,6},{6,5},{1,8},
               {7,1},{7,0},{7,3},{1,1},{3,1},
               {5,1},{6,0},{8,4},{1,4},{1,6}}, 30};
#else
 Carte carte;
#endif

typedef struct { int parcours[NBVILLES];
                 int nbVilles;
               } Chemin;

/*--------------- Des fonctions de gestion de Chemins ----------------*/

void genereChemin(Chemin *chemin)
{
 int nbVillesGenerees=0;

 chemin->nbVilles=NBVILLES;

 while (nbVillesGenerees!=chemin->nbVilles)
 {
   int entier=myRandomMinMax(0,chemin->nbVilles-1);

   /* Il faut rechercher si l'entier est deja present dans le parcours */

   int i=0,trouve=0;
   for(i=0;i<nbVillesGenerees;i++)
   {
    if (chemin->parcours[i]==entier) trouve=1;
   }
   if (!trouve) chemin->parcours[nbVillesGenerees++]=entier;
 }
}
/*--*/
double calculCoutChemin(Chemin chemin)
{
 double cout=0.0;
 int    i=0;

 const Ville *villeInitiale,*villePrecedente,*villeCourante;

 cout=0.0;                 /* Si 1 seule ville, cout: 0.0 */
 if (chemin.nbVilles!=1)   
 {
  villeInitiale=&carte.villes[chemin.parcours[0]];
  villePrecedente=villeInitiale;

  for(i=1;i<chemin.nbVilles;i++)
  {
   villeCourante=&carte.villes[chemin.parcours[i]];

   cout+=distanceVilles(villePrecedente,villeCourante);

   villePrecedente=villeCourante;
  }

  /* Si 2 villes, distance premiere/derniere Ville deja calculee ... */
  if (chemin.nbVilles!=2)     
  {   
   cout+=distanceVilles(villePrecedente,villeInitiale);
  }
 }

 return cout;
}
void dessineChemin(FILE* flot, Chemin chemin)
{
 int    i=0;

 beginPointsToGnuplot(flot,"linespoint");

 for(i=0;i<chemin.nbVilles-1;i++)
 {
  vectorGnuplot(flot, carte.villes[chemin.parcours[i]].x,
                      carte.villes[chemin.parcours[i]].y,
                      carte.villes[chemin.parcours[i+1]].x,
                      carte.villes[chemin.parcours[i+1]].y);
 }

 vectorGnuplot(flot, carte.villes[chemin.parcours[chemin.nbVilles-1]].x,
                     carte.villes[chemin.parcours[chemin.nbVilles-1]].y,
                     carte.villes[chemin.parcours[0]].x,
                     carte.villes[chemin.parcours[0]].y);

 endPointsToGnuplot(flot);
}

/*--*/

void transformationChemin(Chemin *cheminY,Chemin  cheminX, int amplitude, int type)
{

  if(type == 1) //Echange
  {
    *cheminY = cheminX; /* *cheminY est au voisinage de cheminX suivant     */
                     /*  l'amplitude. Apres *cheminY =  cheminX, ne plus */
                     /*  modifier cheminX !!!                            */

    if (amplitude>cheminX.nbVilles) return;

    int nb_boucle = myRandomMinMax(1, amplitude);
    
    for(int i=0; i<nb_boucle; i++)
    {

      int indice_ville_1 = myRandomMinMax(0, cheminY->nbVilles-1);
      int indice_ville_2 = myRandomMinMax(0, cheminY->nbVilles-1);

      int tempc = cheminY->parcours[indice_ville_1];
      cheminY->parcours[indice_ville_1] = cheminY->parcours[indice_ville_2];
      cheminY->parcours[indice_ville_2] = tempc;

    }
  }

  else if(type == 2)//Inversion
  {
    *cheminY = cheminX; /* *cheminY est au voisinage de cheminX suivant     */
                     /*  l'amplitude. Apres *cheminY =  cheminX, ne plus */
                     /*  modifier cheminX !!!                            */

    if (amplitude>cheminX.nbVilles) return;

    int nb_boucle = myRandomMinMax(1, amplitude);

    for (size_t i = 0; i < nb_boucle; i++)
    {
      int indice_ville_1 = myRandomMinMax(0,cheminY->nbVilles -1);
      int indice_ville_2 = myRandomMinMax(0,cheminY->nbVilles -1);

      if(indice_ville_1 > indice_ville_2)
      {
        int temp = indice_ville_1;
        indice_ville_1 = indice_ville_2;
        indice_ville_2 = temp;
      }

      int idx = indice_ville_1 + (indice_ville_2 - indice_ville_1) / 2;

      for (size_t i = indice_ville_1; i < idx; i++)
      {
        int ville_temporaire = cheminY->parcours[i];
        cheminY->parcours[i] = cheminY->parcours[indice_ville_2 - i];
        cheminY->parcours[indice_ville_2 - i] = ville_temporaire;
      }
    }
  }

  else if(type == 3)//Translation
  {
    *cheminY = cheminX; /* *cheminY est au voisinage de cheminX suivant     */
                     /*  l'amplitude. Apres *cheminY =  cheminX, ne plus */
                     /*  modifier cheminX !!!                            */

    if (amplitude>cheminX.nbVilles) return;

    int nb_boucle = myRandomMinMax(1, amplitude);

    for (size_t i = 0; i < nb_boucle; i++)
    {
      //Génération des indices
      int indice_ville_1 = myRandomMinMax(0,cheminY->nbVilles -1);
      int indice_ville_2 = myRandomMinMax(0,cheminY->nbVilles -1);
      int indice_ville_3 = myRandomMinMax(0,cheminY->nbVilles -1);

      //Tri de indices
      if(indice_ville_1 > indice_ville_2)
      {
        int temp = indice_ville_1;
        indice_ville_1 = indice_ville_2;
        indice_ville_2 = temp;
      }

      if(indice_ville_1 > indice_ville_3)
      {
        int temp = indice_ville_1;
        indice_ville_1 = indice_ville_3;
        indice_ville_3 = temp;
      }

      if(indice_ville_2 > indice_ville_3)
      {
      int temp = indice_ville_2;
      indice_ville_2 = indice_ville_3;
      indice_ville_3 = temp;
      }

      int temp;
      for (int i = indice_ville_1; i <= indice_ville_2; i++)
      {

        temp = cheminY->parcours[indice_ville_1];

        for (int j = indice_ville_1; j < indice_ville_3; j++)
        {
          cheminY->parcours[j] = cheminY->parcours[j + 1];
        }
        cheminY->parcours[indice_ville_3] = temp;
      }
    }
  }

  else
  {
    printf("Type de mutation incorrecte !");
    exit(-1);
  }
}

/*---------------------------------------------------------*/

// void transformationChemin(Chemin *cheminY,Chemin  cheminX, int amplitude)
// {

  // *cheminY = cheminX; /* *cheminY est au voisinage de cheminX suivant     */
  //                    /*  l'amplitude. Apres *cheminY =  cheminX, ne plus */
  //                    /*  modifier cheminX !!!                            */

  // if (amplitude>cheminX.nbVilles) return;

  // int nb_boucle = myRandomMinMax(1, amplitude);

  // for (size_t i = 0; i < nb_boucle; i++)
  // {
  //   int indice_ville_1 = myRandomMinMax(0,cheminY->nbVilles -1);
  //   int indice_ville_2 = myRandomMinMax(0,cheminY->nbVilles -1);

  //   if(indice_ville_1 > indice_ville_2)
  //   {
  //     int temp = indice_ville_1;
  //     indice_ville_1 = indice_ville_2;
  //     indice_ville_2 = temp;
  //   }

  //   int idx = indice_ville_1 + (indice_ville_2 - indice_ville_1) / 2;

  //   for (size_t i = indice_ville_1; i < idx; i++)
  //   {
  //     int ville_temporaire = cheminY->parcours[i];
  //     cheminY->parcours[i] = cheminY->parcours[indice_ville_2 - i];
  //     cheminY->parcours[indice_ville_2 - i] = ville_temporaire;
  //   }
  // }
// }

/*---------------------------------------------------------*/

// void transformationChemin(Chemin *cheminY,Chemin  cheminX, int amplitude)
// {

  // *cheminY = cheminX; /* *cheminY est au voisinage de cheminX suivant     */
  //                    /*  l'amplitude. Apres *cheminY =  cheminX, ne plus */
  //                    /*  modifier cheminX !!!                            */

  // if (amplitude>cheminX.nbVilles) return;

  // int nb_boucle = myRandomMinMax(1, amplitude);

  // for (size_t i = 0; i < nb_boucle; i++)
  // {
  //   //Génération des indices
  //   int indice_ville_1 = myRandomMinMax(0,cheminY->nbVilles -1);
  //   int indice_ville_2 = myRandomMinMax(0,cheminY->nbVilles -1);
  //   int indice_ville_3 = myRandomMinMax(0,cheminY->nbVilles -1);

  //   //Tri de indices
  //   if(indice_ville_1 > indice_ville_2)
  //   {
  //     int temp = indice_ville_1;
  //     indice_ville_1 = indice_ville_2;
  //     indice_ville_2 = temp;
  //   }

  //   if(indice_ville_1 > indice_ville_3)
  //   {
  //     int temp = indice_ville_1;
  //     indice_ville_1 = indice_ville_3;
  //     indice_ville_3 = temp;
  //   }

  //   if(indice_ville_2 > indice_ville_3)
  //   {
  //   int temp = indice_ville_2;
  //   indice_ville_2 = indice_ville_3;
  //   indice_ville_3 = temp;
  //   }

  //   int temp;
  //   for (int i = indice_ville_1; i <= indice_ville_2; i++)
  //   {

  //     temp = cheminY->parcours[indice_ville_1];

  //     for (int j = indice_ville_1; j < indice_ville_3; j++)
  //     {
  //       cheminY->parcours[j] = cheminY->parcours[j + 1];
  //     }
  //     cheminY->parcours[indice_ville_3] = temp;
  //   }
  // }
// }

/*--*/

/*--------------- Parametres de controle du recuit --------------------*/

double T;                 /* Temperature T        */
double Ti;                /* Temperature initiale */
double Tf;                /* Temperature finale   */

int    amplitude;         /* Parametre d'amplitude dans transformation() */
double alpha;             /* Facteur de decroissance de la temperature   */

int    NbEssais;          /* Nb total de mouvements essayes              */
int    MaxRepetitions;    /* Nb. max de repetition a temp. constante     */

FILE* fdCout;
char* fileNameCout="Cout";

FILE *fdResults;
char* fileNameResults="Resultats";

FILE* fdGnuplotCout;
FILE* fdGnuplotChemin;

/*_________  Choix de la Fonction d'Evaluation   _________________________*/
#define F(x)     f(x)     /* la fonction de cout (ci-dessous)             */ 
#define FNAME    "f"      /* indiquer aussi son libelle (pour impressions)*/

/*_________  Etats du Recuit  ____________________________________________*/

Chemin x0;               /* Etat initial      */
Chemin x;                /* Solution courante */
Chemin y;                /* Solution voisine  */
Chemin xopt;             /* Solution optimale */
double fx, fy, fxopt;    /* Valeurs */

/*________  Fonctions Exemples  (Fonction de co�t) _______________________*/
double f(Chemin chemin)
{
 return calculCoutChemin(chemin) ;//calcule cout chelin it's a functino
}

/*________  Voisinage (modification configuration)  ______________________*/
void transformation(int typeMutation)
{  /* y est au voisinage de x suivant amplitude */
  transformationChemin(&y,x,amplitude,typeMutation);                   /* Doonnneeee ! */
}

/*________  Modification temperature  ____________________________________*/
double g(int type)
{
  if(type == 1)
    return(T * alpha);       /* On decroit la temperature en utilisant T * alpha */
  if(type == 2)
    return(T - alpha);       /* On decroit la temperature en utilisant T - alpha */
  if(type == 3)
    return(T / (1 + alpha * T));
}

// //logarithmic_cooling
// double g(void)
// {
//   return T / log(Alpha + 1);
// }

// //inverse_cooling
// double g(void)
// {
//   return T / (1 + Alpha * T);
// }

/*------ visu du Cout -------- */
void visualiserCout(FILE *fd, char *fileName, int affichageObligatoire)
{
 static int i=0;

 char type[]="linespoints";

 if (i%10==0 || affichageObligatoire)
 {
#if 0 
  fprintf(fd,"plot \"%s\" with %s\n",fileName,type);
  fflush(fd);
#else

  FILE *fdFileName;

  fdFileName=fopen(fileName,"r");
  if (fdFileName==NULL) {
    perror("fopen (dans visualiserCout)");
    fprintf(fd,"plot \"%s\" with %s\n",fileName,type);
    fflush(fd);
  }
  else {

   long ind; double val;

   fprintf(fd,"plot '-' with %s\n",type);

   fscanf(fdFileName,"%ld %lf",&ind,&val);
   if (ferror(fdFileName)) perror("fscanf (dans visualiserCout)");

   while (!feof(fdFileName) && !ferror(fdFileName))
   {
    fprintf(fd,"%ld %f\n",ind,val);
    fflush(fd);
    fscanf(fdFileName,"%ld %lf",&ind,&val);
    if (ferror(fdFileName)) perror("fscanf (dans visualiserCout)");
   }

   fprintf(fd,"e\n");
   fflush(fd);

   fclose(fdFileName);

  }
#endif

 }
 i++;
}

void ecrireCout(FILE *fdCout, int abscisse, double cout)
{
 if (fdCout==NULL) return;

 fprintf(fdCout,"%d %f\n",abscisse,cout);
 fflush(fdCout);
}

/*------ Sauvegarde fichier Resultats -------- */
void PrintParameters(FILE *fd)
{
 if (fd==NULL) return;

 fprintf(fd,"---> Parametres\n");
 fprintf(fd,"%-15s %s\n"   ,"FNAME",FNAME);
 fprintf(fd,"%-15s %s\n","x0","...");
 fprintf(fd,"%-15s %-.2f\n","FNAME(x0)",F(x0));
 fprintf(fd,"%-15s %-.2f\n","Ti",Ti);
 fprintf(fd,"%-15s %-.2f\n","Tf",Tf);
 fprintf(fd,"%-15s %-.2f\n","alpha",alpha);
 fprintf(fd,"%-15s %d\n","amplitude",amplitude);
 fprintf(fd,"%-15s %d\n"   ,"MaxRepetitions",MaxRepetitions);

 fprintf(fd,"<---\n");
 fflush(fd);
}        

void PrintTitleLine(FILE *fd)
{
 int n=11;

 if (fd==NULL) return;

 fprintf(fd,"%-*s",n,"T");
 fprintf(fd,"%-*s",n,"NbEssais");
 fprintf(fd,"%-*s",n," x");
 fprintf(fd,"%-*s",n," f(x)");
 fprintf(fd,"%-*s",n," xopt");
 fprintf(fd,"%-*s",n," f(xopt)");

 fprintf(fd,"\n");
 fflush(fd);
}

void PrintALine(FILE *fd)
{
 int n=11;

 if (fd==NULL) return;

 fprintf(fd,"%-*.2f",n,T);
 fprintf(fd,"%-*d",n,NbEssais);
 fprintf(fd,"%-*s",n," ...");
 fprintf(fd,"%- *.2f",n,fx);
 fprintf(fd,"%-*s",n," ...");
 fprintf(fd,"%- *.2f",n,fxopt);
 fprintf(fd,"\n");

 fflush(fd);
}

void Visu(Chemin x, int affichageObligatoire)
{
 (void)x;
 visualiserCout(fdGnuplotCout,fileNameCout,affichageObligatoire);
 dessineChemin(fdGnuplotChemin,x);
}

void EcrireCoutEtVisu(double fx, Chemin x,int affichageObligatoire)
{
 (void)fx;(void)x;(void)affichageObligatoire;
 ecrireCout(fdCout,NbEssais,fx);
 Visu(x,affichageObligatoire);
}

void FermetureFlots(void)
{
 if (fdResults!=NULL) fclose(fdResults);

 fclose(fdCout);
 closeGnuplot(fdGnuplotCout);
 closeGnuplot(fdGnuplotChemin);
}

/*________  Initialisation  ___________________________________________*/

int main(int argc, char *argv[])
{
 char rep;

 initRandom();

#if NBVILLES!=8 && NBVILLES!=16 && NBVILLES!=30
 genereCarte(&carte,COTECARTE);
#endif

// Vérifier si au moins un argument a été fourni
  if (argc < 3)
  {
    printf("Usage: %s <type_mutation> <type_modif_temp>\n", argv[0]);
    exit(-1);
  }

  int typeMutation = atoi(argv[1]);
  int typeModificationTemperature = atoi(argv[2]);
/*
 printf("f(x)=%s\n", FNAME );

 printf("Etat initial (x0) ?\n");
 scanf("%lf", &x0);
*/

 genereChemin(&x0);  /* Configuration initiale */

 printf("TInit ?\n");
 scanf("%lf",&Ti);

 printf("TFin ?\n");
 scanf("%lf",&Tf);

 printf("Alpha ?\n");
 scanf("%lf",&alpha);

 printf("Ampli (un int) ?\n");
 scanf("%d",&amplitude);

 printf("MaxRepetitions ?\n");
 scanf("%d",&MaxRepetitions);

 PrintParameters(stdout);

 do {
     printf("Sauvegarde des resultats dans un fichier? (o/n)\n");
     scanf("%c",&rep);
     while (rep=='\n') { scanf("%c",&rep); }
 }while (rep!='o' && rep!='n' && rep!='O' && rep!='N');

 if (rep=='n' || rep=='N') fdResults=NULL;
 else {
       fdResults= fopen(fileNameResults, "w");
       if (fdResults==NULL) {
         fprintf(stderr,
                 "Probleme sur fopen(\"%s\",\"w\")\n",fileNameResults);
       }
 }

 /*  si on veut avoir un en-tete d'identification */    
 PrintParameters(fdResults);
 PrintTitleLine(fdResults);   

 fdCout=fopen(fileNameCout,"w");  /* Ouverture du fichier pour les couts */
 if (fdCout==NULL) {
   fprintf(stderr,"Probleme sur fopen(\"%s\",\"w\")\n",fileNameCout);
   fprintf(stderr,"=> Arret du programme\n");
   fclose(fdResults);
   exit(EXIT_FAILURE);
 }

 fdGnuplotCout=openGnuplot(NULL);            /* pipe + fork pour visu    */
 if (fdGnuplotCout==NULL) {
   fprintf(stderr,"Probleme sur openGnuplot => Arret du programme\n");
   fclose(fdResults);
   fclose(fdCout);
   exit(EXIT_FAILURE);
 }

 fdGnuplotChemin=openGnuplot(NULL);         /* pipe + fork pour visu    */
 if (fdGnuplotCout==NULL) {
   fprintf(stderr,"Probleme sur openGnuplot => Arret du programme\n");
   fclose(fdResults);
   fclose(fdCout);
   closeGnuplot(fdGnuplotCout);
   exit(EXIT_FAILURE);
 }

/*__ Recuit Simule  ___________________________________________*/

    x        = xopt  = x0 ;         /* Configuration initiale */
    fx       = fxopt =  F(x0);         /* Cout initial           */
     
    T        = Ti ;                 /* Temperature initiale   */
    NbEssais = 0;
 
    EcrireCoutEtVisu(fx,x,1);
    PrintALine(fdResults); /* Sauvegarde configuration initiale       */
   
    while ( T> Tf) {                            /* 1er critere d'arret */

       int    rep;    /* Nb de repetitions a temperature constante    */
       double p, Df;  /* p: pour tirage aleatoire, Df: pour Delta f   */

       for(rep=0; rep<MaxRepetitions; rep++){ /* 2eme critere d'arret */

          transformation(typeMutation);                     /* transformation => y, voisin de x */
          fy = F(y);
          Df = fy - fx;                      /* Nouveau - Ancien         */
          if ( Df< 0) {                    /* Descente !!              */
                x  = y ;                /* y devient l'etat courant */
                fx = fy ;                                
                if ( fx< F(xopt)){               /* Mise a jour optimum ?    */
                    xopt  = x ;
                    fxopt = F(xopt) ;
          #if VISUMEILLEUR==1
                    EcrireCoutEtVisu(fxopt,xopt,1);
          #endif
                }                
          } 
          else {                         /* Remontee : acceptee ??   */
		            p = myRandom01();
                if (p < exp(-Df/T)) {
                    x  = y;           /* y devient l'etat courant */
                    fx = fy ;                
                }                
          }

          NbEssais++;

#if VISUMEILLEUR!=1
          EcrireCoutEtVisu(fx,x,0);
#endif
          PrintALine(fdResults);   /* Sauvegarde resulats courants */
       } 
        
       T = g(typeModificationTemperature);                        /* modifier la temperature */

       usleep(10);
    } /* end while */

 printf("-------------------->\n");
 printf("Temperature a la fin de l'algorithme=%f\n",T);
 printf("Cout optimal (fxopt)=%f\n",fxopt);
 printf("<--------------------\n");
 Visu(xopt,1);

 FermetureFlots();

 exit(EXIT_SUCCESS);
}
