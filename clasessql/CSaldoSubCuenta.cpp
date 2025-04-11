#include "CSaldoSubCuenta.hpp"
CSaldoSubCuenta::CSaldoSubCuenta(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=4;
    odbcRet=TRUE;

    nSqlTipo[0] = SQL_SMALLINT;
    nSqlTipo[1] = SQL_SMALLINT;
    nSqlTipo[2] = SQL_DOUBLE;
    nSqlTipo[3] = SQL_DOUBLE;

    nCTipo[0] = SQL_C_SSHORT;
    nCTipo[1] = SQL_C_SSHORT;
    nCTipo[2] = SQL_C_DOUBLE;
    nCTipo[3] = SQL_C_DOUBLE;

    nLongitud[0] = 3;
    nLongitud[1] = 3;
    nLongitud[2] = 9;
    nLongitud[3] = 9;

    pVar[0] =   &subCuenta;
    pVar[1] =   &siefore;
    pVar[2] =   &montoEnAcciones;
    pVar[3] =   &montoEnPesos;

    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }
}

CSaldoSubCuenta::~CSaldoSubCuenta()
{
}

void CSaldoSubCuenta::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)
    {
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }
}

