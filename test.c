#include <pthread.h>
#include <stdio.h>


typedef struct s_box
{
    pthread_mutex_t lock;    
    pthread_cond_t  not_full;   // writer sleeps here
    pthread_cond_t  not_empty;   // reader sleeps here
    int             value;    
    int             full;       // 0 = empty, 1 = full
}   t_box;





void    *writer(void *arg)
{
    t_box   *box;
    int     i;

    box = (t_box *)arg;
    i = 1;
    while (i <= 5)
    {
        pthread_mutex_lock(&box->lock);

        // if (box->full == 1)
        //     pthread_cond_wait(&box->not_full, &box->lock);

        box->value = i;
        box->full = 1;

        pthread_cond_signal(&box->not_empty);
        pthread_mutex_unlock(&box->lock);

        i++;
    }
    return (NULL);
}

void    *reader(void *arg)
{
    t_box   *box;
    int     count;

    box = (t_box *)arg;
    count = 0;
    while (count < 5)
    {
        pthread_mutex_lock(&box->lock);

        if (box->full == 0)
            pthread_cond_wait(&box->not_empty, &box->lock);

        printf("%d\n", box->value);
        box->full = 0;

        pthread_cond_signal(&box->not_full);
        pthread_mutex_unlock(&box->lock);
        count++;
    }
    return (NULL);
}






int main(void)
{
    t_box       box;
    pthread_t   w;
    pthread_t   r;

    pthread_mutex_init(&box.lock, NULL);
    pthread_cond_init(&box.not_full, NULL);
    pthread_cond_init(&box.not_empty, NULL);
    box.value = 0;
    box.full = 0;
    pthread_create(&w, NULL, writer, &box);
    pthread_create(&r, NULL, reader, &box);
    pthread_join(w, NULL);
    pthread_join(r, NULL);
    pthread_cond_destroy(&box.not_full);
    pthread_cond_destroy(&box.not_empty);
    pthread_mutex_destroy(&box.lock);
    return (0);
}