#include "CFnConsultarComplementoResolucion.hpp"
CFnConsultarComplementoResolucion::CFnConsultarComplementoResolucion(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=7;
    odbcRet=TRUE;

    nSqlTipo[0] = SQL_INTEGER;
    nSqlTipo[1] = SQL_INTEGER;
    nSqlTipo[2] = SQL_DOUBLE;
    nSqlTipo[3] = SQL_DOUBLE;
    nSqlTipo[4] = SQL_DOUBLE;
    nSqlTipo[5] = SQL_DOUBLE;
    nSqlTipo[6] = SQL_SMALLINT;

    nCTipo[0] = SQL_C_SLONG;
    nCTipo[1] = SQL_C_SLONG;
    nCTipo[2] = SQL_C_DOUBLE;
    nCTipo[3] = SQL_C_DOUBLE;
    nCTipo[4] = SQL_C_DOUBLE;
    nCTipo[5] = SQL_C_DOUBLE;
    nCTipo[6] = SQL_C_SSHORT;

    nLongitud[0] = 5;
    nLongitud[1] = 5;
    nLongitud[2] = 9;
    nLongitud[3] = 9;
    nLongitud[4] = 9;
    nLongitud[5] = 9;
    nLongitud[6] = 3;

    pVar[0] =   &numeroResolucion;
    pVar[1] =   &idcomplemento;
    pVar[2] =   &montopagado;
    pVar[3] =   &montopagadocomplemento;
    pVar[4] =   &saldorcv;
    pVar[5] =   &saldorcvanterior;
    pVar[6] =   &respuestaotrodatamart;


    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }
}

CFnConsultarComplementoResolucion::~CFnConsultarComplementoResolucion()
{
}

void CFnConsultarComplementoResolucion::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)
    {
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }
}

