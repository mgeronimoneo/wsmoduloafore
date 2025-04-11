#ifndef CConsultaDouble___
#define CConsultaDouble___

#include <RecordSet.hpp>
class CConsultaDouble : public CRecordSet
{
public:
	CConsultaDouble(C_ODBC *odbc_ext, const char *select=NULL);
    ~CConsultaDouble();
    void activarCols();
    
    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[2];
    int nCTipo[2];
    long nLongitud[2];
    long nLongResp[2];
    void *pVar[2];
    
    double campo_double;
};
#endif
