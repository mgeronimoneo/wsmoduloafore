#ifndef CFnMovimientosCuentas__
#define CFnMovimientosCuentas__

#include <RecordSet.hpp>
class CFnMovimientosCuentas : public CRecordSet
{
public:
	CFnMovimientosCuentas(C_ODBC *odbc_ext, const char *select=NULL);
    ~CFnMovimientosCuentas();
    void activarCols();

    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[2];
    int nCTipo[2];
    long nLongitud[2];
    long nLongResp[2];
    void *pVar[2];

    int  	iMovimientos;
};
#endif
