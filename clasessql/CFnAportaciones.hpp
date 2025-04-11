#ifndef CFnAportaciones__
#define CFnAportaciones__

#include <RecordSet.hpp>
class CFnAportaciones : public CRecordSet
{
public:
	CFnAportaciones(C_ODBC *odbc_ext, const char *select=NULL);
    ~CFnAportaciones();
    void activarCols();

    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[2];
    int nCTipo[2];
    long nLongitud[2];
    long nLongResp[2];
    void *pVar[2];

    double  iAportaciones;
};
#endif
