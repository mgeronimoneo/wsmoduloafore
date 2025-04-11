#ifndef CConsultarConvivenciaMarca__
#define CConsultarConvivenciaMarca___

#include <RecordSet.hpp>
class CConsultarConvivenciaMarca : public CRecordSet
{
public:
	CConsultarConvivenciaMarca(C_ODBC *odbc_ext, const char *select=NULL);
    ~CConsultarConvivenciaMarca();
    void activarCols();
    
    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[4];
    int nCTipo[4];
    long nLongitud[4];
    long nLongResp[4];
    void *pVar[4];
    
    int codigo;
    char descripcion[202];
    int tienemarca;
};
#endif
