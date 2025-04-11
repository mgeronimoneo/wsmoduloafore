#ifndef DEFINICIONES_HPP_
#define DEFINICIONES_HPP_

#define __FILE_MIN__ (strrchr(__FILE__,'/') ? strrchr(__FILE__,'/')+1 : __FILE__ )

#define SIZE_BUFF_DAT				33
#define RUTA_LOGX					"/sysx/progs/log/wsmoduloafore"
#define IP_ADMONAFO_DAT				"/sysx/progs/afore/ipadmonafore.dat"
#define BD_ADMON_AFORE				"admonafore"
#define USR_BD_SYSADMONAFORE		"sysadmonafore"

#define IP_SERV_AFO_DAT				"/sysx/progs/afore/ipserviciosafore.dat"
#define BD_SERV_AFORE				"serviciosafore"
#define USR_BD_SYSSERVAFO			"sysserviciosafore"

#define BD_AFOREGLOBAL				"aforeglobal"
#define USR_BD_AFOREGLOBAL			"sysaforeglobal"

#define BD_SAFRE_AF					"safre_af"
#define USR_BD_SAFRE_AF				"syscoppel"
#define PWD_BD_SAFRE_AF				"545f78b743"

#define ID_CTRL_MAE_AFIL			12905
#define ID_SALDO_CUENTA_DIARIO		18001
#define ID_CTA_REGIMEN				12911
#define ID_CTA_SALDO_VOL			18005

#define SIZE_NSS					11
#define SIZE_DECIMAL_				50
#define SIZE_FECHA					12

#define MSJ_DEFAULT__				0
#define MSJ_OK__					1
#define DEFAULT__					0
#define OK__						1
#define ERR_EXEC_SQL				-103
#define ERR_CNX_BASE_DATO			-104
#define ERR_NO_HAY_REG_BD			-105
#define ERR_LEER_REG_BD				-106

#define SERV_DATABASE				2
#define SERV_GLOBAL					3

#define ERR__						-1
#define ERR_RESERVAR_MEM			-100
#define	ERR_LEER_ARCHIVO_CNF		-102
#define ERR_EXEC_SQL				-103
#define ERR_CNX_BASE_DATO			-104
#define ERR_NO_HAY_REG_BD			-105
#define ERR_LEER_REG_BD				-106
#define ERR_EXEC_WEBSERVICE			-107

//Macro definiciones para shm
#define ERR_CONSULTAR_IDSHM			-6
#define ERR_ACCESAR_SHM				-7
#define ERR_NO_HAY_REG_SHM			-8

#define LONG_DESCRIPCION			256


#define CTRL_SVR_INFORMIX			1
#define CVE_RET_DESEMPLEO_IMSS		1024
#define CVE_RET_MATRIMONIO_IMSS		1025

#endif
