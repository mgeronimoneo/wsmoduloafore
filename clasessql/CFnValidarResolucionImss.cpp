#include "CFnValidarResolucionImss.hpp"
CFnValidarResolucionImss::CFnValidarResolucionImss(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=6;
    odbcRet=TRUE;
    nSqlTipo[0] = SQL_CHAR;
    nSqlTipo[1] = SQL_CHAR;
    nSqlTipo[2] = SQL_CHAR;
    nSqlTipo[3] = SQL_INTEGER;
    nSqlTipo[4] = SQL_CHAR;
    nSqlTipo[5] = SQL_INTEGER;
   
    nCTipo[0] = SQL_C_CHAR;
    nCTipo[1] = SQL_C_CHAR;
    nCTipo[2] = SQL_C_CHAR;
    nCTipo[3] = SQL_C_SLONG;
    nCTipo[4] = SQL_C_CHAR;
    nCTipo[5] = SQL_C_SLONG;
 
    nLongitud[0] = 12;
    nLongitud[1] = 12;
    nLongitud[2] = 12;
    nLongitud[3] = 5;
    nLongitud[4] = 52;
    nLongitud[5] = 5;
    pVar[0] = fecha_vigencia;
    pVar[1] = fecha_vigencia_ant;
    pVar[2] = fecha_proxima_sol;
    pVar[3] = &retorno;
    pVar[4] = mensaje;
    pVar[5] = &diagnosticoProcesar;
                                                                  
    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }
}
    
CFnValidarResolucionImss::~CFnValidarResolucionImss()
{
}
    
void CFnValidarResolucionImss::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)                                                              
    {                                                              
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }                                                              
}
 
