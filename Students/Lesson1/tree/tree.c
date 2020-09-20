#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
 
tree createNullTree() {
  return (tree)NULL;
}

tree createTree(element_t x, tree l, tree r) {
  tree t = (tree)malloc(sizeof(struct TreeNode));
  t->data = x;
  t->left = l;
  t->right = r;
  return t;
}

tree createLeaf(element_t x) {
  return createTree(x, createNullTree(), createNullTree());
}

int isNullTree(tree t) {
  return (t == NULL);
}

int isLeaf(tree t) {
  return ((t != NULL) &&
	  (t->left == NULL) &&
	  (t->right == NULL)); 
}

int sizeOfTree(tree t) {
  if (isNullTree(t))
    return 0;
  else
    return (1 + sizeOfTree(t->left) + sizeOfTree(t->right));
}

// int sumOfTree(tree t) {
//   if (isNullTree(t))
//     return 0;
//   else
//     return (t->data + sumOfTree(t->left) + sumOfTree(t->right));
// }

char* treeToString(tree t) {
  char *result;
  char *left;
  char *right;
  
  if (isNullTree(t)) {
    result = (char*)malloc(4);
    strcpy(result, "Nil");
    return result;
  } else {
    left = treeToString(t->left);
    right = treeToString(t->right);
    
    result = (char*)malloc(40 + strlen(left) + strlen(right));
    sprintf(result, "node(%d,%s,%s)", t->data, left, right);
    free(left);
    free(right);
    return result;
  }
}

tree left(tree t) {
  if (isNullTree(t))
    return NULL;
  else return t->left;
}

tree right(tree t) {
  if (isNullTree(t))
    return NULL;
  else return t->right;
}

// element_t value(tree t) {
//   if (isNullTree(t))
//     return NULL;
//   else return t->data;
// }

struct TreeNode* leftMost(tree t) {
  struct TreeNode *n = t;
  
  if (isNullTree(t)) return NULL;
  while (!isNullTree(left(n)))
    n = left(n);
  return n;
}

struct TreeNode* rightMost(tree t) {
  struct TreeNode *n = t;
  
  if (isNullTree(t)) return NULL;
  while (!isNullTree(right(n)))
    n = right(n);
  return n;
}

tree addToLeftMost(tree t, element_t x) {
  tree n = leftMost(t);
  
  if (isNullTree(t))
    return createLeaf(x);
  else {
    n->left = createLeaf(x);
    return t;
  }
}

tree addToRightMost(tree t, element_t x) {
  tree n = rightMost(t);
  
  if (isNullTree(t))
    return createLeaf(x);
  else {
    n->right = createLeaf(x);
    return t;
  }
}

int height(tree t) {
  if(isNullTree(t)==1) return 0;
  if(height(t->left)>=height(t->right)) return 1+height(t->left);
  return 1+height(t->right);
  
}
int leavesCount(tree t) {
  if(isNullTree(t)==1) return 0;
  if((t->left==NULL)&&(t->right==NULL))
    return 1;
  return leavesCount(t->left)+leavesCount(t->right);
}
int innerNodeCount(tree t) {
  if(isNullTree(t)==1) return 0;
  if(t->left==NULL && t->right==NULL) return 0;
  if((t->left!=NULL)||(t->right!=NULL)) return 1+innerNodeCount(t->left)+innerNodeCount(t->right);
}
int rightChildCount(tree t) {
  // TODO
}

struct TreeNode *searchBST(tree t, element_t x) {
  if (isNullTree(t))
    return NULL;
  
  if(t->data==x)
    return t;
  else if (t->data<x)
    return searchBST(right(t), x);
  else
    return searchBST(left(t), x);
}

tree insertBST(tree t, element_t x) {
  if (isNullTree(t))
    return createLeaf(x);

  if (t->data==x)
    return t;
  else if (t->data>x) {
    t->left = insertBST(t->left, x);
    return t;
  } else {
    t->right = insertBST(t->right, x);
    return t;
  }
}
// xoa nut goc va tra ve cay BST sau khi da xoa
tree removeRootBST(tree t) {
  tree p, tmp;
  
  if (isNullTree(t))
    return NULL;
  // Xoa nut la
  if (isLeaf(t)) {
    free(t);
    return NULL;
  }
  // Xoa nut co 1 con phai
  if (left(t) == NULL) {
    tmp = right(t);
    free(t);
    return tmp;
  }
  // Xoa nut co 1 con trai
  if (right(t) == NULL) {
    tmp = left(t);
    free(t);
    return tmp;
  }
  // Xoa nut co hai con
  p = t;
  tmp = right(t);
  while (left(tmp) != NULL) {
    p = tmp;
    tmp = left(tmp);
  }

  t->data = tmp->data;
  tmp = removeRootBST(tmp);
  if (p == t)
    p->right = tmp;
  else
    p->left = tmp;
  return t;
}


void freetree(tree t)
  {
    if(isNullTree(t)!=1)
      {
        freetree(t->left);
        freetree(t->right);
        free((void *)t);
      }
  }

int countNode(tree t){
  if (t == NULL) return 0;
  return 1 + countNode(t->right) + countNode(t->left);
}

int IsEmpty(Queue *_Queue){
    return (_Queue->size == 0);
}
int IsFull(Queue *_Queue){
    return (_Queue->size == _Queue->limit);
}
Queue *CreateNewQueue(tree Root){
    Queue *newQueue = (Queue *) malloc(sizeof(Queue));
    newQueue->front = 0;
    newQueue->rear = 0;
    newQueue->size = 0;
    newQueue->limit = countNode(Root);
    newQueue->_Data = (tree) malloc(sizeof(struct TreeNode) * countNode(Root));
    return newQueue;
}

void enqueue(Queue **_Queue,tree Node){
    if(IsFull((*_Queue))) {
        printf("The queue is full\n");
        return;
    }
    (*_Queue)->_Data[(*_Queue)->rear] = (*Node);
    if((*_Queue)->rear + 1 < (*_Queue)->limit) (*_Queue)->rear +=1;
    else (*_Queue)->rear = 0;
    (*_Queue)->size += 1;
    return;
}   
tree Dequeue(Queue *_Queue){
    if(IsEmpty(_Queue)) {
        printf("The queue is empty\n");
        return NULL;
    }
    tree returnTree = (_Queue->_Data + _Queue->front );
    if(_Queue->front + 1 < _Queue->limit) _Queue->front +=1;
    else _Queue->front = 0;
    _Queue->size -= 1;
    return returnTree;
}

// char lay_ten(char s[],char s1[])

// {

//     int len=strlen(s);
//     int i = len-1,j;
//     int m=0;
//     while(i>=0)
//     {
//       if((s[i]==' ') && (s[i+1]!=' '))
//       {
//                   for(j=i+1;j<len;j++)
//                   {
//                       s1[m]=s[j];
//                       m++; 
//                   }
//                   s[i]='\0';
//                   break;
//       }
//       i--;
//     } 
// }

void breadth_first_search(tree Root){
    int i,count=1,c;
    c=countNode(Root);
    char str[20];
    sort_abc arr[c+2];
    Queue *queue = CreateNewQueue(Root);
    tree RunNode = Root;
    if(RunNode != NULL){
        enqueue(&queue,RunNode);
        while(!IsEmpty(queue)){
            RunNode = Dequeue(queue);
            // fprintf(ptr,"%s %s",RunNode->data->word,RunNode->data->meaning);
            // for(i=1;i<=RunNode->data.total;i++)
            //   printf("%-20s\t%lld\t\t%s\n",RunNode->data.name,RunNode->data.account[i].balance,RunNode->data.account[i].telephone);
            // if(RunNode->left != NULL || RunNode->right != NULL || !IsEmpty(queue)) printf("\n");
            // strcpy(arr[count].key,lay_ten(RunNode->data.name));
            // strcpy(str,RunNode->data.name);
            // lay_ten(str,arr[count].key);
            strcpy(arr[count].key,RunNode->data.name);
            arr[count].data=RunNode->data;
            count++;
            if(RunNode->left != NULL) enqueue(&queue,RunNode->left);
            if(RunNode->right != NULL) enqueue(&queue,RunNode->right);
        }
    }
    int j;
    sort_abc tmp;
    for(i=1;i<c;i++)
      for(j=i+1;j<=c;j++)
        if(strcasecmp(arr[i].key,arr[j].key)>0)
          {
            tmp=arr[i];
            arr[i]=arr[j];
            arr[j]=tmp;
          }
    for(i=1;i<=c;i++)
        printf("%s\t%d\n",arr[i].data.name,arr[i].data.times);
    printf("\n");
}

tree  minValueNode(tree node) 
{ 
    tree current = node; 
  
    /* loop down to find the leftmost leaf */
    while (current && current->left != NULL) 
        current = current->left; 
  
    return current; 
} 
tree deleteNode(tree root,element_t x){
        if (root == NULL) return root;
    if(x<root->data){
        root->left = deleteNode(root->left,x);
    }else if(x>root->data){
        root->right = deleteNode(root->right,x);
    }else{
        if(root->left == NULL){
            tree temp = root->right;
            free(root);
            return temp;
        }else if(root->right == NULL){
            tree temp = root->left;
            free(root);
            return temp; 
        }else{
            tree temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right,temp->data);
        }
    }
    return root;
}
