
#ifndef CFnConsultarComplementoResolucion__
#define CFnConsultarComplementoResolucion__

#include <RecordSet.hpp>
class CFnConsultarComplementoResolucion : public CRecordSet
{
public:
	CFnConsultarComplementoResolucion(C_ODBC *odbc_ext, const char *select=NULL);
    ~CFnConsultarComplementoResolucion();
    void activarCols();

    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[8];
    int nCTipo[8];
    long nLongitud[8];
    long nLongResp[8];
    void *pVar[8];

    int  	numeroResolucion,
    		idcomplemento;
    double	montopagado,
    		montopagadocomplemento,
    		saldorcv,
    		saldorcvanterior;
    short	respuestaotrodatamart;
};
#endif
