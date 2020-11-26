

#include "tiempos.h"
#include "permutaciones.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

/*****************************************************/
/* Función :craneo_medio_ordenacion Fecha:11/10/2020 */
/*****************************************************/

short tiempo_medio_ordenacion(pfunc_ordena metodo, int n_perms, int N, PTIEMPO ptiempo)
{
  int **tabla = NULL;
  int i, min = INT_MAX, max = 0, obs = 0;
  double suma = 0, t = 0;
  clock_t c1, c2;

  assert(metodo != NULL && n_perms > 0 && N > 0 && ptiempo != NULL);

  tabla = genera_permutaciones(n_perms, N);
  if (tabla == NULL)
    return ERR;

  ptiempo->N = N;
  ptiempo->n_elems = n_perms;

  for (i = 0; i < n_perms; i++)
  {
    c1 = clock();
    if (c1 == (clock_t)-1)
      break;

    obs = metodo(tabla[i], 0, N - 1);
    if (obs == ERR)
      break;
    c2 = clock();
    if (c2 == (clock_t)-1)
      break;

    t += (double)(c2 - c1) * (1000000) / CLOCKS_PER_SEC;
    suma += obs;

    if (max < obs)
      max = obs;
    if (min > obs)
      min = obs;
  }
  ptiempo->min_ob = min;
  ptiempo->max_ob = max;

  ptiempo->medio_ob = (double)(suma / n_perms);
  ptiempo->tiempo = (double)(t / n_perms);
  for (i = 0; i < n_perms; i++)
  {
    free(tabla[i]);
  }
  free(tabla);
  return OK;
}

/********************************************************/
/* Funcion: genera_tiempos_ordenacion Fecha: 11/13/2020 */
/********************************************************/

short genera_tiempos_ordenacion(pfunc_ordena metodo, char *fichero, int num_min, int num_max, int incr, int n_perms)
{
  PTIEMPO array;
  int difperms, i, j;
  short flag = OK;

  assert(num_min >= 0 && num_max >= 0 && num_min < num_max && n_perms > 0 && metodo != NULL && fichero != NULL && incr > 0);

  difperms = ((num_max - num_min) / incr + 1);
  array = (PTIEMPO)malloc(difperms * sizeof(array[0]));
  if (array == NULL)
    return ERR;
  for (j = num_min, i = 0; j <= num_max && i < difperms && flag == OK; i++, j += incr)
  {
    flag = tiempo_medio_ordenacion(metodo, n_perms, j, &array[i]);
  }
  if (flag == ERR)
  {
    free(array);
    return ERR;
  }
  flag = guarda_tabla_tiempos(fichero, array, difperms);
  if (flag == ERR)
  {
    free(array);
    return ERR;
  }
  free(array);
  return OK;
}

/***************************************************/
/* Funcion: guarda_tabla_tiempos Fecha: 11/10/2020 */
/***************************************************/

short guarda_tabla_tiempos(char *fichero, PTIEMPO tiempo, int n_tiempos)
{
  FILE *pf = NULL;
  int i = 0;
  short flag = OK;

  assert(tiempo != NULL && n_tiempos > 0 && fichero != NULL);

  pf = fopen(fichero, "w");
  if (!pf)
    return ERR;

  for (i = 0; i < n_tiempos && flag >= 0; i++)
  {
    flag = fprintf(pf, "%d  %f %f %d %d\n", tiempo[i].N, tiempo[i].tiempo, tiempo[i].medio_ob, tiempo[i].min_ob, tiempo[i].max_ob);
  }
  if (flag < 0)
  {
    fclose(pf);
    return ERR;
  }
  if (fclose(pf) != EOF)
    return flag;
  else
  {
    return ERR;
  }
}