#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"



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
        tree p;
        element_t q;
        int count_line = 0;

        a = stop_word("stopw.txt");
        while(fgets(str,120,pt)!=NULL)
            {
                count_line++;
                // printf("%s",str);
                token1 = strtok(str,c1);
                token2 = strtok(token1,c2);
                while(token2 != NULL)
                    {
                        if(check_stopw(a,token2) == 0)
                            {
                                if(searchBST(t,token2) == NULL)
                                    {
                                        strcpy(q.name,token2);
                                        q.time = 1;
                                        q.appea[q.time] = count_line;
                                        t = insertBST(t,q);
                                    }
                                else
                                    {
                                        p = searchBST(t,token2);
                                        p->data.time++;
                                        p->data.appea[p->data.time] = count_line;
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