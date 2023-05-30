#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#define INFINITY INT_MAX

// Structure pour représenter un noeud dans le graphe
typedef struct Noeud {
    int id;
    int distance;
    struct Noeud* pres;
} Noeud;

// Structure pour représenter un arc dans le graphe
typedef struct arc {
    int destination;
    int poid;
    struct arc* suivant;
} arc;

// Structure pour représenter le graphe
typedef struct Graphe {
    int numero_des_noeuds;
    arc** liste_adjacente;
} Graphe;

// Structure pour représenter un élément de la file de priorité
typedef struct element {
    int id;
    int distance;
} element;

// Structure pour représenter une file de priorité basée sur un tas binaire
typedef struct file_tas_binaire {
    element* tas;
    int taille;
    int capacite;
} file_tas_binaire;

// Crée une nouvelle file de priorité vide
file_tas_binaire* cree_file(int capacity) {
    file_tas_binaire* pq = (file_tas_binaire*)malloc(sizeof(file_tas_binaire));
    pq->tas = (element*)malloc(capacity * sizeof(element));
    pq->taille = 0;
    pq->capacite = capacity;
    return pq;
}

// Échange les positions de deux nœuds dans la file de priorité
void change(element* a, element* b) {
    element temp = *a;
    *a = *b;
    *b = temp;
}

// Effectue l'opération "monter"  pour maintenir la propriété du tas
void monter(file_tas_binaire* pq, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (pq->tas[parent].distance > pq->tas[index].distance) {
            change(&pq->tas[parent], &pq->tas[index]);
            index = parent;
        } else {
            break;
        }
    }
}

// Insère un nœud dans la file de priorité
void insere(file_tas_binaire* pq, int id, int distance) {
    if (pq->taille == pq->capacite) {
        return; // File de priorité pleine
    }

    element newNode = {id, distance};
    pq->tas[pq->taille] = newNode;
    monter(pq, pq->taille);
    pq->taille++;
}







// Effectue l'opération "descendre"  pour maintenir la propriété du tas
void descendre(file_tas_binaire* pq, int index) {
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    int smallest = index;

    if (leftChild < pq->taille && pq->tas[leftChild].distance < pq->tas[smallest].distance) {
        smallest = leftChild;
    }

    if (rightChild < pq->taille && pq->tas[rightChild].distance < pq->tas[smallest].distance) {
        smallest = rightChild;
    }

    if (smallest != index) {
        change(&pq->tas[smallest], &pq->tas[index]);
        descendre(pq, smallest);
    }
}

// Retourne le nœud avec la plus petite distance dans la file de priorité
element dequeue(file_tas_binaire* pq) {
    element minNode = pq->tas[0];
    pq->tas[0] = pq->tas[pq->taille - 1];
    pq->taille--;
    descendre(pq, 0);
    return minNode;
}

// Vérifie si la file de priorité est vide
int test_vide(file_tas_binaire* pq) {
    return pq->taille == 0;
}

// Crée un nouveau nœud dans le graphe
Noeud* cree_nouveau_noeud(int id) {
    Noeud* newNode = (Noeud*)malloc(sizeof(Noeud));
    newNode->id = id;
    newNode->distance = INFINITY;
    newNode->pres = NULL;
    return newNode;
}

// Crée un nouvel arc dans le graphe
arc* cree_nouveau_arc(int destination, int weight) {
    arc* newEdge = (arc*)malloc(sizeof(arc));
    newEdge->destination = destination;
    newEdge->poid = weight;
    newEdge->suivant = NULL;
    return newEdge;
}

// Crée un nouveau graphe avec le nombre de nœuds donné
Graphe* cree_nouveau_graphe(int numNodes) {
    Graphe* graph = (Graphe*)malloc(sizeof(Graphe));
    graph->numero_des_noeuds = numNodes;
    graph->liste_adjacente = (arc**)malloc(numNodes * sizeof(arc*));
    int i;
    for (i = 0; i < numNodes; i++) {
        graph->liste_adjacente[i] = NULL;
    }
    return graph;
}

// Ajoute un arc au graphe entre les nœuds source et destination avec le poids donné
void ajouter_arc(Graphe* graph, int source, int destination, int weight) {
    arc* newEdge = cree_nouveau_arc(destination, weight);
    newEdge->suivant = graph->liste_adjacente[source];
    graph->liste_adjacente[source] = newEdge;
}

// Libère la mémoire allouée pour le graphe
void allouee_graphe(Graphe* graph) {
    int i;
    for (i = 0; i < graph->numero_des_noeuds; i++) {
        arc* edge = graph->liste_adjacente[i];
        while (edge != NULL) {
            arc* nextEdge = edge->suivant;
            free(edge);
            edge = nextEdge;
        }
    }
    free(graph->liste_adjacente);
    free(graph);
}






// Implémente l'algorithme de Dijkstra en utilisant une file de priorité basée sur un tas binaire
void djikstra_tas_binaire(Graphe* graph, int source) {
    int numNodes = graph->numero_des_noeuds;
    Noeud** nodes = (Noeud**)malloc(numNodes * sizeof(Noeud*));
    file_tas_binaire* pq = cree_file(numNodes);

    int i;
    for (i = 0; i < numNodes; i++) {
        nodes[i] = cree_nouveau_noeud(i);
        if (i == source) {
            nodes[i]->distance = 0;
        }
        insere(pq, i, nodes[i]->distance);
    }

    while (!test_vide(pq)) {
        element minNode = dequeue(pq);
        int u = minNode.id;
        arc* edge = graph->liste_adjacente[u];
        while (edge != NULL) {
            int v = edge->destination;
            int poid = edge->poid;
            int newDistance = nodes[u]->distance + poid;
            if (newDistance < nodes[v]->distance) {
                nodes[v]->distance = newDistance;
                nodes[v]->pres = nodes[u];
                insere(pq, v, newDistance);
            }
            edge = edge->suivant;
        }
    }

    // Affichage des distances les plus courtes depuis le nœud source
    printf("Distances les plus courtes depuis le noeud %d :\n", source);
    for (i = 0; i < numNodes; i++) {
        printf("Noeud %d : %d\n", i, nodes[i]->distance);
    }

    // Libération de la mémoire
    for (i = 0; i < numNodes; i++) {
        free(nodes[i]);
    }
    free(nodes);
    free(pq->tas);
    free(pq);
}


int main() {
    int numNodes, numEdges;
    printf("Entrez le nombre de noeuds dans le graphe : ");
    scanf("%d", &numNodes);

    Graphe* graph = cree_nouveau_graphe(numNodes);

    printf("Entrez le nombre d'aretes dans le graphe : ");
    scanf("%d", &numEdges);

    printf("Entrez les aretes au format 'source destination poids' (separees par des retours e la ligne) :\n");
    int i;
    for (i = 0; i < numEdges; i++) {
        int source, destination, weight;
        printf("Arete %d :\n", i + 1);
        printf("Source : ");
        scanf("%d", &source);
        printf("Destination : ");
        scanf("%d", &destination);
        printf("Poids : ");
        scanf("%d", &weight);
        ajouter_arc(graph, source, destination, weight);
        printf("\n");
    }

    int sourceNode;
    printf("Entrez le noeud source pour l'algorithme de Dijkstra : ");
    scanf("%d", &sourceNode);

    djikstra_tas_binaire(graph, sourceNode);

    allouee_graphe(graph);

    return 0;
}
