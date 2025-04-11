#ifndef CCAMPOTEXTO
#define CCAMPOTEXTO

#include <RecordSet.hpp>
class CCampoTexto : public CRecordSet 
{
public:
    CCampoTexto(C_ODBC *odbc_ext, const char *select=NULL);
    ~CCampoTexto();
    void activarCols();
    BOOL prepararInsert();
    BOOL prepararInsert(const char *tabla);
    
    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int flagInsertar;
    int nSqlTipo[2];
    int nCTipo[2];
    long nLongitud[2];
    long nLongResp[2];
    void *pVar[2];
    
    char                texto[102];
};
#endif// CCOUNT
