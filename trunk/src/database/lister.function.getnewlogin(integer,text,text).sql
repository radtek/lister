CREATE OR REPLACE FUNCTION public.getnewlogin(ploginid integer, ploginstr text, ploginpwd text)
 RETURNS integer
 LANGUAGE plpgsql
AS $function$
DECLARE
   floginid INTEGER;
BEGIN
   SELECT loginid INTO floginid FROM logins WHERE loginstr = ploginstr and loginpwd = ploginpwd;
   IF FOUND THEN
     RETURN floginid;
   ELSE
     INSERT INTO logins(loginstr, loginpwd) VALUES(ploginstr, ploginpwd) RETURNING loginid INTO floginid;
     RETURN floginid;
   END IF;
     
   RETURN 0;
END;
$function$
