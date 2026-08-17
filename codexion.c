/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 15:39:56 by llafforg          #+#    #+#             */
/*   Updated: 2026/08/17 16:13:29 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

int	thrd_create( thrd_t *thread, thrd_start_t startFunction, void *data )
{

}



bool mustExit = false;

// La fonction sur laquelle vont démarrer nos deux threads
int	threadMain( void *data )
{
    const char * threadName = (const char *) data;
    while	( ! mustExit ) {
        printf( "%s\n", threadName );
    }
    printf( "%s terminé !\n", threadName );
    return thrd_success;
}


int	main()
{

    // On crée un premier thread.
    thrd_t	thread1;
	thrd_t	thread2;
	void	*threadName2 = (void *) "Mon second thread";
    void	*threadName1 = (void *) "Mon premier thread";
	char	ch;
	bool	mustExit = true;

    if ( thrd_create( &thread1, threadMain, threadName1 ) != thrd_success )
	{
        fprintf( stderr, "Impossible de créer le premier thread\n" );
        return EXIT_FAILURE;
    }

    // On crée un second thread.

    if	(thrd_create( &thread2, threadMain, threadName2 ) != thrd_success)
	{
        fprintf( stderr, "Impossible de créer le second thread\n" );
        return EXIT_FAILURE;
    }

    // Appuyez sur la touche <ENTER> pour stopper le programme.
    scanf( "%c", &ch );

    // Tous les threads d'un même processus partagent la même zone de mémoire.
    // Les deux threads doivent donc voir cette variable et se suspendrent
    // quand cela sera nécessaire.

    // On sort du programme.
    printf( "Le thread initial/principal s'arrête.\n" );
    return EXIT_SUCCESS;
}
