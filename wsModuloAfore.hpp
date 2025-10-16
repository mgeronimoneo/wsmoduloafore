/*
 * wsModuloAfore.hpp
 *
 *  Created on: 22/04/2015
 *      Author: carlos
 */

#ifndef WSMODULOAFORE_HPP_
#define WSMODULOAFORE_HPP_

#include "gSoapServer/wsModuloAfore.nsmap"
#include "gSoapServer/soapH.h"

#include "includeStd.hpp"
#include "CUtileriasAfo.hpp"
#include "definiciones.hpp"
#include "estructuras.hpp"

#include "CConsultarAfiliado/CConsultarAfiliado.h"
#include "CSaldosPorSubCuenta/CSaldosPorSubCuenta.hpp"
#include "CFoliadorServicios/CFoliadorServicios.hpp"
#include "CValidarConvivenciaMarca/CValidarConvivenciaMarca.hpp"
#include "CSaldoDiarioRetParcial/CSaldoDiarioRetParcial.hpp"
#include "CValidarResolusionImss/CValidarResolusionImss.hpp"
#include "CMontosFechasDatamart/CMontosFechasDatamart.hpp"
#include "CConsultarResolusion/CConsultarResolusion.hpp"
#include "CComplementoResolucion/CComplementoResolucion.hpp"
#include "CConsultarResolucionTotalImss/CConsultarResolucionTotalImss.hpp"
#include "COperacionesPmg/COperacionesPmg.hpp"
#include "CMovimientosCuentas/CMovimientosCuentas.hpp"
#include "CConsultarResolucionTotalIssste/CConsultarResolucionTotalIssste.hpp"
#include "CSaldosPorSubCuentaPmg/CSaldosPorSubCuentaPmg.hpp"
#include "CAportaciones/CAportaciones.hpp"
#include "CConsultarCtaRegimen/CConsultarCtaRegimen.hpp"
#include "CConsultarCtaSaldoVol/CConsultarCtaSaldoVol.hpp"
#include "CConsultaSaldoVivienda/CConsultaSaldoVivienda.hpp"
#include "CConsultaAportacionesPosteriores/CConsultaAportacionesPosteriores.hpp"
#include "CConsultarResolusionRetAportaciones/CConsultarResolusionRetAportaciones.hpp"


void logSoap();
void obtenerIpRemoto(char *cOutIp);
int is_file_accessible(const char *filename);
int is_path_allowed(const char *path);

struct soap soap;
char	cIpCliente[17],
		cTexto[1024];

#endif /* WSMODULOAFORE_HPP_ */
