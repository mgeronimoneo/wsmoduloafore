#include "CConsultarRetroactivoPmg.hpp"
CConsultarRetroactivoPmg::CConsultarRetroactivoPmg(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=5;
    odbcRet=TRUE;

    nSqlTipo[0] = SQL_DOUBLE;
    nSqlTipo[1] = SQL_DOUBLE;
    nSqlTipo[2] = SQL_C_SLONG;
    nSqlTipo[3] = SQL_C_SLONG;

    nCTipo[0] = SQL_C_DOUBLE;
    nCTipo[1] = SQL_C_DOUBLE;
    nCTipo[2] = SQL_C_SLONG;
    nCTipo[3] = SQL_C_SLONG;

    nLongitud[0] = 9;
    nLongitud[1] = 9;
    nLongitud[2] = 4;
    nLongitud[3] = 4;

    pVar[0] = &dRetroactivo;
    pVar[1] = &dDevengado;
    pVar[2] = &iEstatus;
    pVar[3] = &iRegistro;

    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }
}

CConsultarRetroactivoPmg::~CConsultarRetroactivoPmg()
{
}

void CConsultarRetroactivoPmg::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)
    {
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }
}

