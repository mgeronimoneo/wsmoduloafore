#ifndef CCONSULTAINT
#define CCONSULTAINT

#include <RecordSet.hpp>
class CConsultaInt : public CRecordSet 
{
public:
    CConsultaInt(C_ODBC *odbc_ext, const char *select=NULL);
    ~CConsultaInt();
    void activarCols();
    
    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[2];
    int nCTipo[2];
    long nLongitud[2];
    long nLongResp[2];
    void *pVar[2];
    
    int                campoint;
};
#endif
