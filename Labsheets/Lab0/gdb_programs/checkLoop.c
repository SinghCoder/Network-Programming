#include<stdio.h>
#include<stdlib.h>
 
/* Link list node */
struct node
{
  int data;
  struct node* next;
};
 
void push(struct node** head_ref, int new_data)
{
  
  struct node* new_node =
  (struct node*) malloc(sizeof(struct node));
 
  new_node->data  = new_data;
  
  new_node->next = (*head_ref);
  
  (*head_ref)    = new_node;
}
 
// returns 1 if loop found,else 0
int detectloop(struct node *list)
{
  struct node  *slow_p = list, *fast_p = list;
 
  while(slow_p && fast_p )
  {
    slow_p = slow_p->next;
    fast_p  = fast_p->next->next;
    if (slow_p == fast_p)
    {
           return 1;
    }
  }
  return 0;
}
 
 
int main()
{
  /* create list */
  struct node* head = NULL;
 
  push(&head, 20);
  push(&head, 4);
  push(&head, 15);
  push(&head, 10);
  push(&head, 13);
  
  /* test list without loop */
    if( detectloop(head)==1)
        printf("Loop Found\n");
    else
        printf("No Loop Found\n");
        
  /* Create a loop for testing */
  head->next->next->next->next = head;
  
  /* test list again with loop */
    if( detectloop(head)==1)
        printf("Loop Found\n");
    else
        printf("No Loop Found\n");
  
 
  
}