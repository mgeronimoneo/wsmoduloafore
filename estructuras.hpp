#include <string.h>
#ifndef ESTRUCTURAS_HPP_
#define ESTRUCTURAS_HPP_

#define NUM_SUBCUENTAS            20
#define NUM_SUBCUENTAS_PMG  	  20

#define NUM_CTA_REGIMEN   	      45
#define NUM_CTA_SALDO_VOL         1000

struct SHM_INFOX
{
	int iIdShm,
		iTotalReg;
	SHM_INFOX()
	{
		iIdShm = 0;
		iTotalReg = 0;
	}
};
/*struct SHM_INFOX_PMG
{
	int iIdShm,
		iTotalReg;
	SHM_INFOX_PMG()
	{
		iIdShm = 0;
		iTotalReg = 0;
	}
};*/
struct MAESTRO_AFILIADO
{
	char	cNss[11],
			cCurp[18],
			cRfc[13],
			cNombres[40],
			cPaterno[40],
			cMaterno[40],
			cFechaNac[8],
			cFechaIniCta[8],
			cFechaEntCons[8];
	short	shTipoSol,
			shSexo,
			shEstadoNacimiento;
	long	lFolio;
		MAESTRO_AFILIADO()
		{
			memset(cNss,0, sizeof(cNss));
			memset(cCurp,0, sizeof(cCurp));
			memset(cRfc,0, sizeof(cRfc));
			memset(cNombres,0, sizeof(cNombres));
			memset(cPaterno,0, sizeof(cPaterno));
			memset(cMaterno,0, sizeof(cMaterno));
			memset(cFechaNac,0, sizeof(cFechaNac));
			memset(cFechaIniCta,0, sizeof(cFechaIniCta));
			memset(cFechaEntCons,0, sizeof(cFechaEntCons));
			shTipoSol = 0;
			shSexo = 0;
			shEstadoNacimiento = 0;
			lFolio = 0;
		}
};
struct DATOS_TRABAJADOR_EXP_IDEN
{
	char cApellidoPaterno[40],
		cApellidoMaterno[40],
		cNombre[40],
		cCurp[18],
		cNss[11],
		cFechaServicio[10],
		cFechaNacimiento[10],
		cEntidadNacimiento[2];
	int iSexo;
	DATOS_TRABAJADOR_EXP_IDEN()
	{
		memset(cApellidoPaterno, 0, sizeof(cApellidoPaterno));
		memset(cApellidoMaterno,0, sizeof(cApellidoMaterno));
		memset(cNombre,0, sizeof(cNombre));
		memset(cCurp,0, sizeof(cCurp));
		memset(cNss,0, sizeof(cNss));
		memset(cFechaServicio,0, sizeof(cFechaServicio));
		memset(cFechaNacimiento,0, sizeof(cFechaNacimiento));
		memset(cEntidadNacimiento,0, sizeof(cEntidadNacimiento));
		iSexo = 0;
	}

};
//--- estructuras de las memorias diarias -----
struct  SALDO_SUB_CUENTA
{
	short   shSubCuenta,
			shSiefore;
	double  dPrecioDia,
			dMontoAccion,
			dMontoPesos;
	char    cFechaConv[8]; //--AAAAMMDD
};

struct  SALDO_SUB_CUENTA_PMG
{
	short   shSubCuenta,
			shSiefore;
	double  dPrecioDia,
			dMontoAccion,
			dMontoPesos;
	char    cFechaConv[8]; //--AAAAMMDD
};

struct  SALDO_CUENTA
{
	char    cNss[11];
	double  dSaldoEnAccion;
	SALDO_SUB_CUENTA        arrSubCta[NUM_SUBCUENTAS];
};

struct  SALDO_CUENTA_PMG
{
	char    cNss[11];
	double  dSaldoEnAccion;
	SALDO_SUB_CUENTA_PMG    arrSubCtaPmg[NUM_SUBCUENTAS_PMG];
};

struct  SALDOS_PMG
{
	double	dRetroactivo,
			dDevengado,
			dSaldoDia;
	int 	iSuficiencia,
			iRegistros;
};

struct CONVIVENCIA_MARCAS
{
	short shTieneMarca;
	char  cMensaje[202];
	short shCodigo;
};
struct VAL_RESOLUCION_IMSS
{
	char cFechaVigencia[12],
		 cFechaVigenciaAnterior[12],
		 cFechaProximaSolicitud[12],
		 cMensaje[52];
	int	 iRetorno,
		 iDiagProcesar;
};
struct MONTOS_FECHAS_DATAMART
{
    char 		cNss[13];
    double		dSalarioBaseA,
    			DSalarioBaseB;
    char		cFechaMatDes[12],
    			cFechaIniVigencia[12];
     double		dUltimaSbc,
    			dUltimas250SemanasSbc;
};

//Ignacio Rodelo 16/02/2017
struct  APORTACION_POSTERIORES
{
	double iAportacion;
	APORTACION_POSTERIORES()
	{
		iAportacion =0;
	}
};

struct CONSULTA_RESOLUSION
{
	char cNss[13],
		 cFechaIniVigencia[12],
		 cFechaFinVigencia[12];
	int  iResolucion;
};
struct COMPLEMENTO_RESOLUCION
{
	int iIdComplemento,
		iNumResolucion;
	double	dMontoPagado,
			dMontoPagadoComplemento,
			dSaldoRcv,
			dSaldoRcvAnterior;
	short	shRespOtroDatamart;
	COMPLEMENTO_RESOLUCION()
	{
		iIdComplemento=0;
		iNumResolucion=0;
		dMontoPagado=0;
		dMontoPagadoComplemento=0;
		dSaldoRcv=0;
		dSaldoRcvAnterior=0;
		shRespOtroDatamart=0;
	}
};

struct CONSULTA_RESOLUCION_TOTAL_IMSS
{
	int		iExisteResolucion,
			iExisteAfiliado;
    char	cRegimen[4],
			cTipoSeguro[4],
			cTipoPension[4];
    int		iTipoPrestacion;
	char	cNombreDatamart[52];
	int		iFolio;
    char	cSecPension[12],
			dFechaIni[12],
			dFechaPago[12];
    int		iSemanasCotizadas,
			iDiagDatamart,
			iEstadoSubViv;
    char	dFechaResolucion[12],
			cDescripRegimen[52],
			cDescripTPension[52],
			cDescripTPresta[52],
			cDescripTRetiro[102],
			cDescripTSeguro[52],
			cDescripEdoSubViv[102];
    int		iIdMovimiento;
};

struct  OPERACIONES_PMG_IMSS
{
	char    cMensaje[102];
	int		sCodigo,
			iRespSaldo;
	double	dImporte;
	
};
struct DATOS_DATAMART_ISSSTE
{
	char	cNss	[13],
	cNssissste		[13],
	cCurp			[20],
	cSecpension		[4],
	cNombreDatamart	[42],
	cPaternoDatamart[42],
	cMaternoDatamart[42],			
	cNumconcesion	[11],
	cTiporetiro		[3],
	cRegimen		[4],
	cTiposeguro		[4],
	cTipopension	[4],
	cCvepension		[5],
	cFechainiciopen	[25],
	cFecharesolucion[25];
	short	shDelegacion,
	shTipomovimiento,
	shTipoprestacion,
	shSemanascotizadas,
	shDiagDatamart,
	shEstadoregistro;
	int		iFolio;
	DATOS_DATAMART_ISSSTE()
	{
		memset( cNss,0, 			sizeof(cNss));
		memset( cNssissste,0, 		sizeof(cNssissste));
		memset( cCurp,0, 			sizeof(cCurp));
		memset( cSecpension,0, 		sizeof(cSecpension));
		memset( cNombreDatamart,0, 	sizeof(cNombreDatamart));
		memset( cPaternoDatamart,0, sizeof(cPaternoDatamart));
		memset( cMaternoDatamart,0, sizeof(cMaternoDatamart));		
		memset( cNumconcesion,0, 	sizeof(cNumconcesion));
		memset( cTiporetiro,0, 		sizeof(cTiporetiro));
		memset( cRegimen,0, 		sizeof(cRegimen));
		memset( cTiposeguro,0, 		sizeof(cTiposeguro));
		memset( cTipopension,0, 	sizeof(cTipopension));
		memset( cCvepension,0, 		sizeof(cCvepension));
		memset( cFechainiciopen,0, 	sizeof(cFechainiciopen));
		memset( cFecharesolucion,0, sizeof(cFecharesolucion));
		shDelegacion 		= 0;
		shTipomovimiento 	= 0;
		shTipoprestacion 	= 0;
		shSemanascotizadas 	= 0;
		shDiagDatamart 		= 0;
		shEstadoregistro 	= 0;
		iFolio				= 0;
	}			
};

struct  SIEFORE_SUB_EST
{
	char    cNss[12];
	short	cSubCuenta,
			cSiefore;
};

struct  SIEFORE
{
	char cNss[12];
	SIEFORE_SUB_EST        arrCtaRegimen[NUM_CTA_REGIMEN];
};

struct SALDO_VOL_SUB_EST
{
	int		iFolio,
			iConsecutivoLote;
	char 	cNss[13];
    short	iSiefore,
			iSubCuenta;
	char 	cFechaValor[12],
			cFechaConversion[12];
	double	iMontoEnPesos,
			iMontoEnAcciones,
			iSaldoAcciones;
	char 	cFechaSaldo[12],
			cUsuario[14];
	SALDO_VOL_SUB_EST()
	{
		memset( cNss,0,  			sizeof(cNss));
		memset( cFechaValor,0, 		sizeof(cFechaValor));
		memset( cFechaConversion,0, sizeof(cFechaValor));
		memset( cFechaSaldo,0,  	sizeof(cFechaSaldo));
		memset( cUsuario,0,  		sizeof(cUsuario));
		iFolio=0;
		iConsecutivoLote=0;
		iSiefore=0;
		iSubCuenta=0;
		iMontoEnPesos=0;
		iMontoEnAcciones=0;
		iSaldoAcciones=0;
	}
			
};

struct SALDO_VOL
{	
	char cNss[13];
	char 	cFechaValor[12],
			cFechaConversion[12];
	SALDO_VOL_SUB_EST		*arrCtaSaldoVol;
};

struct CONSULTA_SALDOS_VIVIENDA
{
	
    char	sNss[13],
			dMontoaccviv[26],
			dMontopesviv[26];
};

struct  CONSULTA_APORTACIONES_POSTERIORES
{
	double iAportacion;
	CONSULTA_APORTACIONES_POSTERIORES()
	{
		iAportacion =0;
	}
};

#endif
