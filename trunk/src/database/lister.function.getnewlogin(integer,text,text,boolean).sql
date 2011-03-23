CREATE OR REPLACE FUNCTION public.getnewlogin(ploginid integer, ploginstr text, ploginpwd text, pisosauth boolean)
 RETURNS integer
 LANGUAGE plpgsql
AS $function$
DECLARE
   floginid INTEGER;
BEGIN
   SELECT loginid INTO floginid FROM logins WHERE loginstr = ploginstr and loginpwd = ploginpwd;
   IF FOUND THEN
     UPDATE logins SET isosauth = pisosauth WHERE loginid = floginid;
     RETURN floginid;
   ELSE
     INSERT INTO logins(loginstr, loginpwd, isosauth) VALUES(ploginstr, ploginpwd, pisosauth) RETURNING loginid INTO floginid;
     RETURN floginid;
   END IF;
     
   RETURN 0;
END;
$function$
