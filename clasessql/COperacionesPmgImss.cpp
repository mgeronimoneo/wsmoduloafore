#include "COperacionesPmgImss.hpp"
COperacionesPmgImss::COperacionesPmgImss(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=4;
    odbcRet=TRUE;

    nSqlTipo[0] = SQL_CHAR;
	nSqlTipo[1] = SQL_C_SLONG;
	nSqlTipo[2] = SQL_DOUBLE;

    nCTipo[0] = SQL_CHAR;
	nCTipo[1] = SQL_C_SLONG;
	nCTipo[2] = SQL_C_DOUBLE;

    nLongitud[0] = 1024;
	nLongitud[1] = 4;
	nLongitud[2] = 9;

    pVar[0] =   cMensaje;
	pVar[1] =   &sCodigo;
	pVar[2] =   &dMonto;

    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }
}

COperacionesPmgImss::~COperacionesPmgImss()
{
}

void COperacionesPmgImss::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)
    {
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }
}

