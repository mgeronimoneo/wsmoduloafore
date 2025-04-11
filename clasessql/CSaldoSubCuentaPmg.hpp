
#ifndef CSaldoSubCuentaPmg__
#define CSaldoSubCuentaPmg__

#include <RecordSet.hpp>
class CSaldoSubCuentaPmg : public CRecordSet
{
public:
	CSaldoSubCuentaPmg(C_ODBC *odbc_ext, const char *select=NULL);
    ~CSaldoSubCuentaPmg();
    void activarCols();

    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[5];
    int nCTipo[5];
    long nLongitud[5];
    long nLongResp[5];
    void *pVar[5];

    double		dpRetroactivo,
    			dpDevengado,
				dpSaldoDia;
	int 		iSuficiencia;
};
#endif
