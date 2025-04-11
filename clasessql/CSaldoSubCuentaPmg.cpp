#include "CSaldoSubCuentaPmg.hpp"
CSaldoSubCuentaPmg::CSaldoSubCuentaPmg(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=4;
    odbcRet=TRUE;

    nSqlTipo[0] = SQL_DOUBLE;
    nSqlTipo[1] = SQL_DOUBLE;
    nSqlTipo[2] = SQL_DOUBLE;
    nSqlTipo[3] = SQL_C_SLONG;

    nCTipo[0] = SQL_C_DOUBLE;
    nCTipo[1] = SQL_C_DOUBLE;
    nCTipo[2] = SQL_C_DOUBLE;
    nCTipo[3] = SQL_C_SLONG;

    nLongitud[0] = 9;
    nLongitud[1] = 9;
    nLongitud[2] = 9;
    nLongitud[3] = 4;

    pVar[0] =   &dpRetroactivo;
    pVar[1] =   &dpDevengado;
    pVar[2] =   &dpSaldoDia;
    pVar[3] =   &iSuficiencia;

    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }
}

CSaldoSubCuentaPmg::~CSaldoSubCuentaPmg()
{
}

void CSaldoSubCuentaPmg::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)
    {
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }
}

