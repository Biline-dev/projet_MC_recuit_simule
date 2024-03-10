import subprocess
from concurrent.futures import ProcessPoolExecutor, wait, ALL_COMPLETED
import sys
import random
import csv
import time
import re
import os

def traitement_ais():

    #Changer en fonction
    nbVille = 30
    typeMutation = 1
    #Génération des paramètres aléatoires
    tInit = random.randint(1, 5000)
    tFin = random.uniform(0, 100)
    alpha = random.uniform(0.8, 1.0)
    amplitude = random.randint(nbVille // 10, nbVille // 4)
    MaxRepetitions = random.randint(1, 100)

    #Création du string de la commande à exécuter
    commande = f"printf '{tInit}\n{tFin}\n{alpha}\n{amplitude}\n{MaxRepetitions}n\n' | ../src/recuitVoyageur"

    start_time = 0.0
    execution_time = 0.0

    # Début de l'enregistrement du temps
    start_time = time.time()

    #Lancement du programme ./ais avec des paramètres aléatoires
    resultat = subprocess.run(commande,shell=True,stdout=subprocess.PIPE,text=True)

    # Enregistrez le temps de fin
    end_time = time.time()

    execution_time = round(end_time - start_time,3)
    # Utilisation d'une expression régulière pour extraire le coût après la ligne spécifique
    # L'utilisation des parenthèse représente un groupe qui peut être extrait grâce à match.group()
    pattern = re.compile(r"Temperature a la fin de l'algorithme=([0-9.]+)")
    pattern2 = re.compile(r"Cout optimal \(fxopt\)=([0-9.]+)")

    match = pattern.search(resultat.stdout)
    match2 = pattern2.search(resultat.stdout)

    if match and match2:
        temperature_fin_execution = float(match.group(1))
        cout = float(match2.group(1))
        with open("../data/resultats.csv",'a') as csvfile:
                csvWriter = csv.writer(csvfile)
                csvWriter.writerow([nbVille,typeMutation,tInit,tFin,alpha,amplitude,MaxRepetitions,temperature_fin_execution,cout,execution_time])

    else:
        print("Aucun coût trouvé après la ligne spécifique.")
        exit(-1)

def main():

    #Permet de choisir le répertoire du fichier comme répertoire de travail pour python
    file_directory = os.path.dirname(os.path.abspath(__file__))
    os.chdir(file_directory)

    default = 1
    if(len(sys.argv) > 1):
        default = int(sys.argv[1])
    else:
        print("Utilisation : python3 benchmark.py [nombreProgramme]")
        exit(-1)

    with ProcessPoolExecutor() as executor:

        #Lance un nombre "i" de tâches en parallèle
        futures = [executor.submit(traitement_ais) for i in range(default)]
        #On attends la fin des tâches pour finir le programme
        wait(futures, return_when=ALL_COMPLETED)

        print("Toutes les tâches sont terminées")

if __name__ == "__main__":
    main()