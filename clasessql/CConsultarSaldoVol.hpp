#ifndef CConsultarSaldoVol_
#define CConsultarSaldoVol_

#include <RecordSet.hpp>
class CConsultarSaldoVol : public CRecordSet
{
public:
	CConsultarSaldoVol(C_ODBC *odbc_ext, const char *select=NULL);
    ~CConsultarSaldoVol();
    void activarCols();

   C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[13];
    int nCTipo[13];
    long nLongitud[13];
    long nLongResp[13];
    void *pVar[13];

	
	int		folio,
			consecutivoLote;
	char 	nss[13];
    short	siefore,
			subCuenta;
	char 	fechaValor[12],
			fechaConversion[12];
	double	montoEnPesos,
			montoEnAcciones,
			saldoAcciones;
	char 	fechaSaldo[12],
			usuario[14];


};
#endif
