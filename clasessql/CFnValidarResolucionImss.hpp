#ifndef CFnValidarResolucionImss__
#define CFnValidarResolucionImss__

#include <RecordSet.hpp>
class CFnValidarResolucionImss : public CRecordSet
{
public:
	CFnValidarResolucionImss(C_ODBC *odbc_ext, const char *select=NULL);
    ~CFnValidarResolucionImss();
    void activarCols();
    
    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[7];
    int nCTipo[7];
    long nLongitud[7];
    long nLongResp[7];
    void *pVar[7];
    
    char fecha_vigencia[12],
    	 fecha_vigencia_ant[12],
    	 fecha_proxima_sol[12];
    int  retorno;
    char mensaje[52];
    int  diagnosticoProcesar;
};
#endif
