#include "CConsultadeSaldoVivienda.hpp"
CConsultadeSaldoVivienda::CConsultadeSaldoVivienda(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=3;
    odbcRet=TRUE;
	
	nSqlTipo[0]  = SQL_CHAR;
	nSqlTipo[1]  = SQL_DECIMAL;
	nSqlTipo[2]  = SQL_DECIMAL;

	
	
    nCTipo[0]  = SQL_C_CHAR;
    nCTipo[1]  = SQL_C_CHAR;
    nCTipo[2]  = SQL_C_CHAR;
  
	
	
	nLongitud[0]  = 13;	//cNss;
    nLongitud[1]  = 26; //dMontoaccviv;
    nLongitud[2]  = 26; //dMontopesviv;
   
	
	pVar[0] =   cNss;
	pVar[1] =   dMontoaccviv;
	pVar[2] =   dMontopesviv;
	
	
    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }
}

CConsultadeSaldoVivienda::~CConsultadeSaldoVivienda()
{
}

void CConsultadeSaldoVivienda::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)
    {
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }
}

