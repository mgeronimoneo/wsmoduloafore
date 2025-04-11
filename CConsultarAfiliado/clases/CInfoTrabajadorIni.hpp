// Consulta original solicitada:...
//  select foliosolicitud, nss, curp from recsolicitudservicio
    
#ifndef CINFOTRABAJADORINI
#define CINFOTRABAJADORINI
#include <RecordSet.hpp>
class CInfoTrabajadorIni : public CRecordSet
{
public:
    CInfoTrabajadorIni(C_ODBC *odbc_ext, const char *select=NULL);
    ~CInfoTrabajadorIni();
    void activarCols();
    
    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[4];
    int nCTipo[4];
    long nLongitud[4];
    long nLongResp[4];
    void *pVar[4];
    
    char               curp[20];
    char               nss[13];
    char               fechaservicio[12];
};
#endif
