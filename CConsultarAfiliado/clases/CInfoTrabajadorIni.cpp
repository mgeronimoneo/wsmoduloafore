#include "CInfoTrabajadorIni.hpp"
CInfoTrabajadorIni::CInfoTrabajadorIni(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=3;
    odbcRet=TRUE;
    nSqlTipo[0] = SQL_CHAR;
    nSqlTipo[1] = SQL_CHAR;
    nSqlTipo[2] = SQL_CHAR;
   
    nCTipo[0] = SQL_C_CHAR;
    nCTipo[1] = SQL_C_CHAR;
    nCTipo[2] = SQL_C_CHAR;
 
    nLongitud[0] = 20;
    nLongitud[1] = 13;
    nLongitud[2] = 12;
    pVar[0] = curp;
    pVar[1] = nss;
    pVar[2] = fechaservicio;
                                                                  
    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }                                                              
}


    
CInfoTrabajadorIni::~CInfoTrabajadorIni()
{
}
    
void CInfoTrabajadorIni::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)                                                              
    {                                                              
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }                                                              
}
 
