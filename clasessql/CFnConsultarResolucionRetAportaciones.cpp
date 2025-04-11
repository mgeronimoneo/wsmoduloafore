#include "CFnConsultarResolucionRetAportaciones.hpp"
CFnConsultarResolucionRetAportaciones::CFnConsultarResolucionRetAportaciones(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc            = odbc_ext;
    nCols           = 2;
    odbcRet         = TRUE;

    nSqlTipo[0]     = SQL_INTEGER;
    nSqlTipo[1]     = SQL_CHAR;

    nCTipo[0]       = SQL_C_SLONG;
    nCTipo[1]       = SQL_C_CHAR;

    nLongitud[0]    = 5;
    nLongitud[1]    = 6;

    pVar[0]         = &iOpcion;
    pVar[1]         = tTipo_Pension;

    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }
}

CFnConsultarResolucionRetAportaciones::~CFnConsultarResolucionRetAportaciones()
{
}

void CFnConsultarResolucionRetAportaciones::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)
    {
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }
}

