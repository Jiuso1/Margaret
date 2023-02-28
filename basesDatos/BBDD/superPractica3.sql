SET SERVEROUTPUT ON;--Hay que activarlo para tener salida por consola.

CREATE OR REPLACE PROCEDURE hola IS
BEGIN
    dbms_output.put_line('Hola Mundo');
END;--Cuando creas el procedure te saldrá en "Procedimientos".

CALL hola();--Escribe 'Hola Mundo' por pantalla.

CREATE OR REPLACE PROCEDURE holaAmigo(amigo VARCHAR) IS
BEGIN
    dbms_output.put_line('Hola ' || amigo);
END;

CALL holaAmigo('Jesus');--Escribe 'Hola Jesus' por pantalla.

/*Los procedures son void, las functions tienen return.
Si quieres ver el valor retornado por una function:
CALL dbms_output.put_line(function(...));
*/



