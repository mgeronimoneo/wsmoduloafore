
#ifndef CFnConsultarResolucionRetAportaciones__
#define CFnConsultarResolucionRetAportaciones__

#include <RecordSet.hpp>
class CFnConsultarResolucionRetAportaciones : public CRecordSet
{
public:
	CFnConsultarResolucionRetAportaciones(C_ODBC *odbc_ext, const char *select=NULL);
    ~CFnConsultarResolucionRetAportaciones();
    void activarCols();

    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[3];
    int nCTipo[3];
    long nLongitud[3];
    long nLongResp[3];
    void *pVar[3];

    int	    iOpcion;
    char 	tTipo_Pension[6];
};
#endif
