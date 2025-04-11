
#ifndef CFnConsultarResolucion__
#define CFnConsultarResolucion__

#include <RecordSet.hpp>
class CFnConsultarResolucion : public CRecordSet
{
public:
	CFnConsultarResolucion(C_ODBC *odbc_ext, const char *select=NULL);
    ~CFnConsultarResolucion();
    void activarCols();

    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[5];
    int nCTipo[5];
    long nLongitud[5];
    long nLongResp[5];
    void *pVar[5];

    char 	nss[13];
    char	fechainiciovigencia[12],
    		fechafinvigencia[12];
     int	resolusion;
};
#endif
