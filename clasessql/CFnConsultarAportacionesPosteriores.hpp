#ifndef CFnConsultarAportacionesPosteriores__
#define CFnConsultarAportacionesPosteriores__

#include <RecordSet.hpp>
class CFnConsultarAportacionesPosteriores : public CRecordSet
{
public:
	CFnConsultarAportacionesPosteriores(C_ODBC *odbc_ext, const char *select=NULL);
    ~CFnConsultarAportacionesPosteriores();
    void activarCols();

    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[2];
    int nCTipo[2];
    long nLongitud[2];
    long nLongResp[2];
    void *pVar[2];

	double	montoaportacionposterior;
};
#endif
