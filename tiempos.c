

#include "tiempos.h"
#include "permutaciones.h"
#include "busqueda.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

/*****************************************************/
/* Función :craneo_medio_ordenacion Fecha:11/10/2020 */
/*****************************************************/

short tiempo_medio_busqueda(pfunc_busqueda metodo, pfunc_generador_claves generador, int orden, int N, int n_veces, PTIEMPO ptiempo)
{
  int *perm = NULL;
  int i, j, min = INT_MAX, max = 0, obs = 0;
  double suma = 0, t = 0;
  clock_t c1, c2;
  PDICC pd;
  int *tabla = NULL;
  int ppos;

  assert(metodo != NULL && n_veces > 0 && N > 0 && ptiempo != NULL);

  pd = ini_diccionario(N, orden);

  perm = genera_perm(N);
  if (perm == NULL)
    return ERR;

  insercion_masiva_diccionario(pd, perm, N);

  tabla = (int *)malloc(N * n_veces * sizeof(tabla[0]));

  if (tabla == NULL)
    return ERR;

  generador(tabla, N * n_veces, N);

  ptiempo->N = N;
  ptiempo->n_elems = n_veces;

  for (i = 0; i < N * n_veces; i++)
  {

    c1 = clock();
    if (c1 == (clock_t)-1)
      break;

    obs = metodo(pd->tabla, 0, N - 1, tabla[i], &ppos);
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

  ptiempo->medio_ob = (double)(suma / (N * n_veces));
  ptiempo->tiempo = (double)(t / (N * n_veces));
  free(tabla);
  free(perm);
  libera_diccionario(pd);
  return OK;
}

/********************************************************/
/* Funcion: genera_tiempos_ordenacion Fecha: 11/13/2020 */
/*********************************************************/

short genera_tiempos_busqueda(pfunc_busqueda metodo, pfunc_generador_claves generador, int orden, char *fichero, int num_min, int num_max, int incr, int n_veces)
{

  PTIEMPO array;
  int difperms, i, j;
  short flag = OK;

  assert(num_min >= 0 && num_max >= 0 && num_min < num_max && n_veces > 0 && metodo != NULL && fichero != NULL && incr > 0);

  difperms = ((num_max - num_min) / incr + 1);
  array = (PTIEMPO)malloc(difperms * sizeof(array[0]));
  if (array == NULL)
    return ERR;
  for (j = num_min, i = 0; j <= num_max && i < difperms && flag == OK; i++, j += incr)
  {
    flag = tiempo_medio_busqueda(metodo, generador, orden, j, n_veces, &array[i]);
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