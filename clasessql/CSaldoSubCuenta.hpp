#ifndef CSaldoSubCuenta__
#define CSaldoSubCuenta__

#include <RecordSet.hpp>
class CSaldoSubCuenta : public CRecordSet
{
public:
	CSaldoSubCuenta(C_ODBC *odbc_ext, const char *select=NULL);
    ~CSaldoSubCuenta();
    void activarCols();

    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[5];
    int nCTipo[5];
    long nLongitud[5];
    long nLongResp[5];
    void *pVar[5];

    short		subCuenta,
    			siefore;
    double		montoEnPesos,
    			montoEnAcciones;
};
#endif
