#include "CConsultaDataMartIssste.hpp"
CConsultaDataMartIssste::CConsultaDataMartIssste(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=22;
    odbcRet=TRUE;
	
	nSqlTipo[0] = SQL_INTEGER;
	nSqlTipo[1]  = SQL_CHAR;
	nSqlTipo[2]  = SQL_CHAR;
	nSqlTipo[3]  = SQL_CHAR;
	nSqlTipo[4]  = SQL_CHAR;
	nSqlTipo[5]  = SQL_CHAR;
	nSqlTipo[6]  = SQL_CHAR;
	nSqlTipo[7]  = SQL_CHAR;	
	nSqlTipo[8] = SQL_CHAR;
	nSqlTipo[9] = SQL_SMALLINT;
	nSqlTipo[10] = SQL_SMALLINT;
	nSqlTipo[11] = SQL_CHAR;
	nSqlTipo[12] = SQL_CHAR;
	nSqlTipo[13] = SQL_CHAR;
	nSqlTipo[14] = SQL_CHAR;
	nSqlTipo[15] = SQL_CHAR;
	nSqlTipo[16] = SQL_SMALLINT;
	nSqlTipo[17] = SQL_CHAR;
	nSqlTipo[18] = SQL_CHAR;
	nSqlTipo[19] = SQL_SMALLINT;
	nSqlTipo[20] = SQL_SMALLINT;
	nSqlTipo[21] = SQL_SMALLINT;
	
	
    nCTipo[0]  = SQL_C_SLONG;
    nCTipo[1]  = SQL_C_CHAR;
    nCTipo[2]  = SQL_C_CHAR;
    nCTipo[3]  = SQL_C_CHAR;
    nCTipo[4]  = SQL_C_CHAR;
    nCTipo[5]  = SQL_C_CHAR;
    nCTipo[6]  = SQL_C_CHAR;
    nCTipo[7]  = SQL_C_CHAR;	
    nCTipo[8]  = SQL_C_CHAR;
    nCTipo[9]  = SQL_C_SSHORT;
	nCTipo[10] = SQL_C_SSHORT;
    nCTipo[11] = SQL_C_CHAR;
    nCTipo[12] = SQL_C_CHAR;
	nCTipo[13] = SQL_C_CHAR;
    nCTipo[14] = SQL_C_CHAR;
    nCTipo[15] = SQL_C_CHAR;
    nCTipo[16] = SQL_C_SSHORT;
    nCTipo[17] = SQL_C_CHAR;
    nCTipo[18] = SQL_C_CHAR;
    nCTipo[19] = SQL_C_SSHORT;
    nCTipo[20] = SQL_C_SSHORT;
    nCTipo[21] = SQL_C_SSHORT;
	
	
	nLongitud[0]  = 5;	//&iFolio;
    nLongitud[1]  = 13; //cNss;
    nLongitud[2]  = 13; //cNssissste;
    nLongitud[3]  = 20; //cCurp;
    nLongitud[4]  = 4;  //cSecpension;
    nLongitud[5]  = 42; //cNombreDatamart;
    nLongitud[6]  = 42; //cPaternoDatamart;
    nLongitud[7]  = 42; //cMaternoDatamart;	
	nLongitud[8]  = 11; // cNumconcesion;
	nLongitud[9]  = 5;  // &shDelegacion;
	nLongitud[10] = 5;  // &shTipomovimiento;
	nLongitud[11] = 3;  // cTiporetiro;
	nLongitud[12] = 4;  // cRegimen;
	nLongitud[13] = 4;  // cTiposeguro;
	nLongitud[14] = 4;  // cTipopension;
	nLongitud[15] = 5;  // cCvepension;
	nLongitud[16] = 5;  // &shTipoprestacion;
	nLongitud[17] = 25; // cFechainiciopen;
	nLongitud[18] = 25; // cFecharesolucion;
	nLongitud[19] = 5;  // &shSemanascotizadas;
	nLongitud[20] = 5;  // &shDiagDatamart;
	nLongitud[21] = 5;  // &shEstadoregistro;
	
	pVar[0] =   &iFolio;
	pVar[1] =   cNss;
	pVar[2] =   cNssissste;
	pVar[3] =   cCurp;
	pVar[4] =   cSecpension;
	pVar[5] =   cNombreDatamart;
	pVar[6] =   cPaternoDatamart;
	pVar[7] =   cMaternoDatamart;	
	pVar[8]  =   cNumconcesion;
	pVar[9]  =   &shDelegacion;
	pVar[10] =   &shTipomovimiento;
	pVar[11] =   cTiporetiro;
	pVar[12] =   cRegimen;
	pVar[13] =   cTiposeguro;
	pVar[14] =   cTipopension;
	pVar[15] =   cCvepension;
	pVar[16] =   &shTipoprestacion;
	pVar[17] =   cFechainiciopen;
	pVar[18] =   cFecharesolucion;
	pVar[19] =   &shSemanascotizadas;
	pVar[20] =   &shDiagDatamart;
	pVar[21] =   &shEstadoregistro;
	
    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }
}

CConsultaDataMartIssste::~CConsultaDataMartIssste()
{
}

void CConsultaDataMartIssste::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)
    {
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }
}

