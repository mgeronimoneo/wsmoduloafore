
#ifndef CConsMontosFechasDatamart__
#define CConsMontosFechasDatamart__

#include <RecordSet.hpp>
class CConsMontosFechasDatamart : public CRecordSet
{
public:
	CConsMontosFechasDatamart(C_ODBC *odbc_ext, const char *select=NULL);
    ~CConsMontosFechasDatamart();
    void activarCols();

    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[8];
    int nCTipo[8];
    long nLongitud[8];
    long nLongResp[8];
    void *pVar[8];

    char 		nss[13];
    double		salariobasea,
    			salariobaseb;
    char		fechamatdes[12],
    			fechainivigencia[12];
     double		ultimasbc,
    			ultimo250semanassbc;
};
#endif
