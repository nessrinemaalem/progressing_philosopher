#ifndef PHILOSOPHER
# define PHILOSOPHER

# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>

typedef struct s_philo t_philo;
struct	s_philo
{
	int					*tab;
	int					range;
	int					total;
	unsigned long long	last_meal;
};

void	ft_error(void);
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);

# endif