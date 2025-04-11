#include "CInfoTrabajadorCompleta.hpp"
CInfoTrabajadorCompleta::CInfoTrabajadorCompleta(C_ODBC *odbc_ext, const char *select) : CRecordSet(odbc_ext)
{
    odbc = odbc_ext;
    nCols=9;
    odbcRet=TRUE;
    nSqlTipo[0] = SQL_CHAR;
    nSqlTipo[1] = SQL_CHAR;
    nSqlTipo[2] = SQL_CHAR;
    nSqlTipo[3] = SQL_CHAR;
    nSqlTipo[4] = SQL_CHAR;
    nSqlTipo[5] = SQL_CHAR;
    nSqlTipo[6] = SQL_CHAR;
    nSqlTipo[7] = SQL_SMALLINT;
    nSqlTipo[8] = SQL_CHAR;

    nCTipo[0] = SQL_C_CHAR;
    nCTipo[1] = SQL_C_CHAR;
    nCTipo[2] = SQL_C_CHAR;
    nCTipo[3] = SQL_C_CHAR;
    nCTipo[4] = SQL_C_CHAR;
    nCTipo[5] = SQL_C_CHAR;
    nCTipo[6] = SQL_C_CHAR;
    nCTipo[7] = SQL_C_SHORT;
    nCTipo[8] = SQL_C_CHAR;

    nLongitud[0] = 42;
    nLongitud[1] = 42;
    nLongitud[2] = 42;
    nLongitud[3] = 20;
    nLongitud[4] = 13;
    nLongitud[5] = 12;
    nLongitud[6] = 12;
    nLongitud[7] = 3;
    nLongitud[8] = 4;
    pVar[0] =  capellidopaterno;
    pVar[1] =  capellidomaterno;
    pVar[2] =  cnombre;
    pVar[3] =  ccurp;
    pVar[4] =  cnss;
    pVar[5] =  cfechaservicio;
    pVar[6] =  cfechanacimiento;
    pVar[7] = &shsexo;
    pVar[8] =  centidadnacimiento;

    if (select != NULL)
    {
        odbcRet = Exec(select);
        activarCols();
    }                                                              
}
    

CInfoTrabajadorCompleta::~CInfoTrabajadorCompleta()
{
}
    
void CInfoTrabajadorCompleta::activarCols()
{
    int i;
    for (i=0; i<nCols; i++)                                                              
    {                                                              
        Activar(i, nCTipo[i], pVar[i], nLongitud[i], &nLongResp[i]);
    }                                                              
}
 
