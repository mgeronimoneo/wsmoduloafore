#include "CConsultarSiefore.hpp"
CConsultarSiefore::CConsultarSiefore(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=5;
    odbcRet=TRUE;

    nSqlTipo[0] = SQL_CHAR;
    nSqlTipo[1] = SQL_SMALLINT;
    nSqlTipo[2] = SQL_SMALLINT;

    nCTipo[0] = SQL_C_CHAR;
    nCTipo[1] = SQL_C_SSHORT;
    nCTipo[2] = SQL_C_SSHORT;


    nLongitud[0] = 12;
    nLongitud[1] = 3;
    nLongitud[2] = 3;


    pVar[0] =   &nss;//&solo los datos numericos
    pVar[1] =   &subCuenta;
    pVar[2] =   &siefore;
 

    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }
}

CConsultarSiefore::~CConsultarSiefore()
{
}

void CConsultarSiefore::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)
    {
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }
}

