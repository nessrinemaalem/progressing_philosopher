# include "point_hache.h"

pthread_mutex_t		take;
pthread_mutex_t		drop;
int					id[4] = {11, 13, 17, 19};

void	go_to_sleep(t_philo *philo)
{
	printf("philo %d : 'je vais faire un gros dodo'\n", philo->range);
	usleep(1000);
	printf("philo %d : 'moohh, je suis recinqué'\n", philo->range);
	printf("philo %d 2\n", philo->range);
}

int	take_forks(t_philo *philo)
{
	if (philo->tab[philo->range] != 1 || philo->tab[(philo->range + 1) % philo->total] != 1)
	{
		printf("philo %d is starving\n", philo->range);
		usleep(1000);
		take_forks(philo);
	}
	else
		pthread_mutex_lock(&take);
		philo->tab[philo->range] = 0;
		printf("philo %d has taken the right fork\n", philo->range);
		philo->tab[(philo->range + 1) % philo->total] = 0;
		printf("philo %d has taken the left fork\n", philo->range);
		pthread_mutex_unlock(&take);
	return (0);
}

int	drop_forks(t_philo *philo)
{
	pthread_mutex_lock(&drop);
	philo->tab[philo->range] = 1;
	philo->tab[(philo->range + 1) % philo->total] = 1;
	printf("philo %d has droped forks right %d left %d\n", philo->range, philo->tab[philo->range], philo->tab[(philo->range + 1) % philo->total]);
	pthread_mutex_unlock(&drop);
	return (0);
}

void	eat(t_philo *philo)
{
	printf("philo %d eats right %d left %d\n", philo->range, philo->tab[philo->range], philo->tab[(philo->range + 1) % philo->total]);
	usleep(3000);
	drop_forks(philo);
}


void 	*routine(void *arg)
{
	t_philo		*philo;

	philo = (t_philo *)arg;
	printf("philo %d id is %d\n", philo->range, id[philo->range]);
	while (1)
	{
		printf("philo %d 1\n", philo->range);
		go_to_sleep(philo);
		if (take_forks(philo) == 0)
			printf("ok\n"); // s'il n'en a pas il reesaye
		eat(philo);
		if (drop_forks(philo) == 0)
			printf("philo %d 10\n", philo->range);
	}
	return (NULL);
}

t_philo	*init_philo(int i, int *tab)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (philo == NULL )
		return (NULL);
	philo->range = i;
	philo->tab = tab;
	philo->total = 4;
	return (philo);
}

int	main(void)
{
	int					i;
	pthread_t			philo[4];
	int					forks[4] = {1, 1, 1, 1}; // créer une fonction qui remplit un tableau
	t_philo				*tab_philo[4];

	i = 0;
	pthread_mutex_init(&take, NULL);
	pthread_mutex_init(&drop, NULL);
	while (i < 4)
	{
		tab_philo[i] = init_philo(i, forks);
		//printf("range %d\n", tab_philo[i]->range);
		// int	sas;
		// sas = tab_philo[i]->range;
		if (tab_philo[i] == NULL)
			ft_error();
		if (pthread_create(&philo[i], NULL, &routine, tab_philo[i]) != 0)
		{
			printf("failed to create thread\n");
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < 4)
	{
		if (pthread_join(philo[i], NULL) != 0)
		{
			printf("failed to join thread\n");
			return (2);
		}
		i++;
	}
	pthread_mutex_destroy(&take);
	pthread_mutex_destroy(&drop);
	return (0);
}