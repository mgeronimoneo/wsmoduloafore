# Análisis del Proyecto: wsmoduloafore

## 1. Archivo principal y método `main`

El archivo principal del proyecto es `wsModuloAfore.cpp`, el cual contiene la función `main`. Este archivo es el punto de entrada de la aplicación y se encarga de inicializar, servir y limpiar el entorno del servidor SOAP utilizado para exponer los servicios web.

## 2. Operaciones del Proyecto

El proyecto implementa un **servidor de servicios web (web service)** usando la librería gSOAP. Expone múltiples métodos SOAP que pueden ser llamados remotamente (posiblemente por otras aplicaciones o frontends) para realizar consultas y operaciones sobre información relacionada con AFORE y servicios de seguridad social.

### Operaciones principales expuestas:

- Consulta de afiliados
- Consulta de saldos y movimientos de cuentas
- Obtención de folios de servicio
- Validación de convivencia de marcas operativas
- Consulta y validación de resoluciones IMSS e ISSSTE
- Consulta de montos y fechas de datamart
- Consulta de saldo diario para retiros parciales
- Consulta de operaciones PMG (Pensión Mínima Garantizada)
- Consulta de saldos de vivienda y aportaciones posteriores

Cada operación está implementada como una función que inicia con el prefijo `ns2__`, siguiendo el estándar de funciones generadas por gSOAP.

## 3. Opciones de Línea de Comandos

**No se identifican directamente parámetros de línea de comandos** en el `main`. El programa está diseñado para correr como un servidor CGI/daemon SOAP, recibiendo solicitudes por red y devolviendo respuestas SOAP/XML, no mediante argumentos de consola típicos.

### Ejemplo de inicialización en main:

```c++
int main()
{
	soap_init(&soap);
	obtenerIpRemoto(cIpCliente);
	// ...
	logSoap();
	soap_serve(&soap);	 // Atender solicitudes SOAP
	// ...
}
```

## 4. Parámetros que se pueden enviar a los servicios

Los parámetros varían según el método expuesto. Ejemplos de parámetros típicos que se reciben en los distintos servicios:

- **Afiliado:** `iFolioServicio`
- **Saldo:** `Nss` (Número de Seguridad Social)
- **Folio de Servicio:** `TipoServicio`
- **Convivencia Marca:** `Opcion`, `TipoTrabajador`, `Nss`, `TipoRetiro`
- **Saldo Diario Retiro Parcial:** `Nss`, `TipoRetiro`
- **Resolución IMSS:** `Nss`, `TipoRetiro`
- **Montos Fechas Datamart:** `Nss`, `TipoRetiro`
- **Movimientos de Cuentas:** `Nss`
- **Saldo Vivienda:** `sNss`, `sFechaSolicitud`, `sFechaIncioPension`
- **Aportaciones Posteriores:** `Nss`, `FechaIniPension`, `iOpcion`

### Ejemplo de valores aceptados:

- `Nss`: "12345678901"
- `TipoServicio`: 5
- `Opcion`: 1
- `TipoTrabajador`: 2
- `TipoRetiro`: 1
- `FechaIniPension`: "2024-01-01"

## 5. Reporte Detallado

### Nombre del Proyecto

**wsmoduloafore**

### Lenguaje de Programación

- Principalmente **C++**
- Algunos módulos en **C**
- Scripts/SQL en **PLpgSQL**
- Archivos de construcción en **Makefile**

### Opciones Disponibles y Ejemplos de Ejecución

Dado que es un web service, los "comandos" son llamadas SOAP. Ejemplo con `curl` para una petición SOAP (simplificado):

```bash
curl -X POST http://localhost:8080/wsmoduloafore \
  -H "Content-Type: text/xml; charset=utf-8" \
  -d @consultaAfiliado.xml
```

Donde el archivo `consultaAfiliado.xml` tendría un contenido similar a:

```xml
<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:ns2="http://wsmoduloafore/">
   <soapenv:Header/>
   <soapenv:Body>
      <ns2:consultarAfiliado>
         <iFolioServicio>123456</iFolioServicio>
      </ns2:consultarAfiliado>
   </soapenv:Body>
</soapenv:Envelope>
```

**Cada método SOAP requiere su estructura XML específica** (ver wsdl asociado).

### Tablas y Campos Afectados

No se visualizan nombres de tablas directamente en este archivo, pero las operaciones (por el nombre de las clases y métodos) están orientadas a:

- Lectura de datos de afiliados, saldos, movimientos, resoluciones, etc.
- Escritura solamente para logs (registro de eventos, errores, auditoría).

Para cada servicio:
- **Lectura:** consultas a tablas de afiliados, saldos, movimientos, etc.
- **Escritura:** solo logs internos (no hay escritura de datos de negocio detectada en este archivo).

### Ejemplo de Ejecución de Cada Opción

Debido a que el sistema es un web service, los ejemplos de ejecución varían según el método SOAP que se quiera invocar. Aquí algunos ejemplos de cuerpos de solicitud (request) SOAP para los métodos principales:

#### 1. Consultar Afiliado

```xml
<ns2:consultarAfiliado>
   <iFolioServicio>123456</iFolioServicio>
</ns2:consultarAfiliado>
```

#### 2. Consultar Saldos

```xml
<ns2:consultarSaldos>
   <Nss>12345678901</Nss>
</ns2:consultarSaldos>
```

#### 3. Obtener Folio Servicio

```xml
<ns2:ObtenerFolioServicio>
   <TipoServicio>5</TipoServicio>
</ns2:ObtenerFolioServicio>
```

#### 4. Validar Convivencia Marca Operativa

```xml
<ns2:ValidarConvivenciaMarcaOperativa>
   <Opcion>1</Opcion>
   <TipoTrabajador>2</TipoTrabajador>
   <Nss>12345678901</Nss>
   <TipoRetiro>1</TipoRetiro>
</ns2:ValidarConvivenciaMarcaOperativa>
```

#### 5. Consultar Movimientos de Cuentas

```xml
<ns2:movimientosCuentas>
   <Nss>12345678901</Nss>
</ns2:movimientosCuentas>
```

(El resto de métodos siguen el mismo esquema de XML, cambiando los nombres y parámetros según corresponda).

---

## Nota sobre seguridad y calidad de código

Se detecta el uso incorrecto de `strlen` como segundo argumento de `snprintf` en varias partes del código. Esto puede causar errores de buffer y es un anti-patrón. Debe corregirse para usar el tamaño real del buffer de destino, no la longitud de la cadena fuente.

---

## Conclusión

El proyecto `wsmoduloafore` es un servidor web service especializado en operaciones de consulta y validación para información de AFORE, operando sobre datos de seguridad social, saldos, movimientos y resoluciones IMSS/ISSSTE, principalmente para integración con otros sistemas vía SOAP/XML.
