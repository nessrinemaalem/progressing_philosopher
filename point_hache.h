#ifndef PHILOSOPHER
# define PHILOSOPHER

# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>

typedef struct s_data t_data;
struct s_data
{
	// mutex look take et drop ?
	// time set up
	int					total_philo;
	pthread_t			**philo;
	pthread_mutex_t		**forks_mutex;
	int					**tab_forks;
};

typedef struct s_philo t_philo;
struct	s_philo
{
	t_data				data;
	pthread_mutex_t		r_fork;
	pthread_mutex_t		l_fork;
	//int					*tab;
	int					range;
	int					total;
	unsigned long long	last_meal;
};

void	ft_error(void);
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);

# endif