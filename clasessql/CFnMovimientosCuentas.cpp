#include "CFnMovimientosCuentas.hpp"
CFnMovimientosCuentas::CFnMovimientosCuentas(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=7;
    odbcRet=TRUE;

    nSqlTipo[0] = SQL_INTEGER;

    nCTipo[0] = SQL_C_SLONG;

    nLongitud[0] = 5;

    pVar[0] =   &iMovimientos;


    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }
}

CFnMovimientosCuentas::~CFnMovimientosCuentas()
{
}

void CFnMovimientosCuentas::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)
    {
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }
}

