--SERVICIOSAFORE
--IMSS
ALTER TABLE cattipopensionimss ADD tiposolretaportaciones SMALLINT NOT NULL DEFAULT 0;
--ISSSTE
ALTER TABLE cattipopensionissste ADD tiposolretaportaciones SMALLINT NOT NULL DEFAULT 0;