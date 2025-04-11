
#ifndef CTBMEMORIACOMPARTIDA
#define CTBMEMORIACOMPARTIDA

#include <RecordSet.hpp>
class CTbMemoriaCompartida : public CRecordSet
{
public:
	CTbMemoriaCompartida(C_ODBC *odbc_ext, const char *select=NULL);
    ~CTbMemoriaCompartida();
    void activarCols();
    
    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[3];
    int nCTipo[3];
    long nLongitud[3];
    long nLongResp[3];
    void *pVar[3];

    int		iIdShm,
    		iTotalReg;
};
#endif
