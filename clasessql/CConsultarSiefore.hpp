#ifndef CConsultarSiefore_
#define CConsultarSiefore_

#include <RecordSet.hpp>
class CConsultarSiefore : public CRecordSet
{
public:
	CConsultarSiefore(C_ODBC *odbc_ext, const char *select=NULL);
    ~CConsultarSiefore();
    void activarCols();

    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[5];
    int nCTipo[5];
    long nLongitud[5];
    long nLongResp[5];
    void *pVar[5];
	
	char nss[12];
	short subCuenta,
	siefore;
	
};
#endif