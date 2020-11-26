/**
 *
 * Descripcion: Implementacion de funciones de generacion de permutaciones
 *
 * Fichero: permutaciones.c
 * Autor: Carlos Aguirre
 * Version: 1.0
 * Fecha: 16-09-2017
 *
 */

#include "permutaciones.h"
#include <stdlib.h>

/* Autores: Raul Diaz, Ignacio Bernardez           */
/*                                                 */
/* Rutina que genera un numero aleatorio           */
/* entre dos numeros dados                         */
/*                                                 */
/* Entrada:                                        */
/* int inf: limite inferior                        */
/* int sup: limite superior                        */

/* Salida:                                         */
/* int: numero aleatorio                           */
/***************************************************/

int aleat_num(int inf, int sup)
{
  return (double)rand() / (double)RAND_MAX * (sup + 1 - inf) + inf;
}

/***************************************************/
/* Funcion: genera_perm Fecha: 11/10/2020          */
/* Autores:                                        */
/*                                                 */
/* Rutina que genera una permutacion               */
/* aleatoria                                       */
/*                                                 */
/* Entrada:                                        */
/* int n: Numero de elementos de la                */
/* permutacion                                     */
/* Salida:                                         */
/* int *: puntero a un array de enter ****************/

int *genera_perm(int N)
{
  int *perm = NULL;
  int i;
  int aux = 0, k = 0;
  if (N < 0)
    return NULL;
  perm = (int *)malloc(N * sizeof(perm[0]));
  if (!perm)
    return NULL;

  for (i = 0; i < N; i++)
  {
    perm[i] = i + 1;
  }
  for (i = 0; i < N; i++)
  {
    aux = perm[i];
    k = aleat_num(i, N - 1);
    perm[i] = perm[k];
    perm[k] = aux;
  }
  return perm;
}

/***************************************************/
/* Funcion: genera_permutaciones Fecha: 11/10/2020 */
/* Autores: Raul Diaz, Ignacio Bernardez           */
/*                                                 */
/* Funcion que genera n_perms permutaciones        */
/* aleatorias de tamanio elementos                 */
/*                                                 */
/* Entrada:                                        */
/* int n_perms: Numero de permutaciones            */
/* int N: Numero de elementos de cada              */
/* permutacion                                     */
/* Salida:                                         */
/* int**: Array de punteros a enteros              */
/* que apuntan a cada una de las                   */
/* permutaciones                                   */
/* NULL en caso de error                           */
/***************************************************/

int **genera_permutaciones(int n_perms, int N)
{
  int i, j;
  int **per = NULL;
  if (n_perms < 0 || N < 0)
    return NULL;
  per = (int **)malloc(n_perms * sizeof(per[0]));
  if (!per)
    return NULL;
  for (i = 0; i < n_perms; i++)
  {
    per[i] = genera_perm(N);
    if (!per[i])
    {
      for (j = 0; j < i; j++)
      {
        free(per[i]);
      }
      free(per);
    }
  }
  return per;
}