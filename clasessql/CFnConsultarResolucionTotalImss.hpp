
#ifndef CFnConsultarResolucionTotalImss__
#define CFnConsultarResolucionTotalImss__

#include <RecordSet.hpp>
class CFnConsultarResolucionTotalImss : public CRecordSet
{
public:
	CFnConsultarResolucionTotalImss(C_ODBC *odbc_ext, const char *select=NULL);
    ~CFnConsultarResolucionTotalImss();
    void activarCols();

    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[23];
    int nCTipo[23];
    long nLongitud[23];
    long nLongResp[23];
    void *pVar[23];

	int		iExisteResolucion,
			iExisteAfiliado;
    char	cRegimen[4],
			cTipoSeguro[4],
			cTipoPension[4];
    int		iTipoPrestacion;
	char	cNombreDatamart[52];
	int		iFolio;
    char	cSecPension[12],
			dFechaIni[12],
			dFechaPago[12];
    int		iSemanasCotizadas,
			iDiagDatamart,
			iEstadoSubViv;
    char	dFechaResolucion[12],
			cDescripRegimen[52],
			cDescripTPension[52],
			cDescripTPresta[52],
			cDescripTRetiro[102],
			cDescripTSeguro[52],
			cDescripEdoSubViv[102];
    int		iIdMovimiento;
};
#endif
