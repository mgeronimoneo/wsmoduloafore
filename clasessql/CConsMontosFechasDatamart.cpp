#include "CConsMontosFechasDatamart.hpp"
CConsMontosFechasDatamart::CConsMontosFechasDatamart(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=7;
    odbcRet=TRUE;

    nSqlTipo[0] = SQL_CHAR;
    nSqlTipo[1] = SQL_DOUBLE;
    nSqlTipo[2] = SQL_DOUBLE;
    nSqlTipo[3] = SQL_CHAR;
    nSqlTipo[4] = SQL_CHAR;
    nSqlTipo[5] = SQL_DOUBLE;
    nSqlTipo[6] = SQL_DOUBLE;

    nCTipo[0] = SQL_C_CHAR;
    nCTipo[1] = SQL_C_DOUBLE;
    nCTipo[2] = SQL_C_DOUBLE;
    nCTipo[3] = SQL_C_CHAR;
    nCTipo[4] = SQL_C_CHAR;
    nCTipo[5] = SQL_C_DOUBLE;
    nCTipo[6] = SQL_C_DOUBLE;

    nLongitud[0] = 13;
    nLongitud[1] = 9;
    nLongitud[2] = 9;
    nLongitud[3] = 12;
    nLongitud[4] = 12;
    nLongitud[5] = 9;
    nLongitud[6] = 9;

    pVar[0] =   nss;
    pVar[1] =   &salariobasea;
    pVar[2] =   &salariobaseb;
    pVar[3] =   fechamatdes;
    pVar[4] =   fechainivigencia;
    pVar[5] =   &ultimasbc;
    pVar[6] =   &ultimo250semanassbc;

    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }
}

CConsMontosFechasDatamart::~CConsMontosFechasDatamart()
{
}

void CConsMontosFechasDatamart::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)
    {
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }
}

