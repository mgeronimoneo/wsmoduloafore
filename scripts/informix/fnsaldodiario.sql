CREATE FUNCTION fnsaldodiario( cNss CHAR(11),dFechaCorte DATE)
RETURNING SMALLINT,SMALLINT,DECIMAL(16,6),DECIMAL(16,6);
	
	DEFINE  dPrecioViv DECIMAL(19,14);
	DEFINE  dPrecioFov DECIMAL(19,14);
	DEFINE  iSubCuenta SMALLINT     ;
	DEFINE  iSiefore   SMALLINT     ;
	DEFINE  dSaldoAcc  DECIMAL(16,6);
	DEFINE  dSaldoPes  DECIMAL(16,6);
	DEFINE  dFechaViv  DATE ;

	LET dPrecioViv = 0;
	LET dPrecioFov = 0;
	LET dFechaViv  = MONTH(dFechaCorte)||"/01/"||YEAR(dFechaCorte);
	
	--siefore 11
	SELECT precio_del_dia INTO dPrecioViv FROM glo_valor_accion WHERE fecha_valuacion = dFechaViv AND codigo_siefore = 11;
	
	--siefore 12
	SELECT precio_del_dia INTO dPrecioFov FROM glo_valor_accion WHERE fecha_valuacion = dFechaViv AND codigo_siefore = 12;

	FOREACH 
		SELECT a.subcuenta, a.siefore, SUM(a.monto_en_acciones),SUM(a.monto_en_acciones)* b.precio_del_dia  
		INTO iSubCuenta, iSiefore, dSaldoAcc, dSaldoPes
	   	FROM dis_cuenta AS a 
		JOIN glo_valor_accion AS b ON (a.siefore = b.codigo_siefore) AND  b.fecha_valuacion = dFechaCorte
		WHERE a.nss = cNss
		AND siefore NOT IN(11,12)
		AND a.fecha_conversion <= dFechaCorte
		--AND( ( monto_en_acciones <> 0 AND siefore <> 0 ) OR ( monto_en_pesos    <> 0 AND siefore =  0 ))
		GROUP BY a.subcuenta, a.siefore, b.precio_del_dia  ORDER BY 1,2 DESC

		IF NVL(dSaldoAcc,0)>0 OR NVL(dSaldoPes,0) >0 THEN
			RETURN NVL(iSubCuenta,0), NVL(iSiefore,0), NVL(dSaldoAcc,0), NVL(dSaldoPes,0) WITH RESUME;
		END IF;
	END FOREACH;
	
	FOREACH 
		SELECT a.subcuenta, a.siefore, SUM(a.monto_en_acciones), 
		CASE WHEN siefore = 11 THEN NVL(ROUND(sum(a.monto_en_acciones)* dPrecioViv,2),0) WHEN siefore = 12 THEN NVL(ROUND(sum(a.monto_en_acciones)* dPrecioFov,2),0)  END 
		INTO iSubCuenta, iSiefore, dSaldoAcc, dSaldoPes
		FROM dis_cuenta AS a 
		WHERE a.nss = cNss 
		AND siefore IN(11,12)
		AND a.fecha_conversion <= dFechaCorte
		--AND( ( monto_en_acciones <> 0 AND siefore <> 0 ) OR ( monto_en_pesos <> 0 AND siefore =  0 ))
		GROUP BY a.subcuenta, a.siefore ORDER BY 1,2 DESC
		IF NVL(dSaldoAcc,0)>0 OR NVL(dSaldoPes,0) >0 THEN
			RETURN NVL(iSubCuenta,0), NVL(iSiefore,0), NVL(dSaldoAcc,0), NVL(dSaldoPes,0) WITH RESUME;
		END IF;		
	END FOREACH;
END FUNCTION;