#include "CFnConsultarResolucion.hpp"
CFnConsultarResolucion::CFnConsultarResolucion(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=4;
    odbcRet=TRUE;

    nSqlTipo[0] = SQL_CHAR;
    nSqlTipo[1] = SQL_CHAR;
    nSqlTipo[2] = SQL_CHAR;
    nSqlTipo[3] = SQL_INTEGER;


    nCTipo[0] = SQL_C_CHAR;
    nCTipo[1] = SQL_C_CHAR;
    nCTipo[2] = SQL_C_CHAR;
    nCTipo[3] = SQL_C_SLONG;

    nLongitud[0] = 13;
    nLongitud[1] = 12;
    nLongitud[2] = 12;
    nLongitud[3] = 5;

    pVar[0] =   nss;
    pVar[1] =   fechainiciovigencia;
    pVar[2] =   fechafinvigencia;
    pVar[3] =   &resolusion;

    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }
}

CFnConsultarResolucion::~CFnConsultarResolucion()
{
}

void CFnConsultarResolucion::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)
    {
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }
}

