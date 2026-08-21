# CodeXion_aide

## Ressources :
[Tuto 1](#https://koor.fr/C/cthreads/thrd_create.wp)
[Tuto 2](#https://www.youtube.com/watch?v=o_GbRujGCnM)

Par exemple :
```C
while (get_time() - start < 500)
	usleep(1000);
```
Cela permet de faire une attente sans monopoliser inutilement le CPU.
```C
#include <time.h>

struct timespec	start;
struct timespec	now;

clock_gettime(CLOCK_MONOTONIC, &start);

/* ... */

clock_gettime(CLOCK_MONOTONIC, &now);
```

Et si tu fais une fonction pour récupérer le temps :
```C
long	get_time(void)
{
	struct timespec	time;


	clock_gettime(CLOCK_MONOTONIC, &time);
	return (time.tv_sec * 1000 + time.tv_nsec / 1000000);
}
```
Alors struct timespec time est local à la fonction et tu n'as besoin de le mettre nulle part dans ton .h.