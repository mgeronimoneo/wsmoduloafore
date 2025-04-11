#include "CFnAportaciones.hpp"
CFnAportaciones::CFnAportaciones(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=1;
    odbcRet=TRUE;

    nSqlTipo[0] = SQL_FLOAT;

    nCTipo[0] = SQL_C_DOUBLE;

    nLongitud[0] = 5;

    pVar[0] =   &iAportaciones;


    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }
}

CFnAportaciones::~CFnAportaciones()
{
}

void CFnAportaciones::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)
    {
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }
}

