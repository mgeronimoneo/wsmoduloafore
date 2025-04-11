
#ifndef CCONSULTADATAMARTISSSTE__
#define CCONSULTADATAMARTISSSTE__

#include <RecordSet.hpp>
class CConsultaDataMartIssste : public CRecordSet
{
public:
	CConsultaDataMartIssste(C_ODBC *odbc_ext, const char *select=NULL);
    ~CConsultaDataMartIssste();
    void activarCols();

    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[24];
    int nCTipo[24];
    long nLongitud[24];
    long nLongResp[24];
    void *pVar[24];   
	
	char	cNss			[13],
			cNssissste		[13],
			cCurp			[20],
			cSecpension		[4],
			cNombreDatamart	[42],
			cPaternoDatamart[42],
			cMaternoDatamart[42],			
			cNumconcesion	[11],
			cTiporetiro		[3],
			cRegimen		[4],
			cTiposeguro		[4],
			cTipopension	[4],
			cCvepension		[5],
			cFechainiciopen	[25],
			cFecharesolucion[25];
	short	shDelegacion,
			shTipomovimiento,
			shTipoprestacion,
			shSemanascotizadas,
			shDiagDatamart,
			shEstadoregistro;
	int		iFolio;
};
#endif