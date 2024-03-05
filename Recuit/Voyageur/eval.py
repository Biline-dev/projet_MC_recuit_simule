import os
import subprocess
import random
import csv

if __name__ == "__main__":

    nb_simulations = 2  # Change this to the number of simulations you want

    with open("result_parameters.csv", "w", newline="") as csv_file:
        csv_writer = csv.writer(csv_file)
        csv_writer.writerow(["nbVille", "tInit", "tFin", "alpha", "amplitude", "MaxRepetitions", "Cout"])


        for _ in range(nb_simulations):
            nbVille = 30
            tInit = random.randint(800, 4000)
            tFin = random.uniform(0.01, 0.02)
            alpha = random.uniform(0.8, 1.0)
            amplitude = random.randint(nbVille // 10, nbVille // 4)
            MaxRepetitions = random.randint(100, 2500)
            save_result = "o"

            # Write the variables to a file
            with open("fig.txt", "w") as file:
                file.write(f"{tInit}\n")
                file.write(f"{tFin}\n")
                file.write(f"{alpha}\n")
                file.write(f"{amplitude}\n")
                file.write(f"{MaxRepetitions}\n")
                file.write(f"{save_result}\n")

            # Execute the command and capture the output
            output = subprocess.getoutput("./recuitVoyageur < fig.txt")
            with open("Cout", "r") as file:
            # Read all lines into a list
                lines = file.readlines()

                # Get the last line
                last_line = lines[-1]

                # Split the line into space-separated values
                values = last_line.split()

                # Extract the value you want (assuming it's the second value)
                cout = float(values[1])

            print(f"The desired value is: {cout}")

            # Verify if the file exists before deleting
            if os.path.exists("fig.txt"):
                os.remove("fig.txt")
                print("Le fichier fig.txt a été supprimé avec succès.")
            else:
                print("Le fichier fig.txt n'existe pas.")

            # Extract parameters from the output
            result_line = output.split("\n")[-1]
            parameters = [f"{nbVille}", f"{tInit}", f"{tFin}", f"{alpha}", f"{amplitude}", f"{MaxRepetitions}", f"{cout}"]

            # Append parameters to the CSV file
            csv_writer.writerow(parameters)

    print(f"Results and parameters saved in result_parameters.csv")
