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
int compararNssEnSaldoCuenta(const void *nssBuscar, const void *nssMuestra)
{
	int iRet;
	SALDO_CUENTA	*ptrNssBuscar,
						*ptrNssMuestra;
	ptrNssBuscar = (SALDO_CUENTA *)nssBuscar;
	ptrNssMuestra = (SALDO_CUENTA *)nssMuestra;
	iRet = memcmp(ptrNssBuscar->cNss, ptrNssMuestra->cNss, SIZE_NSS);

	return iRet;
}

int compararNssCtaRegimen(const void *nssBuscar, const void *nssMuestra)
{
	int iRet;
	SIEFORE		*ptrNssBuscar,
						*ptrNssMuestra;
	ptrNssBuscar = (SIEFORE *)nssBuscar;
	ptrNssMuestra = (SIEFORE *)nssMuestra;
	iRet = memcmp(ptrNssBuscar->cNss, ptrNssMuestra->cNss, SIZE_NSS);

	return iRet;
}

int compararNssCtaSaldoVol(const void *nssBuscar, const void *nssMuestra)
{
	int iRet;
	SALDO_VOL		*ptrNssBuscar,
						*ptrNssMuestra;
	ptrNssBuscar = (SALDO_VOL *)nssBuscar;
	ptrNssMuestra = (SALDO_VOL *)nssMuestra;
	iRet = memcmp(ptrNssBuscar->cNss, ptrNssMuestra->cNss, SIZE_NSS);

	return iRet;
}