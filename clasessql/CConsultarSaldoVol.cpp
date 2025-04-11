#include "CConsultarSaldoVol.hpp"
CConsultarSaldoVol::CConsultarSaldoVol(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=12;
    odbcRet=TRUE;

    nSqlTipo[0] = SQL_INTEGER;
    nSqlTipo[1] = SQL_INTEGER;
    nSqlTipo[2] = SQL_CHAR;
	nSqlTipo[3] = SQL_SMALLINT;
	nSqlTipo[4] = SQL_SMALLINT;
    nSqlTipo[5] = SQL_CHAR;  
    nSqlTipo[6] = SQL_CHAR;
    nSqlTipo[7] = SQL_DOUBLE;
	nSqlTipo[8] = SQL_DOUBLE;
	nSqlTipo[9] = SQL_DOUBLE;
	nSqlTipo[10] = SQL_CHAR;  
    nSqlTipo[11] = SQL_CHAR;	

	
	
	
    nCTipo[0] = SQL_C_SLONG;
    nCTipo[1]  = SQL_C_SLONG;
    nCTipo[2]  = SQL_C_CHAR;
	nCTipo[3]  = SQL_C_SSHORT;
    nCTipo[4]  = SQL_C_SSHORT;
    nCTipo[5]  = SQL_C_CHAR;
	nCTipo[6]  = SQL_C_CHAR;
	nCTipo[7]  = SQL_C_DOUBLE;
    nCTipo[8]  = SQL_C_DOUBLE;
    nCTipo[9]  = SQL_C_DOUBLE;
    nCTipo[10] = SQL_C_CHAR;
	nCTipo[11] = SQL_C_CHAR;	
	
	
	
	
    nLongitud[0]  = 5;
    nLongitud[1]  = 5;
    nLongitud[2]  = 13;
    nLongitud[3]  = 3;
    nLongitud[4]  = 3;
    nLongitud[5]  = 12;
    nLongitud[6]  = 12;
    nLongitud[7]  = 9;
    nLongitud[8]  = 9;
    nLongitud[9]  = 9;
	nLongitud[10] = 12;
	nLongitud[11] = 14;
 
 
 
    pVar[0]  =   &folio;
    pVar[1]  =   &consecutivoLote;
    pVar[2]  =   nss;
    pVar[3]  =   &siefore;
    pVar[4]  =   &subCuenta;
    pVar[5]  =   fechaValor;
    pVar[6]  =   fechaConversion;
    pVar[7]  =   &montoEnPesos;
    pVar[8]  =   &montoEnAcciones;
    pVar[9]  =   &saldoAcciones;
    pVar[10] =   fechaSaldo;
    pVar[11] =   usuario;

 
    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }
}

CConsultarSaldoVol::~CConsultarSaldoVol()
{
}

void CConsultarSaldoVol::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)
    {
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }
}

