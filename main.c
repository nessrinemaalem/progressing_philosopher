# include "point_hache.h"

pthread_mutex_t		take;
pthread_mutex_t		drop;
//pthread_mutex_t		look[4];


unsigned long long	what_time_is_it(void)
{
	unsigned long long		time_of_day;
	struct timeval			clock;

	if (gettimeofday(&clock, NULL) == 0)
		return (time_of_day = (unsigned long long)clock.tv_sec * 1000 + (unsigned long long)clock.tv_usec);
	else
		return (-1);
}

void	go_to_sleep(t_philo *philo)
{
	printf("philo %d : 'je vais faire un gros dodo'\n", philo->range);
	usleep(1000);
	printf("philo %d : 'moohh, je suis recinqué'\n", philo->range);
}

int	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&look[philo->range]);
	pthread_mutex_lock(&look[philo->tab[(philo->range + 1) % philo->total]]);
	usleep(3000);
	while (1)
	{
		if (philo->tab[philo->range] == 1 && philo->tab[(philo->range + 1) % philo->total] == 1)
		{
			printf("philo %d is ok? right %d left %d\n", philo->range, philo->tab[philo->range], philo->tab[(philo->range + 1) % philo->total]);
			break;
		}
		else
		{
			printf("%lld philo %d is starving\n", what_time_is_it(), philo->range);
			usleep(1000);
		}
		//are_you_diying(philo);
	}
	printf("philo %d unlock look\n", philo->range);
	pthread_mutex_unlock(&look[philo->range]);
	pthread_mutex_unlock(&look[philo->tab[(philo->range + 1) % philo->total]]);
	if (philo->tab[philo->range] == 1 && philo->tab[(philo->range + 1) % philo->total] == 1)
	{	
		//usleep(30000);
		pthread_mutex_lock(&take);
		// disposition de la table à l'entrée
		printf("philo %d lock take\n", philo->range);
		printf("philo %d table before taking fork :%d%d%d%d\n", philo->range, philo->tab[0], philo->tab[1], philo->tab[2], philo->tab[3]);
		if (philo->tab[philo->range] == 1)
			philo->tab[philo->range] -= 1; // fourchette à sa gauche
		else
		{
			printf("philo %d error left fork\n", philo->range);
			pthread_mutex_unlock(&take);
			return (0);
		}
		if (philo->tab[(philo->range + 1) % philo->total] == 1)
			philo->tab[(philo->range + 1) % philo->total] -= 1; // fourchette à sa droite
		else
		{
			printf("philo %d error right fork\n", philo->range);
			pthread_mutex_unlock(&take);
			return (0);
		}	
		// disposition de la table en sortie
		printf("philo %d table after taking fork :%d%d%d%d\n", philo->range, philo->tab[0], philo->tab[1], philo->tab[2], philo->tab[3]);
		pthread_mutex_unlock(&take);
		printf("philo %d unlock take\n", philo->range);
	}
	return (0);
}

int	drop_forks(t_philo *philo)
{
	pthread_mutex_lock(&drop);
	printf("philo %d lock drop\n", philo->range);
	philo->tab[philo->range] += 1;
	philo->tab[(philo->range + 1) % philo->total] += 1;
	//printf("philo %d has droped forks right %d left %d\n", philo->range, philo->tab[philo->range], philo->tab[(philo->range + 1) % philo->total]);
	printf("philo %d drop forks and table :%d%d%d%d\n", philo->range, philo->tab[0], philo->tab[1], philo->tab[2], philo->tab[3]);
	pthread_mutex_unlock(&drop);
	printf("philo %d unlock drop\n", philo->range);
	return (0);
}

int	eat(t_philo *philo)
{
	if (philo->tab[philo->range] == 0 && philo->tab[(philo->range + 1) % philo->total] == 0)
	{
		printf("philo %d start eating\n", philo->range);
		usleep(3000);
		philo->last_meal = what_time_is_it();
	}
	else
	{
		printf("philo %d no fork to eat\n", philo->range);
		printf("philo %d right %d left %d\n", philo->range, philo->tab[(philo->range + 1) % philo->total], philo->tab[philo->range]);
	}
	return (0);
}

void 	*routine(void *arg)
{
	t_philo				*philo;
	
	philo = (t_philo *)arg;
	while (1)
	{
		go_to_sleep(philo);
		//wait_and_starve(philo);
		printf("philo %d a\n", philo->range);
		take_forks(philo);
		eat(philo);
		drop_forks(philo);
	}
	return (NULL);
}

t_philo	*init_philo(int i, int *tab)
{
	t_philo			*philo;
	pthread_mutex_t	forks[4];

	pthread_mutex_init(&forks[philo->range]);
	philo->l_fork = &forks[philo->range];
	pthread_mutex_init(&forks);
	philo = malloc(sizeof(t_philo));
	if (philo == NULL )
		return (NULL);
	philo->range = i;
	philo->tab = tab;
	philo->total = 4;

	return (philo);
}

t_data	*init_data()
{
	
}

int	main(void)
{
	int					i;
	t_data				data;
	// pthread_t			philo[4];
	// int					forks[4] = {1, 1, 1, 1};
	// t_philo				*tab_philo[4];

	// i = 0;
	// // faire fonction init mutex
	// // while (i < 4)
	// // {	
	// // 	pthread_mutex_init(&look[i], NULL);
	// // 	i++;
	// // }
	// i = 0;
	// // pthread_mutex_init(&take, NULL);
	// // pthread_mutex_init(&drop, NULL);
	data = init_data();
	while (i < 4)
	{
		tab_philo[i] = init_philo(i, forks);
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
	i = 0;
	while (i < 4)
	{	
		pthread_mutex_destroy(&look[i]);
		i++;
	}
	pthread_mutex_destroy(&take);
	pthread_mutex_destroy(&drop);
	return (0);
}
