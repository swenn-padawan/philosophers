typedef struct s_data
{
	int	caca;
} t_data;

typedef struct s_sim
{
	t_data data;
	
}	t_sim;

int	main(void)
{
	t_data data;
	t_sim *sim = {0};

	sim = malloc(sizeof(sim));
	sim->data = data;
}
