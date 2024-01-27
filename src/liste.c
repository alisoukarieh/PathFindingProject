#include "./include/liste.h"

// Met un point dans la liste
void insert(pts_dispo **head, int x, int y, float dist) {
  // Allocate memory for the new node
  pts_dispo *new_node = (pts_dispo *) malloc(sizeof(pts_dispo));
  if (new_node == NULL) {
    fprintf(stderr, "Erreur dans insert : malloc failed\n");
    exit(1);
  }
  new_node->x = x;
  new_node->y = y;
  new_node->dist = dist;
  new_node->prev = NULL;
  new_node->next = NULL;

  // If the list is empty, the new node becomes the first node
  if (*head == NULL) {
      *head = new_node;
      return;
  }

  // Traverse the list to find the last node
  pts_dispo *current = *head;
  while (current->next != NULL) {
      current = current->next;
  }

  // Insert the new node at the end of the list
  current->next = new_node;
  new_node->prev = current;


}

// Libere la memoire de la liste
void free_list(pts_dispo *head) {
    pts_dispo *current = head;
    while (current != NULL) {
        pts_dispo *temp = current;
        current = current->next;
        free(temp);     
    }
    free(current);  
    free(head); 
}

// Retourne le noeud avec la plus petite distance
pts_dispo* first(pts_dispo** head) {
  // Traiter le cas ou la liste est vide
  if (*head == NULL) {
    return NULL;
  }

  // Trouver le noeud avec la plus petite distance
  pts_dispo* current = *head;
  pts_dispo* smallest = current;
  while (current != NULL) {
    if (current->dist < smallest->dist) {
      smallest = current;
    }
    current = current->next;
  }
  // Retirer le noeud de la liste
  if (smallest == *head) {
    *head = smallest->next;
    if (*head != NULL) {
      (*head)->prev = NULL;
    }
  } else {
    smallest->prev->next = smallest->next;
    if (smallest->next != NULL) {
      smallest->next->prev = smallest->prev;
    }
  }

  return smallest;
}

// Retourne la distance entre deux points
float distance_2q(int x1, int y1, int x2, int y2) {
  return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}