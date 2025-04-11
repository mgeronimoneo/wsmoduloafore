#include "CCampoTexto.hpp"
CCampoTexto::CCampoTexto(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=1;
    odbcRet=TRUE;
    flagInsertar = 0;
    nSqlTipo[0] = SQL_CHAR;
   
    nCTipo[0] = SQL_C_CHAR;
 
    nLongitud[0] = 102;
    pVar[0] = texto;
                                                                  
    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }                                                              
}
    
    
CCampoTexto::~CCampoTexto()
{
    Commit();
}
    
void CCampoTexto::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)                                                              
    {                                                              
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }                                                              
    flagInsertar=1;
}
 
BOOL CCampoTexto::prepararInsert()
{
BOOL retorno = FALSE;
    retorno=prepararInsert("");
    return (retorno);
}
BOOL CCampoTexto::prepararInsert(const char *nombreTabla)
{
BOOL retorno = FALSE;
int i;
char sqlTxtInsert[500];
                                                                  
    if (flagInsertar==0) activarCols();
  		sprintf( sqlTxtInsert, "INSERT INTO %s (texto) VALUES (?)",nombreTabla);
    retorno = CRecordSet::PrepararInsert(sqlTxtInsert);
    for (i=0; i<nCols; i++)                                                              
    {                                                              
        ActivarInsert(i, nCTipo[i], nSqlTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }                                                              
    return (retorno);                                                              
}
