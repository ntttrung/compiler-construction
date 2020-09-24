#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include <ctype.h>


int read_col(char *sour, char *str, int begi)
    {
        int i,j;
        i = begi;
        int flag = 2;
        while(i<strlen(sour))
            {
                for(j=0;j<strlen(str);j++)
                    {
                        if(tolower(sour[i+j]) != tolower(str[j]))
                            {
                                flag = 0;
                                break;
                            }
                        else flag = 1;
                    }
                if(flag == 1) break;
                i++;
            }
        return i+1;
    }

char** stop_word(char *file_name)
    {
        FILE *pt;
        pt = fopen(file_name, "r");
        int i = 0;
        char str[20];
        char c[2] = "\n";
        char* token;

        char** a = malloc(16 * sizeof(char *));;
        while(fgets(str, 10, pt) != NULL)
            {
                token = strtok(str,c);
                a[++i] = (char *)malloc(strlen(token+1));
                strcpy(a[i],token);
            }
        return a;
    }


int strlen_arr(char *file_name)
    {
        FILE *pt;
        pt = fopen(file_name, "r");
        int i = 0;
        char str[20];
        while(fgets(str, 10, pt) != NULL)
            {
                i++;
            }
        return i;
    }
// int* check_duplicate(int *a, int l)
//     {
//         int i;
//         int* b;
//         int count = 1;
//         for(i=1;i<=l;i++)
//             if(a[i]!=a[i-1])
//                 {
//                     b[count] = a[i];
//                     count++
//                 }
//         return b;
//     }

int check_stopw(char** a, char *word)
    {
        int i = strlen_arr("stopw.txt"),j;
        for(j=1;j<=i;j++)
            {
                if(strcasecmp(a[j],word) == 0)
                    return 1;
            }
        return 0;
    }


tree read_data()
    {
        char** a;
        tree t = createNullTree();
        FILE *pt;
        char str[120];
        pt = fopen("vanban.txt","r");
        char c1[2] = "\n";
        char c2[2] = " ";
        char* token1;
        char* token2;
        char s1[120];
        tree p;
        element_t q;
        int count_line = 0;
        char* tmp;
        a = stop_word("stopw.txt");
        while(fgets(str,120,pt)!=NULL)
            {
                count_line++;
                strcpy(s1,str);
                token1 = strtok(str,c1);
                token2 = strtok(token1,c2);
                while(token2 != NULL)
                    {
                        if(check_stopw(a,token2) == 0 && isdigit(token2[0]) ==0 && token2!="a")
                            {
                                if(searchBST(t,token2) == NULL)
                                    {
                                        strcpy(q.name,token2);
                                        q.time = 1;
                                        q.name[0] = tolower(q.name[0]);
                                        q.appea[q.time].lin = count_line;
                                        q.appea[q.time].col = read_col(s1, q.name, 0);
                                        t = insertBST(t,q);
                                    }
                                else
                                    {
                                        p = searchBST(t,token2);
                                        p->data.time++;
                                        if(p->data.appea[p->data.time].lin == count_line)
                                            p->data.appea[p->data.time].col = read_col(s1, p->data.name, p->data.appea[p->data.time-1].col+2);
                                        else p->data.appea[p->data.time].col = read_col(s1, p->data.name,0);
                                        p->data.appea[p->data.time].lin = count_line;
                                        
                                    }
                            }
                        token2 = strtok(NULL,c2);
                    }
            }  
        return t;
    }

int main()
    {
        tree t =createNullTree();
        t = read_data();
        breadth_first_search(t);
    }