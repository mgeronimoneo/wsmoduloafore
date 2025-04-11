#ifndef COperacionesPmgImss__
#define COperacionesPmgImss__

#include <RecordSet.hpp>
class COperacionesPmgImss : public CRecordSet
{
public:
	COperacionesPmgImss(C_ODBC *odbc_ext, const char *select=NULL);
    ~COperacionesPmgImss();
    void activarCols();

    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[4];
    int nCTipo[4];
    long nLongitud[4];
    long nLongResp[4];
    void *pVar[4];

    char	cMensaje[1024];
	int		sCodigo;
	double	dMonto;
};          
#endif      