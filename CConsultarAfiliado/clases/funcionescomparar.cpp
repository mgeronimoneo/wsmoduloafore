#include "funcionescomparar.hpp"


int	compararNssMaestroAfiliado(const void *nssBuscar, const void *nssMuestra)
{
	int iRet;
	MAESTRO_AFILIADO	*ptrNssBuscar,
						*ptrNssMuestra;
	ptrNssBuscar = (MAESTRO_AFILIADO *)nssBuscar;
	ptrNssMuestra = (MAESTRO_AFILIADO *)nssMuestra;
	iRet = memcmp(ptrNssBuscar->cNss, ptrNssMuestra->cNss, SIZE_NSS);

	return iRet;
}
