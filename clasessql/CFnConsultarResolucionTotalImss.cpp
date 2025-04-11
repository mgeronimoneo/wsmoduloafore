#include "CFnConsultarResolucionTotalImss.hpp"
CFnConsultarResolucionTotalImss::CFnConsultarResolucionTotalImss(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols = 22;
    odbcRet=TRUE;

    nSqlTipo[0] = SQL_INTEGER;
    nSqlTipo[1] = SQL_INTEGER;
    nSqlTipo[2] = SQL_CHAR;
    nSqlTipo[3] = SQL_CHAR;
    nSqlTipo[4] = SQL_CHAR;
    nSqlTipo[5] = SQL_INTEGER;
    nSqlTipo[6] = SQL_CHAR; //Nuevos
    nSqlTipo[7] = SQL_INTEGER;
    nSqlTipo[8] = SQL_CHAR;
    nSqlTipo[9] = SQL_CHAR;
    nSqlTipo[10] = SQL_CHAR;
    nSqlTipo[11] = SQL_INTEGER; //Nuevos
    nSqlTipo[12] = SQL_INTEGER; //Nuevos
    nSqlTipo[13] = SQL_INTEGER;
    nSqlTipo[14] = SQL_CHAR;
    nSqlTipo[15] = SQL_CHAR; //Nuevos
    nSqlTipo[16] = SQL_CHAR;
    nSqlTipo[17] = SQL_CHAR;
    nSqlTipo[18] = SQL_CHAR;
    nSqlTipo[19] = SQL_CHAR;
    nSqlTipo[20] = SQL_CHAR; //Nuevos
    nSqlTipo[21] = SQL_INTEGER;


    nCTipo[0] = SQL_C_SLONG;
    nCTipo[1] = SQL_C_SLONG;
    nCTipo[2] = SQL_C_CHAR;
    nCTipo[3] = SQL_C_CHAR;
    nCTipo[4] = SQL_C_CHAR;
    nCTipo[5] = SQL_C_SLONG;
    nCTipo[6] = SQL_C_CHAR; //Nuevos
    nCTipo[7] = SQL_C_SLONG;
    nCTipo[8] = SQL_C_CHAR;
    nCTipo[9] = SQL_C_CHAR;
    nCTipo[10] = SQL_C_CHAR;
    nCTipo[11] = SQL_C_SLONG; //Nuevos
    nCTipo[12] = SQL_C_SLONG; //Nuevos
    nCTipo[13] = SQL_C_SLONG;
    nCTipo[14] = SQL_C_CHAR;
    nCTipo[15] = SQL_C_CHAR; //Nuevos
    nCTipo[16] = SQL_C_CHAR;
    nCTipo[17] = SQL_C_CHAR;
    nCTipo[18] = SQL_C_CHAR;
    nCTipo[19] = SQL_C_CHAR;
    nCTipo[20] = SQL_C_CHAR; //Nuevos
    nCTipo[21] = SQL_C_SLONG;


    nLongitud[0] = 5;
    nLongitud[1] = 5;
    nLongitud[2] = 4;
    nLongitud[3] = 4;
	nLongitud[4] = 4;
    nLongitud[5] = 5;
	nLongitud[6] = 52; //Nuevos
    nLongitud[7] = 5;
    nLongitud[8] = 12;
	nLongitud[9] = 12;
    nLongitud[10] = 12;
    nLongitud[11] = 5; //Nuevos
    nLongitud[12] = 5; //Nuevos
    nLongitud[13] = 5;
    nLongitud[14] = 12;
    nLongitud[15] = 52; //Nuevos
	nLongitud[16] = 52;
    nLongitud[17] = 52;
    nLongitud[18] = 102;
    nLongitud[19] = 52;
    nLongitud[20] = 102; //Nuevos
    nLongitud[21] = 5;

    pVar[0] =   &iExisteResolucion;
    pVar[1] =   &iExisteAfiliado;
    pVar[2] =   cRegimen;
    pVar[3] =   cTipoSeguro;
    pVar[4] =   cTipoPension;
    pVar[5] =   &iTipoPrestacion;
    pVar[6] =   cNombreDatamart; //Nuevos
    pVar[7] =   &iFolio;
    pVar[8] =   cSecPension;
    pVar[9] =   dFechaIni;
    pVar[10] =   dFechaPago;
    pVar[11] =   &iSemanasCotizadas; //Nuevos
    pVar[12] =   &iDiagDatamart; //Nuevos
    pVar[13] =   &iEstadoSubViv;
    pVar[14] =   dFechaResolucion;
    pVar[15] =   cDescripRegimen; //Nuevos
    pVar[16] =   cDescripTPension;
    pVar[17] =   cDescripTPresta;
    pVar[18] =   cDescripTRetiro;
    pVar[19] =   cDescripTSeguro;
    pVar[20] =   cDescripEdoSubViv; //Nuevos
    pVar[21] =   &iIdMovimiento;

    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }
}

CFnConsultarResolucionTotalImss::~CFnConsultarResolucionTotalImss()
{
}

void CFnConsultarResolucionTotalImss::activarCols()
{
    int i;
    for (i = 0; i < nCols; i++)
    {
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }
}

