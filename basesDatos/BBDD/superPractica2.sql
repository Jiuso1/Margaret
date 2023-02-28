/*Para ejecutar selecciona el bloque deseado y pulsa Ctrl+Enter.*/
/*P.2*/
DESC EI.ASIGNATURA;
--
SELECT constraint_name,constraint_type
FROM all_constraints
WHERE table_name='ASIGNATURA';
/*P.3*/
SELECT IDASIG,NOMBRE,ESP
FROM EI.asignatura
WHERE CURSO = 3 AND CRÉDITOS>4.5;
/*P.5*/
SELECT A.idasig AS CÓDIGO,NOMBRE,esp AS ESPECIALIDAD
FROM EI.asignatura A
WHERE A.curso=3 AND A.créditos>4.5;
/*P.7*/
SELECT * FROM EI.profesor;
/*P.10*/
SELECT DISTINCT tipo FROM EI.ordenador;
/*P.13 Mostrar el nombre de los -nos, el número de hermanos y el descuento que le corresponde (el descuento es de 300 € por hermano), ordenados de mayor amenor según el descuento, y en caso de igualdad, ordenadosalfabéticamentepornombre*/
SELECT NOMBRE,NH,300*NH AS DESCUENTO 
FROM EI.alumno
ORDER BY DESCUENTO DESC, NOMBRE;
/*P.14 Obtenereldniynombredelosalumnosnacidosentre 1970 y 1974, y cuya localidadde nacimiento sea Huelva o Cádiz*/
SELECT * FROM EI.alumno;
SELECT DNI,NOMBRE,LUGAR,FECHANAC 
FROM EI.alumno
WHERE (LUGAR IN('Huelva','Cádiz')) AND (FECHANAC BETWEEN '01/01/70' AND'31/12/74');
/*P.16 Otra forma de hacerlo*/
SELECT DNI,NOMBRE,LUGAR,FECHANAC 
FROM EI.alumno
WHERE (LUGAR IN('Huelva','Cádiz')) AND (EXTRACT(year FROM FECHANAC) BETWEEN 1970 AND 1974);
/*P.17. Obtener el nombre de todos los alumnos cuyo nombre empiece por la letra M y hayan nacido en una ciudad cuyo nombre tenga, al menos,6caracteresperonocomienceporlaletraP.*/
SELECT NOMBRE
FROM EI.alumno
WHERE (NOMBRE LIKE 'M%') AND (LUGAR NOT LIKE 'P%' AND LUGAR LIKE '______%');
/*P.19. Obtener un listado de los años de nacimiento de los alumnos, ordenadoscrecientemente*/
SELECT DISTINCT EXTRACT(year FROM FECHANAC) 
FROM EI.alumno
ORDER BY EXTRACT(year FROM FECHANAC) ASC;
/*P.21 Obtener compañias cuya web tenga et de por medio en su url y terminen en .com*/
SELECT NOMBRE FROM MF.COMPAÑIA
WHERE (WEB LIKE '%et%') AND (WEB LIKE '%com');
/*P.23*/
--TO_CHAR(numero/fecha,XXXX) te transforma lo que sea con el formato que sea a cadena.
--EXTRACT XXXX FROM DATE/TIMESTAM te extrae lo que sea de una fecha. Puede retornar número o cadena en función de lo que le pidas.
/*P.30 Obtenerelnombreydirecciónde losclientesnacidosen1973o1985 y cuyo código postal comience por 15, ordenado ascendentemente por el nombrey,encasodeigualdad,descendentementeporladirección*/
SELECT NOMBRE,DIRECCION
FROM MF.CLIENTE
WHERE (EXTRACT(YEAR FROM MF.CLIENTE.F_NAC) IN(1973,1985)) AND CP LIKE '15%'
ORDER BY NOMBRE ASC,DIRECCION DESC;
/*P.32 Obtener el teléfono de destino de las llamadas realizadas desde el número“666010101”,enelaño2006.*/
SELECT TF_DESTINO 
FROM MF.LLAMADA
WHERE TF_ORIGEN='666010101' AND EXTRACT(YEAR FROM FECHA_HORA)=2006;
/*P.34 Obtener los números de teléfono que han llamado alguna vez al “666010101”,entrelas10:00ylas12:00horas*/
SELECT TF_ORIGEN
FROM MF.LLAMADA
WHERE TF_DESTINO=666010101 AND EXTRACT(HOUR FROM FECHA_HORA) BETWEEN 10 AND 12;
/*P.38
Las tablas están relacionadas por medio de las claves foráneas o externas.
El join se utiliza para unir estas tablas relacionadas por foreign keys.
Tipos de Join (A ... join B):
-INNER JOIN: intersección de A y B.
-LEFT JOIN: intersección de A y B + todo A.
-RIGHT JOIN: intersección de A y B + todo B.
-FULL JOIN: A + B.
-Variantes que podamos nosotros pensar.
*/
--P.52 Obtener los nombres de las asignaturas junto con el nombredelprofesorresponsable
SELECT A.NOMBRE AS NOMBRE_ASIGNATURA,P.NOMBRE AS NOMBRE_PROFESOR
FROM EI.ASIGNATURA A
JOIN EI.PROFESOR P
ON A.PROF = P.NPR;
--P.55 Obtener los números de los alumnos que se han matriculadoenBasesdeDatos Ienelcurso2002-2003
SELECT * FROM EI.MATRICULA;
SELECT AL.NAL 
FROM EI.ALUMNO AL
JOIN EI.MATRICULA M
ON AL.NAL = M.ALUM
JOIN EI.ASIGNATURA A
ON M.IDASIG = A.IDASIG
WHERE (A.NOMBRE = 'Bases de Datos I') AND M.AÑO=2002;--Enterito bien, vamooooo. A la primera XD.
--P.58 Obtenerlosnombresde losalumnosquehanaprobado la asignatura Algoritmos y Estructuras de Datos I en la convocatoriadefebrero_juniode2001
SELECT AL.NOMBRE FROM 
EI.MATRICULA 
JOIN EI.ALUMNO AL
ON EI.MATRICULA.ALUM = AL.NAL
JOIN EI.ASIGNATURA A
ON MATRICULA.IDASIG=A.IDASIG
WHERE (A.NOMBRE = 'Algoritmos y Estructuras de Datos I') AND (FEB_JUN>=5);--Perfecto también.
--P.61 Obtener un listado con el número de despacho, pero sólodeaquellosdondehayalmenos2profesores
SELECT PROF.DESPACHO,COUNT(*) AS NPROF  --Siempre que usemos Group By hay que poner Count(*) , y el count va a la derecha siempre.
FROM EI.PROFESOR PROF
GROUP BY PROF.DESPACHO  
HAVING COUNT(*)>=2; --Having es como un where que se usa para GROUP BY. Solo puede ir cuando está este.
--Para que funcionase el Group By hemos tenido por cojones que crear los aliases de 93,94.
--P.65 Obtener una lista con todas las asignaturas de las que es responsable o docente la profesora ‘Dolores Toscano Barriga’
SELECT *
FROM EI.ASIGNATURA
WHERE PROF=(SELECT NPR FROM EI.PROFESOR
WHERE NOMBRE='Dolores Toscano Barriga');--Con subconsultas y todo, bendito sea el libro de SQL.
--P.70 Obtener los nombres de los alumnos que no se han presentado a ‘Bases de Datos I’ en diciembre de 2002 por haberla aprobado en una convocatoria anterior del mismo año(2002).
SELECT NOMBRE
FROM EI.MATRICULA
JOIN EI.ALUMNO
ON EI.ALUMNO.NAL = EI.MATRICULA.ALUM
WHERE (EI.MATRICULA.FEB_JUN>=5 OR EI.MATRICULA.SEP>=5) AND EI.MATRICULA.IDASIG=(SELECT EI.ASIGNATURA.IDASIG FROM EI.ASIGNATURA WHERE NOMBRE='Bases de Datos I');
--P.73 Mostrarelcódigoycostede las tarifas juntoconelnombredela compañíaque las ofrecen, deaquellas tarifas cuyadescripción indique queotraspersonasdebenestartambiénenlamismacompañía
SELECT NOMBRE,TARIFA,COSTE    
FROM MF.TARIFA
JOIN MF.COMPAÑIA
ON MF.TARIFA.COMPAÑIA = MF.COMPAÑIA.CIF
WHERE DESCRIPCION LIKE '%en la compañía';
--SELECT * FROM ALL_TABLES  --para ver todas las tablas de la database se usa esta línea.
--WHERE OWNER='MF';
--P.77 Nombre y número de teléfonos de aquellos abonados con contratoquetienentarifas inferioresa0.20€.
SELECT * FROM EI.MATRICULA;
SELECT * FROM EI.ASIGNATURA;
SELECT * FROM EI.ALUMNO;
SELECT * FROM EI.PROFESOR;
SELECT * FROM EI.ORDENADOR;
SELECT * FROM EI.RECOMENDACIONES;
---------------------------------
SELECT * FROM MF.TARIFA;--COSTE...
SELECT * FROM MF.COMPAÑIA;--TARIFA,COMPAÑIA
SELECT * FROM MF.CLIENTE;--DNI
SELECT * FROM MF.TELEFONO;--Contrato (Tipo C)*/
SELECT * FROM MF.LLAMADA;--Llamadas
---------------------------------
SELECT DISTINCT(NOMBRE),NUMERO,TIPO,COSTE FROM MF.CLIENTE
JOIN MF.TELEFONO
ON MF.CLIENTE.DNI = MF.TELEFONO.CLIENTE
JOIN MF.TARIFA
ON MF.TELEFONO.TARIFA = MF.TARIFA.TARIFA
WHERE COSTE<0.20 AND TIPO='C';  --El problema de mi consulta es que hay clientes con varios nº de teléfono que cumplen esta condición. La cosa es que hay gente que no aparece en la solución, pero que creo que están bien.
--P.80 Obtenerelcódigode las tarifas, elnombrede las compañías, los números de teléfono y los puntos, de aquellos teléfonos que se contrataronenelaño2006yquehayanobtenidomásde200puntos
SELECT MF.TARIFA.TARIFA,MF.COMPAÑIA.NOMBRE,MF.TELEFONO.NUMERO,MF.TELEFONO.PUNTOS 
FROM MF.TARIFA
JOIN MF.COMPAÑIA
ON MF.TARIFA.COMPAÑIA = MF.COMPAÑIA.CIF
JOIN MF.TELEFONO
ON MF.TARIFA.TARIFA = MF.TELEFONO.TARIFA
WHERE (PUNTOS>200) AND (EXTRACT(YEAR FROM F_CONTRATO) = 2006);--Los join siempre se deben hacer antes que el where.
--P.83 Obtener los números de teléfono (origen ydestino), asícomo el tipo de contrato, de los clientes que alguna vez hablaron por teléfono entrelas8ylas10delamañana.
SELECT * 
FROM MF.LLAMADA
JOIN MF.TELEFONO
ON MF.LLAMADA.TF_ORIGEN = MF.TELEFONO.NUMERO
WHERE TO_CHAR(MF.LLAMADA.FECHA_HORA,'hh:mi')>='08:00'  AND TO_CHAR(MF.LLAMADA.FECHA_HORA,'hh:mi')<'10:00';--P.26. DUDA. Coincide todo con la solución menos la hora, que en vez de 09 a veces me pone 21. No entiendo.
--P.87 Interesa conocer los nombres y números de teléfono de los clientes (origen y destino) que, perteneciendo a compañías distintas, mantuvieron llamadas que superaron los 15 minutos. Se desea conocer, también, la fecha y la hora de dichas llamadas así como la duracióndeesasllamadas
SELECT M1.NOMBRE,M2.NOMBRE,DURACION
FROM MF.LLAMADA T1
JOIN MF.TELEFONO T2
ON T1.TF_ORIGEN = T2.NUMERO
JOIN MF.TELEFONO T3
ON T1.TF_DESTINO = T3.NUMERO
JOIN MF.CLIENTE M1
ON T2.CLIENTE = M1.DNI
JOIN MF.CLIENTE M2
ON T3.CLIENTE = M2.DNI
WHERE T2.COMPAÑIA <> T3.COMPAÑIA AND DURACION > 900;   --Para los join usa aliases siempre, para evitar errores de sintaxis.
--Perfecto.
--P.98.Obtener una lista de los alumnos que hayan nacido en la misma ciudad que Samuel Toscano Villegas y tengan el mismo númerodehermanosqueBeatrizRicoVázquez
SELECT * 
FROM EI.ALUMNO
WHERE LUGAR = (SELECT LUGAR FROM EI.ALUMNO WHERE NOMBRE = 'Samuel Toscano Villegas') AND NH = (SELECT NH FROM EI.ALUMNO WHERE NOMBRE = 'Beatriz Rico Vázquez');--Perfe, con subconsultas y todo.
--P.102.Obtener una lista con los alumnosmatriculados en BDII y no en BDI.
SELECT * 
FROM EI.ALUMNO AL
WHERE (AL.NAL IN (SELECT M.ALUM FROM EI.MATRICULA M JOIN EI.ASIGNATURA A ON M.IDASIG = A.IDASIG WHERE A.NOMBRE = 'Bases de Datos II'))
AND (AL.NAL NOT IN (SELECT M.ALUM FROM EI.MATRICULA M JOIN EI.ASIGNATURA A ON M.IDASIG = A.IDASIG WHERE A.NOMBRE = 'Bases de Datos I'));
--Perfecto, por subconsoltas lo hemos resuelto.
--P.107 Obtener una lista de los profesores que tienen más antigüedadque alguno de los profesores del despacho FC-7366 (^  sincontarlosdelpropiodespachoFC-7366)
--Es como poner LIMIT 1, pero LIMIT no funciona en Oracle
SELECT *
FROM EI.PROFESOR P
WHERE P.ANT <= (SELECT PROF.ANT FROM EI.PROFESOR PROF 
WHERE PROF.DESPACHO = 'FC-7366' AND ROWNUM <= 1
/*ORDER BY PROF.ANT DESC*/)
AND P.DESPACHO <> 'FC-7366';
--P.110 Obteneruna lista con los nombres de los alumnos que hayan sacado una nota más alta en septiembre que la más alta de las notas de febrero/junio , en elaño2002 yen la asignaturaBases de Datos I
SELECT *
FROM EI.ALUMNO AL
JOIN EI.MATRICULA M
ON AL.NAL = M.ALUM
JOIN EI.ASIGNATURA A
ON M.IDASIG = A.IDASIG
WHERE M.AÑO = 2002 AND M.SEP > (SELECT * FROM (SELECT MAT.FEB_JUN FROM EI.MATRICULA MAT WHERE MAT.FEB_JUN IS NOT NULL ORDER BY MAT.FEB_JUN DESC) WHERE ROWNUM <= 1) AND A.NOMBRE = 'Bases de Datos I';
--Puedes hacer un SELECT de otro SELECT, piensa que el segundo SELECT es una tabla de la que puedes seleccionar nuevamente.
--P. 114 Obtenerlafecha(día,mesyaño)enlaqueserealizóla llamada demayorduración
SELECT TO_CHAR(FECHA_HORA,'DD'||'/'||'MM'||'/'||'YYYY') FROM(SELECT * 
FROM MF.LLAMADA
ORDER BY DURACION DESC) WHERE ROWNUM <= 1;  --Con ROWNUM le digo número de fila. Con to_char hacemos cosas bonitas como esta, concatenando y poniéndolo con el formato que quiera.
--P.117 Obtenerelnombredelosabonadosdela compañía‘Aotra’con elmismotipodetarifaqueladelteléfono“654123321”
SELECT *
FROM MF.TELEFONO TEL
JOIN MF.COMPAÑIA COM
ON TEL.COMPAÑIA = COM.CIF
JOIN MF.CLIENTE CLI
ON TEL.CLIENTE = CLI.DNI
WHERE COM.NOMBRE = 'Aotra' AND TEL.TARIFA = (SELECT T.TARIFA FROM MF.TELEFONO T WHERE T.NUMERO = 654123321);
--P.121 Mostrar, utilizando para ello una subconsulta, el número de teléfono, fecha de contrato y tipode los abonados que han llamadoa teléfonos de clientes de fuera de la provincia de La Coruña durante el mesdeoctubrede2006.
SELECT LLAM.TF_ORIGEN,TEL.F_CONTRATO,TEL.TIPO,LLAM.FECHA_HORA,CLI.CIUDAD
FROM MF.LLAMADA LLAM
JOIN MF.TELEFONO TEL
ON LLAM.TF_ORIGEN = TEL.NUMERO
JOIN MF.TELEFONO TEL2
ON LLAM.TF_DESTINO = TEL2.NUMERO
JOIN MF.CLIENTE CLI
ON TEL2.CLIENTE = CLI.DNI
WHERE CLI.CIUDAD<>'La Coruña' AND EXTRACT(year FROM LLAM.FECHA_HORA) = 2006 AND EXTRACT(month FROM LLAM.FECHA_HORA) = 10;--Lo he hecho tanto por subconsultas como por esta manera y sigue sacando 14. Recuerda usar siempre que hagas join o en cualquier caso usa aliases, siempre usa aliases porque hacen la vida más fácil y con menos errores XD.
--P.124 Se necesita conocer el nombre de los clientes que tienen teléfonos con tarifa “dúo” pero no “autónomos”. Utilice subconsultas paraobtenerlasolución.
--Esta vez sí usaré subconsultas como me exige el enunciado.
SELECT CLI.NOMBRE
FROM MF.CLIENTE CLI
WHERE CLI.DNI IN(SELECT TEL.CLIENTE FROM MF.TELEFONO TEL WHERE TEL.TARIFA='dúo') AND CLI.DNI NOT IN(SELECT TEL2.CLIENTE FROM MF.TELEFONO TEL2 WHERE TEL2.TARIFA='autónomos');--Hicieron falta dos subconsultas y no una porque un cliente puede tener varios teléfonos.
--P.127 Obtener mediante subconsultas los nombres de clientes y números de teléfono que aquellos que hicieron llamadas a teléfonos delacompañíaPetafónperonoAotra
SELECT *--CLI.NOMBRE,TEL.NUMERO
FROM MF.CLIENTE CLI
JOIN MF.TELEFONO TEL
ON CLI.DNI=TEL.CLIENTE
WHERE TEL.NUMERO IN (SELECT LLAM.TF_ORIGEN FROM MF.LLAMADA LLAM JOIN MF.TELEFONO TEL2 ON LLAM.TF_DESTINO=TEL2.NUMERO JOIN MF.COMPAÑIA COMP ON COMP.CIF=TEL2.COMPAÑIA WHERE COMP.NOMBRE='Petafón')
AND TEL.NUMERO NOT IN (SELECT LLAM.TF_ORIGEN FROM MF.LLAMADA LLAM JOIN MF.TELEFONO TEL2 ON LLAM.TF_DESTINO=TEL2.NUMERO JOIN MF.COMPAÑIA COMP ON COMP.CIF=TEL2.COMPAÑIA WHERE COMP.NOMBRE='Aotra');--Perfecto, a la primera, qué maravilla.
--P.132 Nombre de los clientes de la compañía Kietostar que hicieron lasllamadasdemayorduraciónenseptiembrede2006
SELECT CLI.NOMBRE
FROM MF.LLAMADA LLAM
JOIN MF.TELEFONO TEL2
ON LLAM.TF_ORIGEN=TEL2.NUMERO
JOIN MF.CLIENTE CLI
ON TEL2.CLIENTE=CLI.DNI
WHERE TF_ORIGEN IN(SELECT TEL.NUMERO FROM MF.TELEFONO TEL JOIN MF.COMPAÑIA COMP ON TEL.COMPAÑIA=COMP.CIF WHERE COMP.NOMBRE='Kietostar') AND EXTRACT(YEAR FROM LLAM.FECHA_HORA)=2006 AND EXTRACT(MONTH FROM LLAM.FECHA_HORA)=9;
ORDER BY LLAM.DURACION DESC;--Perfectísimo. Lo único es que hay que tener cuidado con poner el join, el join NO se puede poner NUNCA después del where.
--P.136 Se necesita conocer el nombre de los clientes que tienen teléfonos con fecha de contrataciónanterioraalguno de los teléfonos deRamónMartínez Sabina, excluido, claro, elpropioRamónMartínez Sabina.
SELECT CLI.NOMBRE,TEL.F_CONTRATO FROM
MF.CLIENTE CLI
JOIN MF.TELEFONO TEL
ON CLI.DNI=TEL.CLIENTE
WHERE CLI.NOMBRE<>'Ramón Martínez Sabina' AND TEL.F_CONTRATO<ANY(SELECT TEL2.F_CONTRATO FROM MF.TELEFONO TEL2 JOIN MF.CLIENTE CLI2 ON CLI2.DNI=TEL2.CLIENTE WHERE CLI2.NOMBRE='Ramón Martínez Sabina');--Forma rara de casi resolverlo:(SELECT MIN(TEL2.F_CONTRATO) FROM MF.TELEFONO TEL2 JOIN MF.CLIENTE CLI2 ON CLI2.DNI=TEL2.CLIENTE WHERE CLI2.NOMBRE='Ramón Martínez Sabina');
--P.145 Listado de los despachos donde hay profesores que no son responsablesdeningunaasignatura
SELECT DISTINCT PROF.DESPACHO,
FROM EI.PROFESOR PROF
WHERE PROF.nPr NOT IN(SELECT DISTINCT ASIG2.PROF FROM EI.ASIGNATURA ASIG2)
ORDER BY PROF.DESPACHO;--No funciona, tampoco tengo tiempo para revisar por qué. De aquí lo único importante a saber es que en vez de usar EXISTS O NOT EXISTS podemos usar IN y NOT IN simplemente.
--P.149 Listado de los alumnos que se han matriculado de alguna asignatura en el año 2000 ó 2002 , y de ninguna asignatura en el año2001
SELECT AL.NOMBRE
FROM EI.ALUMNO AL
JOIN EI.MATRICULA MAT
ON AL.NAL=MAT.ALUM
JOIN EI.ASIGNATURA ASIG
ON MAT.IDASIG=ASIG.IDASIG
WHERE AL.NAL IN (SELECT MAT2.ALUM FROM EI.MATRICULA MAT2 WHERE MAT2.AÑO IN(2000,2002))
AND AL.NAL NOT IN(SELECT MAT3.ALUM FROM EI.MATRICULA MAT3 WHERE MAT3.AÑO=2001);--Jodidamente perfecto. Programa de manera natural, no pienses en la estructura que enseñen, piensa como te dice tu cabeza que pienses.
--P.151 Obtener el nombre de los profesores, y los números de sus despachos,deaquellosprofesoresquenocompartendespacho
SELECT *
FROM EI.PROFESOR PROF
WHERE PROF.DESPACHO NOT IN(SELECT PROF2.DESPACHO FROM EI.PROFESOR PROF2 WHERE PROF2.NOMBRE<>PROF.NOMBRE);--Perfecto, usa la lógica a tope. Usa todos tus conocimientos de SQL para hacer una consulta, hazlo a tu manera, respetando la elegancia que tu cerebro pide.
--P.155 MF17.Utilizandoconsultas correlacionadas,mostrarelnombrede los abonadosquehanllamadoeldía‘16/10/06’
SELECT CLI.NOMBRE
FROM MF.CLIENTE CLI
WHERE CLI.DNI IN (SELECT TEL.CLIENTE
FROM MF.TELEFONO TEL
WHERE TEL.NUMERO IN(SELECT DISTINCT TF_ORIGEN
FROM MF.LLAMADA LLAM
WHERE TO_CHAR(LLAM.FECHA_HORA,'DD'||'/'||'MM'||'/'||'YYYY')='16/10/2006')); --2 subconsultas, vamoo.
--P.157 Utilizando consultas correlacionadas, obtener elnombrede los abonadosquehanrealizadollamadasdemenosde1minutoymedio
SELECT CLI.NOMBRE
FROM MF.CLIENTE CLI
JOIN
(SELECT CLIENTE
FROM MF.TELEFONO TEL
WHERE TEL.NUMERO IN(SELECT DISTINCT TF_ORIGEN
FROM MF.LLAMADA LLAM
WHERE LLAM.DURACION<90))
ON CLI.DNI=CLIENTE;--Había que quitar el alias, porque cuando retornamos solo la columna y retornamos la tabla hay que quitar el alias para que funcione.
--P.159 Utilizando consultas correlacionadas, obtener elnombrede los abonadosdela compañía‘KietoStar’quenohicieronningunallamada elmesdeseptiembre
SELECT TEL.NUMERO,CLI.NOMBRE
FROM MF.TELEFONO TEL
JOIN MF.CLIENTE CLI
ON CLI.DNI=TEL.CLIENTE
WHERE TEL.NUMERO NOT IN(SELECT TF_ORIGEN FROM MF.LLAMADA LLAM WHERE EXTRACT(MONTH FROM LLAM.FECHA_HORA)=9) AND TEL.COMPAÑIA=(SELECT COMPA.CIF FROM MF.COMPAÑIA COMPA WHERE COMPA.NOMBRE='Kietostar');
--P.162 Utilizando consultas correlacionadas, mostrar todos los datos de los teléfonos que hayan llamado al número 654234234 pero no al 666789789
SELECT *
FROM MF.TELEFONO TEL
WHERE TEL.NUMERO IN
(SELECT TF_ORIGEN
FROM MF.LLAMADA LLAM
WHERE LLAM.TF_DESTINO=654234234)
AND TEL.NUMERO NOT IN
(SELECT TF_ORIGEN
FROM MF.LLAMADA LLAM
WHERE LLAM.TF_DESTINO=666789789);--Perfecto.
--P.165 Utilizando consultas correlacionadas, obtener el nombre y número de teléfono de los clientes de la compañía Kietostar que no hanhechollamadasaotros teléfonosdelamismacompañía
--Vamos a hacer primero un problema más sencillo. Luego extrapolamos a lo que necesitemos. Por ejemplo:
--¿Teléfonos que no hayan llamado al 654345345?
SELECT CLI.NOMBRE
FROM MF.TELEFONO TEL
JOIN (SELECT DISTINCT LLAM.TF_ORIGEN 
FROM MF.LLAMADA LLAM
WHERE LLAM.TF_ORIGEN NOT IN(SELECT DISTINCT LLAM2.TF_ORIGEN FROM MF.LLAMADA LLAM2 WHERE LLAM2.TF_DESTINO=654345345))
ON TEL.NUMERO=TF_ORIGEN
JOIN MF.CLIENTE CLI
ON TEL.CLIENTE=CLI.DNI;--Perfecto el subproblema. Ahora vamos a hacer el problema que nos piden.
SELECT LLAM2.TF_ORIGEN,CLI.NOMBRE   --Teléfonos que son de Kietostar
FROM MF.LLAMADA LLAM2
JOIN MF.TELEFONO TEL2
ON LLAM2.TF_ORIGEN=TEL2.NUMERO
JOIN MF.COMPAÑIA COMP2
ON TEL2.COMPAÑIA=COMP2.CIF
JOIN MF.CLIENTE CLI
ON TEL2.CLIENTE=CLI.DNI
WHERE COMP2.NOMBRE='Kietostar' AND LLAM2.TF_DESTINO NOT IN
/*Nº de teléfonos que han llamado a teléfonos de Kietostar*/(SELECT DISTINCT LLAM.TF_ORIGEN
FROM MF.LLAMADA LLAM
JOIN MF.TELEFONO TEL
ON LLAM.TF_DESTINO=TEL.NUMERO
JOIN MF.COMPAÑIA COMP
ON TEL.COMPAÑIA=COMP.CIF
WHERE COMP.NOMBRE='Kietostar');--Este no funciona, está mal. Npi de por qué, prefiero seguir avanzando Xd...
--P.172 Obtener los nombres de todos los profesores junto con lasasignaturasdelasquesonresponsables
--Mirar tipos de join, si joineamos normal los profes que no tengan asociada una asignatura desaparecen. En cambio con left join (en este caso) siguen.
SELECT PROF.NOMBRE,ASIG.NOMBRE
FROM EI.PROFESOR PROF
LEFT JOIN EI.ASIGNATURA ASIG
ON PROF.NPR=ASIG.PROF;
--P.178
--Obtén el valor absoluto del SALARIO-10000 para todas las filasdelatablaEMPLE:
--SQL>SELECTAPELLIDO,SALARIO,ABS(SALARIO-10000) FROMEMPLE;
--Pruebaconnúmerospositivosynegativos:
SELECT CEIL(20.3),CEIL(16),CEIL(-20.3),CEIL(-16) FROM DUAL;
--Pruebaconnúmerospositivosynegativos:
SELECT FLOOR(20.3), FLOOR(16), FLOOR(-20.3), FLOOR(-16) FROM DUAL;
--Pruebaconnúmerospositivosynegativos:
SELECT MOD(11,4), MOD(10,-15), MOD(-10,-3), MOD(10.4,4.5) FROM DUAL;
--A partir de la tabla EMPLE obtenemos el SALARIO, laCOMISIONylasumadeambos:
--SQL>SELECT SALARIO,COMISION, SALARIO+COMISION FROM EMPLE; Recuerda, la suma de nulos da nulo, y la suma de algo con nulo da nulo también.
--SQL>SELECTSALARIO,COMISION, SALARIO+NVL(COMISION,0) FROMEMPLE; Si usamos NVL podemos decirle qué valor tomará en caso de ser nulo.
--power
SELECT POWER(2,4),POWER(2,-4),POWER(3.5,2.4),POWER(4.5,2) FROM DUAL;
--round
SELECT ROUND(1.56,1), ROUND(1.56), ROUND(1.2234,2), ROUND(1.2676,3)FROM DUAL;
SELECT ROUND(145.5, -1),ROUND(145.5, -2),ROUND(145.5, -3), ROUND(141,-1) FROM DUAL;
--sign
SELECT SIGN(-10), SIGN(10)FROM DUAL;
--trunc
SELECT TRUNC(1.5634, 1), TRUNC(1.1684, 2), TRUNC(1.662) FROM DUAL;
SELECT TRUNC(187.98,-1), TRUNC(187.98,-2), TRUNC(187.98,-3) FROM DUAL;
--sqrt
SELECT SQRT(25), SQRT(25.6) FROM DUAL;
--AVG
SELECT AVG(SALARIO) FROM EMPLE WHERE DEPT_NO=10;
/*COUNT
SELECTCOUNT(*) FROMEMPLE;   Incluye también valores nulos y repetidos.
SELECTCOUNT([ALL] COMISION) FROMEMPLE;  Cuenta solo los valores del campo comisión que no son nulos.
SELECTCOUNT(DISTINCTOFICIO) ‘OFICIOS’ FROMEMPLE;
--También están MAX, MIN, SUM, VARIANCE...
--También hay funciones para cadenas.
--Ejercicios:
*/
--Se desea saber cuántos despachos ocupan los profesores de laescuela
SELECT COUNT(DISTINCT PROF.DESPACHO)
FROM EI.PROFESOR PROF;
--Obtener el número de matriculados, la nota máxima, la mínima y la nota media de la asignatura ‘Bases de Datos I’ en la convocatoriadeseptiembrede2002
SELECT COUNT(SEP) AS N_MATRICULADOS,MAX(MAT.SEP),MIN(MAT.SEP),AVG(MAT.SEP) --Tienen mal la presentación, cuentan los que aprobaron en otra convocatoria cuando en teoría solo estábamos hablando de la convocatoria de septiembre.
FROM EI.MATRICULA MAT
JOIN EI.ASIGNATURA ASIG
ON MAT.IDASIG=ASIG.IDASIG
WHERE MAT.AÑO=2002 AND ASIG.NOMBRE='Bases de Datos I';
--P.196 Obtener,mediante una consulta correlacionada, los nombres delasasignaturasquetienenrecomendadas2ómásasignaturas.
SELECT ASIG.NOMBRE,COUNT(IDASIG1) AS N_RECOMENDACIONES
FROM EI.ASIGNATURA ASIG
JOIN EI.RECOMENDACIONES RECOM
ON ASIG.IDASIG=RECOM.IDASIG1
GROUP BY ASIG.NOMBRE
HAVING (COUNT(IDASIG1)>=2);--Maravilloso, everything's ok!
--P.198 Obtener,mediante una consulta correlacionada, los nombres de los profesores que tienenmás antigüedad que, almenos, otros cincoprofesores
SELECT PROF.NOMBRE
FROM EI.PROFESOR PROF
WHERE (SELECT COUNT(*) FROM EI.PROFESOR PROF2
        WHERE PROF.ANT<PROF2.ANT)>=5;   --Lo comprueba para cada profesor.
--P.204 EI-26. Para cadaasignatura yañoacadémico,mostrar el nombrede la asignatura, el año, el númerodealumnosquesehanpresentado ylanotamediaobtenidaenlaconvocatoriadefebrero_junio
SELECT ASIG.NOMBRE,MAT.AÑO,COUNT(*),AVG(MAT.FEB_JUN)
FROM EI.MATRICULA MAT
JOIN EI.ASIGNATURA ASIG
ON MAT.IDASIG=ASIG.IDASIG
GROUP BY ASIG.NOMBRE,MAT.AÑO;--Puedes hacer un group by de varias columnas. Recuerda siempre poner los nombres de lo que te interesa arriba en el select. 
--P.207 Obtener un listado con el nombre del alumno, el nombre de la asignatura y el número de veces que se ha matriculado en esa asignatura, perosólocuandosehayamatriculado3ómásaños.
SELECT ALUM.NOMBRE,ASIG.NOMBRE,COUNT(*)
FROM EI.MATRICULA MAT
JOIN EI.ALUMNO ALUM
ON MAT.ALUM=ALUM.NAL
JOIN EI.ASIGNATURA ASIG
ON MAT.IDASIG=ASIG.IDASIG
GROUP BY ALUM.NOMBRE,ASIG.NOMBRE
HAVING COUNT(*)>=3;--Perfecto.
--P.209 Nombre de los alumnos que hayan sacado más de un 5 de notamediaenjuniodel2002
SELECT ALUM.NOMBRE,AVG(MAT.FEB_JUN),COUNT(*)
FROM EI.MATRICULA MAT
JOIN EI.ALUMNO ALUM
ON MAT.ALUM=ALUM.NAL
WHERE MAT.AÑO=2002
GROUP BY ALUM.NOMBRE
HAVING AVG(MAT.FEB_JUN) >= 5;--PERFE.
--P.212 Nombre de las asignaturas y número de alumnos matriculados de las asignaturas donde se hayan matriculado más alumnosenelaño2002.
SELECT ASIG2.NOMBRE,COUNT(*)
FROM EI.MATRICULA MAT2
JOIN EI.ASIGNATURA ASIG2
ON MAT2.IDASIG=ASIG2.IDASIG
WHERE MAT2.AÑO=2002
GROUP BY ASIG2.NOMBRE
HAVING COUNT(*) = (SELECT  MAX(COUNT(*))    --Recuerda, max va antes de count pero nunca puede ir antes de una tabla. Max va siempre asociado a una columna, siempre.
FROM EI.MATRICULA MAT
JOIN EI.ASIGNATURA ASIG
ON MAT.IDASIG=ASIG.IDASIG
WHERE MAT.AÑO=2002
GROUP BY ASIG.NOMBRE);
--P.217 Obtener el número total de alumnos quehan suspendidoen cadaasignaturaen juniode2002, pero sólodeaquellas asignaturas enlasquesehayanmatriculadomásde50alumnos
--Este ejercicio no tiene sentido, más de 50 alumnos pero las tablas tienen 20 filas en total XD, y en la solución pone que te debe salir en blanco. Maravilloso.
--P.224 DIVISIÓN
--No nos la estudiamos, es con not exists pero nos daiwá.
--P.230 Mostrar la compañía con la que se realizaron más llamadas duranteeldía16/10/06
SELECT COMP.NOMBRE,COUNT(*)
FROM MF.LLAMADA LLAM
JOIN MF.TELEFONO TEL
ON LLAM.TF_ORIGEN=TEL.NUMERO
JOIN MF.COMPAÑIA COMP
ON TEL.COMPAÑIA=COMP.CIF
WHERE TO_CHAR(LLAM.FECHA_HORA,'DD/MM/YYYY')='16/10/2006'
GROUP BY COMP.NOMBRE
HAVING COUNT(*) = (SELECT MAX(COUNT(*))
FROM MF.LLAMADA LLAM
JOIN MF.TELEFONO TEL
ON LLAM.TF_ORIGEN=TEL.NUMERO
JOIN MF.COMPAÑIA COMP
ON TEL.COMPAÑIA=COMP.CIF
WHERE TO_CHAR(LLAM.FECHA_HORA,'DD/MM/YYYY')='16/10/2006'
GROUP BY COMP.NOMBRE);--Es simplemente copiar y pegar la consulta anterior pero poniendo select max count... Como en el ejemplo anterior.
--P.235 Obtener los números de teléfono y los nombres de los abonados que han llamado a todos los números a los que se llamó desdeel654345345enoctubrede2006
SELECT *
FROM MF.LLAMADA LLAM;
--dejo estas consultas para el día antes del examen repasar.

