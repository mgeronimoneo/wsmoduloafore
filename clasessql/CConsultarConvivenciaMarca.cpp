#include "CConsultarConvivenciaMarca.hpp"
CConsultarConvivenciaMarca::CConsultarConvivenciaMarca(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=3;
    odbcRet=TRUE;
    nSqlTipo[0] = SQL_INTEGER;
    nSqlTipo[1] = SQL_CHAR;
    nSqlTipo[2] = SQL_INTEGER;
   
    nCTipo[0] = SQL_C_SLONG;
    nCTipo[1] = SQL_C_CHAR;
    nCTipo[2] = SQL_C_SLONG;
 
    nLongitud[0] = 5;
    nLongitud[1] = 202;
    nLongitud[2] = 5;
    pVar[0] = &codigo;
    pVar[1] = &descripcion;
    pVar[2] = &tienemarca;
                                                                  
    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }                                                              
}
    
CConsultarConvivenciaMarca::~CConsultarConvivenciaMarca()
{
}
    
void CConsultarConvivenciaMarca::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)                                                              
    {                                                              
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }                                                              
}
 
