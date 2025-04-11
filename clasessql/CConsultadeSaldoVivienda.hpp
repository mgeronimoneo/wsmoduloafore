
#ifndef CCONSULTADATAMARTISSSTE__
#define CCONSULTADATAMARTISSSTE__

#include <RecordSet.hpp>
class CConsultadeSaldoVivienda : public CRecordSet
{
public:
	CConsultadeSaldoVivienda(C_ODBC *odbc_ext, const char *select=NULL);
    ~CConsultadeSaldoVivienda();
    void activarCols();

    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[24];
    int nCTipo[24];
    long nLongitud[24];
    long nLongResp[24];
    void *pVar[24];   
	
	char	cNss        		[13],
            dMontoaccviv        [26],
    		dMontopesviv        [26];
	
};
#endif