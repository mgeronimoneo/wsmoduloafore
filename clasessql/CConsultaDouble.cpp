#include "CConsultaDouble.hpp"
CConsultaDouble::CConsultaDouble(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=1;
    odbcRet=TRUE;
    nSqlTipo[0] = SQL_DOUBLE;
   
    nCTipo[0] = SQL_C_DOUBLE;
 
    nLongitud[0] = 9;
    pVar[0] = &campo_double;
                                                                  
    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }                                                              
}
    
    
CConsultaDouble::~CConsultaDouble()
{
}
    
void CConsultaDouble::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)                                                              
    {                                                              
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }                                                              
}
 
