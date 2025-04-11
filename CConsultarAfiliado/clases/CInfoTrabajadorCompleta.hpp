// Consulta original solicitada:...
//  SELECT 'ANTONIO'::CHAR(40) as cApellidoPaterno,'TORRES'::CHAR(40) as cApellidoMaterno,'ANICETA'::CHAR(40) as cNombre,'AOTA430417MMCNRN05'::CHAR(18) as cCurp,'01004300214'::CHAR(11) as cNss,'2014-09-17'::CHAR(10) as cFechaServicio,'1943-04-17'::CHAR(10) as cFechaNacimiento,2 as shSexo,'15'::CHAR(2) as cEntidadNacimiento FROM COLSOLICITUDES LIMIT 1
    
#ifndef CINFOTRABAJADORCOMPLETA
#define CINFOTRABAJADORCOMPLETA
#include <RecordSet.hpp>
class CInfoTrabajadorCompleta : public CRecordSet
{
public:
    CInfoTrabajadorCompleta(C_ODBC *odbc_ext, const char *select=NULL);
    ~CInfoTrabajadorCompleta();
    void activarCols();
    
    C_ODBC *odbc;
    int odbcRet;
    int nCols;
    int nSqlTipo[10];
    int nCTipo[10];
    long nLongitud[10];
    long nLongResp[10];
    void *pVar[10];
    
    char                capellidopaterno[42];
    char                capellidomaterno[42];
    char                cnombre[42];
    char                ccurp[20];
    char                cnss[13];
    char                cfechaservicio[12];
    char                cfechanacimiento[12];
    short               shsexo;
    char                centidadnacimiento[4];
};
#endif
