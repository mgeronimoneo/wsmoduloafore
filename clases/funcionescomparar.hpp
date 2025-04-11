#ifndef FUNCIONESCOMPARAR_HPP_
#define FUNCIONESCOMPARAR_HPP_

#include <stdio.h>
#include <string.h>
#include "../definiciones.hpp"
#include "../estructuras.hpp"

int	compararNssMaestroAfiliado(const void *nssBuscar, const void *nssMuestra);
int compararNssEnSaldoCuenta(const void *nssBuscar, const void *nssMuestra);
int compararNssCtaRegimen(const void *nssBuscar, const void *nssMuestra);
int compararNssCtaSaldoVol(const void *nssBuscar, const void *nssMuestra);
#endif /* FUNCIONESCOMPARAR_HPP_ */
