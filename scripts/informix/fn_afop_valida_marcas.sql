DROP FUNCTION fn_afop_valida_marcas(SMALLINT, CHAR(11), CHAR(2));
CREATE FUNCTION "informix".fn_afop_valida_marcas(p_opcion SMALLINT,p_nss CHAR(11),p_tiporetiro CHAR(2))
RETURNING INTEGER AS CodigoError, CHAR (60) AS Mensaje;

DEFINE cMensaje                 CHAR(60);
DEFINE nCodigoError     INTEGER;
DEFINE nTipoMovimiento  INTEGER;
DEFINE nRechazoCod              INTEGER;
DEFINE nContMov			INTEGER;

LET cMensaje                    = '';
LET nCodigoError                = 0;
LET nTipoMovimiento     		= 0;
LET nRechazoCod                 = 0;
LET nContMov					= 0;

        IF p_tiporetiro = 'RP' OR p_tiporetiro = 'RC' THEN--La marca de Recertificacion es tanto para IMSS/ISSSTE

                IF p_tiporetiro = 'RP' THEN--Recertificaci▒n en proceso
                        LET nTipoMovimiento = 927;
                ELIF p_tiporetiro = 'RC' THEN--Recertificaci▒n terminada
                        LET nTipoMovimiento = 928;
                END IF;

                FOREACH
                      SELECT rechazo_cod INTO nRechazoCod
                      FROM cta_convivencia
                      WHERE marca_entra = nTipoMovimiento
                      AND marca_activa IN (SELECT marca_cod FROM cta_act_marca WHERE nss = p_nss)

                      IF(nRechazoCod IS NOT NULL AND nRechazoCod <> 0) THEN
                          LET nCodigoError = 4;
                          LET cMensaje = 'LA CUENTA SE ENCUENTRA EN OTRO PROCESO OPERATIVO';
                          EXIT FOREACH;
                      END IF;

                END FOREACH;

        ELSE
		
			SELECT COUNT(*) INTO nContMov FROM tab_ret_issste WHERE tipo_retiro  = p_tiporetiro;

                IF p_opcion = 1 AND nContMov <= 1 THEN
                        IF p_tiporetiro = 'SI' THEN  --TRASPASO ICEFA SAR ISSSTE
                                LET nTipoMovimiento = 260;
                                            ELIF p_tiporetiro = 'MD' THEN   --MODIFICACION DE DATOS
                                LET nTipoMovimiento = 600;
                        ELIF p_tiporetiro = 'AE' THEN   --ALTA DE EXPEDIENTE DE IDENTIFICACION Y BIOMETRICO
                                LET nTipoMovimiento = 950;
                        ELIF p_tiporetiro = 'UA' THEN   --UNIFICACION CUENTAS ISSSTE ACTIVA-ACTIVA
                                LET nTipoMovimiento = 262;
                        ELSE
						
                                SELECT movimiento INTO nTipoMovimiento
                                FROM tab_ret_issste
                                WHERE tipo_retiro = p_tiporetiro;
								

                        END IF;

                        FOREACH
                              SELECT rechazo_cod INTO nRechazoCod
                              FROM cta_convivencia
                              WHERE marca_entra = nTipoMovimiento
                              AND marca_activa IN (SELECT marca_cod FROM cta_act_marca WHERE nss = p_nss)

                              IF(nRechazoCod IS NOT NULL AND nRechazoCod <> 0) THEN
                                                                IF nRechazoCod = 120 OR nRechazoCod = 220 OR nRechazoCod = 225 OR nRechazoCod = 226 OR nRechazoCod = 240 OR nRechazoCod = 241 OR nRechazoCod = 242 OR nRechazoCod = 243 OR nRechazoCod = 244 OR nRechazoCod = 245 OR
                                                                   nRechazoCod = 247 OR nRechazoCod = 248 OR nRechazoCod = 261 OR nRechazoCod = 262 OR nRechazoCod = 263 OR nRechazoCod = 264 OR nRechazoCod = 265 OR nRechazoCod = 267 OR nRechazoCod = 271 OR nRechazoCod = 272 OR
                                                                   nRechazoCod = 273 OR nRechazoCod = 280 OR nRechazoCod = 286 OR nRechazoCod = 290 OR nRechazoCod = 291 OR nRechazoCod = 292 OR nRechazoCod = 293 OR nRechazoCod = 297 OR nRechazoCod = 298 OR nRechazoCod = 490 OR
                                                                   nRechazoCod = 600 OR nRechazoCod = 819 OR nRechazoCod = 820 OR nRechazoCod = 825 OR nRechazoCod = 830 OR nRechazoCod = 835 OR nRechazoCod = 840 OR nRechazoCod = 841 OR nRechazoCod = 850 OR nRechazoCod = 851 OR
                                                                   nRechazoCod = 852 OR nRechazoCod = 853 OR nRechazoCod = 854 OR nRechazoCod = 855 OR nRechazoCod = 856 OR nRechazoCod = 860 OR nRechazoCod = 870 OR nRechazoCod = 875 OR nRechazoCod = 880 OR nRechazoCod = 897 OR
                                                                                                                                   nRechazoCod = 601 OR nRechazoCod = 262 THEN
                                                                   LET nCodigoError = nRechazoCod;
                                                                   LET cMensaje = 'LA CUENTA SE ENCUENTRA EN OTRO PROCESO OPERATIVO ['|| nRechazoCod ||']';
                                                                   EXIT FOREACH;
                                                                ELSE
                                                                   LET nCodigoError = 4;
                                                                   LET cMensaje = 'LA CUENTA SE ENCUENTRA EN OTRO PROCESO OPERATIVO';
                                                                   EXIT FOREACH;
                                                                END IF;
                              END IF;

                        END FOREACH;

				END IF;
				
				IF p_opcion = 1 AND nContMov > 1 THEN
					FOREACH				
						SELECT movimiento INTO nTipoMovimiento
						FROM tab_ret_issste
						WHERE tipo_retiro = p_tiporetiro
						
                        FOREACH
                              SELECT rechazo_cod INTO nRechazoCod
                              FROM cta_convivencia
                              WHERE marca_entra = nTipoMovimiento
                              AND marca_activa IN (SELECT marca_cod FROM cta_act_marca WHERE nss = p_nss)

                              IF(nRechazoCod IS NOT NULL AND nRechazoCod <> 0) THEN
                                                                IF nRechazoCod = 120 OR nRechazoCod = 220 OR nRechazoCod = 225 OR nRechazoCod = 226 OR nRechazoCod = 240 OR nRechazoCod = 241 OR nRechazoCod = 242 OR nRechazoCod = 243 OR nRechazoCod = 244 OR nRechazoCod = 245 OR
                                                                   nRechazoCod = 247 OR nRechazoCod = 248 OR nRechazoCod = 261 OR nRechazoCod = 262 OR nRechazoCod = 263 OR nRechazoCod = 264 OR nRechazoCod = 265 OR nRechazoCod = 267 OR nRechazoCod = 271 OR nRechazoCod = 272 OR
                                                                   nRechazoCod = 273 OR nRechazoCod = 280 OR nRechazoCod = 286 OR nRechazoCod = 290 OR nRechazoCod = 291 OR nRechazoCod = 292 OR nRechazoCod = 293 OR nRechazoCod = 297 OR nRechazoCod = 298 OR nRechazoCod = 490 OR
                                                                   nRechazoCod = 600 OR nRechazoCod = 819 OR nRechazoCod = 820 OR nRechazoCod = 825 OR nRechazoCod = 830 OR nRechazoCod = 835 OR nRechazoCod = 840 OR nRechazoCod = 841 OR nRechazoCod = 850 OR nRechazoCod = 851 OR
                                                                   nRechazoCod = 852 OR nRechazoCod = 853 OR nRechazoCod = 854 OR nRechazoCod = 855 OR nRechazoCod = 856 OR nRechazoCod = 860 OR nRechazoCod = 870 OR nRechazoCod = 875 OR nRechazoCod = 880 OR nRechazoCod = 897 OR
                                                                                                                                   nRechazoCod = 601 OR nRechazoCod = 262 THEN
                                                                   LET nCodigoError = nRechazoCod;
                                                                   LET cMensaje = 'LA CUENTA SE ENCUENTRA EN OTRO PROCESO OPERATIVO ['|| nRechazoCod ||']';
                                                                   EXIT FOREACH;
                                                                ELSE
                                                                   LET nCodigoError = 4;
                                                                   LET cMensaje = 'LA CUENTA SE ENCUENTRA EN OTRO PROCESO OPERATIVO';
                                                                   EXIT FOREACH;
                                                                END IF;
                              END IF;

                        END FOREACH;
						IF nCodigoError != 0 THEN
							EXIT FOREACH;
						END IF						
					END FOREACH;
				END IF;

                IF p_opcion = 2 THEN
                        IF p_tiporetiro = '6' THEN   --RETIRO POR DESEMPLEO
                                LET nTipoMovimiento = 875;
                        ELIF p_tiporetiro = '7' THEN    --RETIRO POR MATRIMONIO
                                LET nTipoMovimiento = 870;
                        ELIF p_tiporetiro = 'MD' THEN   --MODIFICACION DE DATOS
                                LET nTipoMovimiento = 600;
                        ELIF p_tiporetiro = 'S9' THEN   --TRASPASO ICEFA SAR IMSS
                                LET nTipoMovimiento = 250;
                        ELIF p_tiporetiro = 'AE' THEN   --ALTA DE EXPEDIENTE DE IDENTIFICACION Y BIOMETRICO
                                LET nTipoMovimiento = 950;
                                                ELIF p_tiporetiro = 'UM' THEN   --UNIFICACION MIXTA
                                                                LET nTipoMovimiento =263;

                        ELSE
                                SELECT movimiento INTO nTipoMovimiento
                                FROM tab_retiro
                                WHERE tipo_retiro = p_tiporetiro;
                        END IF;

                        FOREACH

                                SELECT rechazo_cod INTO nRechazoCod
                                FROM cta_convivencia
                                WHERE marca_entra = nTipoMovimiento
                                AND marca_activa IN (SELECT marca_cod FROM cta_act_marca WHERE nss = p_nss)

                                IF(nRechazoCod IS NOT NULL AND nRechazoCod <> 0) THEN
                                                                        IF p_tiporetiro = '6' OR p_tiporetiro = '7' THEN
                                                                                IF nRechazoCod = 120 OR nRechazoCod = 220 OR nRechazoCod = 225 OR nRechazoCod = 226 OR nRechazoCod = 240 OR nRechazoCod = 241 OR nRechazoCod = 242 OR nRechazoCod = 243 OR nRechazoCod = 244 OR nRechazoCod = 245 OR
                                                                                   nRechazoCod = 247 OR nRechazoCod = 248 OR nRechazoCod = 261 OR nRechazoCod = 262 OR nRechazoCod = 263 OR nRechazoCod = 264 OR nRechazoCod = 265 OR nRechazoCod = 267 OR nRechazoCod = 271 OR nRechazoCod = 272 OR
                                                                                   nRechazoCod = 273 OR nRechazoCod = 280 OR nRechazoCod = 286 OR nRechazoCod = 290 OR nRechazoCod = 291 OR nRechazoCod = 292 OR nRechazoCod = 293 OR nRechazoCod = 297 OR nRechazoCod = 298 OR nRechazoCod = 490 OR
                                                                                   nRechazoCod = 600 OR nRechazoCod = 819 OR nRechazoCod = 820 OR nRechazoCod = 825 OR nRechazoCod = 830 OR nRechazoCod = 835 OR nRechazoCod = 840 OR nRechazoCod = 841 OR nRechazoCod = 850 OR nRechazoCod = 851 OR
                                                                                   nRechazoCod = 852 OR nRechazoCod = 853 OR nRechazoCod = 854 OR nRechazoCod = 855 OR nRechazoCod = 856 OR nRechazoCod = 860 OR nRechazoCod = 870 OR nRechazoCod = 875 OR nRechazoCod = 880 OR nRechazoCod = 897 OR
                                                                                                                                           nRechazoCod = 601 OR nRechazoCod = 262 THEN
                                                                                   LET nCodigoError = nRechazoCod;
                                                                                   LET cMensaje = 'LA CUENTA SE ENCUENTRA EN OTRO PROCESO OPERATIVO ['|| nRechazoCod ||']';
                                                                                   EXIT FOREACH;
                                                                                ELSE
                                                                                   LET nCodigoError = 4;
                                                                                   LET cMensaje = 'LA CUENTA SE ENCUENTRA EN OTRO PROCESO OPERATIVO';
                                                                                   EXIT FOREACH;
                                                                                END IF;
                                                                        ELSE
                                                                                IF( nRechazoCod = 819 ) THEN
                                                                                        LET nCodigoError = 819;
                                                                                ELSE
                                                                                        LET nCodigoError = 4;
                                                                                END IF;
                                                                                LET cMensaje = 'LA CUENTA SE ENCUENTRA EN OTRO PROCESO OPERATIVO';
                                                                                EXIT FOREACH;
                                                                        END IF;
                                END IF;
                        END FOREACH;

                END IF;

        END IF;

        RETURN nCodigoError, cMensaje;

END FUNCTION;
