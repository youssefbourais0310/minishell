/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:59:39 by ybourais          #+#    #+#             */
/*   Updated: 2023/06/15 16:18:10 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int search(char *str, int c)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == c)
            return 1;
        i++;
    }
    return 0;
}

int is_alphanumeric(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '=')
        {
            i = 0;
            while (str[i] != '=')
            {
                if (!((str[i] >= 48 && str[i] <= 57) || (str[i] >= 65 && str[i] <= 90) || (str[i] >= 97 && str[i] <= 122) || str[i] == 95 || str[i] == ' ')) // (48 -> 57) && (65 -> 90) && (97 -> 122) && 95(underscor) && 91(equal)                       
                    return 0;
                i++;
            }
            break;
        }
        i++;
    }
    if (str[i + 1] == ' ' || !search(str, '='))
        return 0;
    return 1;
}

int *ornot(char *str, int t)
{
    
    int *arr = malloc(sizeof(int) * t);
    int j = 0;
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '$')
        {
            if (str[i - 1] == '\'')
            {
                arr[j] = 0;
                j++;
            }
            else if (str[i - 1] == '\"')
            {
                arr[j] = 1;
                j++;
            }
            i++;
        }
    }
    return arr;    
}

int slen(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    return i;
}

void copy_str(char *dst, char *src)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

char *join(char *s1, char *s2)
{
    char *str = malloc(sizeof(char) * (slen(s1) + slen(s2) + 1));

    copy_str(str, s1);
    copy_str(str + slen(s1), s2);
    return str;
}

int compare(char *s1, char *s2)
{
    int i = 0;
    
    while (s1[i] && s1[i])
    {
        if (s1[i] != s2[i])
            return 0;
        i++;
    }
    return 1;
}

int compare_until(char *s1, char *s2, int n)
{
    int i;
    i = 0;
    while (i < n)
    {
        if (s1[i] == s2[i])
            i++;
        else
            return 0;
    }
    return 1;
}

int copy_tab(char **tab1, char **tab2)
{
    int i = 0;
    while (tab2[i])
    {
        tab1[i] = malloc(sizeof(char) * slen(tab2[i] + 1));
        copy_str(tab1[i], tab2[i]);
        i++;
    }
    tab1[i] = 0;
    return i;
}

int strchrch(char *str, char c)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] == c)
            return i;
        i++;
    }
    return 0;
}

t_node *creat_list(t_node *head, char *env)
{
    t_node *new_node = malloc(sizeof(t_node));
    new_node->var = malloc(sizeof(char) * slen(env) + 1);
    copy_str(new_node->var, env);
    new_node->next = NULL;
    if(head == NULL)
        head = new_node;
    else
    {
        t_node *tmp = head;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = new_node;
    }
    return head;
}

t_node *search_list(t_node *head, char *str, int *p)
{
    t_node *tmp;
    tmp = head;

    while (tmp)
    {
        if (compare_until(tmp->var, str, strchrch(tmp->var, '=')))
        {
            head = replace_node(head, str, tmp->var);
            *p = 1;
            return head;
        }
        tmp = tmp->next;
    }
    *p = 0;
    return head;
}

t_node *replace_node(t_node *head, char *new, char *to_delete)
{
    t_node *new_node;
    new_node = malloc(sizeof(t_node));
    new_node->var = malloc(sizeof(char) * slen(new) + 1);
    copy_str(new_node->var, new);

    t_node *curr;
    curr = head;
    if(compare(curr->var, to_delete))
    {
        new_node->next = head->next;
        head = new_node;
        free(curr->var);
        free(curr);
        return head;
    }
    t_node *prev;
    curr = head;
    prev = head;
    while (!compare(curr->var, to_delete))
    {
        prev = curr;
        curr = curr->next;
    }
    prev->next = new_node;
    new_node->next = curr->next;
    free(curr->var);
    free(curr);
    return head;
}

int compare_len(char *to_delete, char *list_var)
{
    char str[slen(list_var)];
    int indice = strchrch(list_var, '=');
    int i = 0;
    while (i < indice)
    {
        str[i] = list_var[i];
        i++;
    }
    str[i] = '\0';
    printf("%d %d\n", slen(str), slen(to_delete));
    if(slen(str) != slen(to_delete))
        return 0;
    else
        if(compare(to_delete, str))
            return 1;
    return 0;
}

int find_value(t_node *head, char *str)
{
    t_node *curr;
    t_node *after;

    curr = head;
    after = head;

    while (curr)
    {
        if(!compare_len(str, curr->var))
        {
            after = curr;
            curr = after->next;
        }
        else
            return 1;
    }
    return 0;
}

t_node *unset_node(t_node *head, char *to_delete)
{
    t_node *curr;
    t_node *prev;

    curr = head;
    prev = head;

    if(!find_value(head, to_delete))
        return head;
    else
    {
        if(compare_len(to_delete, curr->var))
        {
            head = curr->next;
            free(curr->var);
            free(curr);
            return head;
        }
        while (!compare_len(to_delete, curr->var))
        {
            prev = curr;
            curr = curr->next;
        }
        prev->next = curr->next;
        free(curr->var);
        free(curr);
        return head;
    }
    return head;
}

t_node *commands(char **tab, char **env, t_node *head)
{
    t_node *tmp;
    int i = 0;
    int p = 0;
    static int num;

    if(num == 0)
    {
        while (env[i])
            head = creat_list(head, env[i++]);
        num++;
    }
    if (compare(tab[0], "export"))
    {
        if (tab[0] && !tab[1])
        {
            tmp = head;
            while (tmp != NULL)
            {
                printf("declare -x %s\n", tmp->var);
                tmp= tmp->next;
            }
        }
        else
        {
            i = 1;
            while (tab[i])
            {
                if (is_alphanumeric(tab[i]))
                {
                    head = search_list(head, tab[i], &p);
                    if (!p)
                        head = creat_list(head, tab[i]);
                }
                else
                    printf("my Shell: no matches found: %s\n", tab[i]);
                i++;
            }
        }
    }
    else if (compare(tab[0], "echo"))
    {
        i = 1;
        while (tab[i])
        {
            printf("%s", tab[i]);
            if(tab[i][0] != ' ' && tab[i + 1])
                printf(" ");
            i++;
        }
        printf("\n");
    }
    else if (compare(tab[0], "env"))
    {
        tmp = head;
        while (tmp != NULL)
        {
            printf("%s\n", tmp->var);
            tmp= tmp->next;
        }
    }
    else if (compare(tab[0], "unset") && tab[1])
    {
        int j = 1;
        while (tab[j])
        {
            head = unset_node(head, tab[j]);
            j++;
        }
    }
    else
        printf("%s ERROR\n", tab[0]);
    return(head);
}

int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;

    char *input;
    char **tab;
    int *arr;
    t_node *head = NULL;
    int i = 0;
    while (env[i])
        i++;
    char **my_env = malloc(sizeof(char *) * i + 1);
    copy_tab(my_env, env);

    while(1)
    {
        input = readline("\e[1;32mmy_Shell-310$ \e[0m");
        add_history(input);
        arr = check_quoting(input);
        tab = split(input);
        modification(tab, 25, ' ');
        head = commands(tab, my_env, head);
        free_tab(tab);
    }
    return 0;
}
