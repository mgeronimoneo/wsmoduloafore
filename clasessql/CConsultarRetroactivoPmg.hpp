
#ifndef CConsultarRetroactivoPmg__
#define CConsultarRetroactivoPmg__

#include <RecordSet.hpp>
class CConsultarRetroactivoPmg : public CRecordSet
{
public:
	CConsultarRetroactivoPmg(C_ODBC *odbc_ext, const char *select=NULL);
    ~CConsultarRetroactivoPmg();
    void activarCols();

    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[5];
    int nCTipo[5];
    long nLongitud[5];
    long nLongResp[5];
    void *pVar[5];

    double		dRetroactivo,
    			dDevengado;
	int 		iEstatus,
				iRegistro;
};
#endif
