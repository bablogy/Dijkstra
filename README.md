# Dijkstra
Dijkstra method implementation

The project use the following hardwares : 
- Teensy3
- Arduino TFT A000096
- SD card

The graph used for this experience can be found in : 

https://fr.wikipedia.org/wiki/Algorithme_de_Dijkstra#/media/Fichier:DijkstraBis01.svg

Once the program is executed a French menu is displayed:

 Sélectionnez l'option désirée
 0: Trouver le chemin court
 1: Modifier le coût d'un chemin
 2: Donner le rond-point non-opérationnel
 3: Donner le chemin non-opérationnel
 4: exit

You have to enter a number from 0 to 4:
- 0: gives the shortest path between 2 nodes.
- 1: modifies the weight of an arc of the graph
- 2: allows you to isolate a vertex
- 3: allows you to delete an arc between two vertices
- 4: will end the execution, you have to close the Arduino serial port and reopen it (this resets the Arduino board)

- the graph can be read explicitly by the function readGraphLocal (I wrote this function so that the program works with the Arduino uno board -  It does not have an SD card

- if we choose a board with SD card, we can replace the function readGraphLocal with the function readGraph that reads the weight matrix from a given txt file.

- the program with readGraph works perfectly with the teensy 3.6 too

- I hope this program will be useful to you
