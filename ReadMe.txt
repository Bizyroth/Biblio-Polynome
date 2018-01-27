/--------------------------------------------------------------------------------------------------------------------/
/-------------------------------------------------FICHIER------------------------------------------------------------/
/--------------------------------------------------------------------------------------------------------------------/

arithm.c           Fonction qui permet de calculer le plus petit diviseur premier d'un nombre
f2_poly.c          Bibliothèque sur les polynomes demandée
count_irred.c      Programme test pour vérifier la fonction qui compte le nombre d'irréductible d'un degré donne
count_primitive.c  Programme test pour vérifier la fonction qui compte le nombre de primitif d'un degré donne
rand_irred.c       Affiche un polynome irreductible d'un degre donné
rand_primitive.c   Affiche un polynome primitif d'un degré donné


/--------------------------------------------------------------------------------------------------------------------/
/-------------------------------------------------MAKEFILE-----------------------------------------------------------/
/--------------------------------------------------------------------------------------------------------------------/

   #make                        Compile count_irred.c count_primitive.c random_irred.c random_primitive.c
   #make clean                  Supprime les .o et autre fichier inutile
   #make mrproper               Fais clean et supprime les executables    
   
/--------------------------------------------------------------------------------------------------------------------/
/-------------------------------------------------EXECUTABLE---------------------------------------------------------/
/--------------------------------------------------------------------------------------------------------------------/
   
   
   #./count_irred N        Affiche le nombre d'irreductible de degré N
   #./count_irred N l      Affiche le nombre d'irreductiblie pour tous les degres <=N
   #./count_primitive N    Affiche le nombre de primitif de degre N
   #./count_primitive N l  Affiche le nombre de primitif pour tous les degres <=N
   #./rand_irred N         Affiche un polynome irreductible de degré N
   #./rand_primitive N     Affiche un polynome primitif de degré N
   
